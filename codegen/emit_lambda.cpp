/**
 * @file emit_lambda.cpp
 * @brief Lambda expression emission for the Bishop code generator.
 */

#include "codegen.hpp"
#include <fmt/format.h>

using namespace std;

namespace codegen {

/**
 * Emits C++ code for a lambda expression.
 * Generates a C++ lambda with capture-by-reference for closure semantics.
 */
string emit_lambda_expr(CodeGenState& state, const LambdaExpr& lambda) {
    string out = "[&](";

    // Emit parameters
    bool first = true;

    for (const auto& param : lambda.params) {
        if (!first) {
            out += ", ";
        }
        first = false;

        out += map_type(param.type) + " " + param.name;
    }

    out += ")";

    // Emit return type if present
    if (!lambda.return_type.empty()) {
        out += " -> " + map_type(lambda.return_type);
    }

    out += " {\n";

    // Emit body statements
    for (const auto& stmt : lambda.body) {
        out += "\t\t" + generate_statement(state, *stmt) + "\n";
    }

    out += "\t}";

    return out;
}

} // namespace codegen
