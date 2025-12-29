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
 */
string emit_pair_method_call(CodeGenState& state, const MethodCall& call, const string& obj_str, const vector<string>& args) {
    if (call.method_name == "get") {
        // p.get(index) - returns element at index (0=first, 1=second)
        // This is used with default, so we return the indexed access
        // The default expression wrapper handles out-of-bounds
        return "((" + args[0] + ") == 0 ? " + obj_str + ".first : " + obj_str + ".second)";
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
