/**
 * @file tuples.cpp
 * @brief Tuple method type definitions for the Bishop type checker.
 *
 * Defines type signatures for all built-in Tuple<T> methods.
 * Uses "T" as a placeholder for the element type, which is
 * substituted with the actual type at type check time.
 */

/**
 * @nog_method get
 * @type Tuple<T>
 * @description Returns the element at the specified index (0 to size-1).
 * @param index int - The index (0-based)
 * @returns T - The element at that position
 * @example
 * t := Tuple<int>(10, 20, 30);
 * val := t.get(1) default 0;  // 20
 */

#include "tuples.hpp"

#include <map>

namespace nog {

std::optional<TupleMethodInfo> get_tuple_method_info(const std::string& method_name) {
    // "T" is placeholder for element type, substituted at type check time
    static const std::map<std::string, TupleMethodInfo> tuple_methods = {
        // Access method with default
        {"get", {{"int"}, "T"}},
    };

    auto it = tuple_methods.find(method_name);

    if (it != tuple_methods.end()) {
        return it->second;
    }

    return std::nullopt;
}

}  // namespace nog
