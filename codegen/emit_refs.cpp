/**
 * @file emit_refs.cpp
 * @brief Variable and function reference emission for the Bishop code generator.
 */

#include "codegen.hpp"
#include <fmt/format.h>

using namespace std;

namespace codegen {

/**
 * Emits a variable reference, escaping C++ reserved keywords.
 */
string variable_ref(const string& name) {
    return escape_reserved_name(name);
}

/**
 * Emits a function reference, possibly qualified with ::.
 */
string emit_function_ref(const FunctionRef& ref) {
    string func_name = ref.name;
    size_t dot_pos = func_name.find('.');

    if (dot_pos != string::npos) {
        string module_name = func_name.substr(0, dot_pos);
        string fn_name = func_name.substr(dot_pos + 1);

        // Map module names that conflict with C/C++ identifiers
        if (module_name == "time") {
            module_name = "bishop_time";
        } else if (module_name == "log") {
            module_name = "bishop_log";
        }

        func_name = module_name + "::" + fn_name;
    }

    return func_name;
}

/**
 * Emits a qualified reference: module.name -> module::name.
 */
string emit_qualified_ref(const QualifiedRef& ref) {
    string module_name = ref.module_name;

    // Map module names that conflict with C/C++ identifiers
    if (module_name == "time") {
        module_name = "bishop_time";
    } else if (module_name == "log") {
        module_name = "bishop_log";
    }

    return module_name + "::" + ref.name;
}

/**
 * Looks up a using alias by local name.
 */
const CodeGenUsingAlias* get_using_alias(const CodeGenState& state, const string& name) {
    for (const auto& alias : state.using_aliases) {
        if (alias.local_name == name) {
            return &alias;
        }
    }

    return nullptr;
}

/**
 * Collects using aliases from the program's using statements.
 * For wildcard imports, we need to expand all public members.
 */
void collect_using_aliases(CodeGenState& state, const Program& program) {
    for (const auto& using_stmt : program.usings) {
        if (using_stmt->is_wildcard) {
            // Wildcard import: using module.*;
            const string& module_alias = using_stmt->wildcard_module;
            auto it = state.imported_modules.find(module_alias);

            if (it == state.imported_modules.end()) continue;

            const Module* mod = it->second;

            // Add all public functions
            for (const auto* func : mod->get_public_functions()) {
                CodeGenUsingAlias alias;
                alias.local_name = func->name;
                alias.module_alias = module_alias;
                alias.member_name = func->name;
                state.using_aliases.push_back(alias);
            }

            // Add all public structs
            for (const auto* s : mod->get_public_structs()) {
                CodeGenUsingAlias alias;
                alias.local_name = s->name;
                alias.module_alias = module_alias;
                alias.member_name = s->name;
                state.using_aliases.push_back(alias);
            }

            // Add all public constants
            for (const auto& c : mod->ast->constants) {
                CodeGenUsingAlias alias;
                alias.local_name = c->name;
                alias.module_alias = module_alias;
                alias.member_name = c->name;
                state.using_aliases.push_back(alias);
            }

            // Add all public extern functions
            for (const auto& e : mod->ast->externs) {
                CodeGenUsingAlias alias;
                alias.local_name = e->name;
                alias.module_alias = module_alias;
                alias.member_name = e->name;
                state.using_aliases.push_back(alias);
            }
        } else {
            // Selective imports: using module.member, module.member2;
            for (const auto& member : using_stmt->members) {
                CodeGenUsingAlias alias;
                alias.local_name = member.member_name;
                alias.module_alias = member.module_alias;
                alias.member_name = member.member_name;
                state.using_aliases.push_back(alias);
            }
        }
    }
}

} // namespace codegen
