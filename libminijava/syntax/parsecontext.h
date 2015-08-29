#pragma once
#include "../support/ierrorstream.h"
#include "../lexical/token.h"

namespace minijava
{

class CScanner;

class CParseContext
{
public:
    explicit CParseContext(CScanner &scanner, IErrorStream &stream);

    const Token &LookAhead(size_t ahead);
    const SToken &GetAhead(size_t ahead);
    void Consume(size_t amount);
    void ExtendLAStack(size_t newSize);


    inline void EmitError(SToken const& token, Error code)
    {
        m_errorStream.EmitError(token, code);
    }

    inline void EmitWarning(SToken const& token, Error code)
    {
        m_errorStream.EmitWarning(token, code);
    }

private:
    CScanner &m_scanner;
    IErrorStream &m_errorStream;
    std::vector<SToken> m_lookAheadStack;
};

}
