/**
 * @file typechecker.hpp
 * @brief Static type checker for the Nog language.
 *
 * Performs semantic analysis on the AST before code generation using
 * standalone functions in the typechecker namespace with explicit state passing.
 */

#pragma once
#include <string>
#include <vector>
#include <map>
#include "parser/ast.hpp"
#include "project/module.hpp"

/**
 * @brief A type error found during checking.
 * Contains the error message and source location for reporting.
 */
struct TypeError {
    std::string message;
    int line;
    std::string filename;
};

/**
 * @brief Represents the type of an expression or variable.
 */
struct TypeInfo {
    std::string base_type;
    bool is_optional = false;
    bool is_void = false;

    bool operator==(const TypeInfo& other) const {
        return base_type == other.base_type && is_optional == other.is_optional;
    }

    bool operator!=(const TypeInfo& other) const {
        return !(*this == other);
    }
};

/**
 * @brief Type checker state passed to all checking functions.
 */
struct TypeCheckerState {
    // Symbol tables
    std::map<std::string, const StructDef*> structs;
    std::map<std::string, std::vector<const MethodDef*>> methods;
    std::map<std::string, const FunctionDef*> functions;
    std::map<std::string, const ExternFunctionDef*> extern_functions;
    std::map<std::string, TypeInfo> locals;
    std::map<std::string, const Module*> imported_modules;

    // Current context
    std::string current_struct;
    TypeInfo current_return;
    std::string filename;
    bool in_async_context = false;

    std::vector<TypeError> errors;
};

namespace typechecker {

// Main entry point (typechecker.cpp)
bool check(TypeCheckerState& state, const Program& program, const std::string& filename);
void register_module(TypeCheckerState& state, const std::string& alias, const Module& module);

// Collection functions (typechecker.cpp)
void collect_structs(TypeCheckerState& state, const Program& program);
void collect_methods(TypeCheckerState& state, const Program& program);
void collect_functions(TypeCheckerState& state, const Program& program);
void collect_extern_functions(TypeCheckerState& state, const Program& program);

// Function/method checking (check_function.cpp)
void check_method(TypeCheckerState& state, const MethodDef& method);
void check_function(TypeCheckerState& state, const FunctionDef& func);
bool has_return(const std::vector<std::unique_ptr<ASTNode>>& stmts);

// Statement checking (check_statement.cpp)
void check_statement(TypeCheckerState& state, const ASTNode& stmt);

// Expression type inference (check_expression.cpp)
TypeInfo infer_type(TypeCheckerState& state, const ASTNode& expr);

// Type utilities (typechecker.cpp)
bool is_primitive_type(const std::string& type);
bool is_valid_type(const TypeCheckerState& state, const std::string& type);
bool types_compatible(const TypeInfo& expected, const TypeInfo& actual);
const StructDef* get_struct(const TypeCheckerState& state, const std::string& name);
const MethodDef* get_method(const TypeCheckerState& state, const std::string& struct_name, const std::string& method_name);
std::string get_field_type(const TypeCheckerState& state, const std::string& struct_name, const std::string& field_name);
void error(TypeCheckerState& state, const std::string& msg, int line);

// Module-aware lookups (typechecker.cpp)
const FunctionDef* get_qualified_function(const TypeCheckerState& state, const std::string& module, const std::string& name);
const StructDef* get_qualified_struct(const TypeCheckerState& state, const std::string& module, const std::string& name);
const MethodDef* get_qualified_method(const TypeCheckerState& state, const std::string& module, const std::string& struct_name, const std::string& method_name);

} // namespace typechecker

/**
 * @brief Legacy class API for backwards compatibility.
 */
class TypeChecker {
public:
    bool check(const Program& program, const std::string& filename);
    void register_module(const std::string& alias, const Module& module);
    const std::vector<TypeError>& get_errors() const { return state.errors; }

private:
    TypeCheckerState state;
};
