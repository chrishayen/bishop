/**
 * @file deques.cpp
 * @brief Deque method type definitions for the Bishop type checker.
 *
 * Defines type signatures for all built-in Deque<T> methods.
 * Uses "T" as a placeholder for the element type, which is
 * substituted with the actual type at type check time.
 */

/**
 * @bishop_method push_front
 * @type Deque<T>
 * @description Adds an element to the front of the deque.
 * @param elem T - The element to add
 * @example
 * dq := Deque<int>();
 * dq.push_front(1);
 */

/**
 * @bishop_method push_back
 * @type Deque<T>
 * @description Adds an element to the back of the deque.
 * @param elem T - The element to add
 * @example
 * dq := Deque<int>();
 * dq.push_back(1);
 */

/**
 * @bishop_method pop_front
 * @type Deque<T>
 * @description Removes and returns the front element.
 * @returns T - The removed element
 * @example
 * dq := Deque<int>();
 * dq.push_back(1);
 * val := dq.pop_front();  // 1
 */

/**
 * @bishop_method pop_back
 * @type Deque<T>
 * @description Removes and returns the back element.
 * @returns T - The removed element
 * @example
 * dq := Deque<int>();
 * dq.push_back(1);
 * val := dq.pop_back();  // 1
 */

/**
 * @bishop_method front
 * @type Deque<T>
 * @description Returns the front element without removing it.
 * @returns T - The front element
 * @example
 * dq := Deque<int>();
 * dq.push_back(1);
 * val := dq.front();  // 1
 */

/**
 * @bishop_method back
 * @type Deque<T>
 * @description Returns the back element without removing it.
 * @returns T - The back element
 * @example
 * dq := Deque<int>();
 * dq.push_back(1);
 * val := dq.back();  // 1
 */

/**
 * @bishop_method get
 * @type Deque<T>
 * @description Returns the element at the specified index.
 * @param index int - The index (0-based)
 * @returns T - The element at that position
 * @example
 * dq := Deque<int>();
 * dq.push_back(10);
 * dq.push_back(20);
 * val := dq.get(1);  // 20
 */

/**
 * @bishop_method length
 * @type Deque<T>
 * @description Returns the number of elements in the deque.
 * @returns int - The deque length
 * @example
 * dq := Deque<int>();
 * dq.push_back(1);
 * len := dq.length();  // 1
 */

/**
 * @bishop_method is_empty
 * @type Deque<T>
 * @description Returns true if the deque has no elements.
 * @returns bool - True if empty, false otherwise
 * @example
 * dq := Deque<int>();
 * if dq.is_empty() { print("Empty"); }
 */

/**
 * @bishop_method clear
 * @type Deque<T>
 * @description Removes all elements from the deque.
 * @example
 * dq := Deque<int>();
 * dq.push_back(1);
 * dq.clear();  // dq is now empty
 */

#include "deques.hpp"

#include <map>

namespace bishop {

std::optional<DequeMethodInfo> get_deque_method_info(const std::string& method_name) {
    // "T" is placeholder for element type, substituted at type check time
    static const std::map<std::string, DequeMethodInfo> deque_methods = {
        // Modification methods
        {"push_front", {{"T"}, "void"}},
        {"push_back", {{"T"}, "void"}},
        {"pop_front", {{}, "T"}},
        {"pop_back", {{}, "T"}},
        {"clear", {{}, "void"}},

        // Access methods
        {"front", {{}, "T"}},
        {"back", {{}, "T"}},
        {"get", {{"int"}, "T"}},

        // Query methods
        {"length", {{}, "int"}},
        {"is_empty", {{}, "bool"}},
    };

    auto it = deque_methods.find(method_name);

    if (it != deque_methods.end()) {
        return it->second;
    }

    return std::nullopt;
}

}  // namespace bishop
