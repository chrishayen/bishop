/**
 * @file check_queue.cpp
 * @brief Queue type inference for the Bishop type checker.
 */

#include "typechecker.hpp"
#include "queues.hpp"

using namespace std;

namespace typechecker {

/**
 * Infers the type of a queue creation expression.
 */
TypeInfo check_queue_create(TypeCheckerState& state, const QueueCreate& queue) {
    (void)state;
    return {"Queue<" + queue.element_type + ">", false, false};
}

/**
 * Type checks a method call on a queue.
 */
TypeInfo check_queue_method(TypeCheckerState& state, const MethodCall& mcall, const string& element_type) {
    auto method_info = bishop::get_queue_method_info(mcall.method_name);

    if (!method_info) {
        error(state, "Queue has no method '" + mcall.method_name + "'", mcall.line);
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
