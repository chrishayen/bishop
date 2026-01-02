/**
 * @file sets.hpp
 * @brief Set method type definitions for the Bishop type checker.
 *
 * Defines type signatures for all built-in Set<T> methods.
 * Uses "T" as a placeholder for the element type, which is
 * substituted with the actual type at type check time.
 */

#pragma once

#include <string>
#include <vector>
#include <optional>

namespace bishop {

/**
 * Represents a set method signature with parameter types and return type.
 * Uses "T" as a placeholder for the element type.
 */
struct SetMethodInfo {
    std::vector<std::string> param_types;
    std::string return_type;
};

/**
 * Returns type information for built-in Set methods.
 * Returns nullopt if the method is not found.
 */
std::optional<SetMethodInfo> get_set_method_info(const std::string& method_name);

}  // namespace bishop
