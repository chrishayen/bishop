#include "lexer.hpp"

Lexer::Lexer(const std::string& source) : source(source) {}

char Lexer::current() {
    if (pos >= source.length()) return '\0';
    return source[pos];
}

char Lexer::peek() {
    if (pos + 1 >= source.length()) return '\0';
    return source[pos + 1];
}

void Lexer::advance() {
    pos++;
}

void Lexer::skip_whitespace() {
    while (current() == ' ' || current() == '\n' || current() == '\t' || current() == '\r') {
        advance();
    }
}

Token Lexer::read_string() {
    advance(); // skip opening quote
    std::string value;
    while (current() != '"' && current() != '\0') {
        value += current();
        advance();
    }
    advance(); // skip closing quote
    return {TokenType::STRING, value};
}

Token Lexer::read_identifier() {
    std::string value;
    while (std::isalnum(current()) || current() == '_') {
        value += current();
        advance();
    }

    if (value == "fn") {
        return {TokenType::FN, value};
    }
    return {TokenType::IDENT, value};
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (pos < source.length()) {
        skip_whitespace();

        if (current() == '\0') break;

        if (current() == '(') {
            tokens.push_back({TokenType::LPAREN, "("});
            advance();
        } else if (current() == ')') {
            tokens.push_back({TokenType::RPAREN, ")"});
            advance();
        } else if (current() == '{') {
            tokens.push_back({TokenType::LBRACE, "{"});
            advance();
        } else if (current() == '}') {
            tokens.push_back({TokenType::RBRACE, "}"});
            advance();
        } else if (current() == '"') {
            tokens.push_back(read_string());
        } else if (std::isalpha(current()) || current() == '_') {
            tokens.push_back(read_identifier());
        } else {
            advance(); // skip unknown chars for now
        }
    }

    tokens.push_back({TokenType::EOF_TOKEN, ""});
    return tokens;
}
