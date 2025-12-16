#pragma once

#include <string>
#include <vector>
#include <optional>

namespace nog {

/**
 * Represents a list method signature with parameter types and return type.
 * Uses "T" as a placeholder for the element type.
 */
struct ListMethodInfo {
    std::vector<std::string> param_types;
    std::string return_type;
};

/**
 * Returns type information for built-in List methods.
 * Returns nullopt if the method is not found.
 */
std::optional<ListMethodInfo> get_list_method_info(const std::string& method_name);

}  // namespace nog
