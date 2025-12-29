/**
 * @file parse_postfix.cpp
 * @brief Postfix expression parsing for the Bishop parser.
 */

#include "parser.hpp"

using namespace std;

namespace parser {

/**
 * Checks if the current token can be used as a member name.
 * Accepts identifiers and type keywords (int, str, bool, float, etc.)
 * to support module functions like random.int() or random.float().
 */
static bool is_member_name_token(const ParserState& state) {
    TokenType t = current(state).type;
    return t == TokenType::IDENT ||
           t == TokenType::TYPE_INT ||
           t == TokenType::TYPE_STR ||
           t == TokenType::TYPE_BOOL ||
           t == TokenType::TYPE_F32 ||
           t == TokenType::TYPE_F64 ||
           t == TokenType::TYPE_U32 ||
           t == TokenType::TYPE_U64;
}

/**
 * Consumes a member name token (identifier or type keyword).
 * Throws if the current token is not a valid member name.
 */
static Token consume_member_name(ParserState& state) {
    if (!is_member_name_token(state)) {
        Token tok = current(state);
        string msg = "expected identifier or type name";

        if (!tok.value.empty()) {
            msg += ", got '" + tok.value + "'";
        }

        msg += " at line " + to_string(tok.line);
        throw runtime_error(msg);
    }

    Token tok = current(state);
    advance(state);
    return tok;
}

/**
 * Parses postfix operations: field access (obj.field) and method calls (obj.method()).
 * Chains multiple accesses like a.b.c() or a.b.c.d.
 */
unique_ptr<ASTNode> parse_postfix(ParserState& state, unique_ptr<ASTNode> left) {
    while (check(state, TokenType::DOT)) {
        advance(state);
        Token member_tok = consume_member_name(state);
        string member_name = member_tok.value;

        // Check if it's a method call: obj.method(args)
        if (check(state, TokenType::LPAREN)) {
            auto call = make_unique<MethodCall>();
            call->object = move(left);
            call->method_name = member_name;
            call->line = member_tok.line;

            consume(state, TokenType::LPAREN);

            while (!check(state, TokenType::RPAREN) && !check(state, TokenType::EOF_TOKEN)) {
                auto arg = parse_expression(state);

                if (arg) {
                    call->args.push_back(move(arg));
                }

                if (check(state, TokenType::COMMA)) {
                    advance(state);
                }
            }

            consume(state, TokenType::RPAREN);
            left = move(call);
        } else {
            // Field access: obj.field
            auto access = make_unique<FieldAccess>();
            access->object = move(left);
            access->field_name = member_name;
            access->line = member_tok.line;
            left = move(access);
        }
    }

    return left;
}

} // namespace parser
