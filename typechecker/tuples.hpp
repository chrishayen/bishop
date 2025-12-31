#pragma once

#include <string>
#include <vector>
#include <optional>

namespace bishop {

/**
 * Represents a tuple method signature with parameter types and return type.
 * Uses "T" as a placeholder for the element type.
 */
struct TupleMethodInfo {
    std::vector<std::string> param_types;
    std::string return_type;
};

/**
 * Returns type information for built-in Tuple methods.
 * Returns nullopt if the method is not found.
 */
std::optional<TupleMethodInfo> get_tuple_method_info(const std::string& method_name);

}  // namespace bishop
