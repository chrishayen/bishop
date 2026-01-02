#pragma once

#include <string>
#include <vector>
#include <optional>

namespace bishop {

/**
 * Represents a map method signature with parameter types and return type.
 * Uses "K" as a placeholder for the key type and "V" for the value type.
 */
struct MapMethodInfo {
    std::vector<std::string> param_types;
    std::string return_type;
};

/**
 * Returns type information for built-in Map methods.
 * Returns nullopt if the method is not found.
 */
std::optional<MapMethodInfo> get_map_method_info(const std::string& method_name);

}  // namespace bishop
