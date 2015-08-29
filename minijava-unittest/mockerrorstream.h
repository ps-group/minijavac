#pragma once

#include "support/ierrorstream.h"
#include "lexical/token.h"
#include <vector>

class CMockErrorStream : public minijava::IErrorStream
{
public:
    CMockErrorStream();

    void EmitError(const minijava::SToken &token, minijava::Error code) override;
    void EmitWarning(const minijava::SToken &token, minijava::Error code) override;

private:
    using Diagnostic = std::pair<minijava::SToken, minijava::Error>;

    std::vector<Diagnostic> m_errors;
    std::vector<Diagnostic> m_warnings;
};
