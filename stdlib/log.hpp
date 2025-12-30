/**
 * @file log.hpp
 * @brief Built-in log (logging) module header.
 *
 * Declares the AST creation functions for the log module.
 * The actual runtime is in src/runtime/log/log.hpp.
 */

#pragma once

#include "parser/ast.hpp"
#include <memory>
#include <string>

namespace nog::stdlib {

/**
 * Creates the AST for the built-in log module.
 */
std::unique_ptr<Program> create_log_module();

/**
 * Generates the log runtime code (empty - uses precompiled header).
 */
std::string generate_log_runtime();

}  // namespace nog::stdlib
