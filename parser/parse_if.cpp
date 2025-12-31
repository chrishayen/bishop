/**
 * @file parse_if.cpp
 * @brief If statement parsing for the Bishop parser.
 */

#include "parser.hpp"

using namespace std;

namespace parser {

/**
 * @bishop_syntax if
 * @category Control Flow
 * @order 1
 * @description Conditional branching with if and optional else.
 * @syntax if condition { ... } else { ... } or if condition statement; else statement;
 * @example
 * if x > 10 {
 *     print("big");
 * } else {
 *     print("small");
 * }
 * @example
 * if x < 0 fail NotFound;
 * else return x;
 */
unique_ptr<IfStmt> parse_if(ParserState& state) {
    Token if_tok = consume(state, TokenType::IF);
    auto stmt = make_unique<IfStmt>();
    stmt->line = if_tok.line;
    stmt->condition = parse_expression(state);

    // Check for braced vs braceless syntax
    if (check(state, TokenType::LBRACE)) {
        // Braced block parsing
        consume(state, TokenType::LBRACE);

        while (!check(state, TokenType::RBRACE) && !check(state, TokenType::EOF_TOKEN)) {
            auto s = parse_statement(state);

            if (s) {
                stmt->then_body.push_back(move(s));
            }
        }

        consume(state, TokenType::RBRACE);
    } else {
        // Braceless: parse single statement
        auto s = parse_statement(state);

        if (s) {
            stmt->then_body.push_back(move(s));
        }
    }

    // Handle else branch
    if (check(state, TokenType::ELSE)) {
        advance(state);

        if (check(state, TokenType::LBRACE)) {
            // Braced else block
            consume(state, TokenType::LBRACE);

            while (!check(state, TokenType::RBRACE) && !check(state, TokenType::EOF_TOKEN)) {
                auto s = parse_statement(state);

                if (s) {
                    stmt->else_body.push_back(move(s));
                }
            }

            consume(state, TokenType::RBRACE);
        } else if (check(state, TokenType::IF)) {
            // else if chain - parse as nested if in else block
            auto nested_if = parse_if(state);
            stmt->else_body.push_back(move(nested_if));
        } else {
            // Braceless: parse single statement
            auto s = parse_statement(state);

            if (s) {
                stmt->else_body.push_back(move(s));
            }
        }
    }

    return stmt;
}

} // namespace parser
