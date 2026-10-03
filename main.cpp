#include<iostream>
#include<filesystem>
#include<string>
#include<vector>

#include "reader/codereader.h"
#include "preprocessing/Preprocessor.h"

using namespace std;

int main()
{
    CodeReader reader;
    Preprocessor preprocessor;

    try
    {
        // Find all C++ files
        vector<fs::path> files=reader.getCppFiles("input");

        cout<<"C++ Files Found: "<<files.size() << endl;

        // Read and preprocess each file
        for(const auto &file : files)
        {
            string code = reader.readFile(file.string());
            string cleanCode = preprocessor.cleanCode(code);


            cout<<"\n----------------------------\n"<<endl;
            cout<<"File:"<<file.string()<<endl;
            cout<<"\n-------Original code-------\n"<<endl;
            cout<<code<<endl;
            cout<<"\n-------Cleaned code-------\n"<<endl;
            cout<<cleanCode<<endl;
        }
    }
    catch(const exception &e){
        cout<<"Error: "<<e.what()<<endl;
    }

    return 0;
}