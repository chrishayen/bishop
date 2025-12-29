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
 */
string variable_decl(const string& type, const string& name, const string& value, bool is_optional, bool is_const) {
    string t = type.empty() ? "auto" : map_type(type);
    string const_prefix = is_const ? "const " : "";

    if (is_optional) {
        return fmt::format("{}std::optional<{}> {} = {};", const_prefix, t, name, value);
    }

    return fmt::format("{}{} {} = {};", const_prefix, t, name, value);
}

/**
 * Emits a return statement: return value;
 */
string return_stmt(const string& value) {
    return fmt::format("return {};", value);
}

/**
 * Emits an assignment: name = value;
 */
string assignment(const string& name, const string& value) {
    return fmt::format("{} = {};", name, value);
}

} // namespace codegen
