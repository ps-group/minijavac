#pragma once
#include "token.h"
#include <locale>
#include <unordered_map>

namespace minijava
{

class CBaseScannerStream;
class IErrorStream;

class CScanner final
{
public:
    CScanner(CBaseScannerStream &stream, IErrorStream &errorStream);

    SToken ScanOnce();

private:
    void SkipWhiteSpace();
    void ConsumeId();
    void ConsumeComment();
    void ConsumeString();
    void ConsumeInteger();
    void DetectKeyword();
    void FetchToken(Token token);
    inline void ConsumeChar();

    CBaseScannerStream &m_stream;
    IErrorStream &m_errorStream;
    std::locale m_locale;
    SToken m_token;
    std::unordered_map<char, Token> m_operators;
    std::unordered_map<std::string, Token> m_keywords;
};

}
