#include "parser.hpp"
#include <stdexcept>

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

Token Parser::current() {
    if (pos >= tokens.size()) {
        return {TokenType::EOF_TOKEN, "", 0};
    }
    return tokens[pos];
}

bool Parser::check(TokenType type) {
    return current().type == type;
}

void Parser::advance() {
    pos++;
}

Token Parser::consume(TokenType type) {
    if (!check(type)) {
        throw std::runtime_error("Unexpected token");
    }
    Token tok = current();
    advance();
    return tok;
}

bool Parser::is_type_token() {
    TokenType t = current().type;
    return t == TokenType::TYPE_INT || t == TokenType::TYPE_STR ||
           t == TokenType::TYPE_BOOL || t == TokenType::TYPE_CHAR ||
           t == TokenType::TYPE_F32 || t == TokenType::TYPE_F64 ||
           t == TokenType::TYPE_U32 || t == TokenType::TYPE_U64;
}

std::string Parser::token_to_type(TokenType type) {
    switch (type) {
        case TokenType::TYPE_INT: return "int";
        case TokenType::TYPE_STR: return "str";
        case TokenType::TYPE_BOOL: return "bool";
        case TokenType::TYPE_CHAR: return "char";
        case TokenType::TYPE_F32: return "f32";
        case TokenType::TYPE_F64: return "f64";
        case TokenType::TYPE_U32: return "u32";
        case TokenType::TYPE_U64: return "u64";
        default: return "";
    }
}

std::unique_ptr<Program> Parser::parse() {
    auto program = std::make_unique<Program>();

    while (!check(TokenType::EOF_TOKEN)) {
        if (check(TokenType::FN)) {
            program->functions.push_back(parse_function());
        } else {
            advance();
        }
    }

    return program;
}

std::unique_ptr<FunctionDef> Parser::parse_function() {
    consume(TokenType::FN);
    Token name = consume(TokenType::IDENT);
    consume(TokenType::LPAREN);

    auto func = std::make_unique<FunctionDef>();
    func->name = name.value;

    // Parse parameters: fn foo(int a, int b)
    while (!check(TokenType::RPAREN) && !check(TokenType::EOF_TOKEN)) {
        if (is_type_token()) {
            FunctionParam param;
            param.type = token_to_type(current().type);
            advance();
            param.name = consume(TokenType::IDENT).value;
            func->params.push_back(param);

            if (check(TokenType::COMMA)) {
                advance();
            }
        } else {
            advance();
        }
    }

    consume(TokenType::RPAREN);

    // Parse return type: -> int
    if (check(TokenType::ARROW)) {
        advance();
        if (is_type_token()) {
            func->return_type = token_to_type(current().type);
            advance();
        }
    }

    consume(TokenType::LBRACE);

    while (!check(TokenType::RBRACE) && !check(TokenType::EOF_TOKEN)) {
        auto stmt = parse_statement();
        if (stmt) {
            func->body.push_back(std::move(stmt));
        }
    }

    consume(TokenType::RBRACE);
    return func;
}

std::unique_ptr<ASTNode> Parser::parse_statement() {
    // return statement
    if (check(TokenType::RETURN)) {
        return parse_return();
    }

    // typed variable: int x = 5
    if (is_type_token()) {
        return parse_variable_decl();
    }

    // inferred variable or function call: x := 5 or foo()
    if (check(TokenType::IDENT)) {
        // Look ahead to see if it's := (inferred decl) or ( (function call)
        size_t saved_pos = pos;
        advance();

        if (check(TokenType::COLON_ASSIGN)) {
            pos = saved_pos;
            return parse_inferred_decl();
        }

        pos = saved_pos;
        return parse_function_call();
    }

    advance();
    return nullptr;
}

std::unique_ptr<VariableDecl> Parser::parse_variable_decl() {
    auto decl = std::make_unique<VariableDecl>();
    decl->type = token_to_type(current().type);
    advance();
    decl->name = consume(TokenType::IDENT).value;
    consume(TokenType::ASSIGN);
    decl->value = parse_expression();
    return decl;
}

std::unique_ptr<VariableDecl> Parser::parse_inferred_decl() {
    auto decl = std::make_unique<VariableDecl>();
    decl->name = consume(TokenType::IDENT).value;
    consume(TokenType::COLON_ASSIGN);
    decl->value = parse_expression();
    return decl;
}

std::unique_ptr<ReturnStmt> Parser::parse_return() {
    consume(TokenType::RETURN);
    auto ret = std::make_unique<ReturnStmt>();
    ret->value = parse_expression();
    return ret;
}

std::unique_ptr<ASTNode> Parser::parse_expression() {
    auto left = parse_primary();

    // Handle binary operators
    while (check(TokenType::PLUS) || check(TokenType::MINUS) ||
           check(TokenType::STAR) || check(TokenType::SLASH)) {
        std::string op = current().value;
        advance();
        auto right = parse_primary();

        auto binop = std::make_unique<BinaryExpr>();
        binop->op = op;
        binop->left = std::move(left);
        binop->right = std::move(right);
        left = std::move(binop);
    }

    return left;
}

std::unique_ptr<ASTNode> Parser::parse_primary() {
    if (check(TokenType::NUMBER)) {
        Token tok = current();
        advance();
        return std::make_unique<NumberLiteral>(tok.value);
    }

    if (check(TokenType::FLOAT)) {
        Token tok = current();
        advance();
        return std::make_unique<FloatLiteral>(tok.value);
    }

    if (check(TokenType::STRING)) {
        Token tok = current();
        advance();
        return std::make_unique<StringLiteral>(tok.value);
    }

    if (check(TokenType::TRUE)) {
        advance();
        return std::make_unique<BoolLiteral>(true);
    }

    if (check(TokenType::FALSE)) {
        advance();
        return std::make_unique<BoolLiteral>(false);
    }

    if (check(TokenType::IDENT)) {
        Token tok = current();
        advance();
        // Check if it's a function call
        if (check(TokenType::LPAREN)) {
            auto call = std::make_unique<FunctionCall>();
            call->name = tok.value;
            call->line = tok.line;
            consume(TokenType::LPAREN);
            while (!check(TokenType::RPAREN) && !check(TokenType::EOF_TOKEN)) {
                auto arg = parse_expression();
                if (arg) {
                    call->args.push_back(std::move(arg));
                }
                if (check(TokenType::COMMA)) {
                    advance();
                }
            }
            consume(TokenType::RPAREN);
            return call;
        }
        return std::make_unique<VariableRef>(tok.value);
    }

    advance();
    return nullptr;
}

std::unique_ptr<FunctionCall> Parser::parse_function_call() {
    Token name = consume(TokenType::IDENT);
    consume(TokenType::LPAREN);

    auto call = std::make_unique<FunctionCall>();
    call->name = name.value;
    call->line = name.line;

    while (!check(TokenType::RPAREN) && !check(TokenType::EOF_TOKEN)) {
        auto arg = parse_expression();
        if (arg) {
            call->args.push_back(std::move(arg));
        }
        if (check(TokenType::COMMA)) {
            advance();
        }
    }

    consume(TokenType::RPAREN);
    return call;
}
