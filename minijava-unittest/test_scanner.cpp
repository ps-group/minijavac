#include <boost/test/unit_test.hpp>
#include "lexical/filestream.h"
#include "lexical/scanner.h"
#include "mockscannerstream.h"
#include "mockerrorstream.h"
#include <stdio.h>
#include <boost/format.hpp>

#define BOOST_TEST_MODULE testScanner

using namespace std;

static const char RAW_CODE[] = R"***(
    // File: add2.mj
    // This program adds two numbers.
    class Add2Program
    {
      void main()
      {
        int n1, n2;
        println("This program adds two numbers");
        print("Enter 1st number? ");
        n1 = readInt();
        print("Enter 2nd number? ");
        n2 = readInt();
        println("The sum is " + (n1 + n2));
      }
    }
)***";

BOOST_AUTO_TEST_CASE(testScanner)
{
    using minijava::Token;
    CMockScannerStream stream(RAW_CODE);
    CMockErrorStream errorStream;
    minijava::CScanner scanner(stream, errorStream);
    vector<Token> expectedTokens = {
        Token::Comment, Token::Comment, Token::KeywordClass, Token::Id,
        Token::OpenBrace, Token::KeywordVoid, Token::Id, Token::OpenParen, Token::CloseParen,
        Token::OpenBrace, Token::KeywordInt, Token::Id, Token::Comma, Token::Id, Token::Semicolon,
        Token::Id, Token::OpenParen, Token::String, Token::CloseParen, Token::Semicolon,
        Token::Id, Token::OpenParen, Token::String, Token::CloseParen, Token::Semicolon,
        Token::Id, Token::Assign, Token::Id, Token::OpenParen, Token::CloseParen, Token::Semicolon,
        Token::Id, Token::OpenParen, Token::String, Token::CloseParen, Token::Semicolon,
        Token::Id, Token::Assign, Token::Id, Token::OpenParen, Token::CloseParen, Token::Semicolon,
        Token::Id, Token::OpenParen, Token::String, Token::OperatorAdd, Token::OpenParen,
        Token::Id, Token::OperatorAdd, Token::Id, Token::CloseParen, Token::CloseParen,
        Token::Semicolon, Token::CloseBrace, Token::CloseBrace, Token::End
    };
    auto formatter = boost::format("");
    for (Token expected : expectedTokens)
    {
        auto token = scanner.ScanOnce();
        boost::format formatter("tokens mismatch at line %1%, column %2%");
        string message = boost::str(formatter % token.line % token.column);
        if (token.token != expected)
            BOOST_ERROR(message);
    }
}
