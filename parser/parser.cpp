/**
 * @file parser.cpp
 * @brief Main parser entry point and token navigation.
 *
 * Contains the main parse() function and token navigation helpers.
 * Grammar parsing is split across separate files by concern.
 */

#include "parser.hpp"
#include <stdexcept>

using namespace std;

namespace parser {

/**
 * Returns the current token, or EOF_TOKEN if past end.
 */
Token current(const ParserState& state) {
    if (state.pos >= state.tokens.size()) {
        return {TokenType::EOF_TOKEN, "", 0};
    }

    return state.tokens[state.pos];
}

/**
 * Returns the next token (lookahead), or EOF_TOKEN if past end.
 */
Token peek(const ParserState& state) {
    if (state.pos + 1 >= state.tokens.size()) {
        return {TokenType::EOF_TOKEN, "", 0};
    }

    return state.tokens[state.pos + 1];
}

/**
 * Checks if current token matches the given type.
 */
bool check(const ParserState& state, TokenType type) {
    return current(state).type == type;
}

/**
 * Advances to the next token.
 */
void advance(ParserState& state) {
    state.pos++;
}

/**
 * Consumes and returns the current token if it matches type.
 * Throws runtime_error if the token doesn't match.
 */
Token consume(ParserState& state, TokenType type) {
    if (!check(state, type)) {
        Token tok = current(state);
        string msg = "unexpected token";

        if (!tok.value.empty()) {
            msg += " '" + tok.value + "'";
        }

        msg += " at line " + to_string(tok.line);
        throw runtime_error(msg);
    }

    Token tok = current(state);
    advance(state);
    return tok;
}

/**
 * Main parsing entry point. Parses the complete token stream into a Program AST.
 * Parses imports first, then function definitions (fn), struct definitions (Name :: struct),
 * method definitions (Name :: method_name), and module-level const declarations.
 */
unique_ptr<Program> parse(ParserState& state) {
    auto program = make_unique<Program>();

    // Pre-scan to collect all function and struct names for forward references
    prescan_definitions(state);

    // Skip any doc comments at the start of file
    while (check(state, TokenType::DOC_COMMENT)) {
        advance(state);
    }

    // Parse imports first (must be at top of file)
    while (check(state, TokenType::IMPORT)) {
        program->imports.push_back(parse_import(state));
    }

    // Parse using statements (must come after imports)
    while (check(state, TokenType::USING)) {
        program->usings.push_back(parse_using(state));
    }

    while (!check(state, TokenType::EOF_TOKEN)) {
        // Collect any doc comments before the definition
        string doc = collect_doc_comments(state);

        // Check for @extern("lib") annotation
        if (check(state, TokenType::AT)) {
            size_t at_pos = state.pos;
            advance(state);

            if (check(state, TokenType::EXTERN)) {
                advance(state);
                consume(state, TokenType::LPAREN);
                string library = consume(state, TokenType::STRING).value;
                consume(state, TokenType::RPAREN);

                auto ext = parse_extern_function(state, library);
                ext->doc_comment = doc;
                program->externs.push_back(move(ext));
                continue;
            }

            // Not @extern, restore position for visibility parsing
            state.pos = at_pos;
        }

        // Check for @static annotation (must come before visibility)
        bool is_static = false;
        if (check(state, TokenType::AT)) {
            size_t at_pos = state.pos;
            advance(state);

            if (check(state, TokenType::STATIC)) {
                advance(state);
                is_static = true;
            } else {
                // Not @static, restore position for visibility parsing
                state.pos = at_pos;
            }
        }

        // Check for visibility annotation
        Visibility vis = parse_visibility(state);

        if (check(state, TokenType::FN)) {
            auto fn = parse_function(state, vis);
            fn->doc_comment = doc;
            program->functions.push_back(move(fn));
            continue;
        }

        // Module-level const declaration
        if (check(state, TokenType::CONST)) {
            auto decl = parse_const_decl(state);
            program->constants.push_back(move(decl));
            continue;
        }

        if (!check(state, TokenType::IDENT)) {
            advance(state);
            continue;
        }

        // Check for struct definition: Name :: struct { ... }
        // or method definition: Name :: method_name(...) -> type { ... }
        size_t saved_pos = state.pos;
        Token name_tok = current(state);
        string name = name_tok.value;
        advance(state);

        if (!check(state, TokenType::DOUBLE_COLON)) {
            state.pos = saved_pos;
            advance(state);
            continue;
        }

        advance(state);

        if (check(state, TokenType::STRUCT)) {
            auto s = parse_struct_def(state, name, vis);
            s->doc_comment = doc;
            program->structs.push_back(move(s));
            continue;
        }

        if (check(state, TokenType::ERR)) {
            auto e = parse_error_def(state, name, vis);
            e->doc_comment = doc;
            program->errors.push_back(move(e));
            continue;
        }

        if (check(state, TokenType::IDENT)) {
            auto m = parse_method_def(state, name, vis, is_static);
            m->doc_comment = doc;
            program->methods.push_back(move(m));
            continue;
        }

        state.pos = saved_pos;
        advance(state);
    }

    return program;
}

} // namespace parser
