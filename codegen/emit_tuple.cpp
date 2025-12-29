/**
 * @file emit_tuple.cpp
 * @brief Tuple emission for the Bishop code generator.
 */

#include "codegen.hpp"
#include <fmt/format.h>

using namespace std;

namespace codegen {

/**
 * Emits a tuple creation: Tuple<T>(v1, v2, ...) -> std::vector<T>{v1, v2, ...}.
 */
string emit_tuple_create(CodeGenState& state, const TupleCreate& tuple) {
    string cpp_type = map_type(tuple.element_type);
    string elements;

    for (size_t i = 0; i < tuple.elements.size(); i++) {
        if (i > 0) {
            elements += ", ";
        }

        elements += emit(state, *tuple.elements[i]);
    }

    return "std::vector<" + cpp_type + ">{" + elements + "}";
}

/**
 * Emits a tuple method call, mapping Bishop methods to std::vector equivalents.
 */
string emit_tuple_method_call(CodeGenState& state, const MethodCall& call, const string& obj_str, const vector<string>& args) {
    if (call.method_name == "get") {
        // t.get(index) - returns element at index
        // This is used with default, so we need bounds checking
        // The at() method throws on out of bounds, but we want default value
        // We'll use a conditional expression
        return "(static_cast<size_t>(" + args[0] + ") < " + obj_str + ".size() && " + args[0] + " >= 0 ? " + obj_str + "[" + args[0] + "] : " + obj_str + "[0])";
    }

    // Unknown tuple method - fall back to generic method call
    return method_call(obj_str, call.method_name, args);
}

} // namespace codegen
