#include "parsecontext.h"
#include "lexical/scanner.h"

namespace minijava {

CParseContext::CParseContext(CScanner &scanner, IErrorStream &stream)
    : m_scanner(scanner)
    , m_errorStream(stream)
{
}

const Token &CParseContext::LookAhead(size_t ahead)
{
    return GetAhead(ahead).token;
}

const SToken &CParseContext::GetAhead(size_t ahead)
{
    if (m_lookAheadStack.size() <= ahead)
    {
        ExtendLAStack(ahead + 1);
    }
    return m_lookAheadStack[ahead];
}

void CParseContext::Consume(size_t amount)
{
    m_lookAheadStack.erase(m_lookAheadStack.begin(), m_lookAheadStack.begin() + amount);
}

void CParseContext::ExtendLAStack(size_t newSize)
{
    m_lookAheadStack.reserve(newSize);
    while (m_lookAheadStack.size() < newSize)
    {
        m_lookAheadStack.push_back(m_scanner.ScanOnce());
    }
}

} // namespace minijava
