/**
 * @file check_field.cpp
 * @brief Field access type inference for the Bishop type checker.
 */

#include "typechecker.hpp"

using namespace std;

namespace typechecker {

/**
 * Infers the type of a field access expression.
 * Handles module constant access (module.CONST) and struct field access.
 * Auto-dereferences pointer types (like Go).
 */
TypeInfo check_field_access(TypeCheckerState& state, const FieldAccess& access) {
    // Check if the object is a module reference (for module.CONSTANT access)
    if (auto* var = dynamic_cast<const VariableRef*>(access.object.get())) {
        auto it = state.imported_modules.find(var->name);

        if (it != state.imported_modules.end()) {
            // This is a module reference - check for module constant
            const TypeInfo* c = get_qualified_constant(state, var->name, access.field_name);

            if (c) {
                access.object_type = "__module__";  // Mark as module access for codegen
                return *c;
            }

            // Not a constant - fall through to produce a helpful error
            error(state, "module '" + var->name + "' has no constant '" + access.field_name + "'", access.line);
            return {"unknown", false, false};
        }
    }

    TypeInfo obj_type = infer_type(state, *access.object);
    access.object_type = obj_type.base_type;  // Store for codegen (includes pointer suffix)

    // Auto-dereference pointers for field access (like Go)
    string struct_type = obj_type.base_type;

    if (!struct_type.empty() && struct_type.back() == '*') {
        struct_type = struct_type.substr(0, struct_type.length() - 1);
    }

    // Handle Pair<T> field access (first, second)
    if (struct_type.rfind("Pair<", 0) == 0) {
        string element_type = extract_element_type(struct_type, "Pair<");

        if (element_type.empty()) {
            error(state, "malformed Pair type '" + struct_type + "'", access.line);
            return {"unknown", false, false};
        }

        return check_pair_field(state, access, element_type);
    }

    const StructDef* sdef = get_struct(state, struct_type);

    if (!sdef) {
        error(state, "cannot access field on non-struct type '" + format_type(obj_type) + "'", access.line);
        return {"unknown", false, false};
    }

    for (const auto& field : sdef->fields) {
        if (field.name == access.field_name) {
            return {field.type, false, false};
        }
    }

    error(state, "struct '" + struct_type + "' has no field '" + access.field_name + "'", access.line);
    return {"unknown", false, false};
}

} // namespace typechecker
