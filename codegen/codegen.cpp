/**
 * @file codegen.cpp
 * @brief Main C++ code generation entry points for the Bishop language.
 *
 * Contains the generate() and generate_with_imports() functions that
 * orchestrate the code generation process.
 */

/**
 * @nog_fn print
 * @module builtins
 * @description Prints values to standard output, followed by a newline.
 * @param args ... - One or more values to print (separated by spaces)
 * @example
 * print("Hello, World!");
 * print("x =", x, "y =", y);
 */

/**
 * @nog_fn assert_eq
 * @module builtins
 * @description Asserts that two values are equal. Only available in test mode.
 * @param expected T - The expected value
 * @param actual T - The actual value to compare
 * @note Fails the test with line number and values if not equal
 * @example
 * assert_eq(5, add(2, 3));
 * assert_eq("hello", greet());
 */

/**
 * @nog_struct Channel
 * @module builtins
 * @description A typed channel for communication between goroutines.
 * @example
 * ch := Channel<int>();
 * ch.send(42);
 * val := ch.recv();
 */

/**
 * @nog_method send
 * @type Channel
 * @description Sends a value through the channel (blocks until received).
 * @param value T - The value to send
 * @example ch.send(42);
 */

/**
 * @nog_method recv
 * @type Channel
 * @description Receives a value from the channel (blocks until available).
 * @returns T - The received value
 * @example val := ch.recv();
 */

#include "codegen.hpp"
#include "stdlib/http.hpp"
#include "stdlib/fs.hpp"
#include "stdlib/crypto.hpp"
#include "stdlib/net.hpp"
#include "stdlib/process.hpp"
#include "stdlib/regex.hpp"
#include "stdlib/time.hpp"
#include "stdlib/math.hpp"
#include "stdlib/random.hpp"
#include <fmt/format.h>
#include <fmt/ranges.h>

using namespace std;

