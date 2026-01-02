/**
 * @file cpp_builder.hpp
 * @brief Builder pattern for C++ code emission in the Bishop compiler.
 *
 * This file provides a fluent API for generating C++ code with automatic
 * indentation management and brace matching. The builder pattern ensures
 * well-formed output and catches structural errors at compile time.
 */

#ifndef CODEGEN_CPP_BUILDER_HPP
#define CODEGEN_CPP_BUILDER_HPP

#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>

namespace codegen {
namespace builder {

/**
 * @brief A fluent builder for generating well-formed C++ code.
 *
 * The CppBuilder provides methods for common C++ constructs with
 * automatic indentation management. Methods return *this for chaining.
 *
 * @example
 *   CppBuilder b;
 *   b.for_range("i", "0", "n")
 *       .statement("sum += i")
 *   .end();
 *   std::string code = b.str();
 */
class CppBuilder {
public:
    /**
     * @brief Constructs a new builder with default indentation (tab).
     * @param indent_char Character(s) to use for indentation
     */
    explicit CppBuilder(const std::string& indent_char = "\t");

    /**
     * @brief Returns the built C++ code as a string.
     */
    std::string str() const;

    /**
     * @brief Clears the builder for reuse.
     */
    CppBuilder& clear();

    // ========================================================================
    // Raw output methods
    // ========================================================================

    /**
     * @brief Appends raw text without modification.
     */
    CppBuilder& raw(const std::string& text);

    /**
     * @brief Appends a line with current indentation and newline.
     */
    CppBuilder& line(const std::string& text);

    /**
     * @brief Appends an empty line.
     */
    CppBuilder& blank();

    /**
     * @brief Appends a comment with current indentation.
     */
    CppBuilder& comment(const std::string& text);

    // ========================================================================
    // Statement methods
    // ========================================================================

    /**
     * @brief Appends a statement with semicolon.
     */
    CppBuilder& statement(const std::string& stmt);

    /**
     * @brief Appends a return statement.
     */
    CppBuilder& return_stmt(const std::string& expr = "");

    /**
     * @brief Appends a break statement.
     */
    CppBuilder& break_stmt();

    /**
     * @brief Appends a continue statement.
     */
    CppBuilder& continue_stmt();

    // ========================================================================
    // Block methods (require matching end())
    // ========================================================================

    /**
     * @brief Opens a generic block with braces.
     */
    CppBuilder& block();

    /**
     * @brief Closes the current block.
     * @throws std::runtime_error if no block is open
     */
    CppBuilder& end();

    // ========================================================================
    // Control flow methods
    // ========================================================================

    /**
     * @brief Opens a for-range loop: for (int var = start; var < end; var++)
     */
    CppBuilder& for_range(const std::string& var,
                          const std::string& start,
                          const std::string& end);

    /**
     * @brief Opens a for-each loop: for (auto& var : collection)
     */
    CppBuilder& for_each(const std::string& var,
                         const std::string& collection);

    /**
     * @brief Opens a while loop: while (condition)
     */
    CppBuilder& while_loop(const std::string& condition);

    /**
     * @brief Opens an if statement: if (condition)
     */
    CppBuilder& if_stmt(const std::string& condition);

    /**
     * @brief Continues with else if: } else if (condition) {
     */
    CppBuilder& else_if(const std::string& condition);

    /**
     * @brief Continues with else: } else {
     */
    CppBuilder& else_block();

    // ========================================================================
    // Function/Method methods
    // ========================================================================

    /**
     * @brief Opens a function definition.
     * @param return_type Return type (use "" for void)
     * @param name Function name
     * @param params Parameter list string
     */
    CppBuilder& function(const std::string& return_type,
                         const std::string& name,
                         const std::string& params = "");

    /**
     * @brief Opens a static member function.
     */
    CppBuilder& static_function(const std::string& return_type,
                                const std::string& name,
                                const std::string& params = "");

    // ========================================================================
    // Struct methods
    // ========================================================================

    /**
     * @brief Opens a struct definition.
     */
    CppBuilder& struct_def(const std::string& name);

    /**
     * @brief Adds a struct field.
     */
    CppBuilder& field(const std::string& type, const std::string& name);

    /**
     * @brief Ends a struct definition with semicolon.
     */
    CppBuilder& end_struct();

    // ========================================================================
    // Indentation control
    // ========================================================================

    /**
     * @brief Increases indentation level.
     */
    CppBuilder& indent();

    /**
     * @brief Decreases indentation level.
     */
    CppBuilder& dedent();

    /**
     * @brief Returns current nesting depth (for debugging).
     */
    int depth() const { return nesting_depth_; }

private:
    std::ostringstream buffer_;
    std::string indent_char_;
    int indent_level_ = 0;
    int nesting_depth_ = 0;  // Tracks open blocks

    /**
     * @brief Returns the current indentation string.
     */
    std::string current_indent() const;
};

// ============================================================================
// Convenience functions for one-shot generation
// ============================================================================

/**
 * @brief Generates a for-range loop.
 */
std::string for_range_stmt(const std::string& var,
                           const std::string& start,
                           const std::string& end,
                           const std::vector<std::string>& body);

/**
 * @brief Generates a for-each loop.
 */
std::string for_each_stmt(const std::string& var,
                          const std::string& collection,
                          const std::vector<std::string>& body);

/**
 * @brief Generates a function definition.
 */
std::string function_def(const std::string& return_type,
                         const std::string& name,
                         const std::string& params,
                         const std::vector<std::string>& body);

/**
 * @brief Generates a struct definition.
 */
std::string struct_def(const std::string& name,
                       const std::vector<std::pair<std::string, std::string>>& fields);

/**
 * @brief Generates an if statement with optional else.
 */
std::string if_stmt(const std::string& condition,
                    const std::vector<std::string>& then_body,
                    const std::vector<std::string>& else_body = {});

} // namespace builder
} // namespace codegen

#endif // CODEGEN_CPP_BUILDER_HPP
