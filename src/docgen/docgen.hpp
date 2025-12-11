/**
 * @file docgen.hpp
 * @brief Documentation generator for Nog source files.
 *
 * Generates HTML documentation from parsed Nog programs with doc comments.
 */

#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include "parser/ast.hpp"

namespace fs = std::filesystem;

/**
 * @brief Generates HTML documentation from Nog source files.
 */
class DocGen {
public:
    /**
     * @brief Generates docs for a single file.
     * @param program The parsed program AST
     * @param filename Source filename for display
     * @return HTML documentation string
     */
    std::string generate_file(const Program& program, const std::string& filename);

    /**
     * @brief Generates docs for all .n files in a directory.
     * @param input_dir Directory containing .n files
     * @param output_dir Directory to write HTML files
     */
    void generate_directory(const std::string& input_dir, const std::string& output_dir);

private:
    /**
     * @brief Renders a function definition to HTML.
     */
    std::string render_function(const FunctionDef& fn);

    /**
     * @brief Renders a struct definition with its methods to HTML.
     */
    std::string render_struct(const StructDef& s, const std::vector<const MethodDef*>& methods);

    /**
     * @brief Renders a method definition to HTML.
     */
    std::string render_method(const MethodDef& m);

    /**
     * @brief Converts basic markdown to HTML.
     */
    std::string markdown_to_html(const std::string& md);

    /**
     * @brief Escapes HTML special characters.
     */
    std::string escape_html(const std::string& text);

    /**
     * @brief Generates the CSS stylesheet.
     */
    std::string generate_css();

    /**
     * @brief Wraps content in HTML document structure.
     */
    std::string wrap_html(const std::string& title, const std::string& content);
};
