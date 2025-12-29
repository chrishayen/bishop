/**
 * @file emit_literals.cpp
 * @brief Literal emission for the Bishop code generator.
 */

#include "codegen.hpp"
#include <fmt/format.h>

using namespace std;

namespace codegen {

/**
 * Escapes special characters in a string for C++ string literals.
 */
string escape_string(const string& value) {
    string result;
    result.reserve(value.size() * 2);

    for (char c : value) {
        switch (c) {
            case '"':  result += "\\\""; break;
            case '\\': result += "\\\\"; break;
            case '\n': result += "\\n"; break;
            case '\t': result += "\\t"; break;
            case '\r': result += "\\r"; break;
            default:   result += c; break;
        }
    }

    return result;
}

/**
 * Emits a C++ string literal wrapped in std::string constructor.
 */
string string_literal(const string& value) {
    return fmt::format("std::string(\"{}\")", escape_string(value));
}

/**
 * Emits a C++ integer literal.
 */
string number_literal(const string& value) {
    return value;
}

/**
 * Emits a C++ float literal.
 */
string float_literal(const string& value) {
    return value;
}

/**
 * Emits a C++ bool literal: true or false.
 */
string bool_literal(bool value) {
    return value ? "true" : "false";
}

/**
 * Emits std::nullopt for the none literal.
 */
string none_literal() {
    return "std::nullopt";
}

} // namespace codegen
