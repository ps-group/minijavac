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
    void EnterWhileLoop(CCxxExpr const& condition);
    void EnterIfBranch(CCxxExpr const& condition);
    void EnterElseBranch(CCxxExpr const& condition);

    std::string GetCode() const;

private:
    // endian is ';', '{' or '}'
    template <char endian, typename ...TArgs>
    void AppendLine(TArgs... args)
    {
        for (int i = 0, n = m_indent; i < n; ++i)
        {
            m_code << '\t';
        }
        DoNothing(PrintArg(args)...);
        m_code << endian << std::endl;
    }

    template <typename ...TArgs>
    void DoNothing(TArgs...)
    {
    }

    template <typename T>
    T const& PrintArg(T const& arg)
    {
        m_code << arg;
        return arg;
    }

    std::string const& TransformType(std::string const& value);

    std::map<std::string, std::string> m_typeTransform;
    std::stringstream m_code;
    int m_indent = 0;
};

}
