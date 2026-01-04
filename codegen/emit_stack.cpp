/**
 * @file emit_stack.cpp
 * @brief Stack emission for the Bishop code generator.
 */

#include "codegen.hpp"
#include <fmt/format.h>

using namespace std;

namespace codegen {

/**
 * Emits a stack creation: Stack<T>() -> std::stack<T>{}.
 */
string emit_stack_create(const StackCreate& stack) {
    string cpp_type = map_type(stack.element_type);
    return "std::stack<" + cpp_type + ">{}";
}

/**
 * Emits a stack method call, mapping Bishop methods to std::stack equivalents.
 */
string emit_stack_method_call(CodeGenState& state, const MethodCall& call, const string& obj_str, const vector<string>& args) {
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
            "[](auto& s) {{ auto tmp = s.top(); s.pop(); return tmp; }}({})",
            obj_str
        );
    }

    if (call.method_name == "top") {
        return obj_str + ".top()";
    }

    // Unknown stack method - fall back to generic method call
    return method_call(obj_str, call.method_name, args);
}

} // namespace codegen
