#pragma once

#include <string>

namespace minijava
{

struct SToken;
enum class Token;

class CCxxExpr
{
public:
    CCxxExpr();
    CCxxExpr(SToken const& token);
    CCxxExpr(CCxxExpr const& other) = default;
    CCxxExpr(CCxxExpr && other) = default;
    CCxxExpr& operator =(CCxxExpr const& other) = default;
    CCxxExpr& operator =(CCxxExpr && other) = default;

    void EmitBinExpr(Token token, CCxxExpr const& exprRight);
    void EmitNot();
    void EmitLessThan(CCxxExpr const& exprRight);
    void EmitGetElement(CCxxExpr const& exprIndex);
    void EmitGetLength();

    std::string const& GetCode() const;
    bool IsEmpty() const;

private:
    std::string m_code;
};

}
