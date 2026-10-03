#ifndef CODEREADER_H
#define CODEREADER_H

#include<string>
#include<vector>
#include<filesystem>

using namespace std;

namespace fs = std::filesystem;

class CodeReader{
  public: string readFile(string fileName); // It read one file and return its content as a string


  // find c++ files in folder
  vector<fs::path> getCppFiles(const string& folderpath);
};

#endif
