#pragma once

#include <string>
#include <vector>
#include <optional>

namespace nog {

/**
 * Represents a pair method signature with parameter types and return type.
 * Uses "T" as a placeholder for the element type.
 */
struct PairMethodInfo {
    std::vector<std::string> param_types;
    std::string return_type;
};

/**
 * Returns type information for built-in Pair methods.
 * Returns nullopt if the method is not found.
 */
std::optional<PairMethodInfo> get_pair_method_info(const std::string& method_name);

}  // namespace nog
