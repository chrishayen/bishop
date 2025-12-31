/**
 * @file emit_error.cpp
 * @brief Error type emission for the Bishop code generator.
 *
 * Handles emitting C++ code for error type definitions.
 * Error types inherit from bishop::rt::Error and have message/cause built-in.
 */

#include "codegen.hpp"
#include <fmt/format.h>
#include <fmt/ranges.h>

using namespace std;

namespace codegen {

/**
 * Generates a C++ struct that inherits from bishop::rt::Error.
 * All error types have built-in message (string) and cause (shared_ptr<Error>).
 */
string generate_error(CodeGenState& state, const ErrorDef& def) {
    string out = fmt::format("struct {} : public bishop::rt::Error {{\n", def.name);

    // Generate custom fields
    for (const auto& f : def.fields) {
        string cpp_type = map_type(f.type);

        if (cpp_type == "void") {
            cpp_type = f.type;
        }

        out += fmt::format("\t{} {};\n", cpp_type, f.name);
    }

    // Generate constructor
    vector<string> params;
    vector<string> inits;

    params.push_back("const std::string& msg");

    for (const auto& f : def.fields) {
        string cpp_type = map_type(f.type);

        if (cpp_type == "void") {
            cpp_type = f.type;
        }

        params.push_back(fmt::format("{} {}_", cpp_type, f.name));
        inits.push_back(fmt::format("{}({}_)", f.name, f.name));
    }

    params.push_back("std::shared_ptr<bishop::rt::Error> cause_ = nullptr");

    out += fmt::format("\n\t{}({}) : bishop::rt::Error(msg, cause_)", def.name, fmt::join(params, ", "));

    if (!inits.empty()) {
        out += fmt::format(", {}", fmt::join(inits, ", "));
    }

    out += " {}\n";

    // Generate message-only constructor for bare error type usage: fail ErrorType;
    if (!def.fields.empty()) {
        vector<string> default_inits;

        for (const auto& f : def.fields) {
            string cpp_type = map_type(f.type);

            if (cpp_type == "void") {
                cpp_type = f.type;
            }

            // Default values based on type
            string default_val;

            if (cpp_type == "int" || cpp_type == "int64_t" || cpp_type == "uint64_t") {
                default_val = "0";
            } else if (cpp_type == "double" || cpp_type == "float") {
                default_val = "0.0";
            } else if (cpp_type == "bool") {
                default_val = "false";
            } else if (cpp_type == "std::string" || f.type == "str") {
                default_val = "\"\"";
            } else {
                default_val = "{}";  // Default-construct other types
            }

            default_inits.push_back(fmt::format("{}({})", f.name, default_val));
        }

        out += fmt::format("\texplicit {}(const std::string& msg) : bishop::rt::Error(msg, nullptr), {} {{}}\n",
                           def.name, fmt::join(default_inits, ", "));
    }

    out += "};\n";
    return out;
}

/**
 * Generates error literal construction.
 * Returns shared_ptr creation expression.
 */
string error_literal(const string& name, const string& message, const vector<pair<string, string>>& field_values) {
    vector<string> args;
    args.push_back(message);

    for (const auto& [field_name, value] : field_values) {
        args.push_back(value);
    }

    return fmt::format("std::make_shared<{}>({})", name, fmt::join(args, ", "));
}

} // namespace codegen
