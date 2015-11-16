#include "emittingparser.h"
#include "parsecontext.h"
#include "support/ierrorstream.h"
#include "../cxxgen/ccxxexpr.h"
#include "../cxxgen/ccxxbodybuilder.h"
#include <algorithm>
#include <boost/optional.hpp>

namespace minijava
{

namespace
{

std::string MAIN_FUNC_PREFIX = R"**(
#include <string>
#include <iostream>
#include <vector>

int main()
)**";

static const char EXCEPTION_NAME[] = "source code parsing error";
static const char PROPERTY_LENGTH[] = "length";

class ParseException : public std::runtime_error
{
public:
    ParseException(Error error)
        : std::runtime_error(EXCEPTION_NAME)
        , m_error(error)
    {
    }

    ParseException(SToken const& token, Error error)
        : std::runtime_error(EXCEPTION_NAME)
        , m_token(token)
        , m_error(error)
    {
    }

    Error GetError() const
    {
        return m_error;
    }

    bool HasToken() const
    {
        return m_token.is_initialized();
    }

    SToken const& GetToken() const
    {
        return m_token.get();
    }

private:
    boost::optional<SToken> m_token;
    Error m_error;
};

}

CEmittingParser::CEmittingParser(CParseContext &context)
    : m_context(context)
{
    m_operatorGroups.push_back({Token::OperatorLess});
    m_operatorGroups.push_back({Token::OperatorAdd, Token::OperatorSub});
    m_operatorGroups.push_back({Token::OperatorMul});
    m_operatorGroups.push_back({Token::OperatorAnd});
}

bool CEmittingParser::ParseOneExpr()
{
    CCxxExpr expr = ParseExpr();
    m_code = expr.GetCode();
    return true;
}

bool CEmittingParser::ParseAll()
{
    return ParseMainProc();
}

const std::string &CEmittingParser::GetResultCode() const
{
    return m_code;
}

bool CEmittingParser::ParseMainProc()
{
    std::vector<std::pair<Token, Error>> expectedHeader = {
        {Token::KeywordClass,   Error::MissingClass},
        {Token::Id,             Error::MissingClassName},
        {Token::OpenBrace,      Error::MissingScope},
        {Token::KeywordPublic,  Error::MissingMethodDecl},
        {Token::KeywordStatic,  Error::MissingMethodDecl},
        {Token::KeywordVoid,    Error::MissingMethodDecl},
        {Token::Id,             Error::MissingMethodDecl},
        {Token::OpenParen,      Error::MissingArgumentsList},
        {Token::Id,             Error::MissingArgumentsList},
        {Token::OpenBracket,    Error::MissingArgumentsList},
        {Token::CloseBracket,   Error::MissingArgumentsList},
        {Token::Id,             Error::MissingArgumentsList},
        {Token::CloseParen,     Error::MissingCloseParen},
        {Token::OpenBrace,      Error::MissingScope},
    };

    bool noErrors = ConsumeSequenceOrFail(expectedHeader);
    if (noErrors)
    {
        CCxxBodyBuilder stmt;
        stmt.EnterScope();

        for (;;)
        {
            if (m_context.LookAhead(0) == Token::CloseBrace || m_context.LookAhead(0) == Token::End)
            {
                break;
            }
            if (!ParseStmt(stmt))
            {
                noErrors = false;
            }
        }

        stmt.ExitScope();
        m_code = DecorateMainBody(stmt.GetCode());
    }

    return noErrors;
}

bool CEmittingParser::ParseStmt(CCxxBodyBuilder &stmt)
{
    Error error = Error::InvalidStatement;
    try
    {
        bool found = ParseVarDeclStatement(stmt) || ParseAssignStatement(stmt)
                || ParseReturnStatement(stmt) || ParsePrintln(stmt);
        if (found && m_context.LookAhead(0) == Token::Semicolon)
        {
            m_context.Consume(1);
            return true;
        }
        m_context.EmitError(m_context.GetAhead(0), found ? Error::MissingSemicolon : Error::InvalidStatement);
    }
    catch (ParseException const& ex)
    {
        error = ex.GetError();
        SToken token = ex.HasToken() ? ex.GetToken() : m_context.GetAhead(0);
        m_context.EmitError(token, error);
    }
    RecoverToNextStmt();
    return false;
}

