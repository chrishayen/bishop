/**
 * @file priority_queues.hpp
 * @brief PriorityQueue method type information for the Bishop type checker.
 */

#pragma once

#include <string>
#include <vector>
#include <optional>

namespace bishop {

/**
 * Represents a priority queue method signature with parameter types and return type.
 * Uses "T" as a placeholder for the element type.
 */
struct PriorityQueueMethodInfo {
    std::vector<std::string> param_types;
    std::string return_type;
};

/**
 * Returns type information for built-in PriorityQueue methods.
 * Returns nullopt if the method is not found.
 */
std::optional<PriorityQueueMethodInfo> get_priority_queue_method_info(const std::string& method_name);

}  // namespace bishop
