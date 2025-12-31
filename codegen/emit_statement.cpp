/**
 * @file emit_statement.cpp
 * @brief Statement dispatch for the Bishop code generator.
 *
 * Main entry point for generating C++ code from statement AST nodes.
 * Individual statement types are implemented in separate files.
 */

#include "codegen.hpp"
#include <fmt/format.h>
#include <fmt/ranges.h>

using namespace std;

namespace codegen {

static int or_stmt_counter = 0;

/**
 * Generates C++ code for a statement. Handles print(), assert_eq() (in test mode),
 * if/while statements, method calls, field assignments, and other statements.
 */
string generate_statement(CodeGenState& state, const ASTNode& node) {
    if (auto* call = dynamic_cast<const FunctionCall*>(&node)) {
        if (call->name == "print") {
            vector<string> args;

            for (const auto& arg : call->args) {
                args.push_back(emit(state, *arg));
            }

            return print_multi(args) + ";";
        }

        // Handle assertion functions in test mode
        if (state.test_mode) {
            if (call->name == "assert_eq" && call->args.size() >= 2) {
                return assert_eq(emit(state, *call->args[0]), emit(state, *call->args[1]), call->line) + ";";
            }

            if (call->name == "assert_ne" && call->args.size() >= 2) {
                return assert_ne(emit(state, *call->args[0]), emit(state, *call->args[1]), call->line) + ";";
            }

            if (call->name == "assert_true" && call->args.size() >= 1) {
                return assert_true(emit(state, *call->args[0]), call->line) + ";";
            }

            if (call->name == "assert_false" && call->args.size() >= 1) {
                return assert_false(emit(state, *call->args[0]), call->line) + ";";
            }

            if (call->name == "assert_gt" && call->args.size() >= 2) {
                return assert_gt(emit(state, *call->args[0]), emit(state, *call->args[1]), call->line) + ";";
            }

            if (call->name == "assert_gte" && call->args.size() >= 2) {
                return assert_gte(emit(state, *call->args[0]), emit(state, *call->args[1]), call->line) + ";";
            }

            if (call->name == "assert_lt" && call->args.size() >= 2) {
                return assert_lt(emit(state, *call->args[0]), emit(state, *call->args[1]), call->line) + ";";
            }

            if (call->name == "assert_lte" && call->args.size() >= 2) {
                return assert_lte(emit(state, *call->args[0]), emit(state, *call->args[1]), call->line) + ";";
            }

            if (call->name == "assert_contains" && call->args.size() >= 2) {
                return assert_contains(emit(state, *call->args[0]), emit(state, *call->args[1]), call->line) + ";";
            }

            if (call->name == "assert_starts_with" && call->args.size() >= 2) {
                return assert_starts_with(emit(state, *call->args[0]), emit(state, *call->args[1]), call->line) + ";";
            }

            if (call->name == "assert_ends_with" && call->args.size() >= 2) {
                return assert_ends_with(emit(state, *call->args[0]), emit(state, *call->args[1]), call->line) + ";";
            }

            if (call->name == "assert_near" && call->args.size() >= 3) {
                return assert_near(emit(state, *call->args[0]), emit(state, *call->args[1]), emit(state, *call->args[2]), call->line) + ";";
            }
        }

        if (call->name == "sleep" && call->args.size() == 1) {
            return "bishop::rt::sleep(" + emit(state, *call->args[0]) + ");";
        }

        // Check if this is an extern function call
        auto ext_it = state.extern_functions.find(call->name);

        if (ext_it != state.extern_functions.end()) {
            // Extern function - convert string args to const char* with .c_str()
            const ExternFunctionDef* ext = ext_it->second;
            vector<string> args;

            for (size_t i = 0; i < call->args.size(); i++) {
                string arg_code = emit(state, *call->args[i]);

                // If param type is cstr, convert std::string to const char*
                if (i < ext->params.size() && ext->params[i].type == "cstr") {
                    // Wrap the argument with .c_str() call
                    // Need to handle string literals specially
                    if (auto* str_lit = dynamic_cast<const StringLiteral*>(call->args[i].get())) {
                        // String literal - just emit the raw string for C function
                        (void)str_lit;
                        args.push_back(arg_code + ".c_str()");
                    } else {
                        // Variable or expression - needs .c_str()
                        args.push_back("(" + arg_code + ").c_str()");
                    }
                } else {
                    args.push_back(arg_code);
                }
            }

            return function_call(call->name, args) + ";";
        }

        return emit_function_call(state, *call) + ";";
    }

    if (auto* stmt = dynamic_cast<const IfStmt*>(&node)) {
        vector<string> then_body;

        for (const auto& s : stmt->then_body) {
            then_body.push_back(generate_statement(state, *s));
        }

        vector<string> else_body;

        for (const auto& s : stmt->else_body) {
            else_body.push_back(generate_statement(state, *s));
        }

        return if_stmt(emit(state, *stmt->condition), then_body, else_body);
    }

    if (auto* stmt = dynamic_cast<const WhileStmt*>(&node)) {
        vector<string> body;

        for (const auto& s : stmt->body) {
            body.push_back(generate_statement(state, *s));
        }

        return while_stmt(emit(state, *stmt->condition), body);
    }

    if (auto* stmt = dynamic_cast<const ForStmt*>(&node)) {
        vector<string> body;

        for (const auto& s : stmt->body) {
            body.push_back(generate_statement(state, *s));
        }

        if (stmt->kind == ForLoopKind::Range) {
            return for_range_stmt(
                stmt->loop_var,
                emit(state, *stmt->range_start),
                emit(state, *stmt->range_end),
                body
            );
        } else {
            return for_each_stmt(
                stmt->loop_var,
                emit(state, *stmt->iterable),
                body
            );
        }
    }

    if (auto* select_stmt = dynamic_cast<const SelectStmt*>(&node)) {
        return generate_select(state, *select_stmt);
    }

    if (auto* go_spawn = dynamic_cast<const GoSpawn*>(&node)) {
        return emit_go_spawn(state, *go_spawn) + ";";
    }

    if (auto* with_stmt = dynamic_cast<const WithStmt*>(&node)) {
        return generate_with(state, *with_stmt);
    }

    if (auto* call = dynamic_cast<const MethodCall*>(&node)) {
        return emit(state, node) + ";";
    }

    if (auto* fa = dynamic_cast<const FieldAssignment*>(&node)) {
        return emit(state, node) + ";";
    }

    if (auto* fail = dynamic_cast<const FailStmt*>(&node)) {
        return emit_fail(state, *fail) + ";";
    }

    if (auto* cont = dynamic_cast<const ContinueStmt*>(&node)) {
        return "continue;";
    }

    if (auto* brk = dynamic_cast<const BreakStmt*>(&node)) {
        return "break;";
    }

    // Handle OrExpr as a statement (e.g., "b or fail msg;" or "x or continue;")
    if (auto* or_expr = dynamic_cast<const OrExpr*>(&node)) {
        string temp = "_or_stmt_tmp" + to_string(++or_stmt_counter);
        string preamble = fmt::format("auto {} = {};", temp, emit(state, *or_expr->expr));
        string condition = fmt::format("bishop::is_or_falsy({})", temp);

        string handler_code;
        if (auto* ret = dynamic_cast<const OrReturn*>(or_expr->handler.get())) {
            if (ret->value) {
                handler_code = "return " + emit(state, *ret->value) + ";";
            } else {
                handler_code = state.in_fallible_function ? "return {};" : "return;";
            }
        } else if (auto* fail = dynamic_cast<const OrFail*>(or_expr->handler.get())) {
            // or fail "message" generates error return
            // Check if it's a string literal
            if (auto* str_lit = dynamic_cast<const StringLiteral*>(fail->error_expr.get())) {
                handler_code = fmt::format("return std::make_shared<bishop::rt::Error>({});",
                                           string_literal(str_lit->value));
            } else if (auto* var = dynamic_cast<const VariableRef*>(fail->error_expr.get())) {
                if (var->name == "err") {
                    // 'or fail err' - need to extract error from Result type
                    handler_code = fmt::format("auto err = {}.error(); return err;", temp);
                } else {
                    handler_code = "return " + emit(state, *fail->error_expr) + ";";
                }
            } else if (auto* struct_lit = dynamic_cast<const StructLiteral*>(fail->error_expr.get())) {
                // Bare error type: or fail ErrorType (StructLiteral with empty field_values)
                if (struct_lit->field_values.empty()) {
                    handler_code = fmt::format(
                        "return std::static_pointer_cast<bishop::rt::Error>(std::make_shared<{}>(\"{}\"));",
                        struct_lit->struct_name,
                        struct_lit->struct_name);
                } else {
                    handler_code = "return " + emit(state, *fail->error_expr) + ";";
                }
            } else {
                // For other expressions
                handler_code = "return " + emit(state, *fail->error_expr) + ";";
            }
        } else if (dynamic_cast<const OrContinue*>(or_expr->handler.get())) {
            handler_code = "continue;";
        } else if (dynamic_cast<const OrBreak*>(or_expr->handler.get())) {
            handler_code = "break;";
        } else if (auto* block = dynamic_cast<const OrBlock*>(or_expr->handler.get())) {
            // Bind err using or_error() for block handlers
            string block_code = "";
            for (const auto& stmt : block->body) {
                block_code += generate_statement(state, *stmt) + " ";
            }
            handler_code = fmt::format("auto err = bishop::or_error({}); {}", temp, block_code);
        } else if (auto* match = dynamic_cast<const OrMatch*>(or_expr->handler.get())) {
            // or match err { ... } - generate match arms that all must fail or return
            // Standalone or match doesn't assign to a variable, so all arms must fail/return
            string match_code = "";
            bool first = true;
            for (const auto& arm : match->arms) {
                if (arm.error_type == "_") {
                    // Default arm
                    if (!first) {
                        match_code += " else { ";
                    } else {
                        match_code += "{ ";
                    }
                    if (auto* fail_stmt = dynamic_cast<const FailStmt*>(arm.body.get())) {
                        match_code += emit_fail(state, *fail_stmt) + "; ";
                    } else {
                        match_code += "return " + emit(state, *arm.body) + "; ";
                    }
                    match_code += "}";
                } else {
                    // Typed arm
                    string check = first ? "if" : " else if";
                    match_code += fmt::format("{} (dynamic_cast<{}*>(err.get())) {{ ", check, arm.error_type);
                    if (auto* fail_stmt = dynamic_cast<const FailStmt*>(arm.body.get())) {
                        match_code += emit_fail(state, *fail_stmt) + "; ";
                    } else {
                        match_code += "return " + emit(state, *arm.body) + "; ";
                    }
                    match_code += "}";
                }
                first = false;
            }
            handler_code = fmt::format("auto err = {}.error(); {}", temp, match_code);
        }

        return preamble + "\n\tif (" + condition + ") { " + handler_code + " }";
    }

    return emit(state, node);
}

} // namespace codegen
