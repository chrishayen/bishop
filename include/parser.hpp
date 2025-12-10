#pragma once
#include <vector>
#include <memory>
#include "token.hpp"
#include "ast.hpp"

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens);
    std::unique_ptr<Program> parse();

private:
    std::vector<Token> tokens;
    size_t pos = 0;

    Token current();
    Token consume(TokenType type);
    bool check(TokenType type);
    void advance();

    std::unique_ptr<FunctionDef> parse_function();
    std::unique_ptr<ASTNode> parse_statement();
    std::unique_ptr<FunctionCall> parse_function_call();
};
