#include "compilerdriver.h"
#include <fstream>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    fs::path inputPath;
    fs::path outputPath;
    if (argc == 4 && std::string("-o") == argv[2])
    {
        inputPath = argv[1];
        outputPath = argv[3];
    }
    else if (argc == 2)
    {
        inputPath = argv[1];
        outputPath = inputPath;
        outputPath.replace_extension(".exe");
    }
    else
    {
        fs::path selfpath = argv[0];
        selfpath = selfpath.stem();
        cerr << "Simple compiler for MiniJava programming language\n"
                  << "Usage:\n"
                  << "  " << selfpath << " <input> -o <output>\n"
                  << "  " << selfpath << " <input>"
                  << std::endl;
        return 1;
    }
    CCompilerDriver driver;
    if (!driver.compile(inputPath, outputPath))
    {
        cerr << "Compilation failed." << std::endl;
        return 1;
    }

    return 0;
}
