/**
 * @file emit_method_call.cpp
 * @brief Method call emission for the Bishop code generator.
 */

#include "codegen.hpp"
#include <fmt/format.h>
#include <fmt/ranges.h>

using namespace std;

namespace codegen {

/**
 * Emits a method call: object.method(arg1, arg2, ...).
 */
string method_call(const string& object, const string& method, const vector<string>& args) {
    return fmt::format("{}.{}({})", object, method, fmt::join(args, ", "));
}

/**
 * Helper to check if a method is static.
 */
static bool is_static_method(const CodeGenState& state, const string& struct_name, const string& method_name) {
    if (!state.current_program) return false;

    for (const auto& method : state.current_program->methods) {
        if (method->struct_name == struct_name && method->name == method_name) {
            return method->is_static;
        }
    }

    return false;
}

/**
 * Emits a method call AST node with special handling for self, channels, lists, and static methods.
 */
string emit_method_call(CodeGenState& state, const MethodCall& call) {
    vector<string> args;

    for (const auto& arg : call.args) {
        args.push_back(emit(state, *arg));
    }

    // Handle static method calls: TypeName::method(args)
    // Detected when object is a VariableRef with name matching a struct type
    if (auto* ref = dynamic_cast<const VariableRef*>(call.object.get())) {
        // Check if this is a static method call
        if (is_static_method(state, ref->name, call.method_name)) {
            return fmt::format("{}::{}({})", ref->name, call.method_name, fmt::join(args, ", "));
        }

        // Handle self.method() in instance methods
        if (ref->name == "self") {
            // Check if this is a static method call via self
            if (!state.current_struct.empty() && is_static_method(state, state.current_struct, call.method_name)) {
                return fmt::format("{}::{}({})", state.current_struct, call.method_name, fmt::join(args, ", "));
            }

            // Instance method: emit this->method()
            string args_str;

            for (size_t i = 0; i < args.size(); i++) {
                if (i > 0) args_str += ", ";
                args_str += args[i];
            }

            return "this->" + call.method_name + "(" + args_str + ")";
        }
    }

    // Handle channel methods - direct calls on bishop::rt::Channel
    if (call.method_name == "send") {
        string val = args.empty() ? "" : args[0];
        return emit(state, *call.object) + ".send(" + val + ")";
    }

    if (call.method_name == "recv") {
        return emit(state, *call.object) + ".recv()";
    }

    string obj_str = emit(state, *call.object);

    // Handle str.at() - wrap char result in std::string
    if (call.object_type == "str" && call.method_name == "at") {
        return fmt::format("std::string(1, {}.at({}))", obj_str, args[0]);
    }

    // Handle List methods - map to std::vector equivalents
    if (call.object_type.rfind("List<", 0) == 0) {
        return emit_list_method_call(state, call, obj_str, args);
    }

    // Handle extended string methods
    if (call.object_type == "str") {
        string result = emit_str_method_call(state, call, obj_str, args);

        if (!result.empty()) {
            return result;
        }

        // Fall through to default handling for basic string methods
    }

    // Handle Pair methods
    if (call.object_type.rfind("Pair<", 0) == 0) {
        return emit_pair_method_call(state, call, obj_str, args);
    }

    // Handle Tuple methods
    if (call.object_type.rfind("Tuple<", 0) == 0) {
        return emit_tuple_method_call(state, call, obj_str, args);
    }

    // Handle Deque methods
    if (call.object_type.rfind("Deque<", 0) == 0) {
        return emit_deque_method_call(state, call, obj_str, args);
    }

    // Handle Stack methods
    if (call.object_type.rfind("Stack<", 0) == 0) {
        return emit_stack_method_call(state, call, obj_str, args);
    }

    // Handle Queue methods
    if (call.object_type.rfind("Queue<", 0) == 0) {
        return emit_queue_method_call(state, call, obj_str, args);
    }

    // Use -> for pointer types (auto-deref like Go)
    if (!call.object_type.empty() && call.object_type.back() == '*') {
        return fmt::format("{}->{}({})", obj_str, call.method_name, fmt::join(args, ", "));
    }

    return method_call(obj_str, call.method_name, args);
}

} // namespace codegen
