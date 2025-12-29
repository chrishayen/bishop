/**
 * @file emit_literals.cpp
 * @brief Literal emission for the Bishop code generator.
 */

#include "codegen.hpp"
#include <fmt/format.h>

using namespace std;

namespace codegen {

/**
 * Checks if a character is a valid escape sequence character.
 * Valid escape characters: n, t, r, ", ', \
 */
bool is_escape_char(char c) {
    return c == 'n' || c == 't' || c == 'r' || c == '"' || c == '\'' || c == '\\';
}

/**
 * Escapes special characters in a string for C++ string literals.
 *
 * Preserves valid escape sequences like \n, \t, \r, \", \', \\
 * so they are passed through to the C++ compiler for interpretation.
 * Only escapes backslashes that are NOT part of valid escape sequences.
 */
string escape_string(const string& value) {
    string result;
    result.reserve(value.size() * 2);

    for (size_t i = 0; i < value.size(); i++) {
        char c = value[i];

        if (c == '\\' && i + 1 < value.size() && is_escape_char(value[i + 1])) {
            // Preserve valid escape sequences by copying both characters
            result += c;             // backslash
            result += value[i + 1];  // escape character
            i++;                     // skip the escape character in the next iteration
        } else {
            switch (c) {
                case '"':  result += "\\\""; break;
                case '\\': result += "\\\\"; break;
                case '\n': result += "\\n"; break;
                case '\t': result += "\\t"; break;
                case '\r': result += "\\r"; break;
                default:   result += c; break;
            }
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
