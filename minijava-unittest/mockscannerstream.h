#pragma once

#include "lexical/basescannerstream.h"

class CMockScannerStream : public minijava::CBaseScannerStream
{
public:
    explicit CMockScannerStream(std::string const& code);
    ~CMockScannerStream();

protected:
    size_t ReadString(std::string &buffer, size_t bytesLimit) override;

private:
    std::string m_code;
    size_t m_readSize = 0;
};
