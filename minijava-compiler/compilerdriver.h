#pragma once

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

class CCompilerDriver
{
public:
    CCompilerDriver();
    bool compile(const fs::path &inputPath, const fs::path &outputPath);

private:
    bool generateCode(const fs::path &inputPath, std::string &code);
};
