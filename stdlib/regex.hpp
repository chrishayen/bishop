/**
 * @file regex.hpp
 * @brief Built-in regex module header.
 *
 * Declares the AST creation functions for the regex module.
 * The actual runtime is in runtime/regex/regex.hpp.
 */

#pragma once

#include "parser/ast.hpp"
#include <memory>
#include <string>

namespace bishop::stdlib {

/**
 * Creates the AST for the built-in regex module.
 */
std::unique_ptr<Program> create_regex_module();

/**
 * Generates the regex runtime code (empty - uses precompiled header).
 */
std::string generate_regex_runtime();

}  // namespace bishop::stdlib
