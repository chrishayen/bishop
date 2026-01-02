/**
 * @file check_set.cpp
 * @brief Set type inference for the Bishop type checker.
 */

#include "typechecker.hpp"
#include "sets.hpp"

using namespace std;

namespace typechecker {

/**
 * Infers the type of a set creation expression.
 */
TypeInfo check_set_create(TypeCheckerState& state, const SetCreate& set) {
    (void)state;
    return {"Set<" + set.element_type + ">", false, false};
}

/**
 * Infers the type of a set literal.
 */
TypeInfo check_set_literal(TypeCheckerState& state, const SetLiteral& set) {
    if (set.elements.empty()) {
        error(state, "cannot infer type of empty set literal, use Set<T>() instead", set.line);
        return {"unknown", false, false};
    }

    TypeInfo first_type = infer_type(state, *set.elements[0]);

    for (size_t i = 1; i < set.elements.size(); i++) {
        TypeInfo elem_type = infer_type(state, *set.elements[i]);

        if (elem_type.base_type != first_type.base_type) {
            error(state, "set literal has mixed types: '" + format_type(first_type) +
                  "' and '" + format_type(elem_type) + "'", set.line);
        }
    }

    return {"Set<" + first_type.base_type + ">", false, false};
}

/**
 * Type checks a method call on a set.
 */
TypeInfo check_set_method(TypeCheckerState& state, const MethodCall& mcall, const string& element_type) {
    auto method_info = bishop::get_set_method_info(mcall.method_name);

    if (!method_info) {
        error(state, "Set has no method '" + mcall.method_name + "'", mcall.line);
        return {"unknown", false, false};
    }

    const auto& [param_types, return_type] = *method_info;

    if (mcall.args.size() != param_types.size()) {
        error(state, "method '" + mcall.method_name + "' expects " +
              to_string(param_types.size()) + " arguments, got " +
              to_string(mcall.args.size()), mcall.line);
    }

    // Build the Set<T> type string for parameter checking
    string set_type = "Set<" + element_type + ">";

    for (size_t i = 0; i < mcall.args.size() && i < param_types.size(); i++) {
        TypeInfo arg_type = infer_type(state, *mcall.args[i]);
        string expected_str = param_types[i];

        // Substitute T with actual element type
        if (expected_str == "T") {
            expected_str = element_type;
        } else if (expected_str == "Set<T>") {
            expected_str = set_type;
        }

        TypeInfo expected = {expected_str, false, false};

        if (!types_compatible(expected, arg_type)) {
            error(state, "argument " + to_string(i + 1) + " of method '" +
                  mcall.method_name + "' expects '" + expected_str +
                  "', got '" + format_type(arg_type) + "'", mcall.line);
        }
    }

    string ret = return_type;

    // Substitute T with actual element type in return type
    if (ret == "T") {
        ret = element_type;
    } else if (ret == "Set<T>") {
        ret = set_type;
    }

    if (ret == "void") {
        return {"void", false, true};
    }

    return {ret, false, false};
}

} // namespace typechecker
