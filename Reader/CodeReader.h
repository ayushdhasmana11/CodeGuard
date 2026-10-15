#ifndef CODEREADER_H
#define CODEREADER_H

#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

class CodeReader
{
public:
    std::string readFile(const std::string& fileName);

    std::vector<fs::path> getCppFiles(
        const std::string& folderPath
    );
};

#endif