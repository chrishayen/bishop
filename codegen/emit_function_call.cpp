/**
 * @file emit_function_call.cpp
 * @brief Function call emission for the Bishop code generator.
 */

#include "codegen.hpp"
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <unordered_set>

using namespace std;

namespace codegen {

/**
 * Set of C++ reserved keywords that need to be escaped with a trailing
 * underscore when used as function names.
 */
static const unordered_set<string> CPP_RESERVED_KEYWORDS = {
    "int", "float", "double", "bool", "char", "void", "long", "short",
    "signed", "unsigned", "const", "volatile", "static", "extern",
    "class", "struct", "union", "enum", "template", "typename",
    "namespace", "using", "operator", "new", "delete", "this",
    "if", "else", "switch", "case", "default", "while", "do", "for",
    "break", "continue", "return", "goto", "try", "catch", "throw",
    "true", "false", "nullptr", "auto", "register", "inline", "virtual",
    "explicit", "friend", "mutable", "public", "private", "protected",
    "sizeof", "typedef", "export", "asm", "alignas", "alignof",
    "decltype", "noexcept", "static_assert", "thread_local", "constexpr",
    "consteval", "constinit", "concept", "requires",
    "co_await", "co_return", "co_yield"
};

/**
 * Escapes a function name if it is a C++ reserved keyword.
 */
static string escape_reserved_name(const string& name) {
    if (CPP_RESERVED_KEYWORDS.count(name) > 0) {
        return name + "_";
    }

    return name;
}

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

    // Handle qualified function call: module.func -> module::func
    string func_name = call.name;
    size_t dot_pos = func_name.find('.');

    if (dot_pos != string::npos) {
        string module_name = func_name.substr(0, dot_pos);
        string method_name = func_name.substr(dot_pos + 1);
        method_name = escape_reserved_name(method_name);

        // Map module names that conflict with C/C++ identifiers
        if (module_name == "random") {
            module_name = "bishop_random";
        }

        func_name = module_name + "::" + method_name;
    }

    return function_call(func_name, args);
}

} // namespace codegen
