#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <algorithm>
#include "CodeFile.h"

using namespace std;
namespace fs = filesystem;

int main()
{
    string folderPath = "data";
    vector<string> filenames;

    // Find all C and C++ files in the data folder
    if (!fs::exists(folderPath) || !fs::is_directory(folderPath))
    {
        cout << "Error: Data folder not found!" << endl;
        return 1;
    }

    for (const auto& entry : fs::directory_iterator(folderPath))
    {
        if (!entry.is_regular_file())
            continue;

        string extension = entry.path().extension().string();

        if (extension == ".c" || extension == ".cpp")
        {
            filenames.push_back(entry.path().string());
        }
    }

    // Sort filenames for consistent output
    sort(filenames.begin(), filenames.end());

    if (filenames.empty())
    {
        cout << "No C or C++ source files found in data folder."
             << endl;
        return 0;
    }

    cout << "CodeGuard - Source Code Similarity Detection System"
         << endl;
    cout << "Files found: " << filenames.size() << endl;

    // Read and display each source file
    for (const string& filename : filenames)
    {
        cout << "\n==================================" << endl;
        cout << "File: " << filename << endl;
        cout << "==================================" << endl;

        CodeFile file(filename);

        if (file.readFile())
        {
            cout << file.getSourceCode() << endl;
        }
    }

    return 0;
}