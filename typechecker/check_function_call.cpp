/**
 * @file check_call.cpp
 * @brief Function call type inference for the Bishop type checker.
 */

#include "typechecker.hpp"

using namespace std;

namespace typechecker {

/**
 * Checks if a function name is a built-in assertion function.
 */
static bool is_assertion_function(const string& name) {
    return name == "assert_eq" || name == "assert_ne" ||
           name == "assert_true" || name == "assert_false" ||
           name == "assert_gt" || name == "assert_gte" ||
           name == "assert_lt" || name == "assert_lte" ||
           name == "assert_contains" ||
           name == "assert_starts_with" || name == "assert_ends_with" ||
           name == "assert_near";
}

/**
 * Type-checks a built-in assertion function call.
 * Returns true if the assertion is valid, false if there's an error.
 */
static bool check_assertion_call(TypeCheckerState& state, const FunctionCall& call) {
    const string& name = call.name;

    // assert_eq, assert_ne: two arguments of compatible types
    if (name == "assert_eq" || name == "assert_ne") {
        if (call.args.size() != 2) {
            error(state, name + " expects 2 arguments, got " + to_string(call.args.size()), call.line);
            return false;
        }

        TypeInfo type_a = infer_type(state, *call.args[0]);
        TypeInfo type_b = infer_type(state, *call.args[1]);

        if (!types_compatible(type_a, type_b)) {
            error(state, name + " arguments must have compatible types, got '" +
                  format_type(type_a) + "' and '" + format_type(type_b) + "'", call.line);
            return false;
        }

        return true;
    }

    // assert_true, assert_false: single boolean argument
    if (name == "assert_true" || name == "assert_false") {
        if (call.args.size() != 1) {
            error(state, name + " expects 1 argument, got " + to_string(call.args.size()), call.line);
            return false;
        }

        TypeInfo arg_type = infer_type(state, *call.args[0]);

        if (arg_type.base_type != "bool") {
            error(state, name + " expects bool argument, got '" + format_type(arg_type) + "'", call.line);
            return false;
        }

        return true;
    }

    // assert_gt, assert_gte, assert_lt, assert_lte: two comparable numeric arguments
    if (name == "assert_gt" || name == "assert_gte" ||
        name == "assert_lt" || name == "assert_lte") {
        if (call.args.size() != 2) {
            error(state, name + " expects 2 arguments, got " + to_string(call.args.size()), call.line);
            return false;
        }

        TypeInfo type_a = infer_type(state, *call.args[0]);
        TypeInfo type_b = infer_type(state, *call.args[1]);

        if (!types_compatible(type_a, type_b)) {
            error(state, name + " arguments must have compatible types, got '" +
                  format_type(type_a) + "' and '" + format_type(type_b) + "'", call.line);
            return false;
        }

        return true;
    }

    // assert_contains(item, list): item type must match list element type
    if (name == "assert_contains") {
        if (call.args.size() != 2) {
            error(state, "assert_contains expects 2 arguments, got " + to_string(call.args.size()), call.line);
            return false;
        }

        TypeInfo item_type = infer_type(state, *call.args[0]);
        TypeInfo list_type = infer_type(state, *call.args[1]);

        // Check if second argument is a list
        if (list_type.base_type.rfind("List<", 0) != 0) {
            error(state, "assert_contains second argument must be a List, got '" +
                  format_type(list_type) + "'", call.line);
            return false;
        }

        // Extract element type from List<T>
        size_t start = 5;  // "List<"
        size_t end = list_type.base_type.rfind('>');
        string element_type = list_type.base_type.substr(start, end - start);

        if (!types_compatible({element_type, false, false}, item_type)) {
            error(state, "assert_contains item type '" + format_type(item_type) +
                  "' doesn't match list element type '" + element_type + "'", call.line);
            return false;
        }

        return true;
    }

    // assert_starts_with, assert_ends_with: prefix/suffix is first, string is second
    if (name == "assert_starts_with" || name == "assert_ends_with") {
        if (call.args.size() != 2) {
            error(state, name + " expects 2 arguments, got " + to_string(call.args.size()), call.line);
            return false;
        }

        TypeInfo prefix_type = infer_type(state, *call.args[0]);
        TypeInfo str_type = infer_type(state, *call.args[1]);

        if (prefix_type.base_type != "str") {
            error(state, name + " first argument must be str, got '" +
                  format_type(prefix_type) + "'", call.line);
            return false;
        }

        if (str_type.base_type != "str") {
            error(state, name + " second argument must be str, got '" +
                  format_type(str_type) + "'", call.line);
            return false;
        }

        return true;
    }

    // assert_near(actual, expected, epsilon): all three must be floats
    if (name == "assert_near") {
        if (call.args.size() != 3) {
            error(state, "assert_near expects 3 arguments (actual, expected, epsilon), got " +
                  to_string(call.args.size()), call.line);
            return false;
        }

        TypeInfo actual_type = infer_type(state, *call.args[0]);
        TypeInfo expected_type = infer_type(state, *call.args[1]);
        TypeInfo epsilon_type = infer_type(state, *call.args[2]);

        // Allow int or float for actual and expected (they can be compared)
        bool actual_is_numeric = (actual_type.base_type == "f64" || actual_type.base_type == "f32" ||
                                  actual_type.base_type == "int");
        bool expected_is_numeric = (expected_type.base_type == "f64" || expected_type.base_type == "f32" ||
                                    expected_type.base_type == "int");
        bool epsilon_is_numeric = (epsilon_type.base_type == "f64" || epsilon_type.base_type == "f32" ||
                                   epsilon_type.base_type == "int");

        if (!actual_is_numeric) {
            error(state, "assert_near first argument must be numeric, got '" +
                  format_type(actual_type) + "'", call.line);
            return false;
        }

        if (!expected_is_numeric) {
            error(state, "assert_near second argument must be numeric, got '" +
                  format_type(expected_type) + "'", call.line);
            return false;
        }

        if (!epsilon_is_numeric) {
            error(state, "assert_near third argument (epsilon) must be numeric, got '" +
                  format_type(epsilon_type) + "'", call.line);
            return false;
        }

        return true;
    }

    return true;
}

/**
 * Infers the type of a function call expression.
 */
TypeInfo check_function_call(TypeCheckerState& state, const FunctionCall& call) {
    // Handle built-in assertion functions
    if (is_assertion_function(call.name)) {
        check_assertion_call(state, call);
        return {"void", false, true};
    }

    if (call.name == "print") {
        // Still infer types for arguments (to populate object_type on MethodCalls)
        for (const auto& arg : call.args) {
            infer_type(state, *arg);
        }

        return {"void", false, true};
    }

    if (call.name == "sleep") {
        if (call.args.size() != 1) {
            error(state, "sleep expects 1 argument (milliseconds), got " + to_string(call.args.size()), call.line);
        } else {
            TypeInfo arg_type = infer_type(state, *call.args[0]);

            if (arg_type.base_type != "int") {
                error(state, "sleep expects int argument, got '" + format_type(arg_type) + "'", call.line);
            }
        }

        return {"void", false, true};
    }

    size_t dot_pos = call.name.find('.');

    if (dot_pos != string::npos) {
        string module_name = call.name.substr(0, dot_pos);
        string func_name = call.name.substr(dot_pos + 1);

        const FunctionDef* func = get_qualified_function(state, module_name, func_name);

        if (!func) {
            error(state, "undefined function '" + call.name + "'", call.line);
            return {"unknown", false, false};
        }

        if (call.args.size() != func->params.size()) {
            error(state, "function '" + call.name + "' expects " + to_string(func->params.size()) + " arguments, got " + to_string(call.args.size()), call.line);
        }

        for (size_t i = 0; i < call.args.size() && i < func->params.size(); i++) {
            TypeInfo arg_type = infer_type(state, *call.args[i]);
            TypeInfo param_type = {func->params[i].type, false, false};

            if (!types_compatible(param_type, arg_type)) {
                error(state, "argument " + to_string(i + 1) + " of function '" + call.name +
                      "' expects '" + format_type(param_type) + "', got '" + format_type(arg_type) + "'", call.line);
            }
        }

        bool fallible = !func->error_type.empty();
        return func->return_type.empty() ? TypeInfo{"void", false, true, fallible}
                                         : TypeInfo{func->return_type, false, false, fallible};
    }

    if (const TypeInfo* local = lookup_local(state, call.name)) {
        TypeInfo local_type = *local;

        // Use the shared helper to parse parameter types from the function type
        auto param_types_opt = parse_function_type_params(local_type.base_type);

        if (param_types_opt) {
            vector<string> param_types = *param_types_opt;

            // Check argument count
            if (call.args.size() != param_types.size()) {
                error(state, "function '" + call.name + "' expects " + to_string(param_types.size()) + " arguments, got " + to_string(call.args.size()), call.line);
            }

            // Check argument types
            for (size_t i = 0; i < call.args.size() && i < param_types.size(); i++) {
                TypeInfo arg_type = infer_type(state, *call.args[i]);

                if (!types_compatible({param_types[i], false, false}, arg_type)) {
                    error(state, "argument " + to_string(i + 1) + " of function '" + call.name +
                          "' expects '" + param_types[i] + "', got '" + format_type(arg_type) + "'", call.line);
                }
            }

            // Extract return type (after " -> ")
            size_t params_end = local_type.base_type.find(')');
            size_t arrow_pos = local_type.base_type.find(" -> ", params_end);

            if (arrow_pos != string::npos) {
                string ret_type = local_type.base_type.substr(arrow_pos + 4);
                return {ret_type, false, false};
            }

            return {"void", false, true};
        }
    }

    if (state.functions.find(call.name) != state.functions.end()) {
        const FunctionDef* func = state.functions[call.name];

        if (call.args.size() != func->params.size()) {
            error(state, "function '" + call.name + "' expects " + to_string(func->params.size()) + " arguments, got " + to_string(call.args.size()), call.line);
        }

        for (size_t i = 0; i < call.args.size() && i < func->params.size(); i++) {
            TypeInfo arg_type = infer_type(state, *call.args[i]);
            TypeInfo param_type = {func->params[i].type, false, false};

            if (!types_compatible(param_type, arg_type)) {
                error(state, "argument " + to_string(i + 1) + " of function '" + call.name +
                      "' expects '" + format_type(param_type) + "', got '" + format_type(arg_type) + "'", call.line);
            }
        }

        bool fallible = !func->error_type.empty();
        return func->return_type.empty() ? TypeInfo{"void", false, true, fallible}
                                         : TypeInfo{func->return_type, false, false, fallible};
    }

    if (state.extern_functions.find(call.name) != state.extern_functions.end()) {
        const ExternFunctionDef* ext = state.extern_functions[call.name];

        if (call.args.size() != ext->params.size()) {
            error(state, "function '" + call.name + "' expects " + to_string(ext->params.size()) + " arguments, got " + to_string(call.args.size()), call.line);
        }

        for (size_t i = 0; i < call.args.size() && i < ext->params.size(); i++) {
            TypeInfo arg_type = infer_type(state, *call.args[i]);
            TypeInfo param_type = {ext->params[i].type, false, false};

            if (!types_compatible(param_type, arg_type)) {
                error(state, "argument " + to_string(i + 1) + " of function '" + call.name +
                      "' expects '" + format_type(param_type) + "', got '" + format_type(arg_type) + "'", call.line);
            }
        }

        if (ext->return_type.empty() || ext->return_type == "void") {
            return {"void", false, true};
        }

        return {ext->return_type, false, false};
    }

    error(state, "undefined function '" + call.name + "'", call.line);
    return {"unknown", false, false};
}

} // namespace typechecker