bool CEmittingParser::ParseVarDeclStatement(CCxxBodyBuilder &stmt)
{
    bool found = false;
    switch (m_context.LookAhead(0))
    {
    case Token::KeywordBoolean:
    case Token::KeywordInt:
    case Token::Id:
        if (m_context.LookAhead(1) == Token::Id)
        {
            stmt.AddVariableDecl(m_context.GetAhead(0), m_context.GetAhead(1));
            m_context.Consume(2);
            found = true;
        }
        else if (m_context.LookAhead(1) == Token::OpenBracket
                 && m_context.LookAhead(2) == Token::CloseBracket
                 && m_context.LookAhead(3) == Token::Id)
        {
            stmt.AddArrayDecl(m_context.GetAhead(0), m_context.GetAhead(3));
            m_context.Consume(4);
            found = true;
        }
        break;
    default:
        break;
    }
    return found;
}

bool CEmittingParser::ParseReturnStatement(CCxxBodyBuilder &stmt)
{
    bool found = false;
    if (m_context.LookAhead(0) == Token::KeywordReturn)
    {
        found = true;
        m_context.Consume(1);
        CCxxExpr value{ ParseExpr() };
        if (value.IsEmpty())
        {
            stmt.AddReturnVoid();
        }
        else
        {
            stmt.AddReturn(value);
        }
    }
    return found;
}

bool CEmittingParser::ParseAssignStatement(CCxxBodyBuilder &stmt)
{
    bool found = false;
    if (m_context.LookAhead(0) == Token::Id && m_context.LookAhead(1) == Token::Assign)
    {
        SToken name = m_context.GetAhead(0);
        m_context.Consume(2);
        CCxxExpr value{ ParseExpr() };
        if (value.IsEmpty())
        {
            throw ParseException(Error::MissingExpression);
        }
        found = true;
        stmt.AddAssign(name, value);
    }
    return found;
}

bool CEmittingParser::ParseBracedStatements(CCxxBodyBuilder &stmt)
{
    bool found = false;
    if (m_context.LookAhead(0) == Token::OpenBrace)
    {
        found = true;
        m_context.Consume(1);
        stmt.EnterScope();

        for (;;)
        {
            if (m_context.LookAhead(0) == Token::CloseBrace || m_context.LookAhead(0) == Token::End)
            {
                break;
            }
            ParseStmt(stmt);
        }

        stmt.ExitScope();
    }

    return found;
}

bool CEmittingParser::ParsePrintln(CCxxBodyBuilder &stmt)
{
    std::vector<std::pair<Token, std::string>> sequence = {
        {Token::Id,         "System"},
        {Token::Dot,        ""},
        {Token::Id,         "out"},
        {Token::Dot,        ""},
        {Token::Id,         "println"},
    };
    size_t ahead = 0;
    bool noErrors = std::all_of(sequence.begin(), sequence.end(), [&](std::pair<Token, std::string> const &pair) {
        const SToken& token = m_context.GetAhead(ahead++);
        return (token.token == pair.first) && (pair.second.empty() || token.value == pair.second);
    });
    if (!noErrors)
    {
        return false;
    }
    m_context.Consume(sequence.size());
    std::vector<CCxxExpr> args;
    if (!ParseCallArguments(args))
    {
        throw ParseException(Error::MissingArgumentsList);
    }
    stmt.AddPrintln(args);
    return true;
}

bool CEmittingParser::ParseCallArguments(std::vector<CCxxExpr> &args)
{
    if (m_context.LookAhead(0) != Token::OpenParen)
    {
        return false;
    }
    m_context.Consume(1);
    bool expectsComma = false;
    for (;;)
    {
        if (m_context.LookAhead(0) == Token::CloseParen)
        {
            m_context.Consume(1);
            return true;
        }
        if (expectsComma)
        {
            if (m_context.LookAhead(0) == Token::Comma)
            {
                m_context.Consume(1);
            }
            else
            {
                return false;
            }
        }

        CCxxExpr value{ ParseExpr() };
        if (value.IsEmpty())
        {
            return false;
        }
        args.push_back(value);
        expectsComma = true;
    }
}

CCxxExpr CEmittingParser::ParseExpr()
{
    return ParseBinaryExpr(0);
}

