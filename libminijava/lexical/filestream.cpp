#include "filestream.h"
#include <stdexcept>
#include <cstdio>
#include <locale>
#include <memory>

namespace minijava
{

CFileStream::CFileStream(const std::string &filePath)
    : m_file(std::fopen(filePath.c_str(), "r"))
{
    if (!m_file)
    {
        throw std::runtime_error("Failed to create CFileStream from '" + filePath + "'.");
    }
    MoveBufferAhead();
}

CFileStream::~CFileStream()
{
    if (m_file)
    {
        std::fclose(m_file);
    }
}

size_t CFileStream::ReadString(std::string &buffer, size_t bytesLimit)
{
    std::unique_ptr<char[]> rawBuffer(new char[bytesLimit]);
    size_t readCount = std::fread(rawBuffer.get(), sizeof(char), bytesLimit, m_file);
    buffer.append(rawBuffer.get(), rawBuffer.get() + readCount);
    return readCount;
}

}
