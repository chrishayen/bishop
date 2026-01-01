/**
 * @file emit_name.cpp
 * @brief Name utilities for the Bishop code generator.
 *
 * Handles escaping of C++ reserved keywords when used as identifiers
 * in Bishop code.
 */

#include "codegen.hpp"
#include <unordered_set>

using namespace std;

namespace codegen {

/**
 * Set of C++ reserved keywords that need to be escaped with a trailing
 * underscore when used as identifiers in Bishop code.
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
 * Escapes an identifier if it is a C++ reserved keyword.
 *
 * Bishop users should be able to use any valid Bishop identifier without
 * worrying about C++ keyword conflicts. This function appends an underscore
 * to any C++ reserved keyword to make it a valid C++ identifier.
 *
 * @param name The identifier to escape
 * @return The escaped identifier (with trailing underscore if reserved)
 */
string escape_reserved_name(const string& name) {
    if (CPP_RESERVED_KEYWORDS.count(name) > 0) {
        return name + "_";
    }

    return name;
}

} // namespace codegen
