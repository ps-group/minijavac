#pragma once
#include "../lexical/token.h"
#include "../support/ierrorstream.h"
#include <vector>

namespace minijava
{

class CParseContext;
class CCxxBodyBuilder;
class CCxxExpr;

class CEmittingParser final
{
public:
    explicit CEmittingParser(CParseContext &context);

    bool ParseOneExpr();
    bool ParseAll();
    const std::string &GetResultCode() const;

private:
    bool ParseMainProc();
    bool ParseStmt(CCxxBodyBuilder &stmt);
    bool ParseVarDeclStatement(CCxxBodyBuilder &stmt);
    bool ParseReturnStatement(CCxxBodyBuilder &stmt);
    bool ParseAssignStatement(CCxxBodyBuilder &stmt);
    bool ParseBracedStatements(CCxxBodyBuilder &stmt);
    bool ParsePrintln(CCxxBodyBuilder &stmt);
    bool ParseCallArguments(std::vector<CCxxExpr> &args);
    CCxxExpr ParseExpr();
    CCxxExpr ParseBinaryExpr(size_t priority);
    CCxxExpr ParseAtom();
    void RecoverToNextStmt();
    bool ConsumeSequenceOrFail(const std::vector<std::pair<Token, Error>> &sequence);
    std::string DecorateMainBody(const std::string &bodyCode);

    CParseContext &m_context;
    std::vector<std::vector<Token>> m_operatorGroups;
    std::string m_code;
};

}
