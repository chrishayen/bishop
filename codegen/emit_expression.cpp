/**
 * @file emit_expression.cpp
 * @brief Expression dispatch for the Bishop code generator.
 *
 * Main entry point for generating C++ code from expression AST nodes.
 * Individual expression types are implemented in separate files.
 */

#include "codegen.hpp"
#include <fmt/format.h>
#include <fmt/ranges.h>

using namespace std;

namespace codegen {

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
        // Check if this is a using alias for a constant
        const CodeGenUsingAlias* alias = get_using_alias(state, ref->name);

        if (alias) {
            string module_name = alias->module_alias;

            // Map module names that conflict with C/C++ identifiers
            if (module_name == "random") {
                module_name = "bishop_random";
            }

            return module_name + "::" + alias->member_name;
        }

        return variable_ref(ref->name);
    }

    if (auto* fref = dynamic_cast<const FunctionRef*>(&node)) {
        return emit_function_ref(*fref);
    }

    if (auto* expr = dynamic_cast<const BinaryExpr*>(&node)) {
        return binary_expr(emit(state, *expr->left), expr->op, emit(state, *expr->right));
    }

    if (auto* expr = dynamic_cast<const IsNone*>(&node)) {
        return is_none(emit(state, *expr->value));
    }

    if (auto* expr = dynamic_cast<const NotExpr*>(&node)) {
        return emit_not_expr(state, *expr);
    }

    if (auto* expr = dynamic_cast<const NegateExpr*>(&node)) {
        return emit_negate_expr(state, *expr);
    }

    if (auto* expr = dynamic_cast<const ParenExpr*>(&node)) {
        return "(" + emit(state, *expr->value) + ")";
    }

    if (auto* addr = dynamic_cast<const AddressOf*>(&node)) {
        return emit_address_of(state, *addr);
    }

    if (auto* channel = dynamic_cast<const ChannelCreate*>(&node)) {
        return emit_channel_create(state, *channel);
    }

    if (auto* list = dynamic_cast<const ListCreate*>(&node)) {
        return emit_list_create(*list);
    }

    if (auto* list = dynamic_cast<const ListLiteral*>(&node)) {
        return emit_list_literal(state, *list);
    }

    if (auto* map = dynamic_cast<const MapCreate*>(&node)) {
        return emit_map_create(*map);
    }

    if (auto* map = dynamic_cast<const MapLiteral*>(&node)) {
        return emit_map_literal(state, *map);
    }

    if (auto* pair = dynamic_cast<const PairCreate*>(&node)) {
        return emit_pair_create(state, *pair);
    }

    if (auto* tuple = dynamic_cast<const TupleCreate*>(&node)) {
        return emit_tuple_create(state, *tuple);
    }

    if (auto* deque = dynamic_cast<const DequeCreate*>(&node)) {
        return emit_deque_create(*deque);
    }

    if (auto* stack = dynamic_cast<const StackCreate*>(&node)) {
        return emit_stack_create(*stack);
    }

    if (auto* queue = dynamic_cast<const QueueCreate*>(&node)) {
        return emit_queue_create(*queue);
    }

    if (auto* set = dynamic_cast<const SetCreate*>(&node)) {
        return emit_set_create(*set);
    }

    if (auto* set = dynamic_cast<const SetLiteral*>(&node)) {
        return emit_set_literal(state, *set);
    }

    if (auto* lambda = dynamic_cast<const LambdaExpr*>(&node)) {
        return emit_lambda_expr(state, *lambda);
    }

    if (auto* lcall = dynamic_cast<const LambdaCall*>(&node)) {
        return emit_lambda_call(state, *lcall);
    }

    if (auto* qref = dynamic_cast<const QualifiedRef*>(&node)) {
        return emit_qualified_ref(*qref);
    }

    if (auto* call = dynamic_cast<const FunctionCall*>(&node)) {
        return emit_function_call(state, *call);
    }

    if (auto* decl = dynamic_cast<const VariableDecl*>(&node)) {
        // Special handling for OrExpr - needs to generate multiple statements
        if (auto* or_expr = dynamic_cast<const OrExpr*>(decl->value.get())) {
            auto result = emit_or_for_decl(state, *or_expr, decl->name);

            if (result.is_match) {
                // For match, we declare variable first, then assign in branches
                // Use decltype with remove_reference to get the value type from the Result
                string cpp_type = decl->type.empty()
                    ? "std::remove_reference_t<decltype(" + result.temp_var + ".value())>"
                    : map_type(decl->type);
                string const_prefix = decl->is_const ? "const " : "";
                string out = result.preamble + "\n\t";
                out += const_prefix + cpp_type + " " + decl->name + ";\n\t";
                out += result.check;
                return out;
            }

            string out = result.preamble + "\n\t";
            out += result.check + "\n\t";
            out += variable_decl(decl->type, decl->name, result.value_expr, decl->is_optional, decl->is_const);
            return out;
        }

        return variable_decl(decl->type, decl->name, emit(state, *decl->value), decl->is_optional, decl->is_const);
    }

    if (auto* def_expr = dynamic_cast<const DefaultExpr*>(&node)) {
        return emit_default_expr(state, *def_expr);
    }

    if (auto* or_expr = dynamic_cast<const OrExpr*>(&node)) {
        return emit_or_expr(state, *or_expr);
    }

    if (auto* assign = dynamic_cast<const Assignment*>(&node)) {
        return assignment(assign->name, emit(state, *assign->value));
    }

    if (auto* ret = dynamic_cast<const ReturnStmt*>(&node)) {
        if (ret->value) {
            // Special handling for OrExpr in return - need to generate statements first
            if (auto* or_expr = dynamic_cast<const OrExpr*>(ret->value.get())) {
                auto result = emit_or_for_decl(state, *or_expr, "_ret_tmp");
                string out = result.preamble + "\n\t";
                out += result.check + "\n\t";
                out += "return " + result.value_expr + ";";
                return out;
            }
            return "return " + emit(state, *ret->value) + ";";
        }

        // Void return - use {} for Result<void>, plain return for void
        return state.in_fallible_function ? "return {};" : "return;";
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
            string module_name = struct_name.substr(0, dot_pos);
            string type_name = struct_name.substr(dot_pos + 1);

            // Map module names that conflict with C/C++ identifiers
            if (module_name == "time") {
                module_name = "bishop_time";
            }

            struct_name = module_name + "::" + type_name;
        } else {
            // Check if this is a using alias for a struct
            const CodeGenUsingAlias* alias = get_using_alias(state, lit->struct_name);

            if (alias) {
                string module_name = alias->module_alias;

                // Map module names that conflict with C/C++ identifiers
                if (module_name == "time") {
                    module_name = "bishop_time";
                } else if (module_name == "random") {
                    module_name = "bishop_random";
                }

                struct_name = module_name + "::" + alias->member_name;
            }
        }

        return struct_literal(struct_name, field_values);
    }

    if (auto* access = dynamic_cast<const FieldAccess*>(&node)) {
        return emit_field_access(state, *access);
    }

    if (auto* call = dynamic_cast<const MethodCall*>(&node)) {
        return emit_method_call(state, *call);
    }

    if (auto* fa = dynamic_cast<const FieldAssignment*>(&node)) {
        return emit_field_assignment(state, *fa);
    }

    return "";
}

} // namespace codegen
