/**
 * @file pairs.cpp
 * @brief Pair method type definitions for the Bishop type checker.
 *
 * Defines type signatures for all built-in Pair<T> methods.
 * Uses "T" as a placeholder for the element type, which is
 * substituted with the actual type at type check time.
 */

/**
 * @nog_method get
 * @type Pair<T>
 * @description Returns the element at the specified index (0 or 1).
 * @param index int - The index (0 for first, 1 for second)
 * @returns T - The element at that position
 * @example
 * p := Pair<int>(10, 20);
 * val := p.get(0) default 0;  // 10
 */

#include "pairs.hpp"

#include <map>

namespace bishop {

std::optional<PairMethodInfo> get_pair_method_info(const std::string& method_name) {
    // "T" is placeholder for element type, substituted at type check time
    static const std::map<std::string, PairMethodInfo> pair_methods = {
        // Access method with default
        {"get", {{"int"}, "T"}},
    };

    auto it = pair_methods.find(method_name);

    if (it != pair_methods.end()) {
        return it->second;
    }

    return std::nullopt;
}

}  // namespace bishop
