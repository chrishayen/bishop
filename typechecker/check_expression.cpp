/**
 * @file check_expression.cpp
 * @brief Expression type inference dispatch for the Bishop type checker.
 *
 * Dispatches to specialized check functions based on expression type.
 */

#include "typechecker.hpp"

using namespace std;

namespace typechecker {

/**
 * Infers the type of an expression. Dispatches to specialized check functions.
 */
TypeInfo infer_type(TypeCheckerState& state, const ASTNode& expr) {
    if (auto* num = dynamic_cast<const NumberLiteral*>(&expr)) {
        return check_number_literal(state, *num);
    }

    if (auto* flt = dynamic_cast<const FloatLiteral*>(&expr)) {
        return check_float_literal(state, *flt);
    }

    if (auto* str = dynamic_cast<const StringLiteral*>(&expr)) {
        return check_string_literal(state, *str);
    }

    if (auto* bl = dynamic_cast<const BoolLiteral*>(&expr)) {
        return check_bool_literal(state, *bl);
    }

    if (auto* none = dynamic_cast<const NoneLiteral*>(&expr)) {
        return check_none_literal(state, *none);
    }

    if (auto* var = dynamic_cast<const VariableRef*>(&expr)) {
        return check_variable_ref(state, *var);
    }

    if (auto* fref = dynamic_cast<const FunctionRef*>(&expr)) {
        return check_function_ref(state, *fref);
    }

    if (auto* qref = dynamic_cast<const QualifiedRef*>(&expr)) {
        return check_qualified_ref(state, *qref);
    }

    if (auto* bin = dynamic_cast<const BinaryExpr*>(&expr)) {
        return check_binary_expr(state, *bin);
    }

    if (auto* is_none = dynamic_cast<const IsNone*>(&expr)) {
        return check_is_none(state, *is_none);
    }

    if (auto* not_expr = dynamic_cast<const NotExpr*>(&expr)) {
        return check_not_expr(state, *not_expr);
    }

    if (auto* negate_expr = dynamic_cast<const NegateExpr*>(&expr)) {
        return check_negate_expr(state, *negate_expr);
    }

    if (auto* paren = dynamic_cast<const ParenExpr*>(&expr)) {
        if (!paren->value) {
            return {"unknown", false, false};
        }

        return infer_type(state, *paren->value);
    }

    if (auto* addr = dynamic_cast<const AddressOf*>(&expr)) {
        return check_address_of(state, *addr);
    }

    if (auto* channel = dynamic_cast<const ChannelCreate*>(&expr)) {
        return check_channel_create(state, *channel);
    }

    if (auto* list = dynamic_cast<const ListCreate*>(&expr)) {
        return check_list_create(state, *list);
    }

    if (auto* list = dynamic_cast<const ListLiteral*>(&expr)) {
        return check_list_literal(state, *list);
    }

    if (auto* pair = dynamic_cast<const PairCreate*>(&expr)) {
        return check_pair_create(state, *pair);
    }

    if (auto* tuple = dynamic_cast<const TupleCreate*>(&expr)) {
        return check_tuple_create(state, *tuple);
    }

    if (auto* set = dynamic_cast<const SetCreate*>(&expr)) {
        return check_set_create(state, *set);
    }

    if (auto* set = dynamic_cast<const SetLiteral*>(&expr)) {
        return check_set_literal(state, *set);
    }

    if (auto* lambda = dynamic_cast<const LambdaExpr*>(&expr)) {
        return check_lambda_expr(state, *lambda);
    }

    if (auto* lcall = dynamic_cast<const LambdaCall*>(&expr)) {
        return check_lambda_call(state, *lcall);
    }

    if (auto* call = dynamic_cast<const FunctionCall*>(&expr)) {
        return check_function_call(state, *call);
    }

    if (auto* mcall = dynamic_cast<const MethodCall*>(&expr)) {
        return check_method_call(state, *mcall);
    }

    if (auto* access = dynamic_cast<const FieldAccess*>(&expr)) {
        return check_field_access(state, *access);
    }

    if (auto* lit = dynamic_cast<const StructLiteral*>(&expr)) {
        return check_struct_literal(state, *lit);
    }

    if (auto* or_expr = dynamic_cast<const OrExpr*>(&expr)) {
        TypeInfo expr_type = infer_type(state, *or_expr->expr);

        // Check if expression is fallible (Result<T>) or falsy (bool, int, float, str, list, optional)
        auto is_falsy_type = [](const TypeInfo& type) -> bool {
            if (type.is_fallible) return true;
            if (type.is_optional) return true;
            if (type.base_type == "bool") return true;
            if (type.base_type == "str") return true;
            if (type.base_type == "int") return true;
            if (type.base_type == "float") return true;
            if (type.base_type.rfind("List<", 0) == 0) return true;
            return false;
        };

        bool is_fallible = expr_type.is_fallible;
        bool is_falsy = is_falsy_type(expr_type);

        if (!is_falsy) {
            error(state, "or handler requires a fallible or falsy expression (bool, int, float, str, list, optional)", or_expr->line);
        }

        // Validate handler combinations for falsy (non-fallible) expressions
        if (is_falsy && !is_fallible) {
            // or fail err - not valid with falsy (no err available)
            if (auto* or_fail = dynamic_cast<const OrFail*>(or_expr->handler.get())) {
                if (auto* var = dynamic_cast<const VariableRef*>(or_fail->error_expr.get())) {
                    if (var->name == "err") {
                        error(state, "'or fail err' cannot be used with falsy expressions (no error to propagate); use 'or fail \"message\"' instead", or_fail->line);
                    }
                }
            }

            // or match - not valid with falsy (nothing to match on)
            if (dynamic_cast<const OrMatch*>(or_expr->handler.get())) {
                error(state, "'or match' cannot be used with falsy expressions (no error to match); use 'or return', 'or fail \"message\"', or 'or { block }' instead", or_expr->line);
            }
        }

        // Check if or fail is used in a non-fallible function (allowed in main)
        if (auto* or_fail = dynamic_cast<const OrFail*>(or_expr->handler.get())) {
            if (!state.current_function_is_fallible && !state.in_main) {
                error(state, "or fail can only be used in fallible functions (functions with -> T or err)", or_fail->line);
            }
        }

        // If this is an OrBlock, typecheck its body with 'err' in scope only for fallible expressions
        if (auto* or_block = dynamic_cast<const OrBlock*>(or_expr->handler.get())) {
            push_scope(state);
            // Only add 'err' to scope if the expression is fallible (not just falsy)
            if (is_fallible) {
                declare_local(state, "err", {"err", false, false}, or_expr->line);
            }

            for (const auto& stmt : or_block->body) {
                check_statement(state, *stmt);
            }

            pop_scope(state);
        }

        // The result type is the unwrapped (non-fallible) type
        expr_type.is_fallible = false;
        return expr_type;
    }

    if (auto* def_expr = dynamic_cast<const DefaultExpr*>(&expr)) {
        // Type check both the expression and fallback
        TypeInfo expr_type = infer_type(state, *def_expr->expr);
        TypeInfo fallback_type = infer_type(state, *def_expr->fallback);

        // Verify types are compatible
        if (!types_compatible(expr_type, fallback_type)) {
            error(state, "default fallback type '" + format_type(fallback_type) +
                  "' does not match expression type '" + format_type(expr_type) + "'", def_expr->line);
        }

        return expr_type;
    }

    return {"unknown", false, false};
}

} // namespace typechecker
