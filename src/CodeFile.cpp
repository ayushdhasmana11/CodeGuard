#include "CodeFile.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

CodeFile::CodeFile(string name)
{
    filename = name;
    sourceCode = "";
}

bool CodeFile::readFile()
{
    ifstream file(filename);

    if (!file.is_open())
    {
        cout << "Error: Cannot open file "
             << filename << endl;

        return false;
    }

    stringstream buffer;
    buffer << file.rdbuf();

    sourceCode = buffer.str();

    file.close();

    return true;
}

string CodeFile::getFilename()
{
    return filename;
}

string CodeFile::getSourceCode()
{
    return sourceCode;
}