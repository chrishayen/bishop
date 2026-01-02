/**
 * @file stacks.cpp
 * @brief Stack method type definitions for the Bishop type checker.
 *
 * Defines type signatures for all built-in Stack<T> methods.
 * Uses "T" as a placeholder for the element type, which is
 * substituted with the actual type at type check time.
 */

/**
 * @bishop_method push
 * @type Stack<T>
 * @description Pushes an element onto the top of the stack.
 * @param elem T - The element to push
 * @example
 * s := Stack<int>();
 * s.push(1);
 */

/**
 * @bishop_method pop
 * @type Stack<T>
 * @description Removes and returns the top element (LIFO).
 * @returns T - The removed element
 * @example
 * s := Stack<int>();
 * s.push(1);
 * val := s.pop();  // 1
 */

/**
 * @bishop_method top
 * @type Stack<T>
 * @description Returns the top element without removing it.
 * @returns T - The top element
 * @example
 * s := Stack<int>();
 * s.push(1);
 * val := s.top();  // 1
 */

/**
 * @bishop_method length
 * @type Stack<T>
 * @description Returns the number of elements in the stack.
 * @returns int - The stack size
 * @example
 * s := Stack<int>();
 * s.push(1);
 * len := s.length();  // 1
 */

/**
 * @bishop_method is_empty
 * @type Stack<T>
 * @description Returns true if the stack has no elements.
 * @returns bool - True if empty, false otherwise
 * @example
 * s := Stack<int>();
 * if s.is_empty() { print("Empty"); }
 */

#include "stacks.hpp"

#include <map>

namespace bishop {

std::optional<StackMethodInfo> get_stack_method_info(const std::string& method_name) {
    // "T" is placeholder for element type, substituted at type check time
    static const std::map<std::string, StackMethodInfo> stack_methods = {
        // Modification methods
        {"push", {{"T"}, "void"}},
        {"pop", {{}, "T"}},

        // Access methods
        {"top", {{}, "T"}},

        // Query methods
        {"length", {{}, "int"}},
        {"is_empty", {{}, "bool"}},
    };

    auto it = stack_methods.find(method_name);

    if (it != stack_methods.end()) {
        return it->second;
    }

    return std::nullopt;
}

}  // namespace bishop
