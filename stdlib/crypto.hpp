/**
 * @file crypto.hpp
 * @brief Built-in crypto module header.
 *
 * Declares the AST creation functions for the crypto module.
 * The actual runtime is in runtime/crypto/crypto.hpp.
 */

#pragma once

#include "parser/ast.hpp"
#include <memory>
#include <string>

namespace bishop::stdlib {

/**
 * Creates the AST for the built-in crypto module.
 */
std::unique_ptr<Program> create_crypto_module();

/**
 * Generates the crypto runtime code (empty - uses precompiled header).
 */
std::string generate_crypto_runtime();

}  // namespace bishop::stdlib
