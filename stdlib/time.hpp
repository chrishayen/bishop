/**
 * @file time.hpp
 * @brief Built-in time module header.
 *
 * Declares the AST creation functions for the time module.
 * The actual runtime is in runtime/time/time.hpp.
 */

#pragma once

#include "parser/ast.hpp"
#include <memory>
#include <string>

namespace nog::stdlib {

/**
 * Creates the AST for the built-in time module.
 * Contains:
 * - Duration struct (millis_value field, as_* methods)
 * - Timestamp struct (year, month, day, hour, minute, second, millisecond, weekday fields)
 * - millis(int) -> Duration
 * - seconds(int) -> Duration
 * - minutes(int) -> Duration
 * - hours(int) -> Duration
 * - days(int) -> Duration
 * - now() -> Timestamp
 * - now_utc() -> Timestamp
 * - since(Timestamp) -> Duration
 * - parse(str, str) -> Timestamp or err
 */
std::unique_ptr<Program> create_time_module();

/**
 * Generates the time runtime code (empty - uses precompiled header).
 */
std::string generate_time_runtime();

}  // namespace nog::stdlib
