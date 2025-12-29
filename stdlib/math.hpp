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
 * Contains:
 * - Constants: PI, E, INF, NAN
 * - Basic ops: abs, min, max, clamp
 * - Rounding: floor, ceil, round, trunc
 * - Power/roots: pow, sqrt, cbrt, exp, log, log10, log2
 * - Trigonometry: sin, cos, tan, asin, acos, atan, atan2
 * - Hyperbolic: sinh, cosh, tanh
 * - Utilities: is_nan, is_inf, is_finite, sign, gcd, lcm
 */
std::unique_ptr<Program> create_math_module();

/**
 * Generates the math runtime code (empty - uses precompiled header).
 */
std::string generate_math_runtime();

}  // namespace nog::stdlib
