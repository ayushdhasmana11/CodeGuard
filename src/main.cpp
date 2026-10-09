#include <iostream>
#include "CodeFile.h"

using namespace std;

int main()
{
    CodeFile file("data/student1.cpp");

    if (file.readFile())
    {
        cout << "File read successfully!" << endl;
        cout << "Filename: " << file.getFilename() << endl;

        cout << "\nSource Code:\n";
        cout << file.getSourceCode() << endl;
    }

    return 0;
}