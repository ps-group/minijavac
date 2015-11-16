#include "ccxxexpr.h"
#include <stdexcept>
#include <boost/format.hpp>
#include <sstream>
#include "../lexical/token.h"

namespace minijava
{

CCxxExpr::CCxxExpr()
{
}

CCxxExpr::CCxxExpr(const SToken &token)
{
    switch (token.token)
    {
    case Token::Id:
    case Token::Integer:
        m_code = token.value;
        break;
    case Token::KeywordThis:
        m_code = "this";
        break;
    case Token::KeywordTrue:
        m_code = "1";
        break;
    case Token::KeywordFalse:
        m_code = "0";
        break;
    default:
        throw std::logic_error("Cannot construct CCppExprBuilder with token '" + token.value + "'");
        break;
    }
}

void CCxxExpr::EmitBinExpr(Token token, const CCxxExpr &exprRight)
{
    std::string operation;
    switch (token)
    {
    case Token::OperatorAdd:
        operation = "+";
        break;
    case Token::OperatorAnd:
        operation = "&&";
        break;
    case Token::OperatorLess:
        operation = "<";
        break;
    case Token::OperatorMul:
        operation = "*";
        break;
    case Token::OperatorSub:
        operation = "-";
        break;
    default:
        // TODO: throw exception.
        break;
    }

    boost::format formatter("(%1% %2% %3%)");
    formatter % m_code % operation % exprRight.m_code;
    m_code = boost::str(formatter);
}

void CCxxExpr::EmitNot()
{
    boost::format formatter("(!%1%)");
    formatter % m_code;
    m_code = boost::str(formatter);
}

void CCxxExpr::EmitLessThan(const CCxxExpr &exprRight)
{
    boost::format formatter("(%1% < %2%)");
    formatter % m_code % exprRight.m_code;
    m_code = boost::str(formatter);
}

void CCxxExpr::EmitGetElement(const CCxxExpr &exprIndex)
{
    boost::format formatter("(%1%[%2%])");
    formatter % m_code % exprIndex.m_code;
    m_code = boost::str(formatter);
}

void CCxxExpr::EmitGetLength()
{
    boost::format formatter("(%1%.size())");
    formatter % m_code;
    m_code = boost::str(formatter);
}

void CCxxExpr::EmitCallMethod(const SToken &methodId, const std::vector<CCxxExpr> &args)
{
    std::string argsString = "(";
    for (int i = 0, n = args.size(); i < n; ++i)
    {
        argsString += (i == n) ? ')' : ',';
        argsString += args[i].GetCode();
    }
    boost::format formatter("(%1%.%2%%3%)");
    formatter % m_code % methodId.value % argsString;
    m_code = boost::str(formatter);
}

void CCxxExpr::EmitNewExpr()
{
    boost::format formatter("(new %1%())");
    formatter % m_code;
    m_code = boost::str(formatter);
}

const std::string &CCxxExpr::GetCode() const
{
    return m_code;
}

bool CCxxExpr::IsEmpty() const
{
    return m_code.empty();
}

}
