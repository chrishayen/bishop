/**
 * @file parse_variable.cpp
 * @brief Variable declaration parsing for the Bishop parser.
 */

#include "parser.hpp"

using namespace std;

namespace parser {

/**
 * @bishop_syntax Explicit Declaration
 * @category Variables
 * @order 1
 * @description Declare a variable with an explicit type.
 * @syntax type name = expr;
 * @example
 * int x = 42;
 * str name = "Chris";
 * bool flag = true;
 */
unique_ptr<VariableDecl> parse_variable_decl(ParserState& state, bool is_const) {
    int start_line = current(state).line;
    auto decl = make_unique<VariableDecl>();
    decl->line = start_line;
    decl->is_const = is_const;
    decl->type = token_to_type(current(state).type);
    advance(state);

    if (check(state, TokenType::OPTIONAL)) {
        decl->is_optional = true;
        advance(state);
    }

    decl->name = consume(state, TokenType::IDENT).value;
    consume(state, TokenType::ASSIGN);
    decl->value = parse_expression(state);
    consume(state, TokenType::SEMICOLON);
    return decl;
}

/**
 * @bishop_syntax Type Inference
 * @category Variables
 * @order 2
 * @description Declare a variable with inferred type using :=.
 * @syntax name := expr;
 * @example
 * x := 100;
 * name := "Hello";
 * pi := 3.14;
 */
unique_ptr<VariableDecl> parse_inferred_decl(ParserState& state, bool is_const) {
    auto decl = make_unique<VariableDecl>();
    decl->is_const = is_const;
    Token name_tok = consume(state, TokenType::IDENT);
    decl->name = name_tok.value;
    decl->line = name_tok.line;
    consume(state, TokenType::COLON_ASSIGN);
    decl->value = parse_expression(state);
    consume(state, TokenType::SEMICOLON);
    return decl;
}

/**
 * @bishop_syntax Const Declaration
 * @category Variables
 * @order 3
 * @description Declare a constant with an explicit type or inferred type.
 * @syntax const type name = expr; or const name := expr;
 * @example
 * const int MAX_SIZE = 100;
 * const PI := 3.14159;
 * const str NAME = "Bishop";
 */
unique_ptr<VariableDecl> parse_const_decl(ParserState& state) {
    int start_line = current(state).line;
    consume(state, TokenType::CONST);

    // Check if this is a typed const or inferred const
    if (is_type_token(state)) {
        return parse_variable_decl(state, true);
    }

    // Inferred type const: const NAME := "value";
    if (check(state, TokenType::IDENT)) {
        return parse_inferred_decl(state, true);
    }

    throw runtime_error("expected type or identifier after 'const' at line " + to_string(start_line));
}

} // namespace parser
