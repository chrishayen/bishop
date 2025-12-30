/**
 * @file emit_refs.cpp
 * @brief Variable and function reference emission for the Bishop code generator.
 */

#include "codegen.hpp"
#include <fmt/format.h>

using namespace std;

namespace codegen {

/**
 * Emits a variable reference (just the name).
 */
string variable_ref(const string& name) {
    return name;
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

        // Map 'time' module to 'bishop_time' to avoid conflict with C time()
        if (module_name == "time") {
            module_name = "bishop_time";
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

    // Map 'time' module to 'bishop_time' to avoid conflict with C time()
    if (module_name == "time") {
        module_name = "bishop_time";
    }

    return module_name + "::" + ref.name;
}

} // namespace codegen
