/**
 * @file parse_struct.cpp
 * @brief Struct parsing for the Bishop parser.
 *
 * Handles struct definitions and struct literal expressions.
 */

#include "parser.hpp"

using namespace std;

namespace parser {

/**
 * Checks if the given name is a known struct type.
 * Also checks using aliases that point to structs.
 */
bool is_struct_type(const ParserState& state, const string& name) {
    for (const auto& s : state.struct_names) {
        if (s == name) return true;
    }

    // Check if name is a using alias (parser can't fully resolve types,
    // but we check if the name appears in using_aliases)
    for (const auto& alias : state.using_aliases) {
        if (alias.local_name == name) return true;
    }

    // If there's a wildcard import, we can't know all available names at parse time.
    // In this case, assume any PascalCase identifier followed by { could be a struct.
    // The typechecker will validate if it's actually a valid struct.
    if (state.has_wildcard_using && !name.empty() && isupper(name[0])) {
        return true;
    }

    return false;
}

/**
 * @bishop_syntax Struct Definition
 * @category Structs
 * @order 1
 * @description Define a custom data structure.
 * @syntax Name :: struct { field type, ... }
 * @example
 * Person :: struct {
 *     name str,
 *     age int
 * }
 */
unique_ptr<StructDef> parse_struct_def(ParserState& state, const string& name, Visibility vis) {
    Token struct_tok = consume(state, TokenType::STRUCT);
    consume(state, TokenType::LBRACE);

    auto def = make_unique<StructDef>();
    def->name = name;
    def->visibility = vis;
    def->line = struct_tok.line;
    state.struct_names.push_back(name);

    // Parse fields: name type, name type (with optional doc comments)
    while (!check(state, TokenType::RBRACE) && !check(state, TokenType::EOF_TOKEN)) {
        // Collect doc comment for this field
        string field_doc = collect_doc_comments(state);

        StructField field;
        field.name = consume(state, TokenType::IDENT).value;
        field.doc_comment = field_doc;

        if (is_type_token(state)) {
            field.type = token_to_type(current(state).type);
            advance(state);
        } else if (check(state, TokenType::IDENT)) {
            // Custom type (another struct)
            field.type = current(state).value;
            advance(state);
        }

        def->fields.push_back(field);

        if (check(state, TokenType::COMMA)) {
            advance(state);
        }
    }

    consume(state, TokenType::RBRACE);
    return def;
}

/**
 * @bishop_syntax Struct Instantiation
 * @category Structs
 * @order 2
 * @description Create an instance of a struct.
 * @syntax TypeName { field: value, field: value }
 * @example
 * p := Person { name: "Chris", age: 32 };
 * req := http.Request { method: "GET", path: "/", body: "" };
 */
unique_ptr<StructLiteral> parse_struct_literal(ParserState& state, const string& name) {
    Token lbrace = consume(state, TokenType::LBRACE);

    auto lit = make_unique<StructLiteral>();
    lit->struct_name = name;
    lit->line = lbrace.line;

    // Parse field values: field: value, field: value
    while (!check(state, TokenType::RBRACE) && !check(state, TokenType::EOF_TOKEN)) {
        string field_name = consume(state, TokenType::IDENT).value;
        consume(state, TokenType::COLON);
        auto value = parse_expression(state);

        lit->field_values.push_back({field_name, move(value)});

        if (check(state, TokenType::COMMA)) {
            advance(state);
        }
    }

    consume(state, TokenType::RBRACE);
    return lit;
}

} // namespace parser
