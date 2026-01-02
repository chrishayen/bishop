/**
 * @file emit_map.cpp
 * @brief Map emission for the Bishop code generator.
 */

#include "codegen.hpp"
#include <fmt/format.h>

using namespace std;

namespace codegen {

/**
 * Emits a map creation: Map<K, V>() -> std::unordered_map<K, V>{}.
 */
string emit_map_create(const MapCreate& map) {
    string cpp_key_type = map_type(map.key_type);
    string cpp_value_type = map_type(map.value_type);
    return "std::unordered_map<" + cpp_key_type + ", " + cpp_value_type + ">{}";
}

/**
 * Emits a map literal: {"key": value, ...} -> std::unordered_map{std::make_pair(key, value), ...}.
 *
 * Uses std::make_pair for each entry to help class template argument deduction (CTAD)
 * correctly infer the map types.
 */
string emit_map_literal(CodeGenState& state, const MapLiteral& map) {
    string entries;

    for (size_t i = 0; i < map.entries.size(); i++) {
        if (i > 0) {
            entries += ", ";
        }

        string key = emit(state, *map.entries[i].first);
        string value = emit(state, *map.entries[i].second);
        entries += "std::make_pair(" + key + ", " + value + ")";
    }

    return "std::unordered_map{" + entries + "}";
}

/**
 * Emits a map method call, mapping Bishop methods to std::unordered_map equivalents.
 */
string emit_map_method_call(CodeGenState& state, const MethodCall& call, const string& obj_str, const vector<string>& args) {
    if (call.method_name == "length") {
        return obj_str + ".size()";
    }

    if (call.method_name == "is_empty") {
        return obj_str + ".empty()";
    }

    if (call.method_name == "contains") {
        return "(" + obj_str + ".find(" + args[0] + ") != " + obj_str + ".end())";
    }

    if (call.method_name == "get") {
        // Returns optional - uses find and returns std::optional
        return fmt::format(
            "[](const auto& m, const auto& key) -> std::optional<std::decay_t<decltype(m.begin()->second)>> {{ "
            "auto it = m.find(key); "
            "if (it != m.end()) return it->second; "
            "return std::nullopt; "
            "}}({}, {})",
            obj_str, args[0]
        );
    }

    if (call.method_name == "set") {
        return obj_str + "[" + args[0] + "] = " + args[1];
    }

    if (call.method_name == "remove") {
        return obj_str + ".erase(" + args[0] + ")";
    }

    if (call.method_name == "clear") {
        return obj_str + ".clear()";
    }

    if (call.method_name == "keys") {
        // Returns a vector of keys
        return fmt::format(
            "[](const auto& m) {{ "
            "std::vector<std::decay_t<decltype(m.begin()->first)>> keys; "
            "keys.reserve(m.size()); "
            "for (const auto& [k, v] : m) keys.push_back(k); "
            "return keys; "
            "}}({})",
            obj_str
        );
    }

    if (call.method_name == "values") {
        // Returns a vector of values
        return fmt::format(
            "[](const auto& m) {{ "
            "std::vector<std::decay_t<decltype(m.begin()->second)>> values; "
            "values.reserve(m.size()); "
            "for (const auto& [k, v] : m) values.push_back(v); "
            "return values; "
            "}}({})",
            obj_str
        );
    }

    if (call.method_name == "items") {
        // Returns a vector of MapItem structs with key and value fields
        return fmt::format(
            "[](const auto& m) {{ "
            "struct MapItem {{ "
            "std::decay_t<decltype(m.begin()->first)> key; "
            "std::decay_t<decltype(m.begin()->second)> value; "
            "}}; "
            "std::vector<MapItem> items; "
            "items.reserve(m.size()); "
            "for (const auto& [k, v] : m) items.push_back({{k, v}}); "
            "return items; "
            "}}({})",
            obj_str
        );
    }

    // Unknown map method - fall back to generic method call
    return method_call(obj_str, call.method_name, args);
}

} // namespace codegen
