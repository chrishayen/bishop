/**
 * @file check_map.cpp
 * @brief Map type inference for the Bishop type checker.
 */

#include "typechecker.hpp"
#include "maps.hpp"

using namespace std;

namespace typechecker {

/**
 * Infers the type of a map creation expression.
 */
TypeInfo check_map_create(TypeCheckerState& state, const MapCreate& map) {
    (void)state;
    return {"Map<" + map.key_type + ", " + map.value_type + ">", false, false};
}

/**
 * Infers the type of a map literal.
 */
TypeInfo check_map_literal(TypeCheckerState& state, const MapLiteral& map) {
    if (map.entries.empty()) {
        error(state, "cannot infer type of empty map literal, use Map<K, V>() instead", map.line);
        return {"unknown", false, false};
    }

    // Infer types from the first entry
    TypeInfo first_key_type = infer_type(state, *map.entries[0].first);
    TypeInfo first_value_type = infer_type(state, *map.entries[0].second);

    // Check all entries have consistent types
    for (size_t i = 1; i < map.entries.size(); i++) {
        TypeInfo key_type = infer_type(state, *map.entries[i].first);
        TypeInfo value_type = infer_type(state, *map.entries[i].second);

        if (key_type.base_type != first_key_type.base_type) {
            error(state, "map literal has mixed key types: '" + format_type(first_key_type) +
                  "' and '" + format_type(key_type) + "'", map.line);
        }

        if (value_type.base_type != first_value_type.base_type) {
            error(state, "map literal has mixed value types: '" + format_type(first_value_type) +
                  "' and '" + format_type(value_type) + "'", map.line);
        }
    }

    return {"Map<" + first_key_type.base_type + ", " + first_value_type.base_type + ">", false, false};
}

/**
 * Type checks a method call on a map.
 */
TypeInfo check_map_method(TypeCheckerState& state, const MethodCall& mcall,
                          const string& key_type, const string& value_type) {
    auto method_info = bishop::get_map_method_info(mcall.method_name);

    if (!method_info) {
        error(state, "Map has no method '" + mcall.method_name + "'", mcall.line);
        return {"unknown", false, false};
    }

    const auto& [param_types, return_type] = *method_info;

    if (mcall.args.size() != param_types.size()) {
        error(state, "method '" + mcall.method_name + "' expects " +
              to_string(param_types.size()) + " arguments, got " +
              to_string(mcall.args.size()), mcall.line);
    }

    // Check argument types
    for (size_t i = 0; i < mcall.args.size() && i < param_types.size(); i++) {
        TypeInfo arg_type = infer_type(state, *mcall.args[i]);
        string expected_str = param_types[i];

        // Replace K and V with actual types
        if (expected_str == "K") {
            expected_str = key_type;
        } else if (expected_str == "V") {
            expected_str = value_type;
        }

        TypeInfo expected_type = {expected_str, false, false};

        if (!types_compatible(expected_type, arg_type)) {
            error(state, "argument " + to_string(i + 1) + " of method '" +
                  mcall.method_name + "' expects '" + expected_str +
                  "', got '" + format_type(arg_type) + "'", mcall.line);
        }
    }

    // Compute return type
    string ret = return_type;

    // Replace K and V placeholders
    if (ret == "K") {
        ret = key_type;
    } else if (ret == "V") {
        ret = value_type;
    } else if (ret == "V?") {
        // Optional value type
        return {value_type, true, false};
    } else if (ret == "List<K>") {
        ret = "List<" + key_type + ">";
    } else if (ret == "List<V>") {
        ret = "List<" + value_type + ">";
    } else if (ret == "List<MapItem<K, V>>") {
        ret = "List<MapItem<" + key_type + ", " + value_type + ">>";
    }

    if (ret == "void") {
        return {"void", false, true};
    }

    return {ret, false, false};
}

/**
 * Type checks field access on a MapItem.
 */
TypeInfo check_map_item_field(TypeCheckerState& state, const FieldAccess& access,
                               const string& key_type, const string& value_type) {
    if (access.field_name == "key") {
        return {key_type, false, false};
    }

    if (access.field_name == "value") {
        return {value_type, false, false};
    }

    error(state, "MapItem has no field '" + access.field_name + "'", access.line);
    return {"unknown", false, false};
}

} // namespace typechecker
