/**
 * @file codegen.cpp
 * @brief Main C++ code generation entry points for the Bishop language.
 *
 * Contains the generate() and generate_with_imports() functions that
 * orchestrate the code generation process.
 */

/**
 * @bishop_fn print
 * @module builtins
 * @description Prints values to standard output, followed by a newline.
 * @param args ... - One or more values to print (separated by spaces)
 * @example
 * print("Hello, World!");
 * print("x =", x, "y =", y);
 */

/**
 * @bishop_fn assert_eq
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
 * @bishop_struct Channel
 * @module builtins
 * @description A typed channel for communication between goroutines.
 * @example
 * ch := Channel<int>();
 * ch.send(42);
 * val := ch.recv();
 */

/**
 * @bishop_method send
 * @type Channel
 * @description Sends a value through the channel (blocks until received).
 * @param value T - The value to send
 * @example ch.send(42);
 */

/**
 * @bishop_method recv
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
#include "stdlib/sync.hpp"
#include "stdlib/json.hpp"
#include "stdlib/log.hpp"
#include "stdlib/algo.hpp"
#include "stdlib/yaml.hpp"
#include "stdlib/markdown.hpp"
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
 * Checks if the program imports the log module.
 */
static bool has_log_import(const map<string, const Module*>& imports) {
    return imports.find("log") != imports.end();
}

/**
 * Checks if the program imports the sync module.
 */
static bool has_sync_import(const map<string, const Module*>& imports) {
    return imports.find("sync") != imports.end();
}

/**
 * Checks if the program imports the json module.
 */
static bool has_json_import(const map<string, const Module*>& imports) {
    return imports.find("json") != imports.end();
}

/**
 * Checks if the program imports the algo module.
 */
static bool has_algo_import(const map<string, const Module*>& imports) {
    return imports.find("algo") != imports.end();
}

/**
 * Checks if the program imports the yaml module.
 */
static bool has_yaml_import(const map<string, const Module*>& imports) {
    return imports.find("yaml") != imports.end();
}

/**
 * Checks if the program imports the markdown module.
 */
