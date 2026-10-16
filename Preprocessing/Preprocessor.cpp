#include "Preprocessor.h"
#include <cctype>
using namespace std;

#include <sstream>

string removeBlankLines(const string& code) {
    string result;
    string line;
    istringstream stream(code);

    while (getline(stream, line)) {
        if (line.find_first_not_of(" \t\r") != string::npos) {
            result += line + '\n';
        }
    }

    return result;
}

std::string Preprocessor::cleanCode(const std::string &code){
    std::string result;
    result.reserve(code.size());

    bool inSingleLinecomment = false;
    bool inMultiLineComment = false;
    bool inString = false;
    bool inChar = false;
    bool previousWasSpace = false;


    for(std::size_t i=0; i<code.size(); ++i){
        char ch = code[i];

        if(inSingleLinecomment){
            if(ch=='\n'){
                inSingleLinecomment = false;
                if(!result.empty() && result.back()=='\n'){
                    previousWasSpace=false;
                    continue;
                }
                result+='\n';
                previousWasSpace = false;
            }
            continue;
        }
        else if(inMultiLineComment){
            if(ch=='*' && i+1<code.size() && code[i+1]=='/'){
                inMultiLineComment = false;
                ++i;
                result+=' ';
                previousWasSpace = true;
            }
            else if(ch=='\n'){
                result+='\n';
                previousWasSpace = false;
            }
            continue;
        }

        else if(inString){
            result+=ch;

            if(ch=='\\' && i+1<code.size()){
                result+=code[++i];
            }

            else if(ch=='"'){
                inString = false;
            }
            continue;
        }
        else if(inChar){
            result+=ch;

            if(ch=='\\' && i+1<code.size()){
                result+=code[++i];
            }

            else if(ch=='\''){
                inChar = false;
            }
            continue;
        }

        


        if(ch=='"'){
            inString = true;
            result+=ch;
            previousWasSpace=false;
            continue;
        }

        if(ch=='\''){
            inChar = true;
            result+=ch;
            previousWasSpace=false;
            continue;
        }

        if(ch=='/' && i+1<code.size() && code[i+1]=='/'){
            inSingleLinecomment=true;
            previousWasSpace=false;
            ++i;
            continue;
        }

        if(ch=='/' && i+1<code.size() && code[i+1]=='*'){
            inMultiLineComment=true;
            ++i;
            continue;
        }


        if(std::isspace(ch)){
            if(ch=='\n'){
                result += '\n';
                previousWasSpace = false;
            }
            else if(!previousWasSpace){
                result += ' ';
                previousWasSpace = true;
            }
            continue;
        }

        result+=ch;
        previousWasSpace=false;

    }
return removeBlankLines(result);
}