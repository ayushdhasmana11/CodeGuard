#include "Tokenizer.h"
#include <cctype>
#include <unordered_set>
using namespace std;

bool Tokenizer::isKeyword(const string& word) const {
    static const unordered_set<string> keywords = {
        "alignas","alignof","asm","auto","bool","break","case","catch","char",
        "class","const","constexpr","continue","decltype","default","delete",
        "do","double","else","enum","explicit","extern","false","float","for",
        "friend","goto","if","inline","int","long","namespace","new","noexcept",
        "nullptr","operator","private","protected","public","register","return",
        "short","signed","sizeof","static","struct","switch","template","this",
        "throw","true","try","typedef","typename","union","unsigned","using",
        "virtual","void","volatile","while","const_cast","dynamic_cast",
        "reinterpret_cast","static_cast","export","mutable","thread_local",
        "wchar_t","char16_t","char32_t"
    };
    return keywords.find(word) != keywords.end();
}

vector<string> Tokenizer::tokenize(const string& source) const {
    vector<string> tokens;
    size_t i = 0;
    while (i < source.size()) {
        unsigned char ch = static_cast<unsigned char>(source[i]);
        if (isspace(ch)) { ++i; continue; }
        if (source[i] == '/' && i + 1 < source.size() && source[i+1] == '/') {
            i += 2;
            while (i < source.size() && source[i] != '\\n') ++i;
            continue;
        }
        if (source[i] == '/' && i + 1 < source.size() && source[i+1] == '*') {
            i += 2;
            while (i + 1 < source.size() &&
                   !(source[i] == '*' && source[i+1] == '/')) ++i;
            if (i + 1 < source.size()) i += 2;
            continue;
        }
        if (source[i] == '"' || source[i] == '\\'') {
            char quote = source[i++];
            while (i < source.size()) {
                if (source[i] == '\\\\' && i + 1 < source.size()) i += 2;
                else if (source[i] == quote) { ++i; break; }
                else ++i;
            }
            tokens.push_back(quote == '"' ? "STRING_LITERAL" : "CHAR_LITERAL");
            continue;
        }
        if (isalpha(ch) || source[i] == '_') {
            size_t start = i++;
            while (i < source.size()) {
                unsigned char next = static_cast<unsigned char>(source[i]);
                if (!isalnum(next) && source[i] != '_') break;
                ++i;
            }
            string word = source.substr(start, i-start);
            tokens.push_back(isKeyword(word) ? word : "IDENTIFIER");
            continue;
        }
        if (isdigit(ch)) {
            ++i;
            while (i < source.size()) {
                unsigned char next = static_cast<unsigned char>(source[i]);
                if (!isalnum(next) && source[i] != '.' && source[i] != '_') break;
                ++i;
            }
            tokens.push_back("NUMBER");
            continue;
        }
        static const vector<string> operators = {
            ">>=", "<<=", "->*", "...", "==", "!=", "<=", ">=", "++", "--",
            "&&", "||", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=",
            "<<", ">>", "->", "::", ".*"
        };
        bool matched = false;
        for (const string& op : operators) {
            if (source.compare(i, op.size(), op) == 0) {
                tokens.push_back(op);
                i += op.size();
                matched = true;
                break;
            }
        }
        if (matched) continue;
        tokens.emplace_back(1, source[i++]);
    }
    return tokens;
}

TokenNode* Tokenizer::tokenizeToLinkedList(const string& source) const {
    vector<string> words = tokenize(source);
    TokenNode* head = nullptr;
    for (const string& word : words) {
        if (token_list_append(&head, word.c_str()) == nullptr) {
            token_list_free(&head);
            return nullptr;
        }
    }
    return head;
}
