/**
 * @file emit_queue.cpp
 * @brief Queue emission for the Bishop code generator.
 */

#include "codegen.hpp"
#include <fmt/format.h>

using namespace std;

namespace codegen {

/**
 * Emits a queue creation: Queue<T>() -> std::queue<T>{}.
 */
string emit_queue_create(const QueueCreate& queue) {
    string cpp_type = map_type(queue.element_type);
    return "std::queue<" + cpp_type + ">{}";
}

/**
 * Emits a queue method call, mapping Bishop methods to std::queue equivalents.
 */
string emit_queue_method_call(CodeGenState& state, const MethodCall& call, const string& obj_str, const vector<string>& args) {
    (void)state;

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
        return fmt::format(
            "[](auto& q) {{ auto tmp = q.front(); q.pop(); return tmp; }}({})",
            obj_str
        );
    }

    if (call.method_name == "front") {
        return obj_str + ".front()";
    }

    if (call.method_name == "back") {
        return obj_str + ".back()";
    }

    // Unknown queue method - fall back to generic method call
    return method_call(obj_str, call.method_name, args);
}

} // namespace codegen
