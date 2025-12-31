/**
 * @file check_tuple.cpp
 * @brief Tuple type inference for the Bishop type checker.
 */

#include "typechecker.hpp"
#include "tuples.hpp"

using namespace std;

namespace typechecker {

/**
 * Infers the type of a tuple creation expression.
 */
TypeInfo check_tuple_create(TypeCheckerState& state, const TupleCreate& tuple) {
    // Validate element count (2-5)
    if (tuple.elements.size() < 2) {
        error(state, "Tuple must have at least 2 elements, got " +
              to_string(tuple.elements.size()), tuple.line);
    }

    if (tuple.elements.size() > 5) {
        error(state, "Tuple can have at most 5 elements, got " +
              to_string(tuple.elements.size()), tuple.line);
    }

    // Check that all elements match the declared type
    TypeInfo expected_type = {tuple.element_type, false, false};

    for (size_t i = 0; i < tuple.elements.size(); i++) {
        TypeInfo elem_type = infer_type(state, *tuple.elements[i]);

        if (!types_compatible(expected_type, elem_type)) {
            error(state, "Tuple element " + to_string(i) + " type mismatch: expected '" +
                  tuple.element_type + "', got '" + format_type(elem_type) + "'", tuple.line);
        }
    }

    return {"Tuple<" + tuple.element_type + ">", false, false};
}

/**
 * Type checks a method call on a tuple.
 */
TypeInfo check_tuple_method(TypeCheckerState& state, const MethodCall& mcall, const string& element_type) {
    auto method_info = bishop::get_tuple_method_info(mcall.method_name);

    if (!method_info) {
        error(state, "Tuple has no method '" + mcall.method_name + "'", mcall.line);
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

} // namespace typechecker
