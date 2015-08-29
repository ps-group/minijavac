#include "compilerdriver.h"
#include "lexical/filestream.h"
#include "lexical/scanner.h"
#include "syntax/parsecontext.h"
#include "syntax/emittingparser.h"
#include "stderrorstream.h"
#include <fstream>
#include <sstream>

using namespace std;
using namespace minijava;

CCompilerDriver::CCompilerDriver()
{
}

bool CCompilerDriver::compile(const fs::path &inputPath, const fs::path &outputPath)
{
    string intermediateName = "__" + outputPath.filename().generic_string() + ".cpp";
    fs::path intermediatePath = outputPath;
    intermediatePath.remove_filename();
    intermediatePath /= intermediateName;

    string code;
    if (!generateCode(inputPath, code))
    {
        return false;
    }

    ofstream out;
    out.open(intermediatePath.generic_string().c_str(), ofstream::out);
    out.write(code.c_str(), code.size());
    out.close();

    stringstream command;
    command << "clang++ " << intermediatePath << " -o " << outputPath;
    cout << "Executing: " << command.str() << std::endl;
    system(command.str().c_str());

    return true;
}

bool CCompilerDriver::generateCode(const fs::path &inputPath, std::string &code)
{
    CFileStream stream(inputPath.generic_string());
    CStderrErrorStream errorStream;
    CScanner scanner(stream, errorStream);
    CParseContext context(scanner, errorStream);
    CEmittingParser parser(context);

    if (!parser.ParseAll())
    {
        return false;
    }
    code = parser.GetResultCode();
    return true;
}
