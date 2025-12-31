/**
 * @file emit_or.cpp
 * @brief Or expression emission for the Bishop code generator.
 *
 * Generates C++ code for or expressions that handle errors.
 * This is complex because or expressions need to generate statements
 * (temp variable, error check, value extraction) not just expressions.
 */

#include "codegen.hpp"
#include <fmt/format.h>
#include <fmt/ranges.h>

using namespace std;

namespace codegen {

static int or_temp_counter = 0;

/**
 * Get a unique temp variable name.
 */
static string get_temp_name() {
    return "_or_tmp" + to_string(++or_temp_counter);
}

/**
 * Emit the handler code for an OrReturn.
 */
string emit_or_return_handler(CodeGenState& state, const OrReturn& handler) {
    if (handler.value) {
        return "return " + emit(state, *handler.value) + ";";
    }

    // Use {} for Result<void> functions, plain return for void
    return state.in_fallible_function ? "return {};" : "return;";
}

/**
 * Emit the handler code for an OrContinue.
 */
string emit_or_continue_handler() {
    return "continue;";
}

/**
 * Emit the handler code for an OrBreak.
 */
string emit_or_break_handler() {
    return "break;";
}

/**
 * Emit the handler code for an OrFail.
 */
string emit_or_fail_handler(CodeGenState& state, const OrFail& handler) {
    // The err variable is bound in the generated code
    if (auto* var = dynamic_cast<const VariableRef*>(handler.error_expr.get())) {
        if (var->name == "err") {
            return "return err;";
        }
    }

    // String literal: wrap in Error
    if (auto* str_lit = dynamic_cast<const StringLiteral*>(handler.error_expr.get())) {
        return fmt::format("return std::make_shared<bishop::rt::Error>({});",
                           string_literal(str_lit->value));
    }

    // Bare error type: fail ErrorType; (StructLiteral with empty field_values)
    // Pass err as the cause to preserve error chain
    if (auto* struct_lit = dynamic_cast<const StructLiteral*>(handler.error_expr.get())) {
        if (struct_lit->field_values.empty()) {
            return fmt::format(
                "return std::static_pointer_cast<bishop::rt::Error>(std::make_shared<{}>(\"{}\", err));",
                struct_lit->struct_name,
                struct_lit->struct_name);
        }
    }

    return "return " + emit(state, *handler.error_expr) + ";";
}

/**
 * Emit the handler code for an OrBlock.
 */
string emit_or_block_handler(CodeGenState& state, const OrBlock& handler) {
    string out;

    for (const auto& stmt : handler.body) {
        out += "\t\t" + generate_statement(state, *stmt) + "\n";
    }

    return out;
}

/**
 * Emit the handler code for an OrMatch.
 * The var_name parameter is used to assign values from match arms.
 */
string emit_or_match_handler(CodeGenState& state, const OrMatch& handler, const string& var_name) {
    string out;
    bool first = true;

    for (const auto& arm : handler.arms) {
        if (arm.error_type == "_") {
            // Default arm - close previous typed arm and start else
            if (!first) {
                out += "\t\t} else {\n";
            } else {
                out += "{\n";
            }

            if (auto* fail = dynamic_cast<const FailStmt*>(arm.body.get())) {
                out += "\t\t\t" + emit_fail(state, *fail) + ";\n";
            } else {
                out += fmt::format("\t\t\t{} = {};\n", var_name, emit(state, *arm.body));
            }

            out += "\t\t}";
        } else {
            // Typed arm
            string check = first ? "if" : "\t\t} else if";
            out += fmt::format("{} (dynamic_cast<{}*>(err.get())) {{\n",
                               check, arm.error_type);

            if (auto* fail = dynamic_cast<const FailStmt*>(arm.body.get())) {
                out += "\t\t\t" + emit_fail(state, *fail) + ";\n";
            } else {
                out += fmt::format("\t\t\t{} = {};\n", var_name, emit(state, *arm.body));
            }
        }

        first = false;
    }

    // Close last arm if it wasn't the default
    if (!out.empty() && out.back() == '\n') {
        out += "\t\t}";
    }

    return out;
}

/**
 * Generates code for an OrExpr as part of a variable declaration.
 * Returns the statements that should be emitted before the variable,
 * and sets the value expression to use for the variable.
 *
 * Uses bishop::is_or_falsy() for the condition check, which handles
 * both Result<T> types (.is_error()) and falsy types (!truthy(value)).
 * Uses bishop::or_value() to extract the value, which returns .value()
 * for Result types or the value as-is for falsy types.
 */
OrEmitResult emit_or_for_decl(CodeGenState& state, const OrExpr& expr, const string& var_name) {
    OrEmitResult result;
    string temp = get_temp_name();
    result.temp_var = temp;

    // Generate the temp variable with the result
    result.preamble = fmt::format("auto {} = {};", temp, emit(state, *expr.expr));

    // Use bishop::is_or_falsy() for condition check (works for both Result and falsy types)
    // Use bishop::or_value() for value extraction (returns .value() for Result, value as-is for falsy)
    string condition = fmt::format("bishop::is_or_falsy({})", temp);
    string value_extraction = fmt::format("bishop::or_value({})", temp);

    // Generate error check
    string handler_code;

    if (auto* ret = dynamic_cast<const OrReturn*>(expr.handler.get())) {
        handler_code = emit_or_return_handler(state, *ret);
        result.check = fmt::format("if ({}) {{ {} }}", condition, handler_code);
        result.value_expr = value_extraction;
    } else if (auto* fail = dynamic_cast<const OrFail*>(expr.handler.get())) {
        // For or fail with Result types, we need to define err before returning it
        // For falsy types, the typechecker already rejected 'or fail err' so we just use the message
        bool needs_err = false;

        if (auto* var = dynamic_cast<const VariableRef*>(fail->error_expr.get())) {
            // 'or fail err' - needs err defined
            needs_err = (var->name == "err");
        } else if (auto* struct_lit = dynamic_cast<const StructLiteral*>(fail->error_expr.get())) {
            // Bare error type 'or fail ErrorType;' - needs err for cause chain
            needs_err = struct_lit->field_values.empty();
        }

        if (needs_err) {
            handler_code = "auto err = bishop::or_error(" + temp + "); " + emit_or_fail_handler(state, *fail);
        } else {
            handler_code = emit_or_fail_handler(state, *fail);
        }

        result.check = fmt::format("if ({}) {{ {} }}", condition, handler_code);
        result.value_expr = value_extraction;
    } else if (auto* block = dynamic_cast<const OrBlock*>(expr.handler.get())) {
        // For blocks, bind 'err' using bishop::or_error() which handles both Result and falsy types
        // The typechecker already validated that 'err' is only used with fallible expressions
        handler_code = emit_or_block_handler(state, *block);
        // Bind err using or_error() - for Result types this gets .error(),
        // for falsy types it returns a synthetic error (but should never be accessed)
        result.check = fmt::format("if ({}) {{ auto err = bishop::or_error({}); {} }}",
                                   condition, temp, handler_code);
        result.value_expr = value_extraction;
    } else if (auto* match = dynamic_cast<const OrMatch*>(expr.handler.get())) {
        // Match is only valid for Result types (typechecker already validated this)
        string match_code = emit_or_match_handler(state, *match, var_name);
        result.check = fmt::format(
            "if ({}.is_error()) {{\n\t\tauto err = {}.error();\n\t\t{}\n\t}} else {{\n\t\t{} = {}.value();\n\t}}",
            temp, temp, match_code, var_name, temp);
        result.value_expr = "";  // Empty - variable is assigned in the check
        result.is_match = true;
    } else if (dynamic_cast<const OrContinue*>(expr.handler.get())) {
        handler_code = emit_or_continue_handler();
        result.check = fmt::format("if ({}) {{ {} }}", condition, handler_code);
        result.value_expr = value_extraction;
    } else if (dynamic_cast<const OrBreak*>(expr.handler.get())) {
        handler_code = emit_or_break_handler();
        result.check = fmt::format("if ({}) {{ {} }}", condition, handler_code);
        result.value_expr = value_extraction;
    }

    return result;
}

/**
 * Emit a standalone or expression (not part of a declaration).
 * This is unusual but could happen in some contexts.
 */
string emit_or_expr(CodeGenState& state, const OrExpr& expr) {
    // For standalone use, just emit the expression
    // The caller will need to handle the Result type
    return emit(state, *expr.expr);
}

/**
 * Emit a default expression.
 *
 * For Pair and Tuple get() methods, generates proper bounds-checking code
 * that uses the fallback value for out-of-bounds access.
 *
 * For general expressions, uses bishop::truthy() to check falsiness,
 * which handles strings, containers, numerics, bools, and optionals.
 */
string emit_default_expr(CodeGenState& state, const DefaultExpr& expr) {
    string fallback = emit(state, *expr.fallback);

    // Check if this is a method call on Pair or Tuple
    if (auto* mcall = dynamic_cast<const MethodCall*>(expr.expr.get())) {
        if (mcall->method_name == "get") {
            string obj_type = mcall->object_type;

            // Handle Pair<T>.get(idx) default fallback
            if (obj_type.rfind("Pair<", 0) == 0) {
                string obj_str = emit(state, *mcall->object);
                string idx = emit(state, *mcall->args[0]);
                // Return first for 0, second for 1, fallback otherwise
                return fmt::format("(({}) == 0 ? {}.first : (({}) == 1 ? {}.second : {}))",
                                   idx, obj_str, idx, obj_str, fallback);
            }

            // Handle Tuple<T>.get(idx) default fallback
            if (obj_type.rfind("Tuple<", 0) == 0) {
                string obj_str = emit(state, *mcall->object);
                string idx = emit(state, *mcall->args[0]);
                // Return element at idx if in bounds, fallback otherwise
                return fmt::format("(static_cast<size_t>({}) < {}.size() && ({}) >= 0 ? {}[{}] : {})",
                                   idx, obj_str, idx, obj_str, idx, fallback);
            }
        }
    }

    // Use bishop::is_or_falsy() which handles both Result types and falsy values
    // Use bishop::or_value() to extract the value (works for Results and falsy types)
    // Wrap in lambda to avoid double-evaluation of expr
    string expr_str = emit(state, *expr.expr);
    return fmt::format("[&]() {{ auto _v = {}; return bishop::is_or_falsy(_v) ? {} : bishop::or_value(_v); }}()",
                       expr_str, fallback);
}

} // namespace codegen
