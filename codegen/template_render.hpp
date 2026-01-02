/**
 * @file template_render.hpp
 * @brief Template-based code emission for the Bishop code generator.
 *
 * This file provides template loading and rendering functionality for
 * generating C++ code from .cpp.tmpl template files. Templates use
 * {{PLACEHOLDER}} syntax for variable substitution.
 */

#ifndef CODEGEN_TEMPLATE_RENDER_HPP
#define CODEGEN_TEMPLATE_RENDER_HPP

#include <string>
#include <unordered_map>
#include <vector>

namespace codegen {
namespace templates {

/**
 * @brief Renders a template string with the given substitutions.
 *
 * Replaces all occurrences of {{KEY}} with the corresponding value
 * from the substitutions map.
 *
 * @param template_str The template string with {{PLACEHOLDER}} markers
 * @param subs Map of placeholder names to replacement values
 * @return The rendered string with all placeholders substituted
 *
 * @example
 *   render("Hello {{NAME}}!", {{"NAME", "World"}})
 *   // Returns: "Hello World!"
 */
std::string render(const std::string& template_str,
                   const std::unordered_map<std::string, std::string>& subs);

/**
 * @brief Loads a template from the templates directory.
 *
 * Templates are stored in codegen/templates/ with .cpp.tmpl extension.
 *
 * @param name Template name (without path or extension)
 * @return The template content, or empty string if not found
 *
 * @example
 *   load_template("for_range")
 *   // Loads: codegen/templates/for_range.cpp.tmpl
 */
std::string load_template(const std::string& name);

/**
 * @brief Indents each line of a string by a given number of tabs.
 *
 * Used to properly indent body content within templates.
 *
 * @param text The text to indent
 * @param tabs Number of tab characters to prepend to each line
 * @return The indented text
 */
std::string indent(const std::string& text, int tabs = 1);

/**
 * @brief Joins a vector of strings into a body with proper indentation.
 *
 * @param statements Vector of statement strings
 * @param tabs Indentation level
 * @return Joined and indented string
 */
std::string join_body(const std::vector<std::string>& statements, int tabs = 1);

// ============================================================================
// Convenience functions for common patterns
// ============================================================================

/**
 * @brief Renders a for-range loop using template.
 *
 * @param var Loop variable name
 * @param start Range start expression
 * @param end Range end expression
 * @param body Vector of body statements
 * @return Rendered for-range loop
 */
std::string for_range(const std::string& var,
                      const std::string& start,
                      const std::string& end,
                      const std::vector<std::string>& body);

/**
 * @brief Renders a for-each loop using template.
 *
 * @param var Loop variable name
 * @param collection Collection expression
 * @param body Vector of body statements
 * @return Rendered for-each loop
 */
std::string for_each(const std::string& var,
                     const std::string& collection,
                     const std::vector<std::string>& body);

/**
 * @brief Renders a function definition using template.
 *
 * @param name Function name
 * @param params Parameter list string
 * @param return_type Return type string
 * @param body Vector of body statements
 * @return Rendered function definition
 */
std::string function_def(const std::string& name,
                         const std::string& params,
                         const std::string& return_type,
                         const std::vector<std::string>& body);

/**
 * @brief Renders a struct definition using template.
 *
 * @param name Struct name
 * @param fields Vector of field declarations (already formatted)
 * @return Rendered struct definition
 */
std::string struct_def(const std::string& name,
                       const std::vector<std::string>& fields);

/**
 * @brief Renders an if statement using template.
 *
 * @param condition Condition expression
 * @param then_body Vector of then-body statements
 * @param else_body Vector of else-body statements (empty for no else)
 * @return Rendered if statement
 */
std::string if_stmt(const std::string& condition,
                    const std::vector<std::string>& then_body,
                    const std::vector<std::string>& else_body = {});

} // namespace templates
} // namespace codegen

#endif // CODEGEN_TEMPLATE_RENDER_HPP