namespace codegen {

/**
 * Checks if any function has function type parameters (requires <functional>).
 */
static bool has_function_types(const Program& program) {
    for (const auto& fn : program.functions) {
        for (const auto& param : fn->params) {
            if (param.type.rfind("fn(", 0) == 0) {
                return true;
            }
        }
    }

    for (const auto& m : program.methods) {
        for (const auto& param : m->params) {
            if (param.type.rfind("fn(", 0) == 0) {
                return true;
            }
        }
    }

    return false;
}

/**
 * Checks if the program imports the http module.
 */
static bool has_http_import(const map<string, const Module*>& imports) {
    return imports.find("http") != imports.end();
}

/**
 * Checks if the program imports the fs module.
 */
static bool has_fs_import(const map<string, const Module*>& imports) {
    return imports.find("fs") != imports.end();
}

/**
 * Checks if the program imports the crypto module.
 */
static bool has_crypto_import(const map<string, const Module*>& imports) {
    return imports.find("crypto") != imports.end();
}

/**
 * Checks if the program imports the net module.
 */
static bool has_net_import(const map<string, const Module*>& imports) {
    return imports.find("net") != imports.end();
}

/**
 * Checks if the program imports the process module.
 */
static bool has_process_import(const map<string, const Module*>& imports) {
    return imports.find("process") != imports.end();
}

/**
 * Checks if the program imports the regex module.
 */
static bool has_regex_import(const map<string, const Module*>& imports) {
    return imports.find("regex") != imports.end();
}

/**
 * Checks if the program imports the time module.
 */
static bool has_time_import(const map<string, const Module*>& imports) {
    return imports.find("time") != imports.end();
}

/**
 * Checks if the program imports the math module.
 */
static bool has_math_import(const map<string, const Module*>& imports) {
    return imports.find("math") != imports.end();
}

/**
 * Checks if the program imports the random module.
 */
static bool has_random_import(const map<string, const Module*>& imports) {
    return imports.find("random") != imports.end();
}

/**
 * Checks if the program uses channels (requires boost fiber).
 */
static bool uses_channels(const Program& program) {
    for (const auto& fn : program.functions) {
        for (const auto& param : fn->params) {
            if (param.type.rfind("Channel<", 0) == 0) {
                return true;
            }
        }
    }

    return false;
}

/**
 * Generates a C++ namespace for an imported module.
 */
string generate_module_namespace(CodeGenState& state, const string& name, const Module& module) {
    if (name == "http") {
        return nog::stdlib::generate_http_runtime();
    }

    if (name == "fs") {
        return nog::stdlib::generate_fs_runtime();
    }

    if (name == "crypto") {
        return nog::stdlib::generate_crypto_runtime();
    }

    if (name == "net") {
        return nog::stdlib::generate_net_runtime();
    }

    if (name == "process") {
        return nog::stdlib::generate_process_runtime();
    }

    if (name == "regex") {
        return nog::stdlib::generate_regex_runtime();
    }

    if (name == "time") {
        return nog::stdlib::generate_time_runtime();
    }

    if (name == "math") {
        return nog::stdlib::generate_math_runtime();
    }

    if (name == "random") {
        return nog::stdlib::generate_random_runtime();
    }

    string out = "namespace " + name + " {\n\n";

    const Program* saved_program = state.current_program;
    state.current_program = module.ast.get();

    for (const auto* s : module.get_public_structs()) {
        out += generate_struct(state, *s) + "\n\n";
    }

    // Emit module-level constants
    for (const auto& c : module.ast->constants) {
        out += generate_module_constant(state, *c);
    }

    if (!module.ast->constants.empty()) {
        out += "\n";
    }

    for (const auto* f : module.get_public_functions()) {
        out += generate_function(state, *f);
    }

    state.current_program = saved_program;

    out += "} // namespace " + name + "\n\n";
    return out;
}

/**
 * Main code generation entry point. Generates complete C++ source.
 */
string generate(CodeGenState& state, const unique_ptr<Program>& program, bool test_mode) {
    state.test_mode = test_mode;
    state.current_program = program.get();
    state.imported_modules.clear();

    for (const auto& ext : program->externs) {
        state.extern_functions[ext->name] = ext.get();
    }

    if (test_mode) {
        return generate_test_harness(state, program);
    }

    string out = "#include <bishop/std.hpp>\n";

    if (uses_channels(*program)) {
        out += "#include <bishop/channel.hpp>\n";
    }

    out += "\n";

    out += generate_extern_declarations(program);

    for (const auto& s : program->structs) {
        out += generate_struct(state, *s) + "\n\n";
    }

    for (const auto& e : program->errors) {
        out += generate_error(state, *e) + "\n";
    }

    // Emit module-level constants
    for (const auto& c : program->constants) {
        out += generate_module_constant(state, *c);
    }

    if (!program->constants.empty()) {
        out += "\n";
    }

    for (const auto& fn : program->functions) {
        out += generate_function(state, *fn);
    }

    return out;
}

/**
 * Generates C++ code for a program with imported modules.
 */
string generate_with_imports(
    CodeGenState& state,
    const unique_ptr<Program>& program,
    const map<string, const Module*>& imports,
    bool test_mode
) {
    state.test_mode = test_mode;
    state.current_program = program.get();
    state.imported_modules = imports;

    for (const auto& ext : program->externs) {
        state.extern_functions[ext->name] = ext.get();
    }

    string out;

    if (has_http_import(imports)) {
        out += "#include <bishop/http.hpp>\n";
    } else if (has_net_import(imports)) {
        out += "#include <bishop/net.hpp>\n";
    } else {
        out += "#include <bishop/std.hpp>\n";
    }

    if (has_fs_import(imports)) {
        out += "#include <bishop/fs.hpp>\n";
    }

    if (has_crypto_import(imports)) {
        out += "#include <bishop/crypto.hpp>\n";
    }

    if (has_process_import(imports)) {
        out += "#include <bishop/process.hpp>\n";
    }

    if (has_regex_import(imports)) {
        out += "#include <bishop/regex.hpp>\n";
    }

    if (has_time_import(imports)) {
        out += "#include <bishop/time.hpp>\n";
    }

    if (has_math_import(imports)) {
        out += "#include <bishop/math.hpp>\n";
    }

    if (has_random_import(imports)) {
        out += "#include <bishop/random.hpp>\n";
    }

    if (uses_channels(*program)) {
        out += "#include <bishop/channel.hpp>\n";
    }

    out += "\n";

    out += generate_extern_declarations(program);

    if (test_mode) {
        out += "int _failures = 0;\n\n";
        out += "template<typename T, typename U>\n";
        out += "void _assert_eq(T a, U b, int line) {\n";
        out += "\tif (a != b) {\n";
        out += "\t\tstd::cerr << \"line \" << line << \": FAIL: \" << a << \" != \" << b << std::endl;\n";
        out += "\t\t_failures++;\n";
        out += "\t}\n";
        out += "}\n\n";
    }

    for (const auto& [alias, mod] : imports) {
        out += generate_module_namespace(state, alias, *mod);
    }

    for (const auto& s : program->structs) {
        out += generate_struct(state, *s) + "\n\n";
    }

    for (const auto& e : program->errors) {
        out += generate_error(state, *e) + "\n";
    }

    // Emit module-level constants
    for (const auto& c : program->constants) {
        out += generate_module_constant(state, *c);
    }

    if (!program->constants.empty()) {
        out += "\n";
    }

    for (const auto& fn : program->functions) {
        out += generate_function(state, *fn);
    }

    if (test_mode) {
        vector<string> test_funcs;

        for (const auto& fn : program->functions) {
            if (fn->name.rfind("test_", 0) == 0) {
                test_funcs.push_back(fn->name);
            }
        }

        out += "\nint main() {\n";
        out += "\tbishop::rt::init_runtime();\n";
        out += "\n";

        for (const auto& name : test_funcs) {
            out += "\tbishop::rt::run_in_fiber(" + name + ");\n";
        }

        out += "\treturn _failures;\n";
        out += "}\n";
    }

    return out;
}

} // namespace codegen

// Legacy class API for backwards compatibility
string CodeGen::generate(const unique_ptr<Program>& program, bool test_mode) {
    CodeGenState state;
    return codegen::generate(state, program, test_mode);
}

string CodeGen::generate_with_imports(
    const unique_ptr<Program>& program,
    const map<string, const Module*>& imports,
    bool test_mode
) {
    CodeGenState state;
    return codegen::generate_with_imports(state, program, imports, test_mode);
}
