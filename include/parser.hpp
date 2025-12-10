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
    bool is_type_token();
    void advance();

    std::unique_ptr<FunctionDef> parse_function();
    std::unique_ptr<ASTNode> parse_statement();
    std::unique_ptr<FunctionCall> parse_function_call();
    std::unique_ptr<VariableDecl> parse_variable_decl();
    std::unique_ptr<VariableDecl> parse_inferred_decl();
    std::unique_ptr<ReturnStmt> parse_return();
    std::unique_ptr<ASTNode> parse_expression();
    std::unique_ptr<ASTNode> parse_primary();
    std::string token_to_type(TokenType type);
};
