/**
 * @file parse_fail.cpp
 * @brief Fail statement parsing for the Bishop parser.
 *
 * Handles fail statements for error propagation.
 */

#include "parser.hpp"

using namespace std;

namespace parser {

/**
 * Parse a fail expression without consuming semicolon.
 * Used in match arms where semicolons aren't expected.
 */
unique_ptr<FailStmt> parse_fail_expr(ParserState& state) {
    Token fail_tok = consume(state, TokenType::FAIL);
    auto stmt = make_unique<FailStmt>();
    stmt->line = fail_tok.line;

    // Special case: 'fail err' where err refers to the current error
    if (check(state, TokenType::ERR)) {
        advance(state);
        auto err_ref = make_unique<VariableRef>("err");
        err_ref->line = fail_tok.line;
        stmt->value = move(err_ref);
        return stmt;
    }

    // Check for bare error type: fail ErrorType (no semicolon in expr context)
    if (check(state, TokenType::IDENT)) {
        string name = current(state).value;

        if (is_struct_type(state, name) && !check_ahead(state, 1, TokenType::LBRACE)) {
            advance(state);  // consume the identifier
            auto lit = make_unique<StructLiteral>();
            lit->struct_name = name;
            lit->line = fail_tok.line;
            stmt->value = move(lit);
            return stmt;
        }
    }

    stmt->value = parse_comparison(state);
    return stmt;
}

/**
 * @bishop_syntax Fail Statement
 * @category Error Handling
 * @order 2
 * @description Return an error from a fallible function.
 * @syntax fail "message"; or fail ErrorType { fields }; or fail ErrorType;
 * @example
 * fail "file not found";
 * fail IOError { code: 404, path: path };
 * fail NotFound;
 */
unique_ptr<FailStmt> parse_fail(ParserState& state) {
    Token fail_tok = consume(state, TokenType::FAIL);
    auto stmt = make_unique<FailStmt>();
    stmt->line = fail_tok.line;

    // Check for bare error type: fail ErrorType;
    // An identifier that's a known struct/error type, not followed by '{'
    if (check(state, TokenType::IDENT)) {
        string name = current(state).value;

        if (is_struct_type(state, name) && !check_ahead(state, 1, TokenType::LBRACE)) {
            advance(state);  // consume the identifier
            auto lit = make_unique<StructLiteral>();
            lit->struct_name = name;
            lit->line = fail_tok.line;
            stmt->value = move(lit);
            consume(state, TokenType::SEMICOLON);
            return stmt;
        }
    }

    // Fall through to existing behavior
    stmt->value = parse_expression(state);
    consume(state, TokenType::SEMICOLON);
    return stmt;
}

} // namespace parser
