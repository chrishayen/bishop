/**
 * @file emit_function.cpp
 * @brief Function and method emission for the Bishop code generator.
 *
 * Handles emitting C++ code for function definitions, method definitions,
 * and the test harness.
 */

#include "codegen.hpp"
#include <fmt/format.h>
#include <fmt/ranges.h>

using namespace std;

namespace codegen {

/**
 * Emits a complete function definition with parameters, return type, and body.
 */
string function_def(const string& name, const vector<FunctionParam>& params,
                    const string& return_type, const vector<string>& body) {
    string rt = return_type.empty() ? "void" : map_type(return_type);

    vector<string> param_strs;

    for (const auto& p : params) {
        param_strs.push_back(fmt::format("{} {}", map_type(p.type), p.name));
    }

    string out = fmt::format("{} {}({}) {{\n", rt, name, fmt::join(param_strs, ", "));

    for (const auto& stmt : body) {
        out += fmt::format("\t{}\n", stmt);
    }

    out += "}\n";
    return out;
}

/**
 * Emits a method definition as a C++ member function.
 */
string method_def(const string& name,
                  const vector<pair<string, string>>& params,
                  const string& return_type,
                  const vector<string>& body_stmts) {
    string rt = return_type.empty() ? "void" : map_type(return_type);

    vector<string> param_strs;

    for (const auto& [ptype, pname] : params) {
        param_strs.push_back(fmt::format("{} {}", map_type(ptype), pname));
    }

    string out = fmt::format("\t{} {}({}) {{\n", rt, name, fmt::join(param_strs, ", "));

    for (const auto& stmt : body_stmts) {
        out += fmt::format("\t\t{}\n", stmt);
    }

    out += "\t}\n";
    return out;
}

/**
 * Returns the C++ return type for a function, accounting for fallibility.
 * Fallible functions return bishop::rt::Result<T>.
 */
static string get_cpp_return_type(const string& return_type, const string& error_type) {
    if (error_type.empty()) {
        return return_type.empty() ? "void" : map_type(return_type);
    }

    // Fallible function - return Result<T>
    if (return_type.empty()) {
        return "bishop::rt::Result<void>";
    }

    return "bishop::rt::Result<" + map_type(return_type) + ">";
}

/**
 * Generates a C++ function from a Nog FunctionDef.
 * Maps Bishop types to C++ types and handles main() specially.
 * Main is wrapped in a fiber for goroutine support.
 */
string generate_function(CodeGenState& state, const FunctionDef& fn) {
    bool is_main = (fn.name == "main" && !state.test_mode);
    bool is_fallible = !fn.error_type.empty();

    // Track fallibility for or-return handling
    bool prev_fallible = state.in_fallible_function;
    state.in_fallible_function = is_fallible;

    vector<FunctionParam> params;

    for (const auto& p : fn.params) {
        params.push_back({p.type, p.name});
    }

    vector<string> body;

    for (const auto& stmt : fn.body) {
        body.push_back(generate_statement(state, *stmt));
    }

    // Add implicit return {} for Result<void> functions without explicit return
    if (is_fallible && fn.return_type.empty() && !body.empty()) {
        // Check if the last line of the last statement is a return
        // (Statements can be multi-line, e.g. or-expressions generate multiple lines)
        string last = body.back();
        size_t last_newline = last.rfind('\n');
        string last_line = (last_newline == string::npos) ? last : last.substr(last_newline + 1);

        if (last_line.find("return") == string::npos) {
            body.push_back("return {};");
        }
    }

    state.in_fallible_function = prev_fallible;

    string out;

    if (is_main) {
        // Generate the function as _nog_main()
        string rt_type = fn.return_type.empty() ? "" : fn.return_type;
        out = function_def("_nog_main", params, rt_type, body);

        // Generate int main() using runtime wrapper
        out += "\nint main() {\n";
        out += "\tbishop::rt::run(_nog_main);\n";
        out += "\treturn 0;\n";
        out += "}\n";
    } else {
        // Use raw return type string for fallible functions
        string cpp_rt = get_cpp_return_type(fn.return_type, fn.error_type);

        vector<string> param_strs;

        for (const auto& p : params) {
            param_strs.push_back(fmt::format("{} {}", map_type(p.type), p.name));
        }

        out = fmt::format("{} {}({}) {{\n", cpp_rt, fn.name, fmt::join(param_strs, ", "));

        for (const auto& stmt : body) {
            out += fmt::format("\t{}\n", stmt);
        }

        out += "}\n";
    }

    return out;
}

/**
 * Generates a C++ member function from a Nog MethodDef.
 * Transforms self.field into this->field.
 */
string generate_method(CodeGenState& state, const MethodDef& method) {
    // Skip 'self' parameter for C++ method (it becomes 'this')
    vector<pair<string, string>> params;

    for (size_t i = 1; i < method.params.size(); i++) {
        params.push_back({method.params[i].type, method.params[i].name});
    }

    vector<string> body;

    for (const auto& stmt : method.body) {
        body.push_back(generate_statement(state, *stmt));
    }

    return method_def(method.name, params, method.return_type, body);
}

/**
 * Recursively checks if an AST node contains a ChannelCreate.
 */
static bool node_uses_channels(const ASTNode& node);

static bool statements_use_channels(const vector<unique_ptr<ASTNode>>& stmts) {
    for (const auto& stmt : stmts) {
        if (node_uses_channels(*stmt)) {
            return true;
        }
    }
    return false;
}

static bool node_uses_channels(const ASTNode& node) {
    if (dynamic_cast<const ChannelCreate*>(&node)) {
        return true;
    }

    if (auto* decl = dynamic_cast<const VariableDecl*>(&node)) {
        if (decl->type.rfind("Channel<", 0) == 0) {
            return true;
        }
        if (decl->value && node_uses_channels(*decl->value)) {
            return true;
        }
    }

    if (auto* lambda = dynamic_cast<const LambdaExpr*>(&node)) {
        if (statements_use_channels(lambda->body)) {
            return true;
        }
    }

    if (auto* lcall = dynamic_cast<const LambdaCall*>(&node)) {
        if (node_uses_channels(*lcall->callee)) {
            return true;
        }
        for (const auto& arg : lcall->args) {
            if (node_uses_channels(*arg)) {
                return true;
            }
        }
    }

    if (auto* spawn = dynamic_cast<const GoSpawn*>(&node)) {
        if (spawn->call && node_uses_channels(*spawn->call)) {
            return true;
        }
    }

    if (auto* if_stmt = dynamic_cast<const IfStmt*>(&node)) {
        if (statements_use_channels(if_stmt->then_body)) {
            return true;
        }
        if (statements_use_channels(if_stmt->else_body)) {
            return true;
        }
    }

    if (auto* while_stmt = dynamic_cast<const WhileStmt*>(&node)) {
        if (statements_use_channels(while_stmt->body)) {
            return true;
        }
    }

    if (auto* for_stmt = dynamic_cast<const ForStmt*>(&node)) {
        if (statements_use_channels(for_stmt->body)) {
            return true;
        }
    }

    return false;
}

/**
 * Checks if the program uses channels (requires channel.hpp).
 */
static bool test_uses_channels(const Program& program) {
    for (const auto& fn : program.functions) {
        for (const auto& param : fn->params) {
            if (param.type.rfind("Channel<", 0) == 0) {
                return true;
            }
        }
        if (statements_use_channels(fn->body)) {
            return true;
        }
    }

    for (const auto& m : program.methods) {
        if (statements_use_channels(m->body)) {
            return true;
        }
    }

    return false;
}

/**
 * Generates the test harness main() function.
 * Calls all test_* functions and returns failure count.
 */
string generate_test_harness(CodeGenState& state, const unique_ptr<Program>& program) {
    state.current_program = program.get();

    // Collect extern functions for FFI handling
    for (const auto& ext : program->externs) {
        state.extern_functions[ext->name] = ext.get();
    }

    string out = "#include <bishop/std.hpp>\n";

    if (test_uses_channels(*program)) {
        out += "#include <bishop/channel.hpp>\n";
    }

    out += "\n";

    // Generate extern "C" declarations for FFI
    out += generate_extern_declarations(program);

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

    vector<pair<string, bool>> test_funcs;  // name, is_fallible

    for (const auto& fn : program->functions) {
        if (fn->name.rfind("test_", 0) == 0) {
            test_funcs.push_back({fn->name, !fn->error_type.empty()});
        }

        out += generate_function(state, *fn);
    }

    out += "\nint main() {\n";
    out += "\tbishop::rt::init_runtime();\n";
    out += "\n";

    // Run each test in a fiber, join to wait for completion
    for (const auto& [name, is_fallible] : test_funcs) {
        if (is_fallible) {
            // Fallible test: check for errors
            out += "\tbishop::rt::run_in_fiber([]() {\n";
            out += "\t\tauto result = " + name + "();\n";
            out += "\t\tif (result.is_error()) {\n";
            out += "\t\t\tstd::cerr << \"" + name + ": FAIL: \" << result.error()->message << std::endl;\n";
            out += "\t\t\t_failures++;\n";
            out += "\t\t}\n";
            out += "\t});\n";
        } else {
            out += "\tbishop::rt::run_in_fiber(" + name + ");\n";
        }
    }

    out += "\treturn _failures;\n";
    out += "}\n";

    return out;
}

} // namespace codegen
