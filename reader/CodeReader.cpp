#include "CodeReader.h"
#include<fstream>
#include<stdexcept>

#include<algorithm>

string CodeReader::readFile(string inputfile){
  ifstream file(inputfile);  //file: object used to access file

  if(!file.is_open()) {
    throw runtime_error("Unable to open file:"+inputfile);
  }

  string code;
  string line;
  
  while(getline(file, line))
  {
    code+=line+"\n";
  }

  if(file.bad()){
    throw runtime_error("Failed while reading file"+inputfile);
  }
  return code;
  
}



vector<fs::path> CodeReader::getCppFiles(const string &filepath){
  vector<fs::path> cppFiles;  // create an empty vector name cppFiles . It stroe the path of all the cpp files int th folder

  if(!fs::exists(filepath)) { // check file path exists or not
    throw runtime_error("Error: folder does not exists"+filepath);
  }
  if(!fs::is_directory(filepath)) {  // check the path is a directory
    throw runtime_error("Error: Path is not in a directory"+filepath);
  }


  for(const auto  &entry : fs::directory_iterator(filepath)){ //Visit each entry in the folder one by one, without making unnecessary copies or modifying the entries
    if(entry.is_regular_file() && entry.path().extension()==".cpp"){
      cppFiles.push_back(entry.path());
    }
  }


  // Keep the results in a consistent order
  sort(cppFiles.begin(), cppFiles.end());


  return cppFiles;
}


