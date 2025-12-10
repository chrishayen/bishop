#pragma once
#include <string>
#include <vector>
#include "token.hpp"

using namespace std;

class Lexer {
public:
    explicit Lexer(const string& source);
    vector<Token> tokenize();

private:
    string source;
    size_t pos = 0;
    int line = 1;

    char current();
    char peek();
    void advance();
    void skip_whitespace();
    Token read_string();
    Token read_identifier();
    Token read_number();
};
