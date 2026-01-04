/**
 * @file check_method_call.cpp
 * @brief Method call type inference for the Bishop type checker.
 */

#include "typechecker.hpp"
#include "strings.hpp"
#include "common/type_utils.hpp"

using namespace std;

namespace typechecker {

/**
 * Type checks a method call on a string.
 * Handles optional parameters for padding methods.
 */
TypeInfo check_str_method(TypeCheckerState& state, const MethodCall& mcall) {
    auto method_info = bishop::get_str_method_info(mcall.method_name);

    if (!method_info) {
        error(state, "str has no method '" + mcall.method_name + "'", mcall.line);
        return {"unknown", false, false};
    }

    const auto& [param_types, return_type] = *method_info;

    // Handle padding methods with optional char parameter
    bool is_padding_method = (mcall.method_name == "pad_left" ||
                              mcall.method_name == "pad_right" ||
                              mcall.method_name == "center");

    if (is_padding_method) {
        // Accept 1 or 2 arguments for padding methods
        if (mcall.args.size() < 1 || mcall.args.size() > 2) {
            error(state, "method '" + mcall.method_name + "' expects 1 or 2 arguments, got " +
                  to_string(mcall.args.size()), mcall.line);
        }

        // Check first argument is int
        if (mcall.args.size() >= 1) {
            TypeInfo arg_type = infer_type(state, *mcall.args[0]);
            TypeInfo expected = {"int", false, false};

            if (!types_compatible(expected, arg_type)) {
                error(state, "argument 1 of method '" + mcall.method_name +
                      "' expects 'int', got '" + format_type(arg_type) + "'", mcall.line);
            }
        }

        // Check optional second argument is str (single character string)
        if (mcall.args.size() == 2) {
            TypeInfo arg_type = infer_type(state, *mcall.args[1]);
            TypeInfo expected = {"str", false, false};

            if (!types_compatible(expected, arg_type)) {
                error(state, "argument 2 of method '" + mcall.method_name +
                      "' expects 'str', got '" + format_type(arg_type) + "'", mcall.line);
            }
        }

        return {return_type, false, false};
    }

    // Standard parameter checking for other methods
    if (mcall.args.size() != param_types.size()) {
        error(state, "method '" + mcall.method_name + "' expects " +
              to_string(param_types.size()) + " arguments, got " +
              to_string(mcall.args.size()), mcall.line);
    }

    for (size_t i = 0; i < mcall.args.size() && i < param_types.size(); i++) {
        TypeInfo arg_type = infer_type(state, *mcall.args[i]);
        TypeInfo expected = {param_types[i], false, false};

        if (!types_compatible(expected, arg_type)) {
            error(state, "argument " + to_string(i + 1) + " of method '" +
                  mcall.method_name + "' expects '" + param_types[i] +
                  "', got '" + format_type(arg_type) + "'", mcall.line);
        }
    }

    return {return_type, false, false};
}

/**
 * Type checks a static method call on a struct.
 * Static methods don't have 'self' so all args map directly to params.
 */
TypeInfo check_static_method(TypeCheckerState& state, const MethodCall& mcall, const string& struct_name) {
    const MethodDef* method = nullptr;
    size_t dot_pos = struct_name.find('.');

    if (dot_pos != string::npos) {
        string module_name = struct_name.substr(0, dot_pos);
        string sname = struct_name.substr(dot_pos + 1);
        method = get_qualified_method(state, module_name, sname, mcall.method_name);
    } else {
        method = get_method(state, struct_name, mcall.method_name);
    }

    if (!method) {
        error(state, "static method '" + mcall.method_name + "' not found on struct '" + struct_name + "'", mcall.line);
        return {"unknown", false, false};
    }

    if (!method->is_static) {
        error(state, "method '" + mcall.method_name + "' is not static; use an instance to call it", mcall.line);
        return {"unknown", false, false};
    }

    // Static methods: all params are actual params (no self)
    size_t expected_args = method->params.size();

    if (mcall.args.size() != expected_args) {
        error(state, "static method '" + mcall.method_name + "' expects " + to_string(expected_args) + " arguments, got " + to_string(mcall.args.size()), mcall.line);
    }

    for (size_t i = 0; i < mcall.args.size() && i < method->params.size(); i++) {
        TypeInfo arg_type = infer_type(state, *mcall.args[i]);
        TypeInfo param_type = {method->params[i].type, false, false};

        if (!types_compatible(param_type, arg_type)) {
            error(state, "argument " + to_string(i + 1) + " of static method '" + mcall.method_name +
                  "' expects '" + format_type(param_type) + "', got '" + format_type(arg_type) + "'", mcall.line);
        }
    }

    bool is_fallible = !method->error_type.empty();
    return method->return_type.empty() ? TypeInfo{"void", false, true, is_fallible}
                                       : TypeInfo{method->return_type, false, false, is_fallible};
}

/**
 * Type checks a method call on a struct.
 */
TypeInfo check_struct_method(TypeCheckerState& state, const MethodCall& mcall, const TypeInfo& obj_type) {
    const StructDef* sdef = get_struct(state, obj_type.base_type);

    if (!sdef) {
        error(state, "cannot call method on non-struct type '" + format_type(obj_type) + "'", mcall.line);
        return {"unknown", false, false};
    }

    const MethodDef* method = nullptr;
    size_t dot_pos = obj_type.base_type.find('.');

    if (dot_pos != string::npos) {
        string module_name = obj_type.base_type.substr(0, dot_pos);
        string struct_name = obj_type.base_type.substr(dot_pos + 1);
        method = get_qualified_method(state, module_name, struct_name, mcall.method_name);
    } else {
        method = get_method(state, obj_type.base_type, mcall.method_name);
    }

    if (!method) {
        error(state, "method '" + mcall.method_name + "' not found on struct '" + obj_type.base_type + "'", mcall.line);
        return {"unknown", false, false};
    }

    // If calling a static method via self (e.g., self.staticMethod()),
    // route to static method checking
    if (method->is_static) {
        return check_static_method(state, mcall, obj_type.base_type);
    }

    // Instance method: skip self parameter
    size_t expected_args = method->params.size() - 1;

    if (mcall.args.size() != expected_args) {
        error(state, "method '" + mcall.method_name + "' expects " + to_string(expected_args) + " arguments, got " + to_string(mcall.args.size()), mcall.line);
    }

    for (size_t i = 0; i < mcall.args.size() && i + 1 < method->params.size(); i++) {
        TypeInfo arg_type = infer_type(state, *mcall.args[i]);
        TypeInfo param_type = {method->params[i + 1].type, false, false};

        if (!types_compatible(param_type, arg_type)) {
            error(state, "argument " + to_string(i + 1) + " of method '" + mcall.method_name +
                  "' expects '" + format_type(param_type) + "', got '" + format_type(arg_type) + "'", mcall.line);
        }
    }

    bool is_fallible = !method->error_type.empty();
    return method->return_type.empty() ? TypeInfo{"void", false, true, is_fallible}
                                       : TypeInfo{method->return_type, false, false, is_fallible};
}

/**
 * Infers the type of a method call expression.
 * Auto-dereferences pointer types (like Go).
 * Detects static method calls when the object is a type name.
 */
TypeInfo check_method_call(TypeCheckerState& state, const MethodCall& mcall) {
    // Check for static method call: TypeName.method(...)
    // This happens when the object is a VariableRef whose name is a struct type
    if (auto* ref = dynamic_cast<const VariableRef*>(mcall.object.get())) {
        // Check if the name is a struct type (static method call)
        const StructDef* sdef = get_struct(state, ref->name);

        if (sdef) {
            // This is a static method call: TypeName.method(...)
            mcall.object_type = ref->name;  // Store struct name for codegen
            return check_static_method(state, mcall, ref->name);
        }
    }

    TypeInfo obj_type = infer_type(state, *mcall.object);
    mcall.object_type = obj_type.base_type;  // Store for codegen (includes pointer suffix)

    // Auto-dereference pointers for method calls (like Go)
    TypeInfo effective_type = obj_type;

    if (!effective_type.base_type.empty() && effective_type.base_type.back() == '*') {
        effective_type.base_type = effective_type.base_type.substr(0, effective_type.base_type.length() - 1);
    }

    if (effective_type.base_type.rfind("Channel<", 0) == 0) {
        string element_type = extract_element_type(effective_type.base_type, "Channel<");

        if (element_type.empty()) {
            error(state, "malformed Channel type '" + effective_type.base_type + "'", mcall.line);
            return {"unknown", false, false};
        }

        return check_channel_method(state, mcall, element_type);
    }

    if (effective_type.base_type.rfind("List<", 0) == 0) {
        string element_type = extract_element_type(effective_type.base_type, "List<");

        if (element_type.empty()) {
            error(state, "malformed List type '" + effective_type.base_type + "'", mcall.line);
            return {"unknown", false, false};
        }

        return check_list_method(state, mcall, element_type);
    }

    if (effective_type.base_type.rfind("Map<", 0) == 0) {
        auto [key_type, value_type] = bishop::extract_map_types(effective_type.base_type);

        if (key_type.empty() || value_type.empty()) {
            error(state, "malformed Map type '" + effective_type.base_type + "'", mcall.line);
            return {"unknown", false, false};
        }

        return check_map_method(state, mcall, key_type, value_type);
    }

    if (effective_type.base_type.rfind("Pair<", 0) == 0) {
        string element_type = extract_element_type(effective_type.base_type, "Pair<");

        if (element_type.empty()) {
            error(state, "malformed Pair type '" + effective_type.base_type + "'", mcall.line);
            return {"unknown", false, false};
        }

        return check_pair_method(state, mcall, element_type);
    }

    if (effective_type.base_type.rfind("Tuple<", 0) == 0) {
        string element_type = extract_element_type(effective_type.base_type, "Tuple<");

        if (element_type.empty()) {
            error(state, "malformed Tuple type '" + effective_type.base_type + "'", mcall.line);
            return {"unknown", false, false};
        }

        return check_tuple_method(state, mcall, element_type);
    }

    if (effective_type.base_type.rfind("Deque<", 0) == 0) {
        string element_type = extract_element_type(effective_type.base_type, "Deque<");

        if (element_type.empty()) {
            error(state, "malformed Deque type '" + effective_type.base_type + "'", mcall.line);
            return {"unknown", false, false};
        }

        return check_deque_method(state, mcall, element_type);
    }

    if (effective_type.base_type.rfind("Stack<", 0) == 0) {
        string element_type = extract_element_type(effective_type.base_type, "Stack<");

        if (element_type.empty()) {
            error(state, "malformed Stack type '" + effective_type.base_type + "'", mcall.line);
            return {"unknown", false, false};
        }

        return check_stack_method(state, mcall, element_type);
    }

    if (effective_type.base_type.rfind("Queue<", 0) == 0) {
        string element_type = extract_element_type(effective_type.base_type, "Queue<");

        if (element_type.empty()) {
            error(state, "malformed Queue type '" + effective_type.base_type + "'", mcall.line);
            return {"unknown", false, false};
        }

        return check_queue_method(state, mcall, element_type);
    }

    if (effective_type.base_type.rfind("PriorityQueue<", 0) == 0) {
        string element_type = extract_element_type(effective_type.base_type, "PriorityQueue<");

        if (element_type.empty()) {
            error(state, "malformed PriorityQueue type '" + effective_type.base_type + "'", mcall.line);
            return {"unknown", false, false};
        }

        return check_priority_queue_method(state, mcall, element_type);
    }

    if (effective_type.base_type.rfind("Set<", 0) == 0) {
        string element_type = extract_element_type(effective_type.base_type, "Set<");

        if (element_type.empty()) {
            error(state, "malformed Set type '" + effective_type.base_type + "'", mcall.line);
            return {"unknown", false, false};
        }

        return check_set_method(state, mcall, element_type);
    }

    if (effective_type.base_type == "str") {
        return check_str_method(state, mcall);
    }

    return check_struct_method(state, mcall, effective_type);
}

} // namespace typechecker
