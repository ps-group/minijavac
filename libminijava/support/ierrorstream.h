#pragma once
#include <stdint.h>
#include <string>
#include <vector>

namespace minijava
{

struct SToken;

enum class Error
{
    UnexpectedBinOp,
    MissingBlock,
    MissingCloseParen,
    MissingQuote,
    MissingClass,
    MissingExtends,
    MissingClassName,
    MissingScope,
    MissingMethodDecl,
    MissingMethodParametersList,
    MissingArgumentsList,
    MissingSemicolon,
    MissingExpression,
    InvalidStatement,
};

class IErrorStream
{
public:
    virtual ~IErrorStream() = default;
    virtual void EmitError(SToken const& token, Error code) = 0;
    virtual void EmitWarning(SToken const& token, Error code) = 0;
};

}
