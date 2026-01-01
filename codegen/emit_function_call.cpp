/**
 * @file emit_function_call.cpp
 * @brief Function call emission for the Bishop code generator.
 */

#include "codegen.hpp"
#include <fmt/format.h>
#include <fmt/ranges.h>

using namespace std;

namespace codegen {

/**
 * Emits a function call: name(arg1, arg2, ...).
 */
string function_call(const string& name, const vector<string>& args) {
    return fmt::format("{}({})", name, fmt::join(args, ", "));
}

/**
 * Emits a function call AST node.
 */
string emit_function_call(CodeGenState& state, const FunctionCall& call) {
    vector<string> args;

    for (const auto& arg : call.args) {
        args.push_back(emit(state, *arg));
    }

    // Check if this is an unqualified static method call (resolved by typechecker)
    if (!call.resolved_struct.empty()) {
        string fn_name = escape_reserved_name(call.name);
        return function_call(call.resolved_struct + "::" + fn_name, args);
    }

    // Handle qualified function call: module.func -> module::func
    string func_name = call.name;
    size_t dot_pos = func_name.find('.');

    if (dot_pos != string::npos) {
        string module_name = func_name.substr(0, dot_pos);
        string fn_name = func_name.substr(dot_pos + 1);
        fn_name = escape_reserved_name(fn_name);

        // Map module names that conflict with C/C++ identifiers
        if (module_name == "time") {
            module_name = "bishop_time";
        } else if (module_name == "random") {
            module_name = "bishop_random";
        } else if (module_name == "sync") {
            module_name = "bsync";
        } else if (module_name == "log") {
            module_name = "bishop_log";
        }

        func_name = module_name + "::" + fn_name;
    } else {
        // Check if this is a using alias for a function
        const CodeGenUsingAlias* alias = get_using_alias(state, call.name);

        if (alias) {
            string module_name = alias->module_alias;
            string fn_name = escape_reserved_name(alias->member_name);

            // Map module names that conflict with C/C++ identifiers
            if (module_name == "time") {
                module_name = "bishop_time";
            } else if (module_name == "random") {
                module_name = "bishop_random";
            } else if (module_name == "sync") {
                module_name = "bsync";
            } else if (module_name == "log") {
                module_name = "bishop_log";
            }

            func_name = module_name + "::" + fn_name;
        }
    }

    return function_call(func_name, args);
}

} // namespace codegen
