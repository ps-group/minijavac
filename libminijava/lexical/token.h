#pragma once
#include <stdint.h>
#include <string>

namespace minijava
{

enum class Token
{
    End,
    Error,
    Id,
    Integer,
    String,
    Comment,
    KeywordClass,
    KeywordPublic,
    KeywordStatic,
    KeywordVoid,
    KeywordString,
    KeywordExtends,
    KeywordReturn,
    KeywordInt,
    KeywordBoolean,
    KeywordIf,
    KeywordWhile,
    KeywordTrue,
    KeywordFalse,
    KeywordThis,
    KeywordNew,
    OperatorAnd,    // &&
    OperatorNot,    // !
    OperatorLess,   // <
    OperatorAdd,    // +
    OperatorSub,    // -
    OperatorMul,    // *
    Semicolon,      // ;
    Dot,            // .
    Comma,          // ,
    Assign,         // =
    OpenParen,      // (
    CloseParen,     // )
    OpenBrace,      // {
    CloseBrace,     // }
    OpenBracket,    // [
    CloseBracket,   // ]
};

struct SToken
{
    Token   token   = Token::End;
    int32_t line    = 0;
    int32_t column  = 0;
    int32_t offset  = 0;
    std::string value;  // Only for identifiers and literals.
};

}
