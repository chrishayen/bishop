#pragma once
#include <string>

enum class TokenType {
    FN,
    IDENT,
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    STRING,
    EOF_TOKEN
};

struct Token {
    TokenType type;
    std::string value;
};
