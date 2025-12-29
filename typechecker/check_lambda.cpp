/**
 * @file check_lambda.cpp
 * @brief Lambda expression type inference for the Bishop type checker.
 */

#include "typechecker.hpp"

using namespace std;

namespace typechecker {

/**
 * Infers the type of a lambda expression.
 * Returns the function type: fn(param_types) -> return_type
 */
TypeInfo check_lambda_expr(TypeCheckerState& state, const LambdaExpr& lambda) {
    // Build the function type string: fn(int, str) -> bool
    string fn_type = "fn(";
    bool first = true;

    for (const auto& param : lambda.params) {
        if (!first) {
            fn_type += ", ";
        }
        first = false;

        // Validate parameter type
        if (!is_valid_type(state, param.type)) {
            error(state, "unknown type '" + param.type + "' for parameter '" + param.name + "'", lambda.line);
        }

        fn_type += param.type;
    }

    fn_type += ")";

    // Add return type if present
    if (!lambda.return_type.empty()) {
        if (!is_valid_type(state, lambda.return_type)) {
            error(state, "unknown return type '" + lambda.return_type + "'", lambda.line);
        }

        fn_type += " -> " + lambda.return_type;
    }

    // Type check the body in a new scope with parameters
    push_scope(state);

    // Add parameters to the scope
    for (const auto& param : lambda.params) {
        TypeInfo param_type = {param.type, false, false};
        declare_local(state, param.name, param_type, lambda.line);
    }

    // Save and set the current return type for return statement checking
    TypeInfo saved_return = state.current_return;
    bool saved_fallible = state.current_function_is_fallible;

    if (lambda.return_type.empty()) {
        state.current_return = {"void", false, true};
    } else {
        state.current_return = {lambda.return_type, false, false};
    }

    state.current_function_is_fallible = false;

    // Check all statements in the body
    for (const auto& stmt : lambda.body) {
        check_statement(state, *stmt);
    }

    // Ensure non-void lambdas return on all code paths
    if (!lambda.return_type.empty() && lambda.return_type != "void") {
        if (!has_return(lambda.body)) {
            error(state,
                  "lambda with non-void return type '" + lambda.return_type +
                      "' may not return a value on all paths",
                  lambda.line);
        }
    }

    // Restore previous context
    state.current_return = saved_return;
    state.current_function_is_fallible = saved_fallible;
    pop_scope(state);

    return {fn_type, false, false};
}

/**
 * Type checks a lambda call (immediate invocation of an expression).
 * Returns the return type of the called function.
 */
TypeInfo check_lambda_call(TypeCheckerState& state, const LambdaCall& call) {
    // Get the type of the callee (should be a function type)
    TypeInfo callee_type = infer_type(state, *call.callee);

    // Parse the function type to extract parameter types and return type
    // Format: fn(type1, type2) -> return_type
    if (callee_type.base_type.substr(0, 3) != "fn(") {
        error(state, "cannot call non-function type '" + callee_type.base_type + "'", call.line);
        return {"void", false, true};
    }

    // Parse parameter types from function type
    size_t params_start = 3;  // After "fn("
    size_t params_end = callee_type.base_type.find(')');

    if (params_end == string::npos) {
        error(state, "invalid function type '" + callee_type.base_type + "'", call.line);
        return {"void", false, true};
    }

    string params_str = callee_type.base_type.substr(params_start, params_end - params_start);
    vector<string> param_types;

    // Parse comma-separated parameter types
    if (!params_str.empty()) {
        size_t pos = 0;

        while (pos < params_str.length()) {
            // Skip whitespace
            while (pos < params_str.length() && params_str[pos] == ' ') {
                pos++;
            }

            size_t comma_pos = params_str.find(',', pos);

            if (comma_pos == string::npos) {
                string type = params_str.substr(pos);
                // Trim trailing whitespace
                while (!type.empty() && type.back() == ' ') {
                    type.pop_back();
                }

                if (!type.empty()) {
                    param_types.push_back(type);
                }

                break;
            } else {
                string type = params_str.substr(pos, comma_pos - pos);
                // Trim whitespace
                while (!type.empty() && type.back() == ' ') {
                    type.pop_back();
                }

                if (!type.empty()) {
                    param_types.push_back(type);
                }

                pos = comma_pos + 1;
            }
        }
    }

    // Check argument count
    if (call.args.size() != param_types.size()) {
        error(state, "function expects " + to_string(param_types.size()) + " arguments, got " + to_string(call.args.size()), call.line);
        return {"void", false, true};
    }

    // Check argument types
    for (size_t i = 0; i < call.args.size(); i++) {
        TypeInfo arg_type = infer_type(state, *call.args[i]);

        if (arg_type.base_type != param_types[i]) {
            error(state, "argument " + to_string(i + 1) + " has type '" + arg_type.base_type + "', expected '" + param_types[i] + "'", call.line);
        }
    }

    // Extract return type (after " -> ")
    size_t arrow_pos = callee_type.base_type.find(" -> ", params_end);

    if (arrow_pos == string::npos) {
        return {"void", false, true};
    }

    string return_type = callee_type.base_type.substr(arrow_pos + 4);
    return {return_type, false, false};
}

} // namespace typechecker
