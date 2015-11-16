#include "scanner.h"
#include "basescannerstream.h"
#include "../support/ierrorstream.h"

namespace minijava
{

CScanner::CScanner(CBaseScannerStream &stream, IErrorStream &errorStream)
    : m_stream(stream)
    , m_errorStream(errorStream)
    , m_locale("C")
{
    m_operators = {
        {'{', Token::OpenBrace},
        {'}', Token::CloseBrace},
        {'[', Token::OpenBracket},
        {']', Token::CloseBracket},
        {'(', Token::OpenParen},
        {')', Token::CloseParen},
        {',', Token::Comma},
        {'=', Token::Assign},
        {';', Token::Semicolon},
        {'+', Token::OperatorAdd},
        {'*', Token::OperatorMul},
        {'-', Token::OperatorSub},
        {'<', Token::OperatorLess},
        {'.', Token::Dot},
    };
    m_keywords = {
        {"class", Token::KeywordClass},
        {"public", Token::KeywordPublic},
        {"static", Token::KeywordStatic},
        {"void", Token::KeywordVoid},
        {"string", Token::KeywordString},
        {"extends", Token::KeywordExtends},
        {"return", Token::KeywordReturn},
        {"int", Token::KeywordInt},
        {"boolean", Token::KeywordBoolean},
        {"if", Token::KeywordIf},
        {"while", Token::KeywordWhile},
        {"true", Token::KeywordTrue},
        {"false", Token::KeywordFalse},
        {"this", Token::KeywordThis},
        {"new", Token::KeywordNew},
    };
}

SToken CScanner::ScanOnce()
{
    SkipWhiteSpace();

    const char ch = m_stream.Peek();
    if (ch == '\0')
    {
        FetchToken(Token::End);
    }
    else if (std::isalpha(ch, m_locale))
    {
        FetchToken(Token::Id);
        ConsumeId();
        DetectKeyword();
    }
    else if (std::isdigit(ch, m_locale))
    {
        FetchToken(Token::Integer);
        ConsumeInteger();
    }
    else if (ch == '/' && m_stream.Peek2() == '/')
    {
        FetchToken(Token::Comment);
        ConsumeComment();
    }
    else if (ch == '"')
    {
        FetchToken(Token::String);
        ConsumeString();
    }
    else if (ch == '&' && m_stream.Peek2() == '&')
    {
        FetchToken(Token::OperatorAnd);
        ConsumeChar();
        ConsumeChar();
    }
    else
    {
        auto it = m_operators.find(ch);
        Token token = (it == m_operators.end()) ? Token::Error : it->second;
        FetchToken(token);
        ConsumeChar();
    }

    return m_token;
}

void CScanner::SkipWhiteSpace()
{
    while (std::isspace(m_stream.Peek(), m_locale))
    {
        m_stream.ReadChar();
    }
}

void CScanner::ConsumeId()
{
    ConsumeChar();
    while (std::isalnum(m_stream.Peek(), m_locale))
    {
        ConsumeChar();
    }
}

void CScanner::DetectKeyword()
{
    if (m_token.token == Token::Id)
    {
        auto it = m_keywords.find(m_token.value);
        if (it != m_keywords.end())
        {
            m_token.token = it->second;
        }
    }
}

void CScanner::ConsumeComment()
{
    ConsumeChar();
    ConsumeChar();
    while (m_stream.Peek() != '\0' && m_stream.Peek() != '\n')
    {
        ConsumeChar();
    }
}

void CScanner::ConsumeString()
{
    ConsumeChar();
    for (;;)
    {
        char ch = m_stream.Peek();
        if (ch == '"')
        {
            ConsumeChar();
            break;
        }
        if (ch == '\0' || ch == '\n')
        {
            m_errorStream.EmitError(m_token, Error::MissingQuote);
            break;
        }
        ConsumeChar();
    }
}

void CScanner::ConsumeInteger()
{
    ConsumeChar();
    while (std::isdigit(m_stream.Peek(), m_locale))
    {
        ConsumeChar();
    }
}

void CScanner::FetchToken(Token token)
{
    m_token.token = token;
    m_token.line = m_stream.GetLine();
    m_token.column = m_stream.GetColumn();
    m_token.offset = m_stream.GetOffset();
    m_token.value.clear();
}

void CScanner::ConsumeChar()
{
    m_token.value += m_stream.ReadChar();
}

}
