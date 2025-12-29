/**
 * @file emit_char.cpp
 * @brief Char method emission for the Bishop code generator.
 *
 * Handles emitting C++ code for char methods.
 */

#include "codegen.hpp"
#include <fmt/format.h>

using namespace std;

namespace codegen {

/**
 * Emits C++ code for the is_alpha() char method.
 * Returns true if the character is alphabetic.
 */
string emit_char_is_alpha(const string& obj_str) {
    return fmt::format("static_cast<bool>(std::isalpha(static_cast<unsigned char>({})))", obj_str);
}

/**
 * Emits C++ code for the is_digit() char method.
 * Returns true if the character is a digit.
 */
string emit_char_is_digit(const string& obj_str) {
    return fmt::format("static_cast<bool>(std::isdigit(static_cast<unsigned char>({})))", obj_str);
}

/**
 * Emits C++ code for the is_whitespace() char method.
 * Returns true if the character is whitespace.
 */
string emit_char_is_whitespace(const string& obj_str) {
    return fmt::format("static_cast<bool>(std::isspace(static_cast<unsigned char>({})))", obj_str);
}

/**
 * Emits C++ code for the is_upper() char method.
 * Returns true if the character is uppercase.
 */
string emit_char_is_upper(const string& obj_str) {
    return fmt::format("static_cast<bool>(std::isupper(static_cast<unsigned char>({})))", obj_str);
}

/**
 * Emits C++ code for the is_lower() char method.
 * Returns true if the character is lowercase.
 */
string emit_char_is_lower(const string& obj_str) {
    return fmt::format("static_cast<bool>(std::islower(static_cast<unsigned char>({})))", obj_str);
}

/**
 * Emits C++ code for the to_upper() char method.
 * Converts the character to uppercase.
 */
string emit_char_to_upper(const string& obj_str) {
    return fmt::format("static_cast<char>(std::toupper(static_cast<unsigned char>({})))", obj_str);
}

/**
 * Emits C++ code for the to_lower() char method.
 * Converts the character to lowercase.
 */
string emit_char_to_lower(const string& obj_str) {
    return fmt::format("static_cast<char>(std::tolower(static_cast<unsigned char>({})))", obj_str);
}

/**
 * Emits a char method call, handling char methods.
 * Returns empty string if not a recognized char method.
 */
string emit_char_method_call(CodeGenState& state, const MethodCall& call, const string& obj_str, const vector<string>& args) {
    const string& method = call.method_name;

    if (method == "is_alpha") {
        return emit_char_is_alpha(obj_str);
    }

    if (method == "is_digit") {
        return emit_char_is_digit(obj_str);
    }

    if (method == "is_whitespace") {
        return emit_char_is_whitespace(obj_str);
    }

    if (method == "is_upper") {
        return emit_char_is_upper(obj_str);
    }

    if (method == "is_lower") {
        return emit_char_is_lower(obj_str);
    }

    if (method == "to_upper") {
        return emit_char_to_upper(obj_str);
    }

    if (method == "to_lower") {
        return emit_char_to_lower(obj_str);
    }

    // Not a recognized char method
    return "";
}

} // namespace codegen
