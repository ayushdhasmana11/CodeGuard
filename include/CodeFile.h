#ifndef CODEFILE_H
#define CODEFILE_H

#include <string>

using namespace std;

class CodeFile
{
private:
    string filename;
    string sourceCode;

public:
    CodeFile(string name);

    bool readFile();

    string getFilename();
    string getSourceCode();
};

#endif