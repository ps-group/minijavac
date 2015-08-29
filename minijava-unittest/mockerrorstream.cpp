#include "mockerrorstream.h"

CMockErrorStream::CMockErrorStream()
{

}

void CMockErrorStream::EmitError(const minijava::SToken &token, minijava::Error code)
{
    m_errors.push_back(Diagnostic(token, code));
}

void CMockErrorStream::EmitWarning(const minijava::SToken &token, minijava::Error code)
{
    m_warnings.push_back(Diagnostic(token, code));
}
