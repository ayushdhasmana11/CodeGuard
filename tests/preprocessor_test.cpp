#include <iostream>
#include "Preprocessor.h"

using namespace std;

int main()
{
    Preprocessor processor;

    string code =
        "int/**/count = 5; // comment\n"
        "string text = \"hello // world\";\n"
        "/* block comment */ int value = 10;\n";

    cout << "Original code:\n";
    cout << code << endl;

    cout << "Cleaned code:\n";
    cout << processor.cleanCode(code) << endl;

    return 0;
}