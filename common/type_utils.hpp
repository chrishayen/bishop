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

/**
 * Extracts both key and value types from a Map<K, V> type string.
 *
 * For example:
 *   - extract_map_types("Map<str, int>") returns {"str", "int"}
 *   - extract_map_types("Map<str, List<int>>") returns {"str", "List<int>"}
 *
 * @param map_type The full Map type string (e.g., "Map<str, int>")
 * @return A pair of {key_type, value_type}, or {"", ""} if invalid
 */
inline std::pair<std::string, std::string> extract_map_types(const std::string& map_type) {
    const std::string prefix = "Map<";

    if (map_type.empty() || map_type.rfind(prefix, 0) != 0 || map_type.back() != '>') {
        return {"", ""};
    }

    size_t start = prefix.length();
    int depth = 1;
    size_t comma_pos = std::string::npos;

    // Find the comma at depth 1 that separates K and V
    for (size_t i = start; i < map_type.size(); i++) {
        if (map_type[i] == '<') {
            depth++;
        } else if (map_type[i] == '>') {
            depth--;

            if (depth == 0) {
                // Found the end, extract both types
                if (comma_pos == std::string::npos) {
                    return {"", ""};  // No comma found
                }

                std::string key_type = map_type.substr(start, comma_pos - start);
                std::string value_type = map_type.substr(comma_pos + 2, i - comma_pos - 2);  // +2 to skip ", "
                return {key_type, value_type};
            }
        } else if (map_type[i] == ',' && depth == 1 && comma_pos == std::string::npos) {
            comma_pos = i;
        }
    }

    return {"", ""};
}

} // namespace bishop
