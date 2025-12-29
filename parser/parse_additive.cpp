/**
 * @file parse_additive.cpp
 * @brief Additive/multiplicative expression parsing for the Bishop parser.
 */

#include "parser.hpp"

using namespace std;

namespace parser {

/**
 * Parses a unary expression: -expr for negation.
 */
static unique_ptr<ASTNode> parse_unary(ParserState& state) {
    // Handle unary minus: -expr
    if (check(state, TokenType::MINUS)) {
        Token op_tok = current(state);
        advance(state);

        auto operand = parse_unary(state);  // Recursively handle nested unary

        // Create a binary expression: 0 - operand
        auto zero = make_unique<NumberLiteral>("0");
        zero->line = op_tok.line;

        auto binop = make_unique<BinaryExpr>();
        binop->op = "-";
        binop->line = op_tok.line;
        binop->left = move(zero);
        binop->right = move(operand);
        return binop;
    }

    auto expr = parse_primary(state);
    return parse_postfix(state, move(expr));
}

/**
 * Parses multiplicative expressions: *, /
 * Parses postfix (field access, method calls) on each operand.
 */
static unique_ptr<ASTNode> parse_multiplicative(ParserState& state) {
    auto left = parse_unary(state);

    while (check(state, TokenType::STAR) || check(state, TokenType::SLASH)) {
        Token op_tok = current(state);
        string op = op_tok.value;
        advance(state);

        auto right = parse_unary(state);

        auto binop = make_unique<BinaryExpr>();
        binop->op = op;
        binop->line = op_tok.line;
        binop->left = move(left);
        binop->right = move(right);
        left = move(binop);
    }

    return left;
}

/**
 * Parses additive expressions: +, -
 */
unique_ptr<ASTNode> parse_additive(ParserState& state) {
    auto left = parse_multiplicative(state);

    while (check(state, TokenType::PLUS) || check(state, TokenType::MINUS)) {
        Token op_tok = current(state);
        string op = op_tok.value;
        advance(state);
        auto right = parse_multiplicative(state);

        auto binop = make_unique<BinaryExpr>();
        binop->op = op;
        binop->line = op_tok.line;
        binop->left = move(left);
        binop->right = move(right);
        left = move(binop);
    }

    return left;
}

} // namespace parser