CCxxExpr CEmittingParser::ParseBinaryExpr(size_t priority)
{
    if (priority >= m_operatorGroups.size())
    {
        return ParseCallExpr();
    }

    CCxxExpr expr{ ParseBinaryExpr(priority + 1) };
    if (expr.IsEmpty())
    {
        return expr;
    }

    auto isMatchingOperator = [&]() {
        auto const& group = m_operatorGroups[priority];
        auto it = std::find(group.begin(), group.end(), m_context.LookAhead(0));
        return it != group.end();
    };

    while (isMatchingOperator())
    {
        SToken token = m_context.GetAhead(0);
        m_context.Consume(1);
        CCxxExpr right = ParseBinaryExpr(priority + 1);
        if (right.IsEmpty())
        {
            throw ParseException(token, Error::UnexpectedBinOp);
        }
        expr.EmitBinExpr(token.token, std::move(right));
    }

    return expr;
}

CCxxExpr CEmittingParser::ParseCallExpr()
{
    CCxxExpr object{ ParseNewExpr() };
    if (object.IsEmpty())
    {
        return object;
    }

    switch (m_context.LookAhead(0))
    {
    case Token::Dot:
    {
        m_context.Consume(1);
        SToken id = m_context.GetAhead(0);
        if (id.token != Token::Id)
        {
            throw ParseException(Error::MissingIdentifier);
        }
        m_context.Consume(1);
        if (m_context.LookAhead(0) == Token::OpenParen)
        {
            std::vector<CCxxExpr> args;
            if (!ParseCallArguments(args))
            {
                throw ParseException(Error::MissingArgumentsList);
            }
            object.EmitCallMethod(id, args);
        }
        else if (id.value == PROPERTY_LENGTH)
        {
            object.EmitGetLength();
        }
        else
        {
            throw ParseException(Error::MissingArgumentsList);
        }
        break;
    }
    case Token::OpenBracket:
    {
        m_context.Consume(1);
        CCxxExpr index{ ParseExpr() };
        if (m_context.LookAhead(0) != Token::CloseBracket)
        {
            throw ParseException(m_context.GetAhead(0), Error::UnexpectedBinOp);
        }
        object.EmitGetElement(index);
        break;
    }
    default:
        break;
    }
    return object;
}

CCxxExpr CEmittingParser::ParseNewExpr()
{
    if (m_context.LookAhead(0) == Token::KeywordNew)
    {
        m_context.Consume(1);
        CCxxExpr expr{ ParseAtom() };
        expr.EmitNewExpr();
        if (m_context.LookAhead(0) == Token::OpenParen)
        {
            if (m_context.LookAhead(1) != Token::CloseParen)
            {
                throw ParseException(Error::MissingCloseParen);
            }
            m_context.Consume(2);
        }
        return expr;
    }

    return ParseAtom();
}

CCxxExpr CEmittingParser::ParseAtom()
{
    auto const& tok = m_context.GetAhead(0);
    switch (tok.token)
    {
    case Token::Id:
    case Token::Integer:
    case Token::String:
    case Token::KeywordThis:
    case Token::KeywordTrue:
    case Token::KeywordFalse:
        m_context.Consume(1);
        return CCxxExpr(tok);
    case Token::OpenParen:
    {
        m_context.Consume(1);
        CCxxExpr expr{ ParseExpr() };
        if (m_context.LookAhead(0) != Token::CloseParen)
        {
            m_context.EmitError(m_context.GetAhead(0), Error::MissingCloseParen);
        }
        else
        {
            m_context.Consume(1);
        }
        return expr;
    }
    default:
        return CCxxExpr();
    }
}

void CEmittingParser::RecoverToNextStmt()
{
    size_t ahead = 0;
    for (;; ++ ahead)
    {
        auto const& token = m_context.GetAhead(ahead);
        if (token.token == Token::Semicolon)
        {
            ++ahead;
            break;
        }
        if (token.token == Token::End || token.token == Token::CloseBrace)
        {
            break;
        }
    }
    m_context.Consume(ahead);
}

bool CEmittingParser::ConsumeSequenceOrFail(const std::vector<std::pair<Token, Error> > &sequence)
{
    size_t ahead = 0;
    auto it = std::find_if(sequence.begin(), sequence.end(), [&](std::pair<Token, Error> const& pair) {
        auto const& token = m_context.GetAhead(ahead++);
        return token.token != pair.first;
    });
    if (it != sequence.end())
    {
        m_context.EmitError(m_context.GetAhead(it - sequence.begin()), it->second);
        return false;
    }
    m_context.Consume(sequence.size());
    return true;
}

std::string CEmittingParser::DecorateMainBody(const std::string &bodyCode)
{
    return MAIN_FUNC_PREFIX + bodyCode;
}

} // namespace minijava
