/**
 * @file priority_queues.cpp
 * @brief PriorityQueue method type definitions for the Bishop type checker.
 *
 * Defines type signatures for all built-in PriorityQueue<T> methods.
 * Uses "T" as a placeholder for the element type, which is
 * substituted with the actual type at type check time.
 */

/**
 * @bishop_method push
 * @type PriorityQueue<T>
 * @description Adds an element to the priority queue.
 * @param elem T - The element to add
 * @example
 * pq := PriorityQueue<int>();
 * pq.push(10);
 * pq.push(5);
 * pq.push(20);  // queue now has 3 elements
 */

/**
 * @bishop_method pop
 * @type PriorityQueue<T>
 * @description Removes and returns the highest priority element.
 * For max heap (default), returns largest element.
 * For min heap (.min()), returns smallest element.
 * @returns T - The highest priority element
 * @example
 * pq := PriorityQueue<int>();
 * pq.push(10);
 * pq.push(5);
 * pq.push(20);
 * val := pq.pop();  // 20 (max heap)
 */

/**
 * @bishop_method top
 * @type PriorityQueue<T>
 * @description Returns the highest priority element without removing it.
 * @returns T - The highest priority element
 * @example
 * pq := PriorityQueue<int>();
 * pq.push(10);
 * pq.push(20);
 * val := pq.top();  // 20
 * // pq still has 2 elements
 */

/**
 * @bishop_method length
 * @type PriorityQueue<T>
 * @description Returns the number of elements in the priority queue.
 * @returns int - The queue length
 * @example
 * pq := PriorityQueue<int>();
 * pq.push(10);
 * pq.push(20);
 * len := pq.length();  // 2
 */

/**
 * @bishop_method is_empty
 * @type PriorityQueue<T>
 * @description Returns true if the priority queue has no elements.
 * @returns bool - True if empty, false otherwise
 * @example
 * pq := PriorityQueue<int>();
 * if pq.is_empty() {
 *     print("Queue is empty");
 * }
 */

#include "priority_queues.hpp"

#include <map>

namespace bishop {

std::optional<PriorityQueueMethodInfo> get_priority_queue_method_info(const std::string& method_name) {
    // "T" is placeholder for element type, substituted at type check time
    static const std::map<std::string, PriorityQueueMethodInfo> pq_methods = {
        // Query methods
        {"length", {{}, "int"}},
        {"is_empty", {{}, "bool"}},

        // Access methods
        {"top", {{}, "T"}},

        // Modification methods
        {"push", {{"T"}, "void"}},
        {"pop", {{}, "T"}},
    };

    auto it = pq_methods.find(method_name);

    if (it != pq_methods.end()) {
        return it->second;
    }

    return std::nullopt;
}

}  // namespace bishop
