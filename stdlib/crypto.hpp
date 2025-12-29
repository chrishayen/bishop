/**
 * @file crypto.hpp
 * @brief Built-in crypto module for Bishop.
 *
 * Provides cryptographic utilities including hashing, HMAC,
 * encoding/decoding, UUID generation, and random bytes.
 */

#pragma once
#include "parser/ast.hpp"
#include <memory>
#include <string>

namespace nog::stdlib {

/**
 * Creates the AST for the built-in crypto module.
 * Contains:
 * - md5(str) -> str
 * - sha1(str) -> str
 * - sha256(str) -> str
 * - sha512(str) -> str
 * - hmac_sha256(str key, str data) -> str
 * - base64_encode(str) -> str
 * - base64_decode(str) -> str or err
 * - hex_encode(str) -> str
 * - hex_decode(str) -> str or err
 * - uuid() -> str
 * - uuid_v5(str namespace, str name) -> str
 * - random_bytes(int count) -> List<int>
 */
std::unique_ptr<Program> create_crypto_module();

/**
 * Generates the crypto runtime code (empty - uses precompiled header).
 */
std::string generate_crypto_runtime();

}  // namespace nog::stdlib
