/**
 * @file deques.hpp
 * @brief Deque method type information for the Bishop type checker.
 */

#pragma once

#include <string>
#include <vector>
#include <optional>

namespace bishop {

/**
 * Represents a deque method signature with parameter types and return type.
 * Uses "T" as a placeholder for the element type.
 */
struct DequeMethodInfo {
    std::vector<std::string> param_types;
    std::string return_type;
};

/**
 * Returns type information for built-in Deque methods.
 * Returns nullopt if the method is not found.
 */
std::optional<DequeMethodInfo> get_deque_method_info(const std::string& method_name);

}  // namespace bishop
