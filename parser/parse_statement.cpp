/**
 * @file parse_statement.cpp
 * @brief Statement parsing dispatch for the Bishop parser.
 *
 * Main entry point for parsing statements. Individual statement types
 * are implemented in separate files.
 */

#include "parser.hpp"

using namespace std;

namespace parser {

/**
 * Parses an or handler after an expression.
 * Call only when check(state, TokenType::OR) is true.
 * Takes ownership of expr and returns an OrExpr.
 */
unique_ptr<OrExpr> parse_or_handler(ParserState& state, unique_ptr<ASTNode> expr) {
    int or_line = current(state).line;
    advance(state);  // consume 'or'

    auto or_expr = make_unique<OrExpr>();
    or_expr->line = or_line;
    or_expr->expr = move(expr);

    // Parse handler based on next token
    if (check(state, TokenType::RETURN)) {
        or_expr->handler = parse_or_return(state);
    } else if (check(state, TokenType::FAIL)) {
        or_expr->handler = parse_or_fail(state);
    } else if (check(state, TokenType::MATCH)) {
        or_expr->handler = parse_or_match(state);
    } else if (check(state, TokenType::LBRACE)) {
        or_expr->handler = parse_or_block(state);
    } else if (check(state, TokenType::CONTINUE)) {
        or_expr->handler = parse_or_continue(state);
    } else if (check(state, TokenType::BREAK)) {
        or_expr->handler = parse_or_break(state);
    } else {
        throw runtime_error("expected 'or' handler (return, fail, match, block, continue, or break) at line " + to_string(or_line));
    }

    return or_expr;
}

/**
 * Parses any statement. Dispatches based on the first token:
 * - return: parse return statement
 * - if: parse if/else
 * - while: parse while loop
 * - type keyword: parse typed variable declaration
 * - identifier: could be assignment, inferred decl, function call, or struct variable
 */
unique_ptr<ASTNode> parse_statement(ParserState& state) {
    // return statement
    if (check(state, TokenType::RETURN)) {
        return parse_return(state);
    }

    // fail statement
    if (check(state, TokenType::FAIL)) {
        return parse_fail(state);
    }

    // continue statement
    if (check(state, TokenType::CONTINUE)) {
        int start_line = current(state).line;
        advance(state);
        consume(state, TokenType::SEMICOLON);
        auto stmt = make_unique<ContinueStmt>();
        stmt->line = start_line;
        return stmt;
    }

    // break statement
    if (check(state, TokenType::BREAK)) {
        int start_line = current(state).line;
        advance(state);
        consume(state, TokenType::SEMICOLON);
        auto stmt = make_unique<BreakStmt>();
        stmt->line = start_line;
        return stmt;
    }

    // if statement
    if (check(state, TokenType::IF)) {
        return parse_if(state);
    }

    // while loop
    if (check(state, TokenType::WHILE)) {
        return parse_while(state);
    }

    // for loop
    if (check(state, TokenType::FOR)) {
        return parse_for(state);
    }

    // select statement
    if (check(state, TokenType::SELECT)) {
        return parse_select(state);
    }

    // with statement
    if (check(state, TokenType::WITH)) {
        return parse_with(state);
    }

    // go statement: go func();
    if (check(state, TokenType::GO)) {
        return parse_go_spawn(state);
    }

    // const declaration: const int x = 5 or const x := 5
    if (check(state, TokenType::CONST)) {
        return parse_const_decl(state);
    }

    // typed variable: int x = 5
    if (is_type_token(state)) {
        return parse_variable_decl(state);
    }

    // Function-type variable declaration: fn(int, int) -> int op = ...;
    if (check(state, TokenType::FN) && peek(state).type == TokenType::LPAREN) {
        int start_line = current(state).line;

        // Parse the function type
        string fn_type = parse_type(state);

        // Now we should have an identifier for the variable name
        if (check(state, TokenType::IDENT)) {
            auto decl = make_unique<VariableDecl>();
            decl->type = fn_type;
            decl->line = start_line;
            decl->name = consume(state, TokenType::IDENT).value;
            consume(state, TokenType::ASSIGN);
            decl->value = parse_expression(state);
            consume(state, TokenType::SEMICOLON);
            return decl;
        }

        // Function type was parsed but no variable name found
        throw runtime_error("expected variable name after function type '" + fn_type + "' at line " + to_string(start_line));
    }

    // List<T> variable declaration: List<int> nums = [1, 2, 3]; or List<Pair<int>> x = ...;
    if (check(state, TokenType::LIST)) {
        int start_line = current(state).line;
        advance(state);
        consume(state, TokenType::LT);

        // Use parse_type to support nested generics like List<Pair<int>>
        string element_type = parse_type(state);

        consume(state, TokenType::GT);

        auto decl = make_unique<VariableDecl>();
        decl->type = "List<" + element_type + ">";
        decl->line = start_line;
        decl->name = consume(state, TokenType::IDENT).value;
        consume(state, TokenType::ASSIGN);
        decl->value = parse_expression(state);
        consume(state, TokenType::SEMICOLON);
        return decl;
    }

    // Pair<T> variable declaration: Pair<int> p = Pair<int>(1, 2); or Pair<List<int>> x = ...;
    if (check(state, TokenType::PAIR)) {
        int start_line = current(state).line;
        advance(state);
        consume(state, TokenType::LT);

        // Use parse_type to support nested generics like Pair<List<int>>
        string element_type = parse_type(state);

        consume(state, TokenType::GT);

        auto decl = make_unique<VariableDecl>();
        decl->type = "Pair<" + element_type + ">";
        decl->line = start_line;
        decl->name = consume(state, TokenType::IDENT).value;
        consume(state, TokenType::ASSIGN);
        decl->value = parse_expression(state);
        consume(state, TokenType::SEMICOLON);
        return decl;
    }

    // Tuple<T> variable declaration: Tuple<int> t = Tuple<int>(1, 2, 3); or Tuple<List<int>> x = ...;
    if (check(state, TokenType::TUPLE)) {
        int start_line = current(state).line;
        advance(state);
        consume(state, TokenType::LT);

        // Use parse_type to support nested generics like Tuple<List<int>>
        string element_type = parse_type(state);

        consume(state, TokenType::GT);

        auto decl = make_unique<VariableDecl>();
        decl->type = "Tuple<" + element_type + ">";
        decl->line = start_line;
        decl->name = consume(state, TokenType::IDENT).value;
        consume(state, TokenType::ASSIGN);
        decl->value = parse_expression(state);
        consume(state, TokenType::SEMICOLON);
        return decl;
    }

    // Channel<T> variable declaration: Channel<int> ch = Channel<int>(); or Channel<List<int>> x = ...;
    if (check(state, TokenType::CHANNEL)) {
        int start_line = current(state).line;
        advance(state);
        consume(state, TokenType::LT);

        // Use parse_type to support nested generics like Channel<List<int>>
        string element_type = parse_type(state);

        consume(state, TokenType::GT);

        auto decl = make_unique<VariableDecl>();
        decl->type = "Channel<" + element_type + ">";
        decl->line = start_line;
        decl->name = consume(state, TokenType::IDENT).value;
        consume(state, TokenType::ASSIGN);
        decl->value = parse_expression(state);
        consume(state, TokenType::SEMICOLON);
        return decl;
    }

    // Deque<T> variable declaration: Deque<int> dq = Deque<int>(); or Deque<List<int>> x = ...;
    if (check(state, TokenType::DEQUE)) {
        int start_line = current(state).line;
        advance(state);
        consume(state, TokenType::LT);

        // Use parse_type to support nested generics like Deque<List<int>>
        string element_type = parse_type(state);

        consume(state, TokenType::GT);

        auto decl = make_unique<VariableDecl>();
        decl->type = "Deque<" + element_type + ">";
        decl->line = start_line;
        decl->name = consume(state, TokenType::IDENT).value;
        consume(state, TokenType::ASSIGN);
        decl->value = parse_expression(state);
        consume(state, TokenType::SEMICOLON);
        return decl;
    }

    // Stack<T> variable declaration: Stack<int> s = Stack<int>(); or Stack<List<int>> x = ...;
    if (check(state, TokenType::STACK)) {
        int start_line = current(state).line;
        advance(state);
        consume(state, TokenType::LT);

        // Use parse_type to support nested generics like Stack<List<int>>
        string element_type = parse_type(state);

        consume(state, TokenType::GT);

        auto decl = make_unique<VariableDecl>();
        decl->type = "Stack<" + element_type + ">";
        decl->line = start_line;
        decl->name = consume(state, TokenType::IDENT).value;
        consume(state, TokenType::ASSIGN);
        decl->value = parse_expression(state);
        consume(state, TokenType::SEMICOLON);
        return decl;
    }

    // Queue<T> variable declaration: Queue<int> q = Queue<int>(); or Queue<List<int>> x = ...;
    if (check(state, TokenType::QUEUE)) {
        int start_line = current(state).line;
        advance(state);
        consume(state, TokenType::LT);

        // Use parse_type to support nested generics like Queue<List<int>>
        string element_type = parse_type(state);

        consume(state, TokenType::GT);

        auto decl = make_unique<VariableDecl>();
        decl->type = "Queue<" + element_type + ">";
        decl->line = start_line;
        decl->name = consume(state, TokenType::IDENT).value;
        consume(state, TokenType::ASSIGN);
        decl->value = parse_expression(state);
        consume(state, TokenType::SEMICOLON);
        return decl;
    }

    // Map<K, V> variable declaration: Map<str, int> ages = {"alice": 30}; or Map<str, List<int>> x = ...;
    if (check(state, TokenType::MAP)) {
        int start_line = current(state).line;
        advance(state);
        consume(state, TokenType::LT);

        // Parse key type (supports nested generics)
        string key_type = parse_type(state);
        consume(state, TokenType::COMMA);

        // Parse value type (supports nested generics)
        string value_type = parse_type(state);

        consume(state, TokenType::GT);

        auto decl = make_unique<VariableDecl>();
        decl->type = "Map<" + key_type + ", " + value_type + ">";
        decl->line = start_line;
        decl->name = consume(state, TokenType::IDENT).value;
        consume(state, TokenType::ASSIGN);
        decl->value = parse_expression(state);
        consume(state, TokenType::SEMICOLON);
        return decl;
    }

    // Set<T> variable declaration: Set<int> nums = {1, 2, 3}; or Set<str> names = Set<str>();
    if (check(state, TokenType::SET)) {
        int start_line = current(state).line;
        advance(state);
        consume(state, TokenType::LT);

        // Use parse_type to support nested generics like Set<List<int>>
        string element_type = parse_type(state);

        consume(state, TokenType::GT);

        auto decl = make_unique<VariableDecl>();
        decl->type = "Set<" + element_type + ">";
        decl->line = start_line;
        decl->name = consume(state, TokenType::IDENT).value;
        consume(state, TokenType::ASSIGN);
        decl->value = parse_expression(state);
        consume(state, TokenType::SEMICOLON);
        return decl;
    }

    // NOT expression statement: !expr or fail "msg"; !valid or continue; etc.
    if (check(state, TokenType::NOT)) {
        auto expr = parse_expression(state);
        consume(state, TokenType::SEMICOLON);
        return expr;
    }

    // inferred variable, assignment, function call, method call, field assignment, or struct-typed variable
    if (check(state, TokenType::IDENT)) {
        Token ident_tok = current(state);
        size_t saved_pos = state.pos;
        string ident = ident_tok.value;
        advance(state);

        // Expression statement: x or fail "msg"; x >= 3 or fail "msg"; etc.
        // If the next token is 'or' or a comparison/arithmetic operator, parse as expression
        if (check(state, TokenType::OR) ||
            check(state, TokenType::EQ) || check(state, TokenType::NE) ||
            check(state, TokenType::LT) || check(state, TokenType::GT) ||
            check(state, TokenType::LE) || check(state, TokenType::GE) ||
            check(state, TokenType::PLUS) || check(state, TokenType::MINUS) ||
            check(state, TokenType::STAR) || check(state, TokenType::SLASH)) {
            state.pos = saved_pos;
            auto expr = parse_expression(state);
            consume(state, TokenType::SEMICOLON);
            return expr;
        }

        if (check(state, TokenType::COLON_ASSIGN)) {
            state.pos = saved_pos;
            return parse_inferred_decl(state);
        }

        // struct-typed variable: Person p = ... or Person? p = ...
        if (is_struct_type(state, ident) && (check(state, TokenType::IDENT) || check(state, TokenType::OPTIONAL))) {
            auto decl = make_unique<VariableDecl>();
            decl->type = ident;
            decl->line = ident_tok.line;

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

        // Check for qualified reference: module.func() or module.Type var = ...
        if (check(state, TokenType::DOT) && is_imported_module(state, ident)) {
            advance(state);
            Token member_tok = consume(state, TokenType::IDENT);
            string member_name = member_tok.value;
            string qualified_name = ident + "." + member_name;

            // Qualified function call: module.func() or module.func() or handler
            // Or complex expressions like: module.func() >= 3 or fail "msg";
            if (check(state, TokenType::LPAREN)) {
                auto call = make_unique<FunctionCall>();
                call->name = qualified_name;
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

                // Check for 'or' handler (standalone or expression)
                if (check(state, TokenType::OR)) {
                    auto or_expr = parse_or_handler(state, move(call));
                    consume(state, TokenType::SEMICOLON);
                    return or_expr;
                }

                // If next token is a comparison/arithmetic operator, this is part of a larger expression
                // Fall back to parse_expression to handle cases like: module.func() >= 3 or fail "msg";
                if (check(state, TokenType::EQ) || check(state, TokenType::NE) ||
                    check(state, TokenType::LT) || check(state, TokenType::GT) ||
                    check(state, TokenType::LE) || check(state, TokenType::GE) ||
                    check(state, TokenType::PLUS) || check(state, TokenType::MINUS) ||
                    check(state, TokenType::STAR) || check(state, TokenType::SLASH)) {
                    state.pos = saved_pos;
                    auto expr = parse_expression(state);
                    consume(state, TokenType::SEMICOLON);
                    return expr;
                }

                consume(state, TokenType::SEMICOLON);
                return call;
            }

            // Qualified type declaration: module.Type var = ...
            if (check(state, TokenType::IDENT) || check(state, TokenType::OPTIONAL)) {
                auto decl = make_unique<VariableDecl>();
                decl->type = qualified_name;
                decl->line = ident_tok.line;

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

            // Fallback
            state.pos = saved_pos;
            return parse_function_call(state);
        }

        // field access: obj.field = value or obj.method()
        if (check(state, TokenType::DOT)) {
            advance(state);
            Token member_tok = consume(state, TokenType::IDENT);
            string field_name = member_tok.value;

            // Method call as statement: obj.method(args); or obj.method(args) or handler;
            // Or complex expressions like: obj.method() >= 3 or fail "msg";
            if (check(state, TokenType::LPAREN)) {
                auto call = make_unique<MethodCall>();
                auto obj = make_unique<VariableRef>(ident);
                obj->line = ident_tok.line;
                call->object = move(obj);
                call->method_name = field_name;
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

                // Check for 'or' handler (standalone or expression)
                if (check(state, TokenType::OR)) {
                    auto or_expr = parse_or_handler(state, move(call));
                    consume(state, TokenType::SEMICOLON);
                    return or_expr;
                }

                // If next token is a comparison/arithmetic operator, this is part of a larger expression
                // Fall back to parse_expression to handle cases like: obj.method() >= 3 or fail "msg";
                if (check(state, TokenType::EQ) || check(state, TokenType::NE) ||
                    check(state, TokenType::LT) || check(state, TokenType::GT) ||
                    check(state, TokenType::LE) || check(state, TokenType::GE) ||
                    check(state, TokenType::PLUS) || check(state, TokenType::MINUS) ||
                    check(state, TokenType::STAR) || check(state, TokenType::SLASH)) {
                    state.pos = saved_pos;
                    auto expr = parse_expression(state);
                    consume(state, TokenType::SEMICOLON);
                    return expr;
                }

                consume(state, TokenType::SEMICOLON);
                return call;
            }

            // Field assignment: obj.field = value;
            if (check(state, TokenType::ASSIGN)) {
                consume(state, TokenType::ASSIGN);
                auto assign = make_unique<FieldAssignment>();
                auto obj = make_unique<VariableRef>(ident);
                obj->line = ident_tok.line;
                assign->object = move(obj);
                assign->field_name = field_name;
                assign->line = member_tok.line;
                assign->value = parse_expression(state);
                consume(state, TokenType::SEMICOLON);
                return assign;
            }
        }

        // assignment: x = expr
        if (check(state, TokenType::ASSIGN)) {
            state.pos = saved_pos;
            auto assign = make_unique<Assignment>();
            assign->name = consume(state, TokenType::IDENT).value;
            assign->line = ident_tok.line;
            consume(state, TokenType::ASSIGN);
            assign->value = parse_expression(state);
            consume(state, TokenType::SEMICOLON);
            return assign;
        }

        state.pos = saved_pos;
        return parse_function_call(state);
    }

    advance(state);
    return nullptr;
}

/**
 * Parses a function call statement: name(args); or name(args) or handler;
 * Includes the trailing semicolon.
 */
unique_ptr<ASTNode> parse_function_call(ParserState& state) {
    Token name = consume(state, TokenType::IDENT);
    consume(state, TokenType::LPAREN);

    auto call = make_unique<FunctionCall>();
    call->name = name.value;
    call->line = name.line;

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

    // Check for 'or' handler (standalone or expression)
    if (check(state, TokenType::OR)) {
        auto or_expr = parse_or_handler(state, move(call));
        consume(state, TokenType::SEMICOLON);
        return or_expr;
    }

    consume(state, TokenType::SEMICOLON);
    return call;
}

/**
 * @bishop_syntax go spawn
 * @category Concurrency
 * @order 1
 * @description Spawn a goroutine to run a function call concurrently.
 * @syntax go func();
 * @example
 * go worker(ch);
 * go process_data();
 */
unique_ptr<GoSpawn> parse_go_spawn(ParserState& state) {
    int start_line = current(state).line;
    consume(state, TokenType::GO);

    auto spawn = make_unique<GoSpawn>();
    spawn->line = start_line;
    spawn->call = parse_expression(state);
    consume(state, TokenType::SEMICOLON);
    return spawn;
}

/**
 * @bishop_syntax with
 * @category Resources
 * @order 1
 * @description Resource management block. Calls close() on the resource when exiting.
 * @syntax with expr as name { body }
 * @example
 * with fs.open("file.txt") as file {
 *     content := file.read();
 * }  // file.close() called automatically
 */
unique_ptr<WithStmt> parse_with(ParserState& state) {
    int start_line = current(state).line;
    consume(state, TokenType::WITH);

    auto stmt = make_unique<WithStmt>();
    stmt->line = start_line;

    // Parse the resource expression
    stmt->resource = parse_expression(state);

    // Expect 'as'
    consume(state, TokenType::AS);

    // Get the binding name
    stmt->binding_name = consume(state, TokenType::IDENT).value;

    // Parse the body block
    consume(state, TokenType::LBRACE);

    while (!check(state, TokenType::RBRACE) && !check(state, TokenType::EOF_TOKEN)) {
        auto body_stmt = parse_statement(state);

        if (body_stmt) {
            stmt->body.push_back(move(body_stmt));
        }
    }

    consume(state, TokenType::RBRACE);

    return stmt;
}

} // namespace parser
