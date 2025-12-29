/**
 * @file check_pair.cpp
 * @brief Pair type inference for the Bishop type checker.
 */

#include "typechecker.hpp"
#include "pairs.hpp"

using namespace std;

namespace typechecker {

/**
 * Infers the type of a pair creation expression.
 */
TypeInfo check_pair_create(TypeCheckerState& state, const PairCreate& pair) {
    // Check that both elements match the declared type
    TypeInfo first_type = infer_type(state, *pair.first);
    TypeInfo second_type = infer_type(state, *pair.second);
    TypeInfo expected_type = {pair.element_type, false, false};

    if (!types_compatible(expected_type, first_type)) {
        error(state, "Pair first element type mismatch: expected '" +
              pair.element_type + "', got '" + format_type(first_type) + "'", pair.line);
    }

    if (!types_compatible(expected_type, second_type)) {
        error(state, "Pair second element type mismatch: expected '" +
              pair.element_type + "', got '" + format_type(second_type) + "'", pair.line);
    }

    return {"Pair<" + pair.element_type + ">", false, false};
}

/**
 * Type checks a method call on a pair.
 */
TypeInfo check_pair_method(TypeCheckerState& state, const MethodCall& mcall, const string& element_type) {
    auto method_info = nog::get_pair_method_info(mcall.method_name);

    if (!method_info) {
        error(state, "Pair has no method '" + mcall.method_name + "'", mcall.line);
        return {"unknown", false, false};
    }

    const auto& [param_types, return_type] = *method_info;

    if (mcall.args.size() != param_types.size()) {
        error(state, "method '" + mcall.method_name + "' expects " +
              to_string(param_types.size()) + " arguments, got " +
              to_string(mcall.args.size()), mcall.line);
    }

    for (size_t i = 0; i < mcall.args.size() && i < param_types.size(); i++) {
        TypeInfo arg_type = infer_type(state, *mcall.args[i]);
        string expected = param_types[i] == "T" ? element_type : param_types[i];
        TypeInfo expected_type = {expected, false, false};

        if (!types_compatible(expected_type, arg_type)) {
            error(state, "argument " + to_string(i + 1) + " of method '" +
                  mcall.method_name + "' expects '" + expected +
                  "', got '" + format_type(arg_type) + "'", mcall.line);
        }
    }

    string ret = return_type == "T" ? element_type : return_type;

    if (ret == "void") {
        return {"void", false, true};
    }

    return {ret, false, false};
}

/**
 * Type checks a field access on a pair (first or second).
 */
TypeInfo check_pair_field(TypeCheckerState& state, const FieldAccess& access, const string& element_type) {
    if (access.field_name != "first" && access.field_name != "second") {
        error(state, "Pair has no field '" + access.field_name + "' (valid fields: first, second)", access.line);
        return {"unknown", false, false};
    }

    return {element_type, false, false};
}

} // namespace typechecker
