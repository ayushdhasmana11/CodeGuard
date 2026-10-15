#include <iostream>
#include "CodeReader.h"

using namespace std;

int main()
{
    CodeReader reader;

    try
    {
        vector<fs::path> files = reader.getCppFiles("data");

        cout << "Source files found: " << files.size() << '\n';

        for (const auto& file : files)
        {
            cout << file.filename().string() << '\n';
        }
    }
    catch (const exception& e)
    {
        cerr << "Error: " << e.what() << '\n';
    }

    return 0;
}