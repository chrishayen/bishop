/**
 * @file queues.cpp
 * @brief Queue method type definitions for the Bishop type checker.
 *
 * Defines type signatures for all built-in Queue<T> methods.
 * Uses "T" as a placeholder for the element type, which is
 * substituted with the actual type at type check time.
 */

/**
 * @bishop_method push
 * @type Queue<T>
 * @description Adds an element to the back of the queue.
 * @param elem T - The element to add
 * @example
 * q := Queue<int>();
 * q.push(1);
 */

/**
 * @bishop_method pop
 * @type Queue<T>
 * @description Removes and returns the front element (FIFO).
 * @returns T - The removed element
 * @example
 * q := Queue<int>();
 * q.push(1);
 * val := q.pop();  // 1
 */

/**
 * @bishop_method front
 * @type Queue<T>
 * @description Returns the front element without removing it.
 * @returns T - The front element
 * @example
 * q := Queue<int>();
 * q.push(1);
 * val := q.front();  // 1
 */

/**
 * @bishop_method back
 * @type Queue<T>
 * @description Returns the back element without removing it.
 * @returns T - The back element
 * @example
 * q := Queue<int>();
 * q.push(1);
 * val := q.back();  // 1
 */

/**
 * @bishop_method length
 * @type Queue<T>
 * @description Returns the number of elements in the queue.
 * @returns int - The queue size
 * @example
 * q := Queue<int>();
 * q.push(1);
 * len := q.length();  // 1
 */

/**
 * @bishop_method is_empty
 * @type Queue<T>
 * @description Returns true if the queue has no elements.
 * @returns bool - True if empty, false otherwise
 * @example
 * q := Queue<int>();
 * if q.is_empty() { print("Empty"); }
 */

#include "queues.hpp"

#include <map>

namespace bishop {

std::optional<QueueMethodInfo> get_queue_method_info(const std::string& method_name) {
    // "T" is placeholder for element type, substituted at type check time
    static const std::map<std::string, QueueMethodInfo> queue_methods = {
        // Modification methods
        {"push", {{"T"}, "void"}},
        {"pop", {{}, "T"}},

        // Access methods
        {"front", {{}, "T"}},
        {"back", {{}, "T"}},

        // Query methods
        {"length", {{}, "int"}},
        {"is_empty", {{}, "bool"}},
    };

    auto it = queue_methods.find(method_name);

    if (it != queue_methods.end()) {
        return it->second;
    }

    return std::nullopt;
}

}  // namespace bishop
