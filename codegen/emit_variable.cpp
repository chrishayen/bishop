/**
 * @file emit_variable.cpp
 * @brief Variable declaration and assignment emission for the Bishop code generator.
 */

#include "codegen.hpp"
#include <fmt/format.h>

using namespace std;

namespace codegen {

/**
 * Emits a variable declaration: type name = value;
 * Uses 'auto' for inferred types, std::optional<T> for optional types.
 * Prepends 'const' for constant declarations.
 * Uses map_type_for_decl to strip reference suffixes from Channel types.
 * Escapes C++ reserved keywords in variable names.
 */
string variable_decl(const string& type, const string& name, const string& value, bool is_optional, bool is_const) {
    string t = type.empty() ? "auto" : map_type_for_decl(type);
    string const_prefix = is_const ? "const " : "";
    string escaped_name = escape_reserved_name(name);

    if (is_optional) {
        return fmt::format("{}std::optional<{}> {} = {};", const_prefix, t, escaped_name, value);
    }

    return fmt::format("{}{} {} = {};", const_prefix, t, escaped_name, value);
}

/**
 * Emits a return statement: return value;
 */
string return_stmt(const string& value) {
    return fmt::format("return {};", value);
}

/**
 * Emits an assignment: name = value;
 * Escapes C++ reserved keywords in variable names.
 */
string assignment(const string& name, const string& value) {
    return fmt::format("{} = {};", escape_reserved_name(name), value);
}

/**
 * Generates a module-level constant declaration.
 * Emits: static const type name = value;
 * Uses map_type_for_decl to strip reference suffixes from Channel types.
 * Escapes C++ reserved keywords in constant names.
 */
string generate_module_constant(CodeGenState& state, const VariableDecl& decl) {
    string val_str = emit(state, *decl.value);
    string t = decl.type.empty() ? "auto" : map_type_for_decl(decl.type);
    string escaped_name = escape_reserved_name(decl.name);

    if (decl.is_optional) {
        return fmt::format("static const std::optional<{}> {} = {};\n", t, escaped_name, val_str);
    }

    return fmt::format("static const {} {} = {};\n", t, escaped_name, val_str);
}

} // namespace codegen
