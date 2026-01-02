/**
 * @file emit_set.cpp
 * @brief Set code generation for the Bishop code generator.
 *
 * Generates C++ code for Set<T> creation, literals, and method calls.
 * Sets are implemented using std::unordered_set.
 */

#include "codegen.hpp"
#include <fmt/format.h>

using namespace std;

namespace codegen {

/**
 * Emits a set creation expression: Set<int>() -> std::unordered_set<int>{}
 */
string emit_set_create(const SetCreate& set) {
    return fmt::format("std::unordered_set<{}>{{}}",
                       map_type(set.element_type));
}

/**
 * Emits a set literal expression: {1, 2, 3} -> std::unordered_set<int>{1, 2, 3}
 */
string emit_set_literal(CodeGenState& state, const SetLiteral& set) {
    if (set.elements.empty()) {
        // This shouldn't happen due to parser validation, but handle it
        return "std::unordered_set<void>{}";
    }

    // Infer element type from first element
    string first_elem = emit(state, *set.elements[0]);

    vector<string> elems;
    for (const auto& elem : set.elements) {
        elems.push_back(emit(state, *elem));
    }

    string elements_str;
    for (size_t i = 0; i < elems.size(); i++) {
        if (i > 0) elements_str += ", ";
        elements_str += elems[i];
    }

    // The type will be deduced from the elements using initializer_list
    return fmt::format("std::unordered_set{{{}}}", elements_str);
}

/**
 * Emits a method call on a set.
 */
string emit_set_method_call(CodeGenState& state, const MethodCall& call,
                            const string& obj_str, const vector<string>& args) {
    (void)state;  // Unused
    const string& method = call.method_name;

    // Query methods
    if (method == "length") {
        return fmt::format("{}.size()", obj_str);
    }

    if (method == "is_empty") {
        return fmt::format("{}.empty()", obj_str);
    }

    if (method == "contains") {
        return fmt::format("{}.count({}) > 0", obj_str, args[0]);
    }

    // Modification methods
    if (method == "add") {
        return fmt::format("{}.insert({})", obj_str, args[0]);
    }

    if (method == "remove") {
        return fmt::format("{}.erase({}) > 0", obj_str, args[0]);
    }

    if (method == "clear") {
        return fmt::format("{}.clear()", obj_str);
    }

    // Set operations - these create new sets
    if (method == "union") {
        // Create a new set with elements from both
        // Note: We use a lambda to create and return the union set
        return fmt::format(
            "[&]() {{ auto result = {}; for (const auto& e : {}) result.insert(e); return result; }}()",
            obj_str, args[0]);
    }

    if (method == "intersection") {
        // Create a new set with common elements
        return fmt::format(
            "[&]() {{ std::remove_cvref_t<decltype({})> result; for (const auto& e : {}) if ({}.count(e)) result.insert(e); return result; }}()",
            obj_str, obj_str, args[0]);
    }

    if (method == "difference") {
        // Create a new set with elements in this but not in other
        return fmt::format(
            "[&]() {{ std::remove_cvref_t<decltype({})> result; for (const auto& e : {}) if (!{}.count(e)) result.insert(e); return result; }}()",
            obj_str, obj_str, args[0]);
    }

    if (method == "symmetric_difference") {
        // Create a new set with elements in either but not both
        return fmt::format(
            "[&]() {{ std::remove_cvref_t<decltype({})> result; for (const auto& e : {}) if (!{}.count(e)) result.insert(e); for (const auto& e : {}) if (!{}.count(e)) result.insert(e); return result; }}()",
            obj_str, obj_str, args[0], args[0], obj_str);
    }

    // Set predicates
    if (method == "is_subset") {
        return fmt::format(
            "[&]() {{ for (const auto& e : {}) if (!{}.count(e)) return false; return true; }}()",
            obj_str, args[0]);
    }

    if (method == "is_superset") {
        return fmt::format(
            "[&]() {{ for (const auto& e : {}) if (!{}.count(e)) return false; return true; }}()",
            args[0], obj_str);
    }

    // Unknown method - should be caught by typechecker
    return fmt::format("{}.{}()", obj_str, method);
}

} // namespace codegen
