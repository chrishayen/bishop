/**
 * @file chars.cpp
 * @brief Char method type definitions for the Bishop type checker.
 *
 * Defines type signatures for all built-in char methods.
 */

/**
 * @nog_method is_alpha
 * @type char
 * @description Returns true if the character is alphabetic (a-z, A-Z).
 * @returns bool - True if alphabetic
 * @example
 * c := 'A';
 * if c.is_alpha() {
 *     print("Is a letter");
 * }
 */

/**
 * @nog_method is_digit
 * @type char
 * @description Returns true if the character is a digit (0-9).
 * @returns bool - True if digit
 * @example
 * c := '5';
 * if c.is_digit() {
 *     print("Is a digit");
 * }
 */

/**
 * @nog_method is_whitespace
 * @type char
 * @description Returns true if the character is whitespace (space, tab, newline, etc).
 * @returns bool - True if whitespace
 * @example
 * c := ' ';
 * if c.is_whitespace() {
 *     print("Is whitespace");
 * }
 */

/**
 * @nog_method is_upper
 * @type char
 * @description Returns true if the character is uppercase (A-Z).
 * @returns bool - True if uppercase
 * @example
 * c := 'A';
 * if c.is_upper() {
 *     print("Is uppercase");
 * }
 */

/**
 * @nog_method is_lower
 * @type char
 * @description Returns true if the character is lowercase (a-z).
 * @returns bool - True if lowercase
 * @example
 * c := 'a';
 * if c.is_lower() {
 *     print("Is lowercase");
 * }
 */

/**
 * @nog_method to_upper
 * @type char
 * @description Converts the character to uppercase.
 * @returns char - The uppercase character
 * @example
 * c := 'a';
 * upper := c.to_upper();  // 'A'
 */

/**
 * @nog_method to_lower
 * @type char
 * @description Converts the character to lowercase.
 * @returns char - The lowercase character
 * @example
 * c := 'A';
 * lower := c.to_lower();  // 'a'
 */

#include "chars.hpp"

#include <map>

namespace nog {

/**
 * Returns type information for built-in char methods.
 * Maps method names to their parameter types and return types.
 */
std::optional<CharMethodInfo> get_char_method_info(const std::string& method_name) {
    static const std::map<std::string, CharMethodInfo> char_methods = {
        // Type checking methods
        {"is_alpha", {{}, "bool"}},
        {"is_digit", {{}, "bool"}},
        {"is_whitespace", {{}, "bool"}},

        // Case checking methods
        {"is_upper", {{}, "bool"}},
        {"is_lower", {{}, "bool"}},

        // Case conversion methods
        {"to_upper", {{}, "char"}},
        {"to_lower", {{}, "char"}},
    };

    auto it = char_methods.find(method_name);

    if (it != char_methods.end()) {
        return it->second;
    }

    return std::nullopt;
}

}  // namespace nog
