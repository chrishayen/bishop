/**
 * @file sets.cpp
 * @brief Set method type definitions for the Bishop type checker.
 *
 * Defines type signatures for all built-in Set<T> methods.
 * Uses "T" as a placeholder for the element type, which is
 * substituted with the actual type at type check time.
 */

/**
 * @bishop_method add
 * @type Set<T>
 * @description Adds an element to the set.
 * @param elem T - The element to add
 * @example
 * s := Set<int>();
 * s.add(42);
 */

/**
 * @bishop_method remove
 * @type Set<T>
 * @description Removes an element from the set.
 * @param elem T - The element to remove
 * @returns bool - True if element was found and removed
 * @example
 * s := {1, 2, 3};
 * s.remove(2);  // returns true
 */

/**
 * @bishop_method contains
 * @type Set<T>
 * @description Checks if the set contains the given element.
 * @param elem T - The element to search for
 * @returns bool - True if found, false otherwise
 * @example
 * s := {1, 2, 3};
 * if s.contains(2) {
 *     print("Found it!");
 * }
 */

/**
 * @bishop_method length
 * @type Set<T>
 * @description Returns the number of elements in the set.
 * @returns int - The set size
 * @example
 * s := {1, 2, 3};
 * len := s.length();  // 3
 */

/**
 * @bishop_method is_empty
 * @type Set<T>
 * @description Returns true if the set has no elements.
 * @returns bool - True if empty, false otherwise
 * @example
 * s := Set<int>();
 * if s.is_empty() {
 *     print("Set is empty");
 * }
 */

/**
 * @bishop_method clear
 * @type Set<T>
 * @description Removes all elements from the set.
 * @example
 * s := {1, 2, 3};
 * s.clear();  // set is now empty
 */

/**
 * @bishop_method union
 * @type Set<T>
 * @description Returns a new set with elements from both sets.
 * @param other Set<T> - The other set
 * @returns Set<T> - New set containing all elements from both
 * @example
 * a := {1, 2, 3};
 * b := {3, 4, 5};
 * c := a.union(b);  // {1, 2, 3, 4, 5}
 */

/**
 * @bishop_method intersection
 * @type Set<T>
 * @description Returns a new set with elements common to both sets.
 * @param other Set<T> - The other set
 * @returns Set<T> - New set containing only common elements
 * @example
 * a := {1, 2, 3};
 * b := {2, 3, 4};
 * c := a.intersection(b);  // {2, 3}
 */

/**
 * @bishop_method difference
 * @type Set<T>
 * @description Returns a new set with elements in this set but not in the other.
 * @param other Set<T> - The other set
 * @returns Set<T> - New set containing elements only in this set
 * @example
 * a := {1, 2, 3};
 * b := {2, 3, 4};
 * c := a.difference(b);  // {1}
 */

/**
 * @bishop_method symmetric_difference
 * @type Set<T>
 * @description Returns a new set with elements in either set but not both.
 * @param other Set<T> - The other set
 * @returns Set<T> - New set containing elements exclusive to each set
 * @example
 * a := {1, 2, 3};
 * b := {2, 3, 4};
 * c := a.symmetric_difference(b);  // {1, 4}
 */

/**
 * @bishop_method is_subset
 * @type Set<T>
 * @description Checks if this set is a subset of the other set.
 * @param other Set<T> - The other set
 * @returns bool - True if all elements are in the other set
 * @example
 * a := {1, 2};
 * b := {1, 2, 3};
 * a.is_subset(b);  // true
 */

/**
 * @bishop_method is_superset
 * @type Set<T>
 * @description Checks if this set is a superset of the other set.
 * @param other Set<T> - The other set
 * @returns bool - True if this set contains all elements from the other
 * @example
 * a := {1, 2, 3};
 * b := {1, 2};
 * a.is_superset(b);  // true
 */

#include "sets.hpp"

#include <map>

namespace bishop {

std::optional<SetMethodInfo> get_set_method_info(const std::string& method_name) {
    // "T" is placeholder for element type, substituted at type check time
    // "Set<T>" is placeholder for set type with same element type
    static const std::map<std::string, SetMethodInfo> set_methods = {
        // Query methods
        {"length", {{}, "int"}},
        {"is_empty", {{}, "bool"}},
        {"contains", {{"T"}, "bool"}},

        // Modification methods
        {"add", {{"T"}, "void"}},
        {"remove", {{"T"}, "bool"}},
        {"clear", {{}, "void"}},

        // Set operations - return new Set<T>
        {"union", {{"Set<T>"}, "Set<T>"}},
        {"intersection", {{"Set<T>"}, "Set<T>"}},
        {"difference", {{"Set<T>"}, "Set<T>"}},
        {"symmetric_difference", {{"Set<T>"}, "Set<T>"}},

        // Set predicates
        {"is_subset", {{"Set<T>"}, "bool"}},
        {"is_superset", {{"Set<T>"}, "bool"}},
    };

    auto it = set_methods.find(method_name);

    if (it != set_methods.end()) {
        return it->second;
    }

    return std::nullopt;
}

}  // namespace bishop
