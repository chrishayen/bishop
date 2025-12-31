/**
 * @file sync.hpp
 * @brief Built-in sync (synchronization) module header.
 *
 * Declares the AST creation functions for the sync module.
 * The actual runtime is in src/runtime/sync/sync.hpp.
 */

#pragma once

#include "parser/ast.hpp"
#include <memory>
#include <string>

namespace bishop::stdlib {

/**
 * Creates the AST for the built-in sync module.
 */
std::unique_ptr<Program> create_sync_module();

/**
 * Generates the sync runtime code (empty - uses precompiled header).
 */
std::string generate_sync_runtime();

}  // namespace bishop::stdlib
