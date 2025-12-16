#pragma once

#include <string>
#include <vector>
#include <optional>

namespace nog {

/**
 * Represents a string method signature with parameter types and return type.
 */
struct StrMethodInfo {
    std::vector<std::string> param_types;
    std::string return_type;
};

/**
 * Returns type information for built-in str methods.
 * Returns nullopt if the method is not found.
 */
std::optional<StrMethodInfo> get_str_method_info(const std::string& method_name);

}  // namespace nog
