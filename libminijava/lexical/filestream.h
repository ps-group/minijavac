#pragma once
#include <string>
#include "basescannerstream.h"

namespace minijava
{

class CFileStream : public CBaseScannerStream
{
public:
    explicit CFileStream(const std::string &filePath);
    ~CFileStream();

protected:
    size_t ReadString(std::string &buffer, size_t bytesLimit) override;

private:
    FILE *m_file = nullptr;
};

}
