#pragma once
#include <string>

enum class TokenType {
    // Keywords
    FN,
    RETURN,
    TRUE,
    FALSE,

    // Types
    TYPE_INT,
    TYPE_STR,
    TYPE_BOOL,
    TYPE_CHAR,
    TYPE_F32,
    TYPE_F64,
    TYPE_U32,
    TYPE_U64,

    // Literals
    IDENT,
    STRING,
    NUMBER,
    FLOAT,

    // Punctuation
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    COMMA,
    COLON,
    ASSIGN,
    COLON_ASSIGN,
    ARROW,
    PLUS,
    MINUS,
    STAR,
    SLASH,

    EOF_TOKEN
};

struct Token {
    TokenType type;
    std::string value;
    int line = 1;
};
