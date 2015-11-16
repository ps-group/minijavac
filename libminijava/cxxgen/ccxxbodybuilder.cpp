#include "ccxxbodybuilder.h"
#include "../lexical/token.h"
#include <stdexcept>

namespace minijava
{

static const char ERROR_INDENT_MISMATCH[] = "code generation error: CCxxBodyBuilder exits scope but never entered";

CCxxBodyBuilder::CCxxBodyBuilder()
{
    m_typeTransform = {
        {"", ""}
    };
}

void CCxxBodyBuilder::EnterScope()
{
    AppendLine<'{'>();
    ++m_indent;
}

void CCxxBodyBuilder::ExitScope()
{
    if (m_indent <= 0)
    {
        throw std::runtime_error(ERROR_INDENT_MISMATCH);
    }
    --m_indent;
    AppendLine<'}'>();
}

void CCxxBodyBuilder::AddVariableDecl(const SToken &type, const SToken &name)
{
    switch (type.token)
    {
    case Token::KeywordInt:
    case Token::KeywordBoolean:
        AppendLine<';'>("int " + name.value);
        break;
    case Token::Id:
        AppendLine<';'>(TransformType(type.value) + " " + name.value);
        break;
    default:
        throw std::logic_error("CCxxStmt cannot add variable with type '" + type.value + "'");
    }
}

void CCxxBodyBuilder::AddArrayDecl(const SToken &type, const SToken &name)
{
    switch (type.token)
    {
    case Token::KeywordInt:
    case Token::KeywordBoolean:
        AppendLine<';'>("std::vector<int> " + name.value);
        break;
    case Token::Id:
        AppendLine<';'>("std::vector<" + TransformType(type.value) + "> " + name.value);
        break;
    default:
        throw std::logic_error("CCxxStmt cannot add variable with type '" + type.value + "'");
    }
}

void CCxxBodyBuilder::AddAssign(const SToken &name, const CCxxExpr &value)
{
    AppendLine<';'>(name.value + " = " + value.GetCode());
}

void CCxxBodyBuilder::AddElementAssign(const SToken &name, const CCxxExpr &index, const CCxxExpr &value)
{
    AppendLine<';'>(name.value + "[" + index.GetCode() + "] = " + value.GetCode());
}

void CCxxBodyBuilder::AddPrintln(const std::vector<CCxxExpr> &printable)
{
    for (auto const& expr : printable)
    {
        AppendLine<';'>("std::cout << " + expr.GetCode() + " << std::endl");
    }
}

void CCxxBodyBuilder::AddReturn(const CCxxExpr &expr)
{
    AppendLine<';'>("return " + expr.GetCode());
}

void CCxxBodyBuilder::AddReturnVoid()
{
    AppendLine<';'>("return");
}

void CCxxBodyBuilder::EnterWhileLoop(const CCxxExpr &condition)
{
    AppendLine<'{'>("while (" + condition.GetCode() + ")");
    ++m_indent;
}

void CCxxBodyBuilder::EnterIfBranch(const CCxxExpr &condition)
{
    AppendLine<'{'>("if (" + condition.GetCode() + ")");
    ++m_indent;
}

void CCxxBodyBuilder::EnterElseBranch(const CCxxExpr &condition)
{
    AppendLine<'{'>("else (" + condition.GetCode() + ")");
    ++m_indent;
}

std::string CCxxBodyBuilder::GetCode() const
{
    return m_code.str();
}

const std::string &CCxxBodyBuilder::TransformType(const std::string &value)
{
    auto it = m_typeTransform.find(value);
    if (it != m_typeTransform.end())
    {
        return it->second;
    }
    return value;
}

}
