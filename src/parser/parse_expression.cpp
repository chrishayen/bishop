/**
 * @file parse_expression.cpp
 * @brief Expression parsing for the Nog parser.
 *
 * Handles all expression types using precedence climbing:
 * comparisons, arithmetic, primary expressions, and postfix operations.
 */

#include "parser.hpp"

using namespace std;

namespace parser {

/**
 * Entry point for expression parsing. Delegates to comparison (lowest precedence).
 */
unique_ptr<ASTNode> parse_expression(ParserState& state) {
    return parse_comparison(state);
}

/**
 * Parses comparison expressions: handles "is none" and comparison operators (==, !=, <, >, <=, >=).
 * Also chains multiple comparisons.
 */
unique_ptr<ASTNode> parse_comparison(ParserState& state) {
    auto left = parse_additive(state);

    // Handle "x is none"
    if (check(state, TokenType::IS)) {
        advance(state);
        consume(state, TokenType::NONE);
        auto is_none = make_unique<IsNone>();
        is_none->value = move(left);
        return is_none;
    }

    while (check(state, TokenType::EQ) || check(state, TokenType::NE) ||
           check(state, TokenType::LT) || check(state, TokenType::GT) ||
           check(state, TokenType::LE) || check(state, TokenType::GE)) {
        string op = current(state).value;
        advance(state);
        auto right = parse_additive(state);

        auto binop = make_unique<BinaryExpr>();
        binop->op = op;
        binop->left = move(left);
        binop->right = move(right);
        left = move(binop);
    }

    return left;
}

/**
 * Parses additive/multiplicative expressions: +, -, *, /
 * Parses postfix (field access, method calls) on each operand.
 */
unique_ptr<ASTNode> parse_additive(ParserState& state) {
    auto left = parse_primary(state);
    left = parse_postfix(state, move(left));

    while (check(state, TokenType::PLUS) || check(state, TokenType::MINUS) ||
           check(state, TokenType::STAR) || check(state, TokenType::SLASH)) {
        string op = current(state).value;
        advance(state);
        auto right = parse_primary(state);
        right = parse_postfix(state, move(right));

        auto binop = make_unique<BinaryExpr>();
        binop->op = op;
        binop->left = move(left);
        binop->right = move(right);
        left = move(binop);
    }

    return left;
}

/**
 * @nog_syntax await
 * @category Async
 * @order 3
 * @description Await an async operation.
 * @syntax await expr
 * @example
 * result := await fetch_data();
 * await ch.send(42);
 * val := await ch.recv();
 */

/**
 * @nog_syntax Channel Creation
 * @category Channels
 * @order 1
 * @description Create a typed channel for communication between async tasks.
 * @syntax Channel<type>()
 * @example
 * ch := Channel<int>();
 * ch_str := Channel<str>();
 */
unique_ptr<ASTNode> parse_primary(ParserState& state) {
    // Handle NOT expression: !expr
    if (check(state, TokenType::NOT)) {
        int start_line = current(state).line;
        advance(state);
        auto not_expr = make_unique<NotExpr>();
        not_expr->value = parse_primary(state);
        not_expr->line = start_line;
        return not_expr;
    }

    // Handle await expression: await expr
    if (check(state, TokenType::AWAIT)) {
        advance(state);
        auto await_expr = make_unique<AwaitExpr>();
        await_expr->value = parse_primary(state);
        await_expr->value = parse_postfix(state, move(await_expr->value));
        return await_expr;
    }

    // Handle channel creation: Channel<int>()
    if (check(state, TokenType::CHANNEL)) {
        int start_line = current(state).line;
        advance(state);
        consume(state, TokenType::LT);

        string element_type;

        if (is_type_token(state)) {
            element_type = token_to_type(current(state).type);
            advance(state);
        } else if (check(state, TokenType::IDENT)) {
            element_type = current(state).value;
            advance(state);
        }

        consume(state, TokenType::GT);
        consume(state, TokenType::LPAREN);
        consume(state, TokenType::RPAREN);

        auto channel = make_unique<ChannelCreate>();
        channel->element_type = element_type;
        channel->line = start_line;
        return channel;
    }

    if (check(state, TokenType::NUMBER)) {
        Token tok = current(state);
        advance(state);
        return make_unique<NumberLiteral>(tok.value);
    }

    if (check(state, TokenType::FLOAT)) {
        Token tok = current(state);
        advance(state);
        return make_unique<FloatLiteral>(tok.value);
    }

    if (check(state, TokenType::STRING)) {
        Token tok = current(state);
        advance(state);
        return make_unique<StringLiteral>(tok.value);
    }

    if (check(state, TokenType::TRUE)) {
        advance(state);
        return make_unique<BoolLiteral>(true);
    }

    if (check(state, TokenType::FALSE)) {
        advance(state);
        return make_unique<BoolLiteral>(false);
    }

    if (check(state, TokenType::NONE)) {
        advance(state);
        return make_unique<NoneLiteral>();
    }

    if (check(state, TokenType::IDENT)) {
        Token tok = current(state);
        advance(state);

        // Check for qualified reference: module.item (e.g., math.add)
        if (check(state, TokenType::DOT) && is_imported_module(state, tok.value)) {
            advance(state);
            Token item_tok = consume(state, TokenType::IDENT);
            string item_name = item_tok.value;

            // Check if it's a qualified function call: module.func(args)
            if (check(state, TokenType::LPAREN)) {
                auto call = make_unique<FunctionCall>();
                call->name = tok.value + "." + item_name;  // Store as "module.func"
                call->line = tok.line;
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
                return call;
            }

            // Check if it's a qualified struct literal: module.Type { ... }
            if (check(state, TokenType::LBRACE)) {
                return parse_struct_literal(state, tok.value + "." + item_name);
            }

            // Check if it's a qualified function reference: module.func (without parens)
            // This is for passing module functions as arguments
            auto fref = make_unique<FunctionRef>(tok.value + "." + item_name);
            fref->line = tok.line;
            return fref;
        }

        // Check if it's a struct literal: TypeName { field: value, ... }
        if (check(state, TokenType::LBRACE) && is_struct_type(state, tok.value)) {
            return parse_struct_literal(state, tok.value);
        }

        // Check if it's a function call
        if (check(state, TokenType::LPAREN)) {
            auto call = make_unique<FunctionCall>();
            call->name = tok.value;
            call->line = tok.line;
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
            return call;
        }

        // Check if it's a function reference (function name without parentheses)
        if (is_function_name(state, tok.value)) {
            auto ref = make_unique<FunctionRef>(tok.value);
            ref->line = tok.line;
            return ref;
        }

        return make_unique<VariableRef>(tok.value);
    }

    advance(state);
    return nullptr;
}

/**
 * Parses postfix operations: field access (obj.field) and method calls (obj.method()).
 * Chains multiple accesses like a.b.c() or a.b.c.d.
 */
unique_ptr<ASTNode> parse_postfix(ParserState& state, unique_ptr<ASTNode> left) {
    while (check(state, TokenType::DOT)) {
        advance(state);
        Token member_tok = consume(state, TokenType::IDENT);
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
            left = move(access);
        }
    }

    return left;
}

} // namespace parser
