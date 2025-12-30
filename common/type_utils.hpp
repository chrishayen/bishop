/**
 * @file type_utils.hpp
 * @brief Common type utility functions shared between typechecker and codegen.
 *
 * This header provides utility functions for working with generic type strings
 * that are used by both the typechecker and code generator.
 */

#pragma once
#include <string>

namespace bishop {

/**
 * Extracts the element type from a generic type string using proper bracket matching.
 *
 * For example:
 *   - extract_element_type("List<int>", "List<") returns "int"
 *   - extract_element_type("Pair<List<int>>", "Pair<") returns "List<int>"
 *   - extract_element_type("Channel<Pair<str>>", "Channel<") returns "Pair<str>"
 *
 * @param generic_type The full generic type string (e.g., "Pair<List<int>>")
 * @param prefix The expected prefix including the opening bracket (e.g., "Pair<")
 * @return The element type, or empty string if the type doesn't match the prefix
 */
inline std::string extract_element_type(const std::string& generic_type, const std::string& prefix) {
    if (generic_type.empty() || generic_type.rfind(prefix, 0) != 0 || generic_type.back() != '>') {
        return "";
    }

    size_t start = prefix.length();
    int depth = 1;

    for (size_t i = start; i < generic_type.size(); i++) {
        if (generic_type[i] == '<') {
            depth++;
        } else if (generic_type[i] == '>') {
            depth--;

            if (depth == 0) {
                return generic_type.substr(start, i - start);
            }
        }
    }

    return "";
}

} // namespace bishop
