#include "ccxxbodybuilder.h"
#include "../lexical/token.h"
#include <stdexcept>

namespace minijava
{

CCxxBodyBuilder::CCxxBodyBuilder()
{
    m_typeTransform = {
        {"", ""}
    };
}

void CCxxBodyBuilder::EnterScope()
{
    m_code << "{\n";
}

void CCxxBodyBuilder::ExitScope()
{
    m_code << "}\n";
}

void CCxxBodyBuilder::AddVariableDecl(const SToken &type, const SToken &name)
{
    switch (type.token)
    {
    case Token::KeywordInt:
    case Token::KeywordBoolean:
        m_code << "int " + name.value + ";\n";
        break;
    case Token::Id:
        m_code << TransformType(type.value) + " " + name.value + ";\n";
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
        m_code << "std::vector<int> " + name.value + ";\n";
        break;
    case Token::Id:
        m_code << "std::vector<" + TransformType(type.value) + "> " + name.value + ";\n";
        break;
    default:
        throw std::logic_error("CCxxStmt cannot add variable with type '" + type.value + "'");
    }
}

void CCxxBodyBuilder::AddAssign(const SToken &name, const CCxxExpr &value)
{
    m_code << name.value + " = " + value.GetCode() + ";\n";
}

void CCxxBodyBuilder::AddElementAssign(const SToken &name, const CCxxExpr &index, const CCxxExpr &value)
{
    m_code << name.value + "[" + index.GetCode() + "] = " + value.GetCode() + ";\n";
}

void CCxxBodyBuilder::AddPrintln(const std::vector<CCxxExpr> &printable)
{
    for (auto const& expr : printable)
    {
        m_code << "std::cout << " << expr.GetCode() << ";\n";
        m_code << "std::cout << std::endl;\n";
    }
}

void CCxxBodyBuilder::AddReturn(const CCxxExpr &expr)
{
    m_code << "return " << expr.GetCode() << ";\n";
}

void CCxxBodyBuilder::AddReturnVoid()
{
    m_code << "return;\n";
}

void CCxxBodyBuilder::AddWhileLoop(const CCxxExpr &condition, const CCxxBodyBuilder &body)
{
    m_code << "while (" << condition.GetCode() << ")\n"
           << body.GetCode()
           << "\n";
}

void CCxxBodyBuilder::AddIfElseBranch(const CCxxExpr &condition, const CCxxBodyBuilder &bodyYes, const CCxxBodyBuilder &bodyNo)
{
    m_code << "if (" << condition.GetCode() << ")\n"
           << bodyYes.GetCode()
           << "else\n"
           << bodyNo.GetCode()
           << "\n";
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
