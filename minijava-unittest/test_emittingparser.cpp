#include <boost/test/unit_test.hpp>
#include "syntax/emittingparser.h"
#include "syntax/parsecontext.h"
#include "lexical/scanner.h"
#include "mockerrorstream.h"
#include "mockscannerstream.h"

using minijava::CEmittingParser;
using minijava::CParseContext;
using minijava::CScanner;
using namespace std;

static const char RAW_CODE[] = "vec + (2 * 12) < (vec2 - 2) * 3 * 7 * 19";
static const char RESULT_CODE[] = "((vec + (2 * 12)) < ((((vec2 - 2) * 3) * 7) * 19))";
static const char RAW_CODE_2[] = R"***(
    class Add2Program
    {
        public static void main(String []args)
        {
            int n1;
            int n2;
            n1 = 1;
            n2 = 2;
            return 3 * (n2 - n1) - 3;
        }
    }
)***";

BOOST_AUTO_TEST_CASE(testEmittingParserOneExpr)
{
    CMockScannerStream stream(RAW_CODE);
    CMockErrorStream errorStream;
    CScanner scanner(stream, errorStream);
    CParseContext context(scanner, errorStream);
    CEmittingParser parser(context);

    BOOST_CHECK(parser.ParseOneExpr());
    if (parser.GetResultCode() != RESULT_CODE)
    {
        string message = "unexpected result code `" + parser.GetResultCode() + "`";
        BOOST_ERROR(message);
    }
}

BOOST_AUTO_TEST_CASE(testEmittingParserAll)
{
    CMockScannerStream stream(RAW_CODE_2);
    CMockErrorStream errorStream;
    CScanner scanner(stream, errorStream);
    CParseContext context(scanner, errorStream);
    CEmittingParser parser(context);

    BOOST_CHECK(parser.ParseAll());
    string code = parser.GetResultCode();
    FILE *out = fopen("out.txt", "w");
    fwrite(code.c_str(), sizeof(char), code.size(), out);
    fclose(out);
}
