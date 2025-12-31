/**
 * @file net.hpp
 * @brief Built-in net (networking) module header.
 *
 * Declares the AST creation functions for the net module.
 * The actual runtime is in runtime/net/net.hpp.
 */

#pragma once

#include "parser/ast.hpp"
#include <memory>
#include <string>

namespace bishop::stdlib {

/**
 * Creates the AST for the built-in net module.
 */
std::unique_ptr<Program> create_net_module();

/**
 * Generates the net runtime code (empty - uses precompiled header).
 */
std::string generate_net_runtime();

}  // namespace bishop::stdlib
