#pragma once
#include "support/ierrorstream.h"
#include <map>

class CStderrErrorStream : public minijava::IErrorStream
{
public:
    CStderrErrorStream();
    void EmitError(const minijava::SToken &token, minijava::Error code) override;
    void EmitWarning(const minijava::SToken &token, minijava::Error code) override;

private:
    std::string Format(const std::string &severity,
                       const minijava::SToken &token,
                       minijava::Error code);
    std::string FormatMessaage(minijava::Error code);

    std::map<minijava::Error, std::string> m_messages;
};
