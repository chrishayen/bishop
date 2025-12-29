/**
 * @file math.hpp
 * @brief Bishop math runtime library.
 *
 * Provides mathematical constants and functions for Bishop programs.
 * This header is included when programs import the math module.
 */

#pragma once

#include <cmath>
#include <limits>
#include <algorithm>
#include <numeric>
#include <cstdint>

namespace math {

// ============================================================================
// Constants
// ============================================================================

/**
 * Pi - ratio of a circle's circumference to its diameter.
 */
inline constexpr double PI = 3.14159265358979323846;

/**
 * Euler's number - base of natural logarithms.
 */
inline constexpr double E = 2.71828182845904523536;

/**
 * Positive infinity.
 */
inline const double INF = std::numeric_limits<double>::infinity();

/**
 * Not a number.
 */
inline const double NAN_VALUE = std::numeric_limits<double>::quiet_NaN();

// ============================================================================
// Basic Operations
// ============================================================================

/**
 * Returns the absolute value of x.
 */
inline double abs(double x) {
    return std::abs(x);
}

/**
 * Returns the absolute value of an integer.
 */
inline int64_t abs_int(int64_t x) {
    return std::abs(x);
}

/**
 * Returns the smaller of two values.
 */
inline double min(double a, double b) {
    return std::min(a, b);
}

/**
 * Returns the smaller of two integers.
 */
inline int64_t min_int(int64_t a, int64_t b) {
    return std::min(a, b);
}

/**
 * Returns the larger of two values.
 */
inline double max(double a, double b) {
    return std::max(a, b);
}

/**
 * Returns the larger of two integers.
 */
inline int64_t max_int(int64_t a, int64_t b) {
    return std::max(a, b);
}

/**
 * Clamps a value to a range [min_val, max_val].
 */
inline double clamp(double x, double min_val, double max_val) {
    return std::clamp(x, min_val, max_val);
}

/**
 * Clamps an integer to a range [min_val, max_val].
 */
inline int64_t clamp_int(int64_t x, int64_t min_val, int64_t max_val) {
    return std::clamp(x, min_val, max_val);
}

// ============================================================================
// Rounding Functions
// ============================================================================

/**
 * Returns the largest integer less than or equal to x.
 */
inline double floor(double x) {
    return std::floor(x);
}

/**
 * Returns the smallest integer greater than or equal to x.
 */
inline double ceil(double x) {
    return std::ceil(x);
}

/**
 * Rounds x to the nearest integer.
 */
inline double round(double x) {
    return std::round(x);
}

/**
 * Truncates x towards zero.
 */
inline double trunc(double x) {
    return std::trunc(x);
}

// ============================================================================
// Power and Root Functions
// ============================================================================

/**
 * Returns base raised to the power of exp.
 */
inline double pow(double base, double exp) {
    return std::pow(base, exp);
}

/**
 * Returns the square root of x.
 */
inline double sqrt(double x) {
    return std::sqrt(x);
}

/**
 * Returns the cube root of x.
 */
inline double cbrt(double x) {
    return std::cbrt(x);
}

/**
 * Returns e raised to the power of x.
 */
inline double exp(double x) {
    return std::exp(x);
}

/**
 * Returns the natural logarithm of x.
 */
inline double log(double x) {
    return std::log(x);
}

/**
 * Returns the base-10 logarithm of x.
 */
inline double log10(double x) {
    return std::log10(x);
}

/**
 * Returns the base-2 logarithm of x.
 */
inline double log2(double x) {
    return std::log2(x);
}

// ============================================================================
// Trigonometric Functions
// ============================================================================

/**
 * Returns the sine of x (x in radians).
 */
inline double sin(double x) {
    return std::sin(x);
}

/**
 * Returns the cosine of x (x in radians).
 */
inline double cos(double x) {
    return std::cos(x);
}

/**
 * Returns the tangent of x (x in radians).
 */
inline double tan(double x) {
    return std::tan(x);
}

/**
 * Returns the arc sine of x.
 */
inline double asin(double x) {
    return std::asin(x);
}

/**
 * Returns the arc cosine of x.
 */
inline double acos(double x) {
    return std::acos(x);
}

/**
 * Returns the arc tangent of x.
 */
inline double atan(double x) {
    return std::atan(x);
}

/**
 * Returns the arc tangent of y/x, using signs to determine quadrant.
 */
inline double atan2(double y, double x) {
    return std::atan2(y, x);
}

// ============================================================================
// Hyperbolic Functions
// ============================================================================

/**
 * Returns the hyperbolic sine of x.
 */
inline double sinh(double x) {
    return std::sinh(x);
}

/**
 * Returns the hyperbolic cosine of x.
 */
inline double cosh(double x) {
    return std::cosh(x);
}

/**
 * Returns the hyperbolic tangent of x.
 */
inline double tanh(double x) {
    return std::tanh(x);
}

// ============================================================================
// Utility Functions
// ============================================================================

/**
 * Returns true if x is NaN.
 */
inline bool is_nan(double x) {
    return std::isnan(x);
}

/**
 * Returns true if x is positive or negative infinity.
 */
inline bool is_inf(double x) {
    return std::isinf(x);
}

/**
 * Returns true if x is finite (not NaN or infinity).
 */
inline bool is_finite(double x) {
    return std::isfinite(x);
}

/**
 * Returns the sign of x: -1, 0, or 1.
 */
inline int64_t sign(double x) {
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}

/**
 * Returns the sign of an integer: -1, 0, or 1.
 */
inline int64_t sign_int(int64_t x) {
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}

/**
 * Returns the greatest common divisor of a and b.
 */
inline int64_t gcd(int64_t a, int64_t b) {
    return std::gcd(a, b);
}

/**
 * Returns the least common multiple of a and b.
 */
inline int64_t lcm(int64_t a, int64_t b) {
    return std::lcm(a, b);
}

}  // namespace math
