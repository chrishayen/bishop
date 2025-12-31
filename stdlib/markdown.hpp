/**
 * @file markdown.hpp
 * @brief Built-in markdown module header.
 *
 * Declares the AST creation functions for the markdown module.
 * The actual runtime is in src/runtime/markdown/markdown.hpp.
 */

#pragma once

#include "parser/ast.hpp"
#include <memory>
#include <string>

namespace nog::stdlib {

/**
 * Creates the AST for the built-in markdown module.
 */
std::unique_ptr<Program> create_markdown_module();

/**
 * Generates the markdown runtime code (empty - uses precompiled header).
 */
std::string generate_markdown_runtime();

}  // namespace nog::stdlib
