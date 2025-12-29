/**
 * @file math.hpp
 * @brief Built-in math module header.
 *
 * Declares the AST creation functions for the math module.
 * The actual runtime is in runtime/math/math.hpp.
 */

#pragma once

#include "parser/ast.hpp"
#include <memory>
#include <string>

namespace nog::stdlib {

/**
 * Creates the AST for the built-in math module.
 */
std::unique_ptr<Program> create_math_module();

/**
 * Generates the math runtime code (empty - uses header include).
 */
std::string generate_math_runtime();

}  // namespace nog::stdlib
