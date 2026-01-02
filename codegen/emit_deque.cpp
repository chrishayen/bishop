/**
 * @file emit_deque.cpp
 * @brief Deque emission for the Bishop code generator.
 */

#include "codegen.hpp"
#include <fmt/format.h>

using namespace std;

namespace codegen {

/**
 * Emits a deque creation: Deque<T>() -> std::deque<T>{}.
 */
string emit_deque_create(const DequeCreate& deque) {
    string cpp_type = map_type(deque.element_type);
    return "std::deque<" + cpp_type + ">{}";
}

/**
 * Emits a deque method call, mapping Bishop methods to std::deque equivalents.
 */
string emit_deque_method_call(CodeGenState& state, const MethodCall& call, const string& obj_str, const vector<string>& args) {
    (void)state;

    if (call.method_name == "length") {
        return obj_str + ".size()";
    }

    if (call.method_name == "is_empty") {
        return obj_str + ".empty()";
    }

    if (call.method_name == "push_front") {
        return obj_str + ".push_front(" + args[0] + ")";
    }

    if (call.method_name == "push_back") {
        return obj_str + ".push_back(" + args[0] + ")";
    }

    if (call.method_name == "pop_front") {
        return fmt::format(
            "[](auto& d) {{ auto tmp = d.front(); d.pop_front(); return tmp; }}({})",
            obj_str
        );
    }

    if (call.method_name == "pop_back") {
        return fmt::format(
            "[](auto& d) {{ auto tmp = d.back(); d.pop_back(); return tmp; }}({})",
            obj_str
        );
    }

    if (call.method_name == "front") {
        return obj_str + ".front()";
    }

    if (call.method_name == "back") {
        return obj_str + ".back()";
    }

    if (call.method_name == "get") {
        return obj_str + ".at(" + args[0] + ")";
    }

    if (call.method_name == "clear") {
        return obj_str + ".clear()";
    }

    // Unknown deque method - fall back to generic method call
    return method_call(obj_str, call.method_name, args);
}

} // namespace codegen
