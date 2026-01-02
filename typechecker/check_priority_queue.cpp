/**
 * @file check_priority_queue.cpp
 * @brief PriorityQueue type inference for the Bishop type checker.
 */

#include "typechecker.hpp"
#include "priority_queues.hpp"

using namespace std;

namespace typechecker {

/**
 * Infers the type of a priority queue creation expression.
 * The is_min_heap flag is stored in the AST but doesn't affect the type.
 */
TypeInfo check_priority_queue_create(TypeCheckerState& state, const PriorityQueueCreate& pq) {
    // Validate that the element type is valid
    if (!is_valid_type(state, pq.element_type)) {
        error(state, "unknown type '" + pq.element_type + "' in PriorityQueue", pq.line);
        return {"unknown", false, false};
    }

    return {"PriorityQueue<" + pq.element_type + ">", false, false};
}

/**
 * Type checks a method call on a priority queue.
 */
TypeInfo check_priority_queue_method(TypeCheckerState& state, const MethodCall& mcall, const string& element_type) {
    auto method_info = bishop::get_priority_queue_method_info(mcall.method_name);

    if (!method_info) {
        error(state, "PriorityQueue has no method '" + mcall.method_name + "'", mcall.line);
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
