#include "stderrorstream.h"
#include "lexical/token.h"
#include <sstream>
#include <iostream>

using std::cerr;
using minijava::Error;

CStderrErrorStream::CStderrErrorStream()
{
    m_messages = {
        {Error::UnexpectedBinOp, "unexpected operator"},
        {Error::MissingScope, "expected '{'"},
        {Error::MissingCloseParen, "expected ')'"},
        {Error::MissingQuote, "mismatched '\"'"},
        {Error::MissingClass, "missed 'class' keyword"},
        {Error::MissingMethodDecl, "missed method declaration"},
        {Error::MissingMethodParametersList, "missed method parameters list"},
        {Error::MissingExtends, "missed 'extends' keyword"},
        {Error::MissingSemicolon, "missing ';'"},
        {Error::MissingExpression, "missing expression"},
        {Error::MissingClassName, "missing class name"},
        {Error::InvalidStatement, "unknown statement"},
    };
}

void CStderrErrorStream::EmitError(const minijava::SToken &token, minijava::Error code)
{
    cerr << Format("error", token, code) << std::endl;
}

void CStderrErrorStream::EmitWarning(const minijava::SToken &token, minijava::Error code)
{
    cerr << Format("warning", token, code) << std::endl;
}

std::string CStderrErrorStream::Format(const std::string &severity,
                                       const minijava::SToken &token,
                                       minijava::Error code)
{
    std::stringstream stream;
    stream << severity << " at " << token.line << "," << token.column << ": ";
    stream << FormatMessaage(code);

    return stream.str();
}

std::string CStderrErrorStream::FormatMessaage(minijava::Error code)
{
    auto it = m_messages.find(code);
    if (it != m_messages.end())
    {
        return it->second;
    }
    return "internal compiler error";
}
