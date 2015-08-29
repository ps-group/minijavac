#include "mockscannerstream.h"

CMockScannerStream::CMockScannerStream(const std::string &code)
    : m_code(code)
{
    MoveBufferAhead();
}

CMockScannerStream::~CMockScannerStream()
{
}

size_t CMockScannerStream::ReadString(std::string &buffer, size_t bytesLimit)
{
    size_t read = std::min(m_code.size() - m_readSize, bytesLimit);
    buffer += m_code.substr(m_readSize, read);
    m_readSize += read;

    return m_readSize;
}

