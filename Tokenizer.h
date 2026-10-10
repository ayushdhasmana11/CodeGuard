#ifndef CODEGUARD_TOKENIZER_H
#define CODEGUARD_TOKENIZER_H
#include <string>
#include <vector>
#include "token_list.h"
using namespace std;
class Tokenizer {
public:
    vector<string> tokenize(const string& source) const;
    TokenNode* tokenizeToLinkedList(const string& source) const;
private:
    bool isKeyword(const string& word) const;
};
#endif
