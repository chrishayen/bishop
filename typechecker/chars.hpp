#pragma once

#include <string>
#include <vector>
#include <optional>

namespace nog {

/**
 * Represents a char method signature with parameter types and return type.
 */
struct CharMethodInfo {
    std::vector<std::string> param_types;
    std::string return_type;
};

/**
 * Returns type information for built-in char methods.
 * Returns nullopt if the method is not found.
 */
std::optional<CharMethodInfo> get_char_method_info(const std::string& method_name);

}  // namespace nog
