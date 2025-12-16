/**
 * @file check_expression.cpp
 * @brief Expression type inference for the Nog type checker.
 *
 * Handles inferring types for all expression types: literals,
 * variables, binary expressions, function calls, method calls, etc.
 */

#include "typechecker.hpp"
#include "strings.hpp"
#include "lists.hpp"

using namespace std;

namespace typechecker {

/**
 * Infers the type of an expression. Recursively handles all expression types:
 * literals, variables, binary ops, function calls, method calls, field access, etc.
 */
TypeInfo infer_type(TypeCheckerState& state, const ASTNode& expr) {
    if (auto* num = dynamic_cast<const NumberLiteral*>(&expr)) {
        (void)num;
        return {"int", false, false};
    }

    if (auto* flt = dynamic_cast<const FloatLiteral*>(&expr)) {
        (void)flt;
        return {"f64", false, false};
    }

    if (auto* str = dynamic_cast<const StringLiteral*>(&expr)) {
        (void)str;
        return {"str", false, false};
    }

    if (auto* bl = dynamic_cast<const BoolLiteral*>(&expr)) {
        (void)bl;
        return {"bool", false, false};
    }

    if (auto* none = dynamic_cast<const NoneLiteral*>(&expr)) {
        (void)none;
        return {"none", true, false};
    }

    if (auto* var = dynamic_cast<const VariableRef*>(&expr)) {
        if (state.locals.find(var->name) != state.locals.end()) {
            return state.locals[var->name];
        }

        error(state, "undefined variable '" + var->name + "'", var->line);
        return {"unknown", false, false};
    }

    if (auto* fref = dynamic_cast<const FunctionRef*>(&expr)) {
        size_t dot_pos = fref->name.find('.');

        if (dot_pos != string::npos) {
            string module_name = fref->name.substr(0, dot_pos);
            string func_name = fref->name.substr(dot_pos + 1);
            const FunctionDef* func = get_qualified_function(state, module_name, func_name);

            if (!func) {
                error(state, "undefined function '" + fref->name + "'", fref->line);
                return {"unknown", false, false};
            }

            return {"fn:" + fref->name, false, false};
        }

        if (state.functions.find(fref->name) == state.functions.end()) {
            error(state, "undefined function '" + fref->name + "'", fref->line);
            return {"unknown", false, false};
        }

        return {"fn:" + fref->name, false, false};
    }

    if (auto* bin = dynamic_cast<const BinaryExpr*>(&expr)) {
        TypeInfo left_type = infer_type(state, *bin->left);
        TypeInfo right_type = infer_type(state, *bin->right);

        if (bin->op == "==" || bin->op == "!=" || bin->op == "<" ||
            bin->op == ">" || bin->op == "<=" || bin->op == ">=") {
            return {"bool", false, false};
        }

        if (bin->op == "+" && left_type.base_type == "str") {
            return {"str", false, false};
        }

        if (left_type.base_type != right_type.base_type) {
            error(state, "type mismatch in binary expression: '" + left_type.base_type + "' " + bin->op + " '" + right_type.base_type + "'", bin->line);
        }

        return left_type;
    }

    if (auto* is_none = dynamic_cast<const IsNone*>(&expr)) {
        (void)is_none;
        return {"bool", false, false};
    }

    if (auto* not_expr = dynamic_cast<const NotExpr*>(&expr)) {
        TypeInfo inner_type = infer_type(state, *not_expr->value);

        if (inner_type.base_type != "bool") {
            error(state, "'!' operator requires bool, got '" + inner_type.base_type + "'", not_expr->line);
        }

        return {"bool", false, false};
    }

    if (auto* await_expr = dynamic_cast<const AwaitExpr*>(&expr)) {
        if (!state.in_async_context) {
            error(state, "'await' can only be used inside async functions", await_expr->line);
        }

        return infer_type(state, *await_expr->value);
    }

    if (auto* channel = dynamic_cast<const ChannelCreate*>(&expr)) {
        return {"Channel<" + channel->element_type + ">", false, false};
    }

    if (auto* list = dynamic_cast<const ListCreate*>(&expr)) {
        return {"List<" + list->element_type + ">", false, false};
    }

    if (auto* list = dynamic_cast<const ListLiteral*>(&expr)) {
        if (list->elements.empty()) {
            error(state, "cannot infer type of empty list literal, use List<T>() instead", list->line);
            return {"unknown", false, false};
        }

        TypeInfo first_type = infer_type(state, *list->elements[0]);

        for (size_t i = 1; i < list->elements.size(); i++) {
            TypeInfo elem_type = infer_type(state, *list->elements[i]);

            if (elem_type.base_type != first_type.base_type) {
                error(state, "list literal has mixed types: '" + first_type.base_type +
                      "' and '" + elem_type.base_type + "'", list->line);
            }
        }

        return {"List<" + first_type.base_type + ">", false, false};
    }

    if (auto* qref = dynamic_cast<const QualifiedRef*>(&expr)) {
        const StructDef* s = get_qualified_struct(state, qref->module_name, qref->name);

        if (s) {
            return {qref->module_name + "." + qref->name, false, false};
        }

        error(state, "undefined reference '" + qref->module_name + "." + qref->name + "'", qref->line);
        return {"unknown", false, false};
    }

    if (auto* call = dynamic_cast<const FunctionCall*>(&expr)) {
        if (call->name == "assert_eq" || call->name == "print") {
            // Still infer types for arguments (to populate object_type on MethodCalls)
            for (const auto& arg : call->args) {
                infer_type(state, *arg);
            }

            return {"void", false, true};
        }

        size_t dot_pos = call->name.find('.');

        if (dot_pos != string::npos) {
            string module_name = call->name.substr(0, dot_pos);
            string func_name = call->name.substr(dot_pos + 1);

            const FunctionDef* func = get_qualified_function(state, module_name, func_name);

            if (!func) {
                error(state, "undefined function '" + call->name + "'", call->line);
                return {"unknown", false, false};
            }

            if (call->args.size() != func->params.size()) {
                error(state, "function '" + call->name + "' expects " + to_string(func->params.size()) + " arguments, got " + to_string(call->args.size()), call->line);
            }

            for (size_t i = 0; i < call->args.size() && i < func->params.size(); i++) {
                TypeInfo arg_type = infer_type(state, *call->args[i]);
                TypeInfo param_type = {func->params[i].type, false, false};

                if (!types_compatible(param_type, arg_type)) {
                    error(state, "argument " + to_string(i + 1) + " of function '" + call->name + "' expects '" + param_type.base_type + "', got '" + arg_type.base_type + "'", call->line);
                }
            }

            if (func->return_type.empty()) {
                return {"void", false, true};
            }

            return {func->return_type, false, false};
        }

        if (state.locals.find(call->name) != state.locals.end()) {
            TypeInfo local_type = state.locals[call->name];

            if (local_type.base_type.rfind("fn(", 0) == 0) {
                size_t arrow_pos = local_type.base_type.find(" -> ");

                if (arrow_pos != string::npos) {
                    string ret_type = local_type.base_type.substr(arrow_pos + 4);
                    return {ret_type, false, false};
                }

                return {"void", false, true};
            }
        }

        if (state.functions.find(call->name) != state.functions.end()) {
            const FunctionDef* func = state.functions[call->name];

            if (call->args.size() != func->params.size()) {
                error(state, "function '" + call->name + "' expects " + to_string(func->params.size()) + " arguments, got " + to_string(call->args.size()), call->line);
            }

            for (size_t i = 0; i < call->args.size() && i < func->params.size(); i++) {
                TypeInfo arg_type = infer_type(state, *call->args[i]);
                TypeInfo param_type = {func->params[i].type, false, false};

                if (!types_compatible(param_type, arg_type)) {
                    error(state, "argument " + to_string(i + 1) + " of function '" + call->name + "' expects '" + param_type.base_type + "', got '" + arg_type.base_type + "'", call->line);
                }
            }

            if (func->return_type.empty()) {
                return {"void", false, true};
            }

            return {func->return_type, false, false};
        }

        if (state.extern_functions.find(call->name) != state.extern_functions.end()) {
            const ExternFunctionDef* ext = state.extern_functions[call->name];

            if (call->args.size() != ext->params.size()) {
                error(state, "function '" + call->name + "' expects " + to_string(ext->params.size()) + " arguments, got " + to_string(call->args.size()), call->line);
            }

            for (size_t i = 0; i < call->args.size() && i < ext->params.size(); i++) {
                TypeInfo arg_type = infer_type(state, *call->args[i]);
                TypeInfo param_type = {ext->params[i].type, false, false};

                if (!types_compatible(param_type, arg_type)) {
                    error(state, "argument " + to_string(i + 1) + " of function '" + call->name + "' expects '" + param_type.base_type + "', got '" + arg_type.base_type + "'", call->line);
                }
            }

            if (ext->return_type.empty() || ext->return_type == "void") {
                return {"void", false, true};
            }

            return {ext->return_type, false, false};
        }

        error(state, "undefined function '" + call->name + "'", call->line);
        return {"unknown", false, false};
    }

    if (auto* mcall = dynamic_cast<const MethodCall*>(&expr)) {
        TypeInfo obj_type = infer_type(state, *mcall->object);
        mcall->object_type = obj_type.base_type;  // Store for codegen

        if (obj_type.base_type.rfind("Channel<", 0) == 0) {
            size_t start = 8;
            size_t end = obj_type.base_type.find('>', start);
            string element_type = obj_type.base_type.substr(start, end - start);

            if (mcall->method_name == "send") {
                if (mcall->args.size() != 1) {
                    error(state, "Channel.send expects 1 argument, got " + to_string(mcall->args.size()), mcall->line);
                } else {
                    TypeInfo arg_type = infer_type(state, *mcall->args[0]);
                    TypeInfo expected = {element_type, false, false};

                    if (!types_compatible(expected, arg_type)) {
                        error(state, "Channel.send expects '" + element_type + "', got '" + arg_type.base_type + "'", mcall->line);
                    }
                }

                return {"void", false, true};
            } else if (mcall->method_name == "recv") {
                if (!mcall->args.empty()) {
                    error(state, "Channel.recv expects 0 arguments, got " + to_string(mcall->args.size()), mcall->line);
                }

                return {element_type, false, false};
            } else {
                error(state, "Channel has no method '" + mcall->method_name + "'", mcall->line);
                return {"unknown", false, false};
            }
        }

        if (obj_type.base_type.rfind("List<", 0) == 0) {
            size_t start = 5;
            size_t end = obj_type.base_type.find('>', start);
            string element_type = obj_type.base_type.substr(start, end - start);

            auto method_info = nog::get_list_method_info(mcall->method_name);

            if (!method_info) {
                error(state, "List has no method '" + mcall->method_name + "'", mcall->line);
                return {"unknown", false, false};
            }

            const auto& [param_types, return_type] = *method_info;

            if (mcall->args.size() != param_types.size()) {
                error(state, "method '" + mcall->method_name + "' expects " +
                      to_string(param_types.size()) + " arguments, got " +
                      to_string(mcall->args.size()), mcall->line);
            }

            for (size_t i = 0; i < mcall->args.size() && i < param_types.size(); i++) {
                TypeInfo arg_type = infer_type(state, *mcall->args[i]);
                string expected = param_types[i] == "T" ? element_type : param_types[i];
                TypeInfo expected_type = {expected, false, false};

                if (!types_compatible(expected_type, arg_type)) {
                    error(state, "argument " + to_string(i + 1) + " of method '" +
                          mcall->method_name + "' expects '" + expected +
                          "', got '" + arg_type.base_type + "'", mcall->line);
                }
            }

            string ret = return_type == "T" ? element_type : return_type;

            if (ret == "void") {
                return {"void", false, true};
            }

            return {ret, false, false};
        }

        if (obj_type.base_type == "str") {
            auto method_info = nog::get_str_method_info(mcall->method_name);

            if (!method_info) {
                error(state, "str has no method '" + mcall->method_name + "'", mcall->line);
                return {"unknown", false, false};
            }

            const auto& [param_types, return_type] = *method_info;

            if (mcall->args.size() != param_types.size()) {
                error(state, "method '" + mcall->method_name + "' expects " +
                      to_string(param_types.size()) + " arguments, got " +
                      to_string(mcall->args.size()), mcall->line);
            }

            for (size_t i = 0; i < mcall->args.size() && i < param_types.size(); i++) {
                TypeInfo arg_type = infer_type(state, *mcall->args[i]);
                TypeInfo expected = {param_types[i], false, false};

                if (!types_compatible(expected, arg_type)) {
                    error(state, "argument " + to_string(i + 1) + " of method '" +
                          mcall->method_name + "' expects '" + param_types[i] +
                          "', got '" + arg_type.base_type + "'", mcall->line);
                }
            }

            return {return_type, false, false};
        }

        const StructDef* sdef = get_struct(state, obj_type.base_type);

        if (!sdef) {
            error(state, "cannot call method on non-struct type '" + obj_type.base_type + "'", mcall->line);
            return {"unknown", false, false};
        }

        const MethodDef* method = nullptr;
        size_t dot_pos = obj_type.base_type.find('.');

        if (dot_pos != string::npos) {
            string module_name = obj_type.base_type.substr(0, dot_pos);
            string struct_name = obj_type.base_type.substr(dot_pos + 1);
            method = get_qualified_method(state, module_name, struct_name, mcall->method_name);
        } else {
            method = get_method(state, obj_type.base_type, mcall->method_name);
        }

        if (!method) {
            error(state, "method '" + mcall->method_name + "' not found on struct '" + obj_type.base_type + "'", mcall->line);
            return {"unknown", false, false};
        }

        size_t expected_args = method->params.size() - 1;

        if (mcall->args.size() != expected_args) {
            error(state, "method '" + mcall->method_name + "' expects " + to_string(expected_args) + " arguments, got " + to_string(mcall->args.size()), mcall->line);
        }

        for (size_t i = 0; i < mcall->args.size() && i + 1 < method->params.size(); i++) {
            TypeInfo arg_type = infer_type(state, *mcall->args[i]);
            TypeInfo param_type = {method->params[i + 1].type, false, false};

            if (!types_compatible(param_type, arg_type)) {
                error(state, "argument " + to_string(i + 1) + " of method '" + mcall->method_name + "' expects '" + param_type.base_type + "', got '" + arg_type.base_type + "'", mcall->line);
            }
        }

        if (method->return_type.empty()) {
            return {"void", false, true};
        }

        return {method->return_type, false, false};
    }

    if (auto* access = dynamic_cast<const FieldAccess*>(&expr)) {
        TypeInfo obj_type = infer_type(state, *access->object);

        const StructDef* sdef = get_struct(state, obj_type.base_type);

        if (!sdef) {
            error(state, "cannot access field on non-struct type '" + obj_type.base_type + "'", access->line);
            return {"unknown", false, false};
        }

        for (const auto& field : sdef->fields) {
            if (field.name == access->field_name) {
                return {field.type, false, false};
            }
        }

        error(state, "struct '" + obj_type.base_type + "' has no field '" + access->field_name + "'", access->line);
        return {"unknown", false, false};
    }

    if (auto* lit = dynamic_cast<const StructLiteral*>(&expr)) {
        const StructDef* sdef = get_struct(state, lit->struct_name);

        if (!sdef) {
            error(state, "unknown struct '" + lit->struct_name + "'", lit->line);
            return {"unknown", false, false};
        }

        for (const auto& [field_name, field_val] : lit->field_values) {
            string expected_type;

            for (const auto& f : sdef->fields) {
                if (f.name == field_name) {
                    expected_type = f.type;
                    break;
                }
            }

            if (expected_type.empty()) {
                error(state, "struct '" + lit->struct_name + "' has no field '" + field_name + "'", lit->line);
                continue;
            }

            TypeInfo val_type = infer_type(state, *field_val);
            TypeInfo exp_type = {expected_type, false, false};

            if (!types_compatible(exp_type, val_type)) {
                error(state, "field '" + field_name + "' expects '" + expected_type + "', got '" + val_type.base_type + "'", lit->line);
            }
        }

        return {lit->struct_name, false, false};
    }

    return {"unknown", false, false};
}

} // namespace typechecker
