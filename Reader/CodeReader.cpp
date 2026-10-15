#include "CodeReader.h"

#include <fstream>
#include <stdexcept>
#include <algorithm>

std::string CodeReader::readFile(const std::string& fileName)
{
    std::ifstream file(fileName);

    if (!file.is_open())
    {
        throw std::runtime_error(
            "Unable to open file: " + fileName
        );
    }

    std::string code;
    std::string line;

    while (std::getline(file, line))
    {
        code += line + '\n';
    }

    if (file.bad())
    {
        throw std::runtime_error(
            "Failed while reading file: " + fileName
        );
    }

    return code;
}

std::vector<fs::path> CodeReader::getCppFiles(
    const std::string& folderPath)
{
    std::vector<fs::path> sourceFiles;

    if (!fs::exists(folderPath))
    {
        throw std::runtime_error(
            "Folder does not exist: " + folderPath
        );
    }

    if (!fs::is_directory(folderPath))
    {
        throw std::runtime_error(
            "Path is not a directory: " + folderPath
        );
    }

    for (const auto& entry : fs::directory_iterator(folderPath))
    {
        if (!entry.is_regular_file())
        {
            continue;
        }

        std::string extension =
            entry.path().extension().string();

        if (extension == ".c" || extension == ".cpp")
        {
            sourceFiles.push_back(entry.path());
        }
    }

    std::sort(sourceFiles.begin(), sourceFiles.end());

    return sourceFiles;
}