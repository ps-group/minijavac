#pragma once

#include "ccxxexpr.h"
#include <string>
#include <vector>
#include <sstream>
#include <map>

namespace minijava
{

struct SToken;

class CCxxBodyBuilder
{
public:
    CCxxBodyBuilder();

    void EnterScope();
    void ExitScope();

    void AddVariableDecl(SToken const& type, SToken const& name);
    void AddArrayDecl(SToken const& type, SToken const& name);
    void AddAssign(SToken const& name, CCxxExpr const& value);
    void AddElementAssign(SToken const& name, CCxxExpr const& index, CCxxExpr const& value);
    void AddPrintln(const std::vector<CCxxExpr> &printable);
    void AddReturn(CCxxExpr const& expr);
    void AddReturnVoid();
    void AddWhileLoop(CCxxExpr const& condition, CCxxBodyBuilder const& body);
    void AddIfElseBranch(CCxxExpr const& condition, CCxxBodyBuilder const& bodyYes, CCxxBodyBuilder const& bodyNo);

    std::string GetCode() const;

private:
    std::string const& TransformType(std::string const& value);

    std::map<std::string, std::string> m_typeTransform;
    std::stringstream m_code;
};

}
