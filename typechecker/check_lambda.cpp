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

    // Restore previous context
    state.current_return = saved_return;
    state.current_function_is_fallible = saved_fallible;
    pop_scope(state);

    return {fn_type, false, false};
}

} // namespace typechecker
