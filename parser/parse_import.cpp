/**
 * @file parse_import.cpp
 * @brief Import parsing and definition prescanning for the Bishop parser.
 *
 * Handles import statements, prescan for forward references,
 * doc comment collection, and name lookup utilities.
 */

#include "parser.hpp"

using namespace std;

namespace parser {

/**
 * Checks if the given name is an imported module alias.
 */
bool is_imported_module(const ParserState& state, const string& name) {
    for (const auto& m : state.imported_modules) {
        if (m == name) return true;
    }

    return false;
}

/**
 * Checks if the given name is a known function.
 */
bool is_function_name(const ParserState& state, const string& name) {
    for (const auto& f : state.function_names) {
        if (f == name) return true;
    }

    return false;
}

/**
 * Checks if the given name is a using alias.
 */
bool is_using_alias(const ParserState& state, const string& name) {
    for (const auto& alias : state.using_aliases) {
        if (alias.local_name == name) return true;
    }

    return false;
}

/**
 * Gets the using alias for a given name, or nullptr if not found.
 */
const UsingAlias* get_using_alias(const ParserState& state, const string& name) {
    for (const auto& alias : state.using_aliases) {
        if (alias.local_name == name) return &alias;
    }

    return nullptr;
}

/**
 * Pre-scans the token stream to collect function and struct names.
 * This allows forward references to functions defined later in the file.
 */
void prescan_definitions(ParserState& state) {
    size_t saved_pos = state.pos;

    while (!check(state, TokenType::EOF_TOKEN)) {
        // Function: fn name(...)
        if (check(state, TokenType::FN)) {
            advance(state);

            if (check(state, TokenType::IDENT)) {
                state.function_names.push_back(current(state).value);
            }
        }

        // Struct: Name :: struct or Error: Name :: err
        if (check(state, TokenType::IDENT)) {
            string name = current(state).value;
            advance(state);

            if (check(state, TokenType::DOUBLE_COLON)) {
                advance(state);

                // Both structs and errors can be instantiated with { field: value }
                if (check(state, TokenType::STRUCT) || check(state, TokenType::ERR)) {
                    state.struct_names.push_back(name);
                }
            }
        }

        advance(state);
    }

    state.pos = saved_pos;
}

/**
 * @bishop_syntax Doc Comments
 * @category Visibility
 * @order 2
 * @description Document functions, structs, and fields with /// comments.
 * @syntax /// description
 * @example
 * /// This is a doc comment for the function
 * fn add(int a, int b) -> int {
 *     return a + b;
 * }
 */
string collect_doc_comments(ParserState& state) {
    string doc;

    while (check(state, TokenType::DOC_COMMENT)) {
        if (!doc.empty()) {
            doc += "\n";
        }
        doc += current(state).value;
        advance(state);
    }

    return doc;
}

/**
 * @bishop_syntax import
 * @category Imports
 * @order 1
 * @description Import a module to use its types and functions.
 * @syntax import module_name;
 * @example
 * import http;
 * import myproject.utils;
 * @note Use dot notation for nested modules
 */
unique_ptr<ImportStmt> parse_import(ParserState& state) {
    Token import_tok = consume(state, TokenType::IMPORT);

    // Parse module path: math or utils.helpers
    string module_path = consume(state, TokenType::IDENT).value;

    while (check(state, TokenType::DOT)) {
        advance(state);
        module_path += "." + consume(state, TokenType::IDENT).value;
    }

    consume(state, TokenType::SEMICOLON);

    auto import = make_unique<ImportStmt>(module_path);
    import->line = import_tok.line;
    state.imported_modules.push_back(import->alias);
    return import;
}

/**
 * @bishop_syntax using
 * @category Imports
 * @order 2
 * @description Bring module members into the local namespace.
 * @syntax using module.member, module.member2; or using module.*;
 * @example
 * import log;
 * using log.info, log.debug;
 * info("message");  // no log. prefix needed
 * @example
 * import math;
 * using math.*;
 * x := sin(PI / 2.0);  // all math members available
 */
unique_ptr<UsingStmt> parse_using(ParserState& state) {
    Token using_tok = consume(state, TokenType::USING);

    auto stmt = make_unique<UsingStmt>();
    stmt->line = using_tok.line;

    // Parse first module.member or module.*
    string module_alias = consume(state, TokenType::IDENT).value;
    consume(state, TokenType::DOT);

    // Check for wildcard: module.*
    if (check(state, TokenType::STAR)) {
        advance(state);
        stmt->is_wildcard = true;
        stmt->wildcard_module = module_alias;
        state.has_wildcard_using = true;  // Mark that we have a wildcard import
        consume(state, TokenType::SEMICOLON);
        return stmt;
    }

    // Parse member name
    string member_name = consume(state, TokenType::IDENT).value;
    UsingMember first_member;
    first_member.module_alias = module_alias;
    first_member.member_name = member_name;
    stmt->members.push_back(first_member);

    // Record the alias (member_name becomes the local name)
    UsingAlias alias;
    alias.local_name = member_name;
    alias.module_alias = module_alias;
    alias.member_name = member_name;
    state.using_aliases.push_back(alias);

    // Parse additional members: , module.member
    while (check(state, TokenType::COMMA)) {
        advance(state);

        module_alias = consume(state, TokenType::IDENT).value;
        consume(state, TokenType::DOT);
        member_name = consume(state, TokenType::IDENT).value;

        UsingMember member;
        member.module_alias = module_alias;
        member.member_name = member_name;
        stmt->members.push_back(member);

        // Record the alias
        UsingAlias additional_alias;
        additional_alias.local_name = member_name;
        additional_alias.module_alias = module_alias;
        additional_alias.member_name = member_name;
        state.using_aliases.push_back(additional_alias);
    }

    consume(state, TokenType::SEMICOLON);
    return stmt;
}

} // namespace parser
