/**
 * @file json.hpp
 * @brief Built-in json module header.
 *
 * Declares the AST creation functions for the json module.
 * The actual runtime is in src/runtime/json/json.hpp.
 */

#pragma once

#include "parser/ast.hpp"
#include <memory>
#include <string>

namespace nog::stdlib {

/**
 * Creates the AST for the built-in json module.
 */
std::unique_ptr<Program> create_json_module();

/**
 * Generates the json runtime code (empty - uses precompiled header).
 */
std::string generate_json_runtime();

}  // namespace nog::stdlib
