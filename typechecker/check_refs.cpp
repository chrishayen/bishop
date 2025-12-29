/**
 * @file check_refs.cpp
 * @brief Reference type inference for the Bishop type checker.
 */

#include "typechecker.hpp"

using namespace std;

namespace typechecker {

/**
 * Infers the type of a variable reference.
 * Checks local scope first, then module-level constants.
 */
TypeInfo check_variable_ref(TypeCheckerState& state, const VariableRef& var) {
    const TypeInfo* local = lookup_local(state, var.name);

    if (local) {
        return *local;
    }

    // Check module-level constants
    const TypeInfo* module_const = get_module_constant(state, var.name);

    if (module_const) {
        return *module_const;
    }

    error(state, "undefined variable '" + var.name + "'", var.line);
    return {"unknown", false, false};
}

/**
 * Infers the type of a function reference.
 */
TypeInfo check_function_ref(TypeCheckerState& state, const FunctionRef& fref) {
    size_t dot_pos = fref.name.find('.');

    if (dot_pos != string::npos) {
        string module_name = fref.name.substr(0, dot_pos);
        string func_name = fref.name.substr(dot_pos + 1);
        const FunctionDef* func = get_qualified_function(state, module_name, func_name);

        if (!func) {
            error(state, "undefined function '" + fref.name + "'", fref.line);
            return {"unknown", false, false};
        }

        return {"fn:" + fref.name, false, false};
    }

    if (state.functions.find(fref.name) == state.functions.end()) {
        error(state, "undefined function '" + fref.name + "'", fref.line);
        return {"unknown", false, false};
    }

    return {"fn:" + fref.name, false, false};
}

/**
 * Infers the type of a qualified reference.
 * Checks for qualified structs, qualified constants, and qualified functions.
 */
TypeInfo check_qualified_ref(TypeCheckerState& state, const QualifiedRef& qref) {
    const StructDef* s = get_qualified_struct(state, qref.module_name, qref.name);

    if (s) {
        return {qref.module_name + "." + qref.name, false, false};
    }

    // Check for qualified constant
    const TypeInfo* c = get_qualified_constant(state, qref.module_name, qref.name);

    if (c) {
        return *c;
    }

    // Check for qualified function (for function references as arguments)
    const FunctionDef* func = get_qualified_function(state, qref.module_name, qref.name);

    if (func) {
        return {"fn:" + qref.module_name + "." + qref.name, false, false};
    }

    error(state, "undefined reference '" + qref.module_name + "." + qref.name + "'", qref.line);
    return {"unknown", false, false};
}

} // namespace typechecker
