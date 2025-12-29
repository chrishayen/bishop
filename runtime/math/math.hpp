/**
 * @file math.hpp
 * @brief Bishop math runtime library.
 *
 * Provides mathematical functions and constants for Bishop programs.
 * This header is included when programs import the math module.
 */

#pragma once

#include <bishop/std.hpp>
#include <cmath>
#include <limits>
#include <numeric>

namespace math {

// ============================================================
// Constants
// ============================================================

/**
 * Mathematical constant pi.
 */
constexpr double PI = 3.14159265358979323846;

/**
 * Euler's number e.
 */
constexpr double E = 2.71828182845904523536;

/**
 * Positive infinity.
 */
constexpr double INF = std::numeric_limits<double>::infinity();

/**
 * Not a Number (NaN).
 * Note: Named NAN_ to avoid conflict with C macro NAN.
 * Accessed via math.NAN in Bishop.
 */
inline double get_nan() {
    return std::nan("");
}

// Macro to allow math.NAN to work (used via #define in generated code)
#ifdef NAN
#undef NAN
#endif
inline const double NAN = get_nan();

// ============================================================
// Basic operations
// ============================================================

/**
 * Returns the absolute value of a floating-point number.
 */
inline double abs(double x) {
    return std::fabs(x);
}

/**
 * Returns the absolute value of an integer.
 */
inline int64_t abs_int(int64_t x) {
    return std::abs(x);
}

/**
 * Returns the smaller of two floating-point numbers.
 */
inline double min(double a, double b) {
    return std::fmin(a, b);
}

/**
 * Returns the smaller of two integers.
 */
inline int64_t min_int(int64_t a, int64_t b) {
    return (a < b) ? a : b;
}

/**
 * Returns the larger of two floating-point numbers.
 */
inline double max(double a, double b) {
    return std::fmax(a, b);
}

/**
 * Returns the larger of two integers.
 */
inline int64_t max_int(int64_t a, int64_t b) {
    return (a > b) ? a : b;
}

/**
 * Clamps a value between a minimum and maximum.
 */
inline double clamp(double x, double min_val, double max_val) {
    if (x < min_val) {
        return min_val;
    }

    if (x > max_val) {
        return max_val;
    }

    return x;
}

/**
 * Clamps an integer between a minimum and maximum.
 */
inline int64_t clamp_int(int64_t x, int64_t min_val, int64_t max_val) {
    if (x < min_val) {
        return min_val;
    }

    if (x > max_val) {
        return max_val;
    }

    return x;
}

// ============================================================
// Rounding functions
// ============================================================

/**
 * Rounds down to the nearest integer.
 */
inline double floor(double x) {
    return std::floor(x);
}

/**
 * Rounds up to the nearest integer.
 */
inline double ceil(double x) {
    return std::ceil(x);
}

/**
 * Rounds to the nearest integer.
 */
inline double round(double x) {
    return std::round(x);
}

/**
 * Truncates to integer part (rounds toward zero).
 */
inline double trunc(double x) {
    return std::trunc(x);
}

// ============================================================
// Power and root functions
// ============================================================

/**
 * Raises base to the power of exp.
 */
inline double pow(double base, double exp) {
    return std::pow(base, exp);
}

/**
 * Computes the square root.
 */
inline double sqrt(double x) {
    return std::sqrt(x);
}

/**
 * Computes the cube root.
 */
inline double cbrt(double x) {
    return std::cbrt(x);
}

/**
 * Computes e raised to the power of x.
 */
inline double exp(double x) {
    return std::exp(x);
}

/**
 * Computes the natural logarithm (base e).
 */
inline double log(double x) {
    return std::log(x);
}

/**
 * Computes the base-10 logarithm.
 */
inline double log10(double x) {
    return std::log10(x);
}

/**
 * Computes the base-2 logarithm.
 */
inline double log2(double x) {
    return std::log2(x);
}

// ============================================================
// Trigonometric functions
// ============================================================

/**
 * Computes the sine of an angle in radians.
 */
inline double sin(double x) {
    return std::sin(x);
}

/**
 * Computes the cosine of an angle in radians.
 */
inline double cos(double x) {
    return std::cos(x);
}

/**
 * Computes the tangent of an angle in radians.
 */
inline double tan(double x) {
    return std::tan(x);
}

/**
 * Computes the arc sine (inverse sine).
 */
inline double asin(double x) {
    return std::asin(x);
}

/**
 * Computes the arc cosine (inverse cosine).
 */
inline double acos(double x) {
    return std::acos(x);
}

/**
 * Computes the arc tangent (inverse tangent).
 */
inline double atan(double x) {
    return std::atan(x);
}

/**
 * Computes the arc tangent of y/x using signs to determine quadrant.
 */
inline double atan2(double y, double x) {
    return std::atan2(y, x);
}

// ============================================================
// Hyperbolic functions
// ============================================================

/**
 * Computes the hyperbolic sine.
 */
inline double sinh(double x) {
    return std::sinh(x);
}

/**
 * Computes the hyperbolic cosine.
 */
inline double cosh(double x) {
    return std::cosh(x);
}

/**
 * Computes the hyperbolic tangent.
 */
inline double tanh(double x) {
    return std::tanh(x);
}

// ============================================================
// Utility functions
// ============================================================

/**
 * Checks if a value is NaN (Not a Number).
 */
inline bool is_nan(double x) {
    return std::isnan(x);
}

/**
 * Checks if a value is infinite (positive or negative).
 */
inline bool is_inf(double x) {
    return std::isinf(x);
}

/**
 * Checks if a value is finite (not NaN or infinite).
 */
inline bool is_finite(double x) {
    return std::isfinite(x);
}

/**
 * Returns the sign of a number (-1, 0, or 1).
 */
inline int64_t sign(double x) {
    if (x < 0) {
        return -1;
    }

    if (x > 0) {
        return 1;
    }

    return 0;
}

/**
 * Computes the greatest common divisor of two integers.
 */
inline int64_t gcd(int64_t a, int64_t b) {
    a = std::abs(a);
    b = std::abs(b);
    return std::gcd(a, b);
}

/**
 * Computes the least common multiple of two integers.
 */
inline int64_t lcm(int64_t a, int64_t b) {
    a = std::abs(a);
    b = std::abs(b);
    return std::lcm(a, b);
}

}  // namespace math
