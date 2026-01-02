/**
 * @file maps.cpp
 * @brief Map method type definitions for the Bishop type checker.
 *
 * Defines type signatures for all built-in Map<K, V> methods.
 * Uses "K" as a placeholder for the key type and "V" for the value type,
 * which are substituted with the actual types at type check time.
 */

/**
 * @bishop_method length
 * @type Map<K, V>
 * @description Returns the number of entries in the map.
 * @returns int - The map length
 * @example
 * ages := {"alice": 30, "bob": 25};
 * len := ages.length();  // 2
 */

/**
 * @bishop_method is_empty
 * @type Map<K, V>
 * @description Returns true if the map has no entries.
 * @returns bool - True if empty, false otherwise
 * @example
 * ages := Map<str, int>();
 * if ages.is_empty() {
 *     print("Map is empty");
 * }
 */

/**
 * @bishop_method contains
 * @type Map<K, V>
 * @description Checks if the map contains the given key.
 * @param key K - The key to search for
 * @returns bool - True if found, false otherwise
 * @example
 * ages := {"alice": 30, "bob": 25};
 * if ages.contains("alice") {
 *     print("Found alice!");
 * }
 */

/**
 * @bishop_method get
 * @type Map<K, V>
 * @description Returns the value for the given key, or none if not found.
 * @param key K - The key to look up
 * @returns V? - The value (optional)
 * @example
 * ages := {"alice": 30, "bob": 25};
 * age := ages.get("alice") default 0;  // 30
 */

/**
 * @bishop_method set
 * @type Map<K, V>
 * @description Sets the value for the given key.
 * @param key K - The key
 * @param value V - The value to store
 * @example
 * ages := Map<str, int>();
 * ages.set("alice", 30);
 */

/**
 * @bishop_method remove
 * @type Map<K, V>
 * @description Removes the entry for the given key.
 * @param key K - The key to remove
 * @example
 * ages := {"alice": 30, "bob": 25};
 * ages.remove("bob");  // ages now only has alice
 */

/**
 * @bishop_method clear
 * @type Map<K, V>
 * @description Removes all entries from the map.
 * @example
 * ages := {"alice": 30, "bob": 25};
 * ages.clear();  // ages is now empty
 */

/**
 * @bishop_method keys
 * @type Map<K, V>
 * @description Returns a list of all keys in the map.
 * @returns List<K> - List of keys
 * @example
 * ages := {"alice": 30, "bob": 25};
 * for key in ages.keys() {
 *     print(key);
 * }
 */

/**
 * @bishop_method values
 * @type Map<K, V>
 * @description Returns a list of all values in the map.
 * @returns List<V> - List of values
 * @example
 * ages := {"alice": 30, "bob": 25};
 * for val in ages.values() {
 *     print(val);
 * }
 */

/**
 * @bishop_method items
 * @type Map<K, V>
 * @description Returns a list of all key-value pairs in the map.
 * @returns List<MapItem<K, V>> - List of entries with .key and .value fields
 * @example
 * ages := {"alice": 30, "bob": 25};
 * for item in ages.items() {
 *     print(item.key + ": " + str(item.value));
 * }
 */

#include "maps.hpp"

#include <map>

namespace bishop {

std::optional<MapMethodInfo> get_map_method_info(const std::string& method_name) {
    // "K" is placeholder for key type, "V" for value type
    static const std::map<std::string, MapMethodInfo> map_methods = {
        // Query methods
        {"length", {{}, "int"}},
        {"is_empty", {{}, "bool"}},
        {"contains", {{"K"}, "bool"}},

        // Access methods - get returns optional
        {"get", {{"K"}, "V?"}},

        // Modification methods
        {"set", {{"K", "V"}, "void"}},
        {"remove", {{"K"}, "void"}},
        {"clear", {{}, "void"}},

        // Iteration methods
        {"keys", {{}, "List<K>"}},
        {"values", {{}, "List<V>"}},
        {"items", {{}, "List<MapItem<K, V>>"}},
    };

    auto it = map_methods.find(method_name);

    if (it != map_methods.end()) {
        return it->second;
    }

    return std::nullopt;
}

}  // namespace bishop
