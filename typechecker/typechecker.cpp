/**
 * @file typechecker.cpp
 * @brief Main type checker entry point and utility functions.
 *
 * Contains the main check() function, collection functions for building
 * symbol tables, and type utility functions.
 */

#include "typechecker.hpp"
#include "common/type_utils.hpp"
#include <optional>

using namespace std;

namespace typechecker {

// -----------------------------------------------------------------------------
// Local scope helpers
// -----------------------------------------------------------------------------

void push_scope(TypeCheckerState& state) {
    state.local_scopes.emplace_back();
}

void pop_scope(TypeCheckerState& state) {
    if (!state.local_scopes.empty()) {
        state.local_scopes.pop_back();
    }
}

bool is_declared_in_current_scope(const TypeCheckerState& state, const string& name) {
    if (state.local_scopes.empty()) {
        return false;
    }

    const auto& current = state.local_scopes.back();
    return current.find(name) != current.end();
}

void declare_local(TypeCheckerState& state, const string& name, const TypeInfo& type, int line) {
    if (state.local_scopes.empty()) {
        // All checking should start with an initial scope; this fallback keeps the
        // checker resilient if a new entry point forgets to push one.
        push_scope(state);
    }

    if (is_declared_in_current_scope(state, name)) {
        error(state, "variable '" + name + "' is already defined in this scope", line);
        return;
    }

    state.local_scopes.back()[name] = type;
}

TypeInfo* lookup_local(TypeCheckerState& state, const string& name) {
    for (auto it = state.local_scopes.rbegin(); it != state.local_scopes.rend(); ++it) {
        auto found = it->find(name);

        if (found != it->end()) {
            return &found->second;
        }
    }

    return nullptr;
}

const TypeInfo* lookup_local(const TypeCheckerState& state, const string& name) {
    for (auto it = state.local_scopes.rbegin(); it != state.local_scopes.rend(); ++it) {
        auto found = it->find(name);

        if (found != it->end()) {
            return &found->second;
        }
    }

    return nullptr;
}

/**
 * Main entry point for type checking. Collects all declarations into symbol
 * tables, then validates each function and method body.
 */
bool check(TypeCheckerState& state, const Program& program, const string& fname) {
    state.filename = fname;
    state.errors.clear();

    collect_structs(state, program);
    collect_methods(state, program);
    collect_functions(state, program);
    collect_extern_functions(state, program);
    collect_constants(state, program);
    collect_using_aliases(state, program);

    for (const auto& method : program.methods) {
        check_method(state, *method);
    }

    for (const auto& func : program.functions) {
        check_function(state, *func);
    }

    return state.errors.empty();
}

/**
 * Registers an imported module for cross-module type checking.
 */
void register_module(TypeCheckerState& state, const string& alias, const Module& module) {
    state.imported_modules[alias] = &module;
}

/**
 * Collects all struct definitions into the structs symbol table.
 */
void collect_structs(TypeCheckerState& state, const Program& program) {
    for (const auto& s : program.structs) {
        state.structs[s->name] = s.get();
    }
}

/**
 * Collects all method definitions. Validates that the struct exists
 * and checks for duplicate method definitions.
 */
void collect_methods(TypeCheckerState& state, const Program& program) {
    for (const auto& m : program.methods) {
        if (state.structs.find(m->struct_name) == state.structs.end()) {
            error(state, "method '" + m->name + "' defined on unknown struct '" + m->struct_name + "'", m->line);
            continue;
        }

        auto& struct_methods = state.methods[m->struct_name];

        for (const auto* existing : struct_methods) {
            if (existing->name == m->name) {
                error(state, "duplicate method '" + m->name + "' on struct '" + m->struct_name + "'", m->line);
                break;
            }
        }

        struct_methods.push_back(m.get());
    }
}

/**
 * Collects all function definitions into the functions symbol table.
 */
void collect_functions(TypeCheckerState& state, const Program& program) {
    for (const auto& f : program.functions) {
        state.functions[f->name] = f.get();
    }
}

/**
 * Collects all extern function declarations into the extern_functions symbol table.
 */
void collect_extern_functions(TypeCheckerState& state, const Program& program) {
    for (const auto& e : program.externs) {
        state.extern_functions[e->name] = e.get();
    }
}

/**
 * Collects all module-level const declarations into the module_constants symbol table.
 * Also validates that each const initializer has a valid type.
 */
void collect_constants(TypeCheckerState& state, const Program& program) {
    for (const auto& c : program.constants) {
        if (state.module_constants.find(c->name) != state.module_constants.end()) {
            error(state, "duplicate module-level const '" + c->name + "'", c->line);
            continue;
        }

        if (c->value) {
            TypeInfo init_type = infer_type(state, *c->value);

            if (!c->type.empty()) {
                TypeInfo expected = {c->type, c->is_optional, false};

                if (!types_compatible(expected, init_type)) {
                    error(state, "cannot assign '" + format_type(init_type) + "' to const of type '" + format_type(expected) + "'", c->line);
                }

                TypeInfo type_info = {c->type, c->is_optional, false, false, true};
                state.module_constants[c->name] = type_info;
            } else {
                TypeInfo type_info = init_type;
                type_info.is_const = true;
                state.module_constants[c->name] = type_info;
            }
        }
    }
}

/**
 * Checks if a type is a built-in primitive (int, str, bool, etc).
 */
bool is_primitive_type(const string& type) {
    return type == "int" || type == "str" || type == "bool" ||
           type == "f32" || type == "f64" ||
           type == "u32" || type == "u64" ||
           type == "cint" || type == "cstr" || type == "void";
}

/**
 * Extracts the element type from a generic type string using proper bracket matching.
 * Delegates to the shared implementation in common/type_utils.hpp.
 */
string extract_element_type(const string& generic_type, const string& prefix) {
    return bishop::extract_element_type(generic_type, prefix);
}

/**
 * Checks if a type is valid (either a primitive, a known struct, channel,
 * function type, or a qualified module type).
 */
bool is_valid_type(const TypeCheckerState& state, const string& type) {
    if (is_primitive_type(type)) {
        return true;
    }

    if (state.structs.find(type) != state.structs.end()) {
        return true;
    }

    if (type.rfind("fn:", 0) == 0 || type.rfind("fn(", 0) == 0) {
        return true;
    }

    if (type.rfind("Channel<", 0) == 0 && type.back() == '>') {
        string element_type = extract_element_type(type, "Channel<");

        if (element_type.empty()) {
            return false;
        }

        return is_valid_type(state, element_type);
    }

    if (type.rfind("List<", 0) == 0 && type.back() == '>') {
        string element_type = extract_element_type(type, "List<");

        if (element_type.empty()) {
            return false;
        }

        return is_valid_type(state, element_type);
    }

    if (type.rfind("Pair<", 0) == 0 && type.back() == '>') {
        string element_type = extract_element_type(type, "Pair<");

        if (element_type.empty()) {
            return false;
        }

        return is_valid_type(state, element_type);
    }

    if (type.rfind("Tuple<", 0) == 0 && type.back() == '>') {
        string element_type = extract_element_type(type, "Tuple<");

        if (element_type.empty()) {
            return false;
        }

        return is_valid_type(state, element_type);
    }

    if (type.rfind("Map<", 0) == 0 && type.back() == '>') {
        auto [key_type, value_type] = bishop::extract_map_types(type);

        if (key_type.empty() || value_type.empty()) {
            return false;
        }

        return is_valid_type(state, key_type) && is_valid_type(state, value_type);
    }

    // Pointer type: StructName* -> check that base is a valid struct
    if (!type.empty() && type.back() == '*') {
        string pointee = type.substr(0, type.length() - 1);
        // Only struct pointers are allowed, not primitive pointers
        return state.structs.find(pointee) != state.structs.end();
    }

    size_t dot_pos = type.find('.');

    if (dot_pos != string::npos) {
        string module_name = type.substr(0, dot_pos);
        string type_name = type.substr(dot_pos + 1);
        return get_qualified_struct(state, module_name, type_name) != nullptr;
    }

    return false;
}

/**
 * Checks if actual type can be assigned to expected type.
 */
bool types_compatible(const TypeInfo& expected, const TypeInfo& actual) {
    if (actual.base_type == "none" && expected.is_optional) {
        return true;
    }

    if (actual.base_type == "unknown" || expected.base_type == "unknown") {
        return true;
    }

    if (actual.base_type.rfind("fn:", 0) == 0 && expected.base_type.rfind("fn(", 0) == 0) {
        return true;
    }

    if (expected.base_type == "cstr" && actual.base_type == "str") {
        return true;
    }

    if (expected.base_type == "cint" && actual.base_type == "int") {
        return true;
    }

    // Numeric type conversions
    if (expected.base_type == "u32" && actual.base_type == "int") {
        return true;
    }

    if (expected.base_type == "u64" && actual.base_type == "int") {
        return true;
    }

    if (expected.base_type == "f32" && actual.base_type == "f64") {
        return true;
    }

    return expected.base_type == actual.base_type;
}

/**
 * Parses parameter types from a function type string.
 * Format: fn(type1, type2) -> return_type
 * Returns an empty optional if the function type is invalid.
 */
optional<vector<string>> parse_function_type_params(const string& fn_type) {
    if (fn_type.substr(0, 3) != "fn(") {
        return nullopt;
    }

    size_t params_start = 3;  // After "fn("
    size_t params_end = fn_type.find(')');

    if (params_end == string::npos) {
        return nullopt;
    }

    string params_str = fn_type.substr(params_start, params_end - params_start);
    vector<string> param_types;

    if (params_str.empty()) {
        return param_types;
    }

    size_t pos = 0;

    while (pos < params_str.length()) {
        // Skip whitespace
        while (pos < params_str.length() && params_str[pos] == ' ') {
            pos++;
        }

        size_t comma_pos = params_str.find(',', pos);

        if (comma_pos == string::npos) {
            string type = params_str.substr(pos);
            // Trim trailing whitespace
            while (!type.empty() && type.back() == ' ') {
                type.pop_back();
            }

            if (!type.empty()) {
                param_types.push_back(type);
            }

            break;
        } else {
            string type = params_str.substr(pos, comma_pos - pos);
            // Trim whitespace
            while (!type.empty() && type.back() == ' ') {
                type.pop_back();
            }

            if (!type.empty()) {
                param_types.push_back(type);
            }

            pos = comma_pos + 1;
        }
    }

    return param_types;
}

/**
 * Formats a type for error messages, including optional modifiers.
 */
string format_type(const TypeInfo& type) {
    string inner = type.base_type;

    if (type.is_optional && inner != "none") {
        inner += "?";
    }

    return inner;
}

/**
 * Looks up a struct definition by name. Handles both local structs
 * and qualified module types (module.Type).
 */
const StructDef* get_struct(const TypeCheckerState& state, const string& name) {
    auto it = state.structs.find(name);

    if (it != state.structs.end()) {
        return it->second;
    }

    size_t dot_pos = name.find('.');

    if (dot_pos != string::npos) {
        string module_name = name.substr(0, dot_pos);
        string type_name = name.substr(dot_pos + 1);
        return get_qualified_struct(state, module_name, type_name);
    }

    return nullptr;
}

/**
 * Looks up a method on a struct. Returns nullptr if not found.
 */
const MethodDef* get_method(const TypeCheckerState& state, const string& struct_name, const string& method_name) {
    auto it = state.methods.find(struct_name);

    if (it == state.methods.end()) {
        return nullptr;
    }

    for (const auto* m : it->second) {
        if (m->name == method_name) {
            return m;
        }
    }

    return nullptr;
}

/**
 * Gets the type of a field on a struct. Returns empty string if not found.
 */
string get_field_type(const TypeCheckerState& state, const string& struct_name, const string& field_name) {
    const StructDef* sdef = get_struct(state, struct_name);

    if (!sdef) {
        return "";
    }

    for (const auto& field : sdef->fields) {
        if (field.name == field_name) {
            return field.type;
        }
    }

    return "";
}

/**
 * Records a type error with the given message and line number.
 */
void error(TypeCheckerState& state, const string& msg, int line) {
    state.errors.push_back({msg, line, state.filename});
}

/**
 * Looks up a function in an imported module by module alias and function name.
 */
const FunctionDef* get_qualified_function(const TypeCheckerState& state, const string& module, const string& name) {
    auto it = state.imported_modules.find(module);

    if (it == state.imported_modules.end()) {
        return nullptr;
    }

    for (const auto* func : it->second->get_public_functions()) {
        if (func->name == name) {
            return func;
        }
    }

    return nullptr;
}

/**
 * Looks up a struct in an imported module by module alias and struct name.
 */
const StructDef* get_qualified_struct(const TypeCheckerState& state, const string& module, const string& name) {
    auto it = state.imported_modules.find(module);

    if (it == state.imported_modules.end()) {
        return nullptr;
    }

    for (const auto* s : it->second->get_public_structs()) {
        if (s->name == name) {
            return s;
        }
    }

    return nullptr;
}

/**
 * Looks up a method on a struct in an imported module.
 */
const MethodDef* get_qualified_method(const TypeCheckerState& state, const string& module, const string& struct_name, const string& method_name) {
    auto it = state.imported_modules.find(module);

    if (it == state.imported_modules.end()) {
        return nullptr;
    }

    for (const auto* m : it->second->get_public_methods(struct_name)) {
        if (m->name == method_name) {
            return m;
        }
    }

    return nullptr;
}

/**
 * Looks up a module-level constant by name.
 */
const TypeInfo* get_module_constant(const TypeCheckerState& state, const string& name) {
    auto it = state.module_constants.find(name);

    if (it != state.module_constants.end()) {
        return &it->second;
    }

    return nullptr;
}

/**
 * Looks up a constant in an imported module by module alias and constant name.
 */
const TypeInfo* get_qualified_constant(const TypeCheckerState& state, const string& module, const string& name) {
    auto it = state.imported_modules.find(module);

    if (it == state.imported_modules.end()) {
        return nullptr;
    }

    for (const auto& c : it->second->ast->constants) {
        if (c->name == name) {
            // Build a TypeInfo for the constant
            // This is a simplification - we rely on type inference
            static TypeInfo cached;
            cached.base_type = c->type;
            cached.is_const = true;
            return &cached;
        }
    }

    return nullptr;
}

/**
 * Collects all using statement aliases into the using_aliases vector.
 * Validates that each member exists in the imported module and is public.
 */
void collect_using_aliases(TypeCheckerState& state, const Program& program) {
    for (const auto& using_stmt : program.usings) {
        if (using_stmt->is_wildcard) {
            // Wildcard import: using module.*;
            const string& module_alias = using_stmt->wildcard_module;
            auto it = state.imported_modules.find(module_alias);

            if (it == state.imported_modules.end()) {
                error(state, "using: module '" + module_alias + "' is not imported", using_stmt->line);
                continue;
            }

            const Module* mod = it->second;

            // Add all public functions
            for (const auto* func : mod->get_public_functions()) {
                ResolvedUsingAlias alias;
                alias.local_name = func->name;
                alias.module_alias = module_alias;
                alias.member_name = func->name;
                alias.member_type = "function";
                state.using_aliases.push_back(alias);
            }

            // Add all public structs
            for (const auto* s : mod->get_public_structs()) {
                ResolvedUsingAlias alias;
                alias.local_name = s->name;
                alias.module_alias = module_alias;
                alias.member_name = s->name;
                alias.member_type = "struct";
                state.using_aliases.push_back(alias);
            }

            // Add all public constants (module-level consts are always public currently)
            for (const auto& c : mod->ast->constants) {
                ResolvedUsingAlias alias;
                alias.local_name = c->name;
                alias.module_alias = module_alias;
                alias.member_name = c->name;
                alias.member_type = "constant";
                alias.type_info.base_type = c->type;
                alias.type_info.is_const = true;
                state.using_aliases.push_back(alias);
            }

            // Add all public extern functions
            for (const auto& e : mod->ast->externs) {
                if (e->visibility == Visibility::Private) continue;

                ResolvedUsingAlias alias;
                alias.local_name = e->name;
                alias.module_alias = module_alias;
                alias.member_name = e->name;
                alias.member_type = "extern";
                state.using_aliases.push_back(alias);
            }
        } else {
            // Selective imports: using module.member, module.member2;
            for (const auto& member : using_stmt->members) {
                const string& module_alias = member.module_alias;
                const string& member_name = member.member_name;

                auto it = state.imported_modules.find(module_alias);

                if (it == state.imported_modules.end()) {
                    error(state, "using: module '" + module_alias + "' is not imported", using_stmt->line);
                    continue;
                }

                const Module* mod = it->second;
                bool found = false;

                // Check if it's a function
                const FunctionDef* func = get_qualified_function(state, module_alias, member_name);

                if (func) {
                    ResolvedUsingAlias alias;
                    alias.local_name = member_name;
                    alias.module_alias = module_alias;
                    alias.member_name = member_name;
                    alias.member_type = "function";
                    state.using_aliases.push_back(alias);
                    found = true;
                }

                // Check if it's a struct
                if (!found) {
                    const StructDef* s = get_qualified_struct(state, module_alias, member_name);

                    if (s) {
                        ResolvedUsingAlias alias;
                        alias.local_name = member_name;
                        alias.module_alias = module_alias;
                        alias.member_name = member_name;
                        alias.member_type = "struct";
                        state.using_aliases.push_back(alias);
                        found = true;
                    }
                }

                // Check if it's a constant
                if (!found) {
                    const TypeInfo* c = get_qualified_constant(state, module_alias, member_name);

                    if (c) {
                        ResolvedUsingAlias alias;
                        alias.local_name = member_name;
                        alias.module_alias = module_alias;
                        alias.member_name = member_name;
                        alias.member_type = "constant";
                        alias.type_info = *c;
                        state.using_aliases.push_back(alias);
                        found = true;
                    }
                }

                // Check if it's an extern function
                if (!found) {
                    for (const auto& e : mod->ast->externs) {
                        if (e->name == member_name && e->visibility != Visibility::Private) {
                            ResolvedUsingAlias alias;
                            alias.local_name = member_name;
                            alias.module_alias = module_alias;
                            alias.member_name = member_name;
                            alias.member_type = "extern";
                            state.using_aliases.push_back(alias);
                            found = true;
                            break;
                        }
                    }
                }

                if (!found) {
                    error(state, "using: member '" + member_name + "' not found in module '" + module_alias + "'", using_stmt->line);
                }
            }
        }
    }
}

/**
 * Looks up a using alias by local name.
 */
const ResolvedUsingAlias* get_using_alias(const TypeCheckerState& state, const string& name) {
    for (const auto& alias : state.using_aliases) {
        if (alias.local_name == name) {
            return &alias;
        }
    }

    return nullptr;
}

} // namespace typechecker

// Legacy class API for backwards compatibility
bool TypeChecker::check(const Program& program, const string& filename) {
    return typechecker::check(state, program, filename);
}

void TypeChecker::register_module(const string& alias, const Module& module) {
    typechecker::register_module(state, alias, module);
}
