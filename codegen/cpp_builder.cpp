/**
 * @file cpp_builder.cpp
 * @brief Implementation of the fluent C++ code builder.
 *
 * This module provides a builder pattern for generating well-formed
 * C++ code with automatic indentation and brace matching.
 */

#include "cpp_builder.hpp"

namespace codegen {
namespace builder {

// ============================================================================
// CppBuilder implementation
// ============================================================================

/**
 * @brief Constructs a builder with the specified indentation character.
 */
CppBuilder::CppBuilder(const std::string& indent_char)
    : indent_char_(indent_char) {}

/**
 * @brief Returns the accumulated code as a string.
 */
std::string CppBuilder::str() const {
    return buffer_.str();
}

/**
 * @brief Clears the builder for reuse.
 */
CppBuilder& CppBuilder::clear() {
    buffer_.str("");
    buffer_.clear();
    indent_level_ = 0;
    nesting_depth_ = 0;
    return *this;
}

/**
 * @brief Returns the current indentation string.
 */
std::string CppBuilder::current_indent() const {
    std::string result;

    for (int i = 0; i < indent_level_; ++i) {
        result += indent_char_;
    }

    return result;
}

// ============================================================================
// Raw output methods
// ============================================================================

/**
 * @brief Appends raw text without any modification.
 */
CppBuilder& CppBuilder::raw(const std::string& text) {
    buffer_ << text;
    return *this;
}

/**
 * @brief Appends a line with current indentation and newline.
 */
CppBuilder& CppBuilder::line(const std::string& text) {
    buffer_ << current_indent() << text << "\n";
    return *this;
}

/**
 * @brief Appends an empty line.
 */
CppBuilder& CppBuilder::blank() {
    buffer_ << "\n";
    return *this;
}

/**
 * @brief Appends a comment with current indentation.
 */
CppBuilder& CppBuilder::comment(const std::string& text) {
    buffer_ << current_indent() << "// " << text << "\n";
    return *this;
}

// ============================================================================
// Statement methods
// ============================================================================

/**
 * @brief Appends a statement with semicolon and newline.
 */
CppBuilder& CppBuilder::statement(const std::string& stmt) {
    buffer_ << current_indent() << stmt << ";\n";
    return *this;
}

/**
 * @brief Appends a return statement.
 */
CppBuilder& CppBuilder::return_stmt(const std::string& expr) {
    if (expr.empty()) {
        buffer_ << current_indent() << "return;\n";
    } else {
        buffer_ << current_indent() << "return " << expr << ";\n";
    }

    return *this;
}

/**
 * @brief Appends a break statement.
 */
CppBuilder& CppBuilder::break_stmt() {
    buffer_ << current_indent() << "break;\n";
    return *this;
}

/**
 * @brief Appends a continue statement.
 */
CppBuilder& CppBuilder::continue_stmt() {
    buffer_ << current_indent() << "continue;\n";
    return *this;
}

// ============================================================================
// Block methods
// ============================================================================

/**
 * @brief Opens a generic block with opening brace.
 */
CppBuilder& CppBuilder::block() {
    buffer_ << current_indent() << "{\n";
    ++indent_level_;
    ++nesting_depth_;
    return *this;
}

/**
 * @brief Closes the current block with closing brace.
 */
CppBuilder& CppBuilder::end() {
    if (nesting_depth_ <= 0) {
        throw std::runtime_error("CppBuilder::end() called with no open block");
    }

    --indent_level_;
    --nesting_depth_;
    buffer_ << current_indent() << "}\n";
    return *this;
}

// ============================================================================
// Control flow methods
// ============================================================================

/**
 * @brief Opens a for-range loop.
 */
CppBuilder& CppBuilder::for_range(const std::string& var,
                                   const std::string& start,
                                   const std::string& end) {
    buffer_ << current_indent() << "for (int " << var << " = " << start
            << "; " << var << " < " << end << "; " << var << "++) {\n";
    ++indent_level_;
    ++nesting_depth_;
    return *this;
}

/**
 * @brief Opens a for-each loop.
 */
CppBuilder& CppBuilder::for_each(const std::string& var,
                                  const std::string& collection) {
    buffer_ << current_indent() << "for (auto& " << var << " : "
            << collection << ") {\n";
    ++indent_level_;
    ++nesting_depth_;
    return *this;
}

/**
 * @brief Opens a while loop.
 */
CppBuilder& CppBuilder::while_loop(const std::string& condition) {
    buffer_ << current_indent() << "while (" << condition << ") {\n";
    ++indent_level_;
    ++nesting_depth_;
    return *this;
}

/**
 * @brief Opens an if statement.
 */
CppBuilder& CppBuilder::if_stmt(const std::string& condition) {
    buffer_ << current_indent() << "if (" << condition << ") {\n";
    ++indent_level_;
    ++nesting_depth_;
    return *this;
}

/**
 * @brief Continues with else if clause.
 */
CppBuilder& CppBuilder::else_if(const std::string& condition) {
    if (nesting_depth_ <= 0) {
        throw std::runtime_error("CppBuilder::else_if() called with no open if block");
    }

    --indent_level_;
    buffer_ << current_indent() << "} else if (" << condition << ") {\n";
    ++indent_level_;
    return *this;
}

/**
 * @brief Continues with else clause.
 */
CppBuilder& CppBuilder::else_block() {
    if (nesting_depth_ <= 0) {
        throw std::runtime_error("CppBuilder::else_block() called with no open if block");
    }

    --indent_level_;
    buffer_ << current_indent() << "} else {\n";
    ++indent_level_;
    return *this;
}

// ============================================================================
// Function/Method methods
// ============================================================================

/**
 * @brief Opens a function definition.
 */
CppBuilder& CppBuilder::function(const std::string& return_type,
                                  const std::string& name,
                                  const std::string& params) {
    std::string rt = return_type.empty() ? "void" : return_type;
    buffer_ << current_indent() << rt << " " << name << "(" << params << ") {\n";
    ++indent_level_;
    ++nesting_depth_;
    return *this;
}

/**
 * @brief Opens a static member function.
 */
CppBuilder& CppBuilder::static_function(const std::string& return_type,
                                         const std::string& name,
                                         const std::string& params) {
    std::string rt = return_type.empty() ? "void" : return_type;
    buffer_ << current_indent() << "static " << rt << " " << name << "(" << params << ") {\n";
    ++indent_level_;
    ++nesting_depth_;
    return *this;
}

// ============================================================================
// Struct methods
// ============================================================================

/**
 * @brief Opens a struct definition.
 */
CppBuilder& CppBuilder::struct_def(const std::string& name) {
    buffer_ << current_indent() << "struct " << name << " {\n";
    ++indent_level_;
    ++nesting_depth_;
    return *this;
}

/**
 * @brief Adds a struct field.
 */
CppBuilder& CppBuilder::field(const std::string& type, const std::string& name) {
    buffer_ << current_indent() << type << " " << name << ";\n";
    return *this;
}

/**
 * @brief Ends a struct definition with semicolon.
 */
CppBuilder& CppBuilder::end_struct() {
    if (nesting_depth_ <= 0) {
        throw std::runtime_error("CppBuilder::end_struct() called with no open struct");
    }

    --indent_level_;
    --nesting_depth_;
    buffer_ << current_indent() << "};\n";
    return *this;
}

// ============================================================================
// Indentation control
// ============================================================================

/**
 * @brief Increases indentation level.
 */
CppBuilder& CppBuilder::indent() {
    ++indent_level_;
    return *this;
}

/**
 * @brief Decreases indentation level.
 */
CppBuilder& CppBuilder::dedent() {
    if (indent_level_ > 0) {
        --indent_level_;
    }

    return *this;
}

// ============================================================================
// Convenience functions for one-shot generation
// ============================================================================

/**
 * @brief Generates a for-range loop as a complete string.
 */
std::string for_range_stmt(const std::string& var,
                           const std::string& start,
                           const std::string& end,
                           const std::vector<std::string>& body) {
    CppBuilder b;

    b.for_range(var, start, end);

    for (const auto& stmt : body) {
        b.raw(b.str().empty() ? "" : "");  // no-op, just need to add statements
        b.line(stmt);
    }

    // Fix: we need to manually add statements
    CppBuilder builder;
    builder.raw("for (int " + var + " = " + start + "; " + var + " < " + end + "; " + var + "++) {\n");

    for (const auto& stmt : body) {
        builder.raw("\t" + stmt + "\n");
    }

    builder.raw("}");
    return builder.str();
}

/**
 * @brief Generates a for-each loop as a complete string.
 */
std::string for_each_stmt(const std::string& var,
                          const std::string& collection,
                          const std::vector<std::string>& body) {
    CppBuilder builder;
    builder.raw("for (auto& " + var + " : " + collection + ") {\n");

    for (const auto& stmt : body) {
        builder.raw("\t" + stmt + "\n");
    }

    builder.raw("}");
    return builder.str();
}

/**
 * @brief Generates a function definition as a complete string.
 */
std::string function_def(const std::string& return_type,
                         const std::string& name,
                         const std::string& params,
                         const std::vector<std::string>& body) {
    std::string rt = return_type.empty() ? "void" : return_type;
    CppBuilder builder;
    builder.raw(rt + " " + name + "(" + params + ") {\n");

    for (const auto& stmt : body) {
        builder.raw("\t" + stmt + "\n");
    }

    builder.raw("}\n");
    return builder.str();
}

/**
 * @brief Generates a struct definition as a complete string.
 */
std::string struct_def(const std::string& name,
                       const std::vector<std::pair<std::string, std::string>>& fields) {
    CppBuilder builder;
    builder.raw("struct " + name + " {\n");

    for (const auto& [field_type, field_name] : fields) {
        builder.raw("\t" + field_type + " " + field_name + ";\n");
    }

    builder.raw("};");
    return builder.str();
}

/**
 * @brief Generates an if statement as a complete string.
 */
std::string if_stmt(const std::string& condition,
                    const std::vector<std::string>& then_body,
                    const std::vector<std::string>& else_body) {
    CppBuilder builder;
    builder.raw("if (" + condition + ") {\n");

    for (const auto& stmt : then_body) {
        builder.raw("\t" + stmt + "\n");
    }

    if (else_body.empty()) {
        builder.raw("}");
    } else {
        builder.raw("} else {\n");

        for (const auto& stmt : else_body) {
            builder.raw("\t" + stmt + "\n");
        }

        builder.raw("}");
    }

    return builder.str();
}

} // namespace builder
} // namespace codegen
