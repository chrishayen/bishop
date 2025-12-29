/**
 * @file emit_pair.cpp
 * @brief Pair emission for the Bishop code generator.
 */

#include "codegen.hpp"
#include <fmt/format.h>

using namespace std;

namespace codegen {

/**
 * Emits a pair creation: Pair<T>(a, b) -> std::make_pair(a, b).
 */
string emit_pair_create(CodeGenState& state, const PairCreate& pair) {
    string first = emit(state, *pair.first);
    string second = emit(state, *pair.second);
    return "std::make_pair(" + first + ", " + second + ")";
}

/**
 * Emits a pair method call, mapping Bishop methods to std::pair equivalents.
 *
 * Note: When get() is used with the `default` expression pattern, bounds
 * checking is handled by emit_default_expr in emit_or.cpp. When used
 * without `default`, invalid indices throw std::out_of_range.
 */
string emit_pair_method_call(CodeGenState& state, const MethodCall& call, const string& obj_str, const vector<string>& args) {
    if (call.method_name == "get") {
        // p.get(index) - returns element at index (0=first, 1=second)
        // Throw std::out_of_range for invalid indices when not using default
        return "([&]() { int _idx = " + args[0] + "; if (_idx == 0) return " + obj_str +
               ".first; if (_idx == 1) return " + obj_str +
               ".second; throw std::out_of_range(\"Pair index out of range\"); }())";
    }

    // Unknown pair method - fall back to generic method call
    return method_call(obj_str, call.method_name, args);
}

/**
 * Emits a pair field access (first or second).
 */
string emit_pair_field_access(const string& obj_str, const string& field_name) {
    if (field_name == "first") {
        return obj_str + ".first";
    }

    if (field_name == "second") {
        return obj_str + ".second";
    }

    // Unknown field - shouldn't happen if typechecker did its job
    return obj_str + "." + field_name;
}

} // namespace codegen
