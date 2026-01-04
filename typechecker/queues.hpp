/**
 * @file queues.hpp
 * @brief Queue method type information for the Bishop type checker.
 */

#pragma once

#include <string>
#include <vector>
#include <optional>

namespace bishop {

/**
 * Represents a queue method signature with parameter types and return type.
 * Uses "T" as a placeholder for the element type.
 */
struct QueueMethodInfo {
    std::vector<std::string> param_types;
    std::string return_type;
};

/**
 * Returns type information for built-in Queue methods.
 * Returns nullopt if the method is not found.
 */
std::optional<QueueMethodInfo> get_queue_method_info(const std::string& method_name);

}  // namespace bishop
