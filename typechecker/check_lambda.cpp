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
                      "' must return a value on all paths",
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

    // Use the shared helper to parse parameter types from the function type
    auto param_types_opt = parse_function_type_params(callee_type.base_type);

    if (!param_types_opt) {
        error(state, "cannot call non-function type '" + callee_type.base_type + "'", call.line);
        return {"void", false, true};
    }

    vector<string> param_types = *param_types_opt;

    // Check argument count
    if (call.args.size() != param_types.size()) {
        error(state, "function expects " + to_string(param_types.size()) + " arguments, got " + to_string(call.args.size()), call.line);
        return {"void", false, true};
    }

    // Check argument types using types_compatible for proper nullable/fallible handling
    for (size_t i = 0; i < call.args.size(); i++) {
        TypeInfo arg_type = infer_type(state, *call.args[i]);
        TypeInfo expected_type{param_types[i], false, false};

        if (!types_compatible(expected_type, arg_type)) {
            error(state, "argument " + to_string(i + 1) + " has type '" + format_type(arg_type) + "', expected '" + param_types[i] + "'", call.line);
        }
    }

    // Extract return type (after " -> ")
    size_t params_end = callee_type.base_type.find(')');
    size_t arrow_pos = callee_type.base_type.find(" -> ", params_end);

    if (arrow_pos == string::npos) {
        return {"void", false, true};
    }

    string return_type = callee_type.base_type.substr(arrow_pos + 4);
    return {return_type, false, false};
}

} // namespace typechecker
