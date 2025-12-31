/**
 * @file process.hpp
 * @brief Built-in process module header.
 *
 * Declares the AST creation functions for the process module.
 * The actual runtime is in runtime/process/process.hpp.
 */

#pragma once

#include "parser/ast.hpp"
#include <memory>
#include <string>

namespace bishop::stdlib {

/**
 * Creates the AST for the built-in process module.
 * Contains:
 * - ProcessResult struct (stdout, stderr, exit_code, success fields)
 * - ProcessOptions struct (cwd, env, timeout, stdin, capture settings)
 * - run(str, List<str>) -> ProcessResult or err
 * - shell(str) -> ProcessResult or err
 * - env(str) -> str
 * - set_env(str, str)
 * - cwd() -> str
 * - chdir(str) -> bool or err
 * - args() -> List<str>
 */
std::unique_ptr<Program> create_process_module();

/**
 * Generates the process runtime code (empty - uses precompiled header).
 */
std::string generate_process_runtime();

}  // namespace bishop::stdlib
