/**
 * @file emit_expression.cpp
 * @brief Expression emission for the Nog code generator.
 *
 * Handles emitting C++ code for all expression types: literals,
 * variable references, binary expressions, function calls, etc.
 */

#include "codegen.hpp"
#include <fmt/format.h>
#include <fmt/ranges.h>

using namespace std;

namespace codegen {

/**
 * Emits a C++ string literal wrapped in std::string constructor.
 */
string string_literal(const string& value) {
    return fmt::format("std::string(\"{}\")", value);
}

/**
 * Emits a C++ integer literal.
 */
string number_literal(const string& value) {
    return value;
}

/**
 * Emits a C++ float literal.
 */
string float_literal(const string& value) {
    return value;
}

/**
 * Emits a C++ bool literal: true or false.
 */
string bool_literal(bool value) {
    return value ? "true" : "false";
}

/**
 * Emits std::nullopt for the none literal.
 */
string none_literal() {
    return "std::nullopt";
}

/**
 * Emits a variable reference (just the name).
 */
string variable_ref(const string& name) {
    return name;
}

/**
 * Emits a binary expression: left op right.
 */
string binary_expr(const string& left, const string& op, const string& right) {
    return fmt::format("{} {} {}", left, op, right);
}

/**
 * Emits a function call: name(arg1, arg2, ...).
 */
string function_call(const string& name, const vector<string>& args) {
    return fmt::format("{}({})", name, fmt::join(args, ", "));
}

/**
 * Emits an "is none" check using std::optional::has_value().
 */
string is_none(const string& value) {
    return fmt::format("!{}.has_value()", value);
}

/**
 * Emits a method call: object.method(arg1, arg2, ...).
 */
string method_call(const string& object, const string& method, const vector<string>& args) {
    return fmt::format("{}.{}({})", object, method, fmt::join(args, ", "));
}

/**
 * Emits C++ code for an expression AST node. Handles all expression types:
 * literals, variable refs, binary expressions, function calls, method calls,
 * field access, struct literals, etc.
 */
string emit(CodeGenState& state, const ASTNode& node) {
    if (auto* lit = dynamic_cast<const StringLiteral*>(&node)) {
        return string_literal(lit->value);
    }

    if (auto* lit = dynamic_cast<const NumberLiteral*>(&node)) {
        return number_literal(lit->value);
    }

    if (auto* lit = dynamic_cast<const FloatLiteral*>(&node)) {
        return float_literal(lit->value);
    }

    if (auto* lit = dynamic_cast<const BoolLiteral*>(&node)) {
        return bool_literal(lit->value);
    }

    if (dynamic_cast<const NoneLiteral*>(&node)) {
        return none_literal();
    }

    if (auto* ref = dynamic_cast<const VariableRef*>(&node)) {
        return variable_ref(ref->name);
    }

    if (auto* fref = dynamic_cast<const FunctionRef*>(&node)) {
        // Function reference - emit the function name (possibly qualified with ::)
        string func_name = fref->name;
        size_t dot_pos = func_name.find('.');

        if (dot_pos != string::npos) {
            func_name = func_name.substr(0, dot_pos) + "::" + func_name.substr(dot_pos + 1);
        }

        return func_name;
    }

    if (auto* expr = dynamic_cast<const BinaryExpr*>(&node)) {
        return binary_expr(emit(state, *expr->left), expr->op, emit(state, *expr->right));
    }

    if (auto* expr = dynamic_cast<const IsNone*>(&node)) {
        return is_none(emit(state, *expr->value));
    }

    if (auto* expr = dynamic_cast<const NotExpr*>(&node)) {
        return "!" + emit(state, *expr->value);
    }

    if (auto* expr = dynamic_cast<const AwaitExpr*>(&node)) {
        // Special handling for channel recv: await ch.recv() -> std::get<1>(co_await ch.async_receive(...))
        if (auto* mcall = dynamic_cast<const MethodCall*>(expr->value.get())) {
            if (mcall->method_name == "recv") {
                return "std::get<1>(co_await " + emit(state, *mcall->object) +
                       ".async_receive(asio::as_tuple(asio::use_awaitable)))";
            }
        }

        // Wrap in parentheses since co_await has low precedence in C++
        return "(co_await " + emit(state, *expr->value) + ")";
    }

    if (auto* channel = dynamic_cast<const ChannelCreate*>(&node)) {
        // Channel<T>() -> asio::experimental::channel<void(asio::error_code, T)>(ex, 1)
        string cpp_type = map_type(channel->element_type);
        return "asio::experimental::channel<void(asio::error_code, " + cpp_type + ")>(co_await asio::this_coro::executor, 1)";
    }

    if (auto* list = dynamic_cast<const ListCreate*>(&node)) {
        // List<T>() -> std::vector<T>{}
        string cpp_type = map_type(list->element_type);
        return "std::vector<" + cpp_type + ">{}";
    }

    if (auto* list = dynamic_cast<const ListLiteral*>(&node)) {
        // [1, 2, 3] -> std::vector{1, 2, 3} (uses CTAD)
        string elements;

        for (size_t i = 0; i < list->elements.size(); i++) {
            if (i > 0) {
                elements += ", ";
            }

            elements += emit(state, *list->elements[i]);
        }

        return "std::vector{" + elements + "}";
    }

    if (auto* qref = dynamic_cast<const QualifiedRef*>(&node)) {
        // Qualified reference: module.name -> module::name
        return qref->module_name + "::" + qref->name;
    }

    if (auto* call = dynamic_cast<const FunctionCall*>(&node)) {
        vector<string> args;

        for (const auto& arg : call->args) {
            args.push_back(emit(state, *arg));
        }

        // Handle qualified function call: module.func -> module::func
        string func_name = call->name;
        size_t dot_pos = func_name.find('.');

        if (dot_pos != string::npos) {
            func_name = func_name.substr(0, dot_pos) + "::" + func_name.substr(dot_pos + 1);
        }

        return function_call(func_name, args);
    }

    if (auto* decl = dynamic_cast<const VariableDecl*>(&node)) {
        return variable_decl(decl->type, decl->name, emit(state, *decl->value), decl->is_optional);
    }

    if (auto* assign = dynamic_cast<const Assignment*>(&node)) {
        return assignment(assign->name, emit(state, *assign->value));
    }

    if (auto* ret = dynamic_cast<const ReturnStmt*>(&node)) {
        string keyword = state.in_async_function ? "co_return" : "return";
        return keyword + " " + emit(state, *ret->value) + ";";
    }

    if (auto* lit = dynamic_cast<const StructLiteral*>(&node)) {
        vector<pair<string, string>> field_values;

        for (const auto& [name, value] : lit->field_values) {
            field_values.push_back({name, emit(state, *value)});
        }

        // Handle qualified struct name: module.Type -> module::Type
        string struct_name = lit->struct_name;
        size_t dot_pos = struct_name.find('.');

        if (dot_pos != string::npos) {
            struct_name = struct_name.substr(0, dot_pos) + "::" + struct_name.substr(dot_pos + 1);
        }

        return struct_literal(struct_name, field_values);
    }

    if (auto* access = dynamic_cast<const FieldAccess*>(&node)) {
        // Handle self.field -> this->field in methods
        if (auto* ref = dynamic_cast<const VariableRef*>(access->object.get())) {
            if (ref->name == "self") {
                return "this->" + access->field_name;
            }
        }

        return field_access(emit(state, *access->object), access->field_name);
    }

    if (auto* call = dynamic_cast<const MethodCall*>(&node)) {
        vector<string> args;

        for (const auto& arg : call->args) {
            args.push_back(emit(state, *arg));
        }

        // Handle self.method() -> this->method() in methods
        if (auto* ref = dynamic_cast<const VariableRef*>(call->object.get())) {
            if (ref->name == "self") {
                string args_str;

                for (size_t i = 0; i < args.size(); i++) {
                    if (i > 0) args_str += ", ";
                    args_str += args[i];
                }

                return "this->" + call->method_name + "(" + args_str + ")";
            }
        }

        // Handle channel methods using ASIO experimental channel
        if (call->method_name == "send") {
            string val = args.empty() ? "" : args[0];
            return emit(state, *call->object) + ".async_send(asio::error_code{}, " + val + ", asio::use_awaitable)";
        }

        if (call->method_name == "recv") {
            // Return raw async call - will be wrapped by AwaitExpr or used in select
            return emit(state, *call->object) + ".async_receive(asio::as_tuple(asio::use_awaitable))";
        }

        string obj_str = emit(state, *call->object);

        // Handle List methods - map to std::vector equivalents
        // Only apply when object_type indicates a List
        if (call->object_type.rfind("List<", 0) == 0) {
            if (call->method_name == "length") {
                return obj_str + ".size()";
            }

            if (call->method_name == "is_empty") {
                return obj_str + ".empty()";
            }

            if (call->method_name == "append") {
                return obj_str + ".push_back(" + args[0] + ")";
            }

            if (call->method_name == "pop") {
                return obj_str + ".pop_back()";
            }

            if (call->method_name == "get") {
                return obj_str + ".at(" + args[0] + ")";
            }

            if (call->method_name == "set") {
                return obj_str + "[" + args[0] + "] = " + args[1];
            }

            if (call->method_name == "clear") {
                return obj_str + ".clear()";
            }

            if (call->method_name == "first") {
                return obj_str + ".front()";
            }

            if (call->method_name == "last") {
                return obj_str + ".back()";
            }

            if (call->method_name == "insert") {
                return obj_str + ".insert(" + obj_str + ".begin() + " + args[0] + ", " + args[1] + ")";
            }

            if (call->method_name == "remove") {
                return obj_str + ".erase(" + obj_str + ".begin() + " + args[0] + ")";
            }

            if (call->method_name == "contains") {
                return "(std::find(" + obj_str + ".begin(), " + obj_str + ".end(), " + args[0] + ") != " + obj_str + ".end())";
            }
        }

        return method_call(obj_str, call->method_name, args);
    }

    if (auto* fa = dynamic_cast<const FieldAssignment*>(&node)) {
        // Handle self.field = value -> this->field = value in methods
        if (auto* ref = dynamic_cast<const VariableRef*>(fa->object.get())) {
            if (ref->name == "self") {
                return "this->" + fa->field_name + " = " + emit(state, *fa->value);
            }
        }

        return field_assignment(emit(state, *fa->object), fa->field_name, emit(state, *fa->value));
    }

    return "";
}

} // namespace codegen
