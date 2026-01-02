/**
 * @file emit_priority_queue.cpp
 * @brief PriorityQueue emission for the Bishop code generator.
 */

#include "codegen.hpp"
#include <fmt/format.h>

using namespace std;

namespace codegen {

/**
 * Emits a priority queue creation.
 * Max heap: PriorityQueue<T>() -> bishop::MaxPriorityQueue<T>()
 * Min heap: PriorityQueue<T>.min() -> bishop::MinPriorityQueue<T>()
 */
string emit_priority_queue_create(CodeGenState& state, const PriorityQueueCreate& pq) {
    (void)state;  // Unused
    string cpp_type = map_type(pq.element_type);

    if (pq.is_min_heap) {
        return "bishop::MinPriorityQueue<" + cpp_type + ">()";
    } else {
        return "bishop::MaxPriorityQueue<" + cpp_type + ">()";
    }
}

/**
 * Emits a priority queue method call.
 */
string emit_priority_queue_method_call(CodeGenState& state, const MethodCall& call, const string& obj_str, const vector<string>& args) {
    (void)state;  // Unused

    if (call.method_name == "length") {
        return obj_str + ".size()";
    }

    if (call.method_name == "is_empty") {
        return obj_str + ".empty()";
    }

    if (call.method_name == "push") {
        return obj_str + ".push(" + args[0] + ")";
    }

    if (call.method_name == "pop") {
        return obj_str + ".pop()";
    }

    if (call.method_name == "top") {
        return obj_str + ".top()";
    }

    // Unknown method - fall back to generic method call
    return method_call(obj_str, call.method_name, args);
}

} // namespace codegen