static bool has_markdown_import(const map<string, const Module*>& imports) {
    return imports.find("markdown") != imports.end();
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
        return bishop::stdlib::generate_http_runtime();
    }

    if (name == "fs") {
        return bishop::stdlib::generate_fs_runtime();
    }

    if (name == "crypto") {
        return bishop::stdlib::generate_crypto_runtime();
    }

    if (name == "net") {
        return bishop::stdlib::generate_net_runtime();
    }

    if (name == "process") {
        return bishop::stdlib::generate_process_runtime();
    }

    if (name == "regex") {
        return bishop::stdlib::generate_regex_runtime();
    }

    if (name == "time") {
        return bishop::stdlib::generate_time_runtime();
    }

    if (name == "math") {
        return bishop::stdlib::generate_math_runtime();
    }

    if (name == "random") {
        return bishop::stdlib::generate_random_runtime();
    }

    if (name == "sync") {
        return bishop::stdlib::generate_sync_runtime();
    }

    if (name == "json") {
        return bishop::stdlib::generate_json_runtime();
    }

    if (name == "log") {
        return bishop::stdlib::generate_log_runtime();
    }

    if (name == "algo") {
        return bishop::stdlib::generate_algo_runtime();
    }

    if (name == "yaml") {
        return bishop::stdlib::generate_yaml_runtime();
    }

    if (name == "markdown") {
        return bishop::stdlib::generate_markdown_runtime();
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

    // Forward declare all structs
    for (const auto& s : program->structs) {
        out += "struct " + s->name + ";\n";
    }

    if (!program->structs.empty()) {
        out += "\n";
    }

    // Generate struct definitions with fields and method declarations only
    for (const auto& s : program->structs) {
        out += generate_struct_fields_only(state, *s) + "\n\n";
    }

    for (const auto& e : program->errors) {
        out += generate_error(state, *e) + "\n";
    }

    // Generate standalone method implementations after all structs are defined
    for (const auto& method : program->methods) {
        out += generate_standalone_method(state, *method) + "\n";
    }

    // Emit module-level constants
    for (const auto& c : program->constants) {
        out += generate_module_constant(state, *c);
    }

    if (!program->constants.empty()) {
        out += "\n";
    }

    // Forward declare all functions
    for (const auto& fn : program->functions) {
        out += generate_function_declaration(state, *fn);
    }

    if (!program->functions.empty()) {
        out += "\n";
    }

    // Generate function definitions
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

    // Collect using aliases for code generation
    collect_using_aliases(state, *program);

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

    if (has_log_import(imports)) {
        out += "#include <bishop/log.hpp>\n";
    }

    if (has_sync_import(imports)) {
        out += "#include <bishop/sync.hpp>\n";
    }

    if (has_json_import(imports)) {
        out += "#include <bishop/json.hpp>\n";
    }

    if (has_algo_import(imports)) {
        out += "#include <bishop/algo.hpp>\n";
    }

    if (has_yaml_import(imports)) {
        out += "#include <bishop/yaml.hpp>\n";
    }

    if (has_markdown_import(imports)) {
        out += "#include <bishop/markdown.hpp>\n";
    }

    if (uses_channels(*program)) {
        out += "#include <bishop/channel.hpp>\n";
    }

    out += "\n";

    out += generate_extern_declarations(program);

    if (test_mode) {
        out += "#include <cmath>\n\n";
        out += "int _failures = 0;\n\n";

        // assert_eq: a == b
        out += "template<typename T, typename U>\n";
        out += "void _assert_eq(T a, U b, int line) {\n";
        out += "\tif (a != b) {\n";
        out += "\t\tstd::cerr << \"line \" << line << \": FAIL: \" << a << \" != \" << b << std::endl;\n";
        out += "\t\t_failures++;\n";
        out += "\t}\n";
        out += "}\n\n";

        // assert_ne: a != b
        out += "template<typename T, typename U>\n";
        out += "void _assert_ne(T a, U b, int line) {\n";
        out += "\tif (a == b) {\n";
        out += "\t\tstd::cerr << \"line \" << line << \": FAIL: expected not equal, but both are \" << a << std::endl;\n";
        out += "\t\t_failures++;\n";
        out += "\t}\n";
        out += "}\n\n";

        // assert_true: condition is true
        out += "void _assert_true(bool condition, int line) {\n";
        out += "\tif (!condition) {\n";
        out += "\t\tstd::cerr << \"line \" << line << \": FAIL: expected true, got false\" << std::endl;\n";
        out += "\t\t_failures++;\n";
        out += "\t}\n";
        out += "}\n\n";

        // assert_false: condition is false
        out += "void _assert_false(bool condition, int line) {\n";
        out += "\tif (condition) {\n";
        out += "\t\tstd::cerr << \"line \" << line << \": FAIL: expected false, got true\" << std::endl;\n";
        out += "\t\t_failures++;\n";
        out += "\t}\n";
        out += "}\n\n";

        // assert_gt: a > b
        out += "template<typename T, typename U>\n";
        out += "void _assert_gt(T a, U b, int line) {\n";
        out += "\tif (!(a > b)) {\n";
        out += "\t\tstd::cerr << \"line \" << line << \": FAIL: expected \" << a << \" > \" << b << std::endl;\n";
        out += "\t\t_failures++;\n";
        out += "\t}\n";
        out += "}\n\n";

        // assert_gte: a >= b
        out += "template<typename T, typename U>\n";
        out += "void _assert_gte(T a, U b, int line) {\n";
        out += "\tif (!(a >= b)) {\n";
        out += "\t\tstd::cerr << \"line \" << line << \": FAIL: expected \" << a << \" >= \" << b << std::endl;\n";
        out += "\t\t_failures++;\n";
        out += "\t}\n";
        out += "}\n\n";

        // assert_lt: a < b
        out += "template<typename T, typename U>\n";
        out += "void _assert_lt(T a, U b, int line) {\n";
        out += "\tif (!(a < b)) {\n";
        out += "\t\tstd::cerr << \"line \" << line << \": FAIL: expected \" << a << \" < \" << b << std::endl;\n";
        out += "\t\t_failures++;\n";
        out += "\t}\n";
        out += "}\n\n";

        // assert_lte: a <= b
        out += "template<typename T, typename U>\n";
        out += "void _assert_lte(T a, U b, int line) {\n";
        out += "\tif (!(a <= b)) {\n";
        out += "\t\tstd::cerr << \"line \" << line << \": FAIL: expected \" << a << \" <= \" << b << std::endl;\n";
        out += "\t\t_failures++;\n";
        out += "\t}\n";
        out += "}\n\n";

        // assert_contains: item is in collection
        out += "template<typename T, typename C>\n";
        out += "void _assert_contains(T item, C& collection, int line) {\n";
        out += "\tbool found = false;\n";
        out += "\tfor (const auto& elem : collection) {\n";
        out += "\t\tif (elem == item) {\n";
        out += "\t\t\tfound = true;\n";
        out += "\t\t\tbreak;\n";
        out += "\t\t}\n";
        out += "\t}\n";
        out += "\tif (!found) {\n";
        out += "\t\tstd::cerr << \"line \" << line << \": FAIL: collection does not contain \" << item << std::endl;\n";
        out += "\t\t_failures++;\n";
        out += "\t}\n";
        out += "}\n\n";

        // assert_starts_with: str starts with prefix
        out += "void _assert_starts_with(const std::string& prefix, const std::string& str, int line) {\n";
        out += "\tif (str.rfind(prefix, 0) != 0) {\n";
        out += "\t\tstd::cerr << \"line \" << line << \": FAIL: \\\"\" << str << \"\\\" does not start with \\\"\" << prefix << \"\\\"\" << std::endl;\n";
        out += "\t\t_failures++;\n";
        out += "\t}\n";
        out += "}\n\n";

        // assert_ends_with: str ends with suffix
        out += "void _assert_ends_with(const std::string& suffix, const std::string& str, int line) {\n";
        out += "\tif (str.length() < suffix.length() || str.compare(str.length() - suffix.length(), suffix.length(), suffix) != 0) {\n";
        out += "\t\tstd::cerr << \"line \" << line << \": FAIL: \\\"\" << str << \"\\\" does not end with \\\"\" << suffix << \"\\\"\" << std::endl;\n";
        out += "\t\t_failures++;\n";
        out += "\t}\n";
        out += "}\n\n";

        // assert_near: actual is within epsilon of expected
        out += "template<typename T, typename U, typename E>\n";
        out += "void _assert_near(T actual, U expected, E epsilon, int line) {\n";
        out += "\tif (std::abs(static_cast<double>(actual) - static_cast<double>(expected)) > static_cast<double>(epsilon)) {\n";
        out += "\t\tstd::cerr << \"line \" << line << \": FAIL: \" << actual << \" is not within \" << epsilon << \" of \" << expected << std::endl;\n";
        out += "\t\t_failures++;\n";
        out += "\t}\n";
        out += "}\n\n";
    }

    for (const auto& [alias, mod] : imports) {
        out += generate_module_namespace(state, alias, *mod);
    }

    // Forward declare all structs
    for (const auto& s : program->structs) {
        out += "struct " + s->name + ";\n";
    }

    if (!program->structs.empty()) {
        out += "\n";
    }

    // Generate struct definitions with fields and method declarations only
    for (const auto& s : program->structs) {
        out += generate_struct_fields_only(state, *s) + "\n\n";
    }

    for (const auto& e : program->errors) {
        out += generate_error(state, *e) + "\n";
    }

    // Generate standalone method implementations after all structs are defined
    for (const auto& method : program->methods) {
        out += generate_standalone_method(state, *method) + "\n";
    }

    // Emit module-level constants
    for (const auto& c : program->constants) {
        out += generate_module_constant(state, *c);
    }

    if (!program->constants.empty()) {
        out += "\n";
    }

    // Forward declare all functions
    for (const auto& fn : program->functions) {
        out += generate_function_declaration(state, *fn);
    }

    if (!program->functions.empty()) {
        out += "\n";
    }

    // Generate function definitions
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
