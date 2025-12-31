/**
 * @file math.cpp
 * @brief Built-in math module implementation.
 *
 * Creates the AST definitions for the math module.
 * The actual runtime is in runtime/math/math.hpp.
 */

/**
 * @bishop_const PI
 * @module math
 * @description Mathematical constant pi (3.14159265358979323846).
 * @example
 * import math;
 * circumference := 2 * math.PI * radius;
 */

/**
 * @bishop_const E
 * @module math
 * @description Euler's number e (2.71828182845904523536).
 * @example
 * import math;
 * growth := math.E ** x;
 */

/**
 * @bishop_const INF
 * @module math
 * @description Positive infinity.
 * @example
 * if value == math.INF { print("Infinite!"); }
 */

/**
 * @bishop_const NAN
 * @module math
 * @description Not a Number (NaN).
 * @example
 * if math.is_nan(result) { print("Invalid result"); }
 */

/**
 * @bishop_fn abs
 * @module math
 * @description Returns the absolute value of a number.
 * @param x f64 - Input value
 * @returns f64 - Absolute value
 * @example
 * x := math.abs(-5.5);  // 5.5
 */

/**
 * @bishop_fn abs_int
 * @module math
 * @description Returns the absolute value of an integer.
 * @param x int - Input value
 * @returns int - Absolute value
 * @example
 * x := math.abs_int(-5);  // 5
 */

/**
 * @bishop_fn min
 * @module math
 * @description Returns the smaller of two numbers.
 * @param a f64 - First value
 * @param b f64 - Second value
 * @returns f64 - Minimum value
 * @example
 * smaller := math.min(3.0, 7.0);  // 3.0
 */

/**
 * @bishop_fn min_int
 * @module math
 * @description Returns the smaller of two integers.
 * @param a int - First value
 * @param b int - Second value
 * @returns int - Minimum value
 * @example
 * smaller := math.min_int(3, 7);  // 3
 */

/**
 * @bishop_fn max
 * @module math
 * @description Returns the larger of two numbers.
 * @param a f64 - First value
 * @param b f64 - Second value
 * @returns f64 - Maximum value
 * @example
 * larger := math.max(3.0, 7.0);  // 7.0
 */

/**
 * @bishop_fn max_int
 * @module math
 * @description Returns the larger of two integers.
 * @param a int - First value
 * @param b int - Second value
 * @returns int - Maximum value
 * @example
 * larger := math.max_int(3, 7);  // 7
 */

/**
 * @bishop_fn clamp
 * @module math
 * @description Clamps a value between a minimum and maximum.
 * @param x f64 - Value to clamp
 * @param min_val f64 - Minimum bound
 * @param max_val f64 - Maximum bound
 * @returns f64 - Clamped value
 * @example
 * clamped := math.clamp(15.0, 0.0, 10.0);  // 10.0
 */

/**
 * @bishop_fn clamp_int
 * @module math
 * @description Clamps an integer between a minimum and maximum.
 * @param x int - Value to clamp
 * @param min_val int - Minimum bound
 * @param max_val int - Maximum bound
 * @returns int - Clamped value
 * @example
 * clamped := math.clamp_int(15, 0, 10);  // 10
 */

/**
 * @bishop_fn floor
 * @module math
 * @description Rounds down to the nearest integer.
 * @param x f64 - Value to round
 * @returns f64 - Rounded value
 * @example
 * f := math.floor(3.7);  // 3.0
 */

/**
 * @bishop_fn ceil
 * @module math
 * @description Rounds up to the nearest integer.
 * @param x f64 - Value to round
 * @returns f64 - Rounded value
 * @example
 * c := math.ceil(3.2);  // 4.0
 */

/**
 * @bishop_fn round
 * @module math
 * @description Rounds to the nearest integer.
 * @param x f64 - Value to round
 * @returns f64 - Rounded value
 * @example
 * r := math.round(3.5);  // 4.0
 */

/**
 * @bishop_fn trunc
 * @module math
 * @description Truncates to integer part (rounds toward zero).
 * @param x f64 - Value to truncate
 * @returns f64 - Truncated value
 * @example
 * t := math.trunc(-3.7);  // -3.0
 */

/**
 * @bishop_fn pow
 * @module math
 * @description Raises base to the power of exponent.
 * @param base f64 - Base value
 * @param exp f64 - Exponent
 * @returns f64 - Result
 * @example
 * p := math.pow(2.0, 10.0);  // 1024.0
 */

/**
 * @bishop_fn sqrt
 * @module math
 * @description Computes the square root.
 * @param x f64 - Input value (must be >= 0)
 * @returns f64 - Square root
 * @example
 * s := math.sqrt(16.0);  // 4.0
 */

/**
 * @bishop_fn cbrt
 * @module math
 * @description Computes the cube root.
 * @param x f64 - Input value
 * @returns f64 - Cube root
 * @example
 * c := math.cbrt(27.0);  // 3.0
 */

/**
 * @bishop_fn exp
 * @module math
 * @description Computes e raised to the power of x.
 * @param x f64 - Exponent
 * @returns f64 - e^x
 * @example
 * e := math.exp(1.0);  // ~2.718
 */

/**
 * @bishop_fn log
 * @module math
 * @description Computes the natural logarithm (base e).
 * @param x f64 - Input value (must be > 0)
 * @returns f64 - Natural logarithm
 * @example
 * l := math.log(math.E);  // 1.0
 */

/**
 * @bishop_fn log10
 * @module math
 * @description Computes the base-10 logarithm.
 * @param x f64 - Input value (must be > 0)
 * @returns f64 - Base-10 logarithm
 * @example
 * l := math.log10(100.0);  // 2.0
 */

/**
 * @bishop_fn log2
 * @module math
 * @description Computes the base-2 logarithm.
 * @param x f64 - Input value (must be > 0)
 * @returns f64 - Base-2 logarithm
 * @example
 * l := math.log2(8.0);  // 3.0
 */

/**
 * @bishop_fn sin
 * @module math
 * @description Computes the sine of an angle in radians.
 * @param x f64 - Angle in radians
 * @returns f64 - Sine value
 * @example
 * s := math.sin(math.PI / 2);  // 1.0
 */

/**
 * @bishop_fn cos
 * @module math
 * @description Computes the cosine of an angle in radians.
 * @param x f64 - Angle in radians
 * @returns f64 - Cosine value
 * @example
 * c := math.cos(0.0);  // 1.0
 */

/**
 * @bishop_fn tan
 * @module math
 * @description Computes the tangent of an angle in radians.
 * @param x f64 - Angle in radians
 * @returns f64 - Tangent value
 * @example
 * t := math.tan(math.PI / 4);  // ~1.0
 */

/**
 * @bishop_fn asin
 * @module math
 * @description Computes the arc sine (inverse sine).
 * @param x f64 - Value between -1 and 1
 * @returns f64 - Angle in radians
 * @example
 * a := math.asin(1.0);  // PI/2
 */

/**
 * @bishop_fn acos
 * @module math
 * @description Computes the arc cosine (inverse cosine).
 * @param x f64 - Value between -1 and 1
 * @returns f64 - Angle in radians
 * @example
 * a := math.acos(1.0);  // 0.0
 */

/**
 * @bishop_fn atan
 * @module math
 * @description Computes the arc tangent (inverse tangent).
 * @param x f64 - Input value
 * @returns f64 - Angle in radians
 * @example
 * a := math.atan(1.0);  // PI/4
 */

/**
 * @bishop_fn atan2
 * @module math
 * @description Computes the arc tangent of y/x using signs to determine quadrant.
 * @param y f64 - Y coordinate
 * @param x f64 - X coordinate
 * @returns f64 - Angle in radians
 * @example
 * angle := math.atan2(1.0, 1.0);  // PI/4
 */

/**
 * @bishop_fn sinh
 * @module math
 * @description Computes the hyperbolic sine.
 * @param x f64 - Input value
 * @returns f64 - Hyperbolic sine
 * @example
 * s := math.sinh(0.0);  // 0.0
 */

/**
 * @bishop_fn cosh
 * @module math
 * @description Computes the hyperbolic cosine.
 * @param x f64 - Input value
 * @returns f64 - Hyperbolic cosine
 * @example
 * c := math.cosh(0.0);  // 1.0
 */

/**
 * @bishop_fn tanh
 * @module math
 * @description Computes the hyperbolic tangent.
 * @param x f64 - Input value
 * @returns f64 - Hyperbolic tangent
 * @example
 * t := math.tanh(0.0);  // 0.0
 */

/**
 * @bishop_fn is_nan
 * @module math
 * @description Checks if a value is NaN (Not a Number).
 * @param x f64 - Value to check
 * @returns bool - True if NaN
 * @example
 * if math.is_nan(0.0 / 0.0) { print("NaN detected"); }
 */

/**
 * @bishop_fn is_inf
 * @module math
 * @description Checks if a value is infinite (positive or negative).
 * @param x f64 - Value to check
 * @returns bool - True if infinite
 * @example
 * if math.is_inf(1.0 / 0.0) { print("Infinite!"); }
 */

/**
 * @bishop_fn is_finite
 * @module math
 * @description Checks if a value is finite (not NaN or infinite).
 * @param x f64 - Value to check
 * @returns bool - True if finite
 * @example
 * if math.is_finite(x) { print("x is a valid number"); }
 */

/**
 * @bishop_fn sign
 * @module math
 * @description Returns the sign of a number (-1, 0, or 1).
 * @param x f64 - Input value
 * @returns int - -1 if negative, 0 if zero, 1 if positive
 * @example
 * s := math.sign(-5.0);  // -1
 */

/**
 * @bishop_fn gcd
 * @module math
 * @description Computes the greatest common divisor of two integers.
 * @param a int - First integer
 * @param b int - Second integer
 * @returns int - Greatest common divisor
 * @example
 * g := math.gcd(12, 8);  // 4
 */

/**
 * @bishop_fn lcm
 * @module math
 * @description Computes the least common multiple of two integers.
 * @param a int - First integer
 * @param b int - Second integer
 * @returns int - Least common multiple
 * @example
 * l := math.lcm(4, 6);  // 12
 */

#include "math.hpp"

using namespace std;

namespace bishop::stdlib {

/**
 * Creates the AST for the built-in math module.
 */
unique_ptr<Program> create_math_module() {
    auto program = make_unique<Program>();

    // ============================================================
    // Constants
    // ============================================================

    // const f64 PI = 3.14159265358979323846
    auto pi_const = make_unique<VariableDecl>();
    pi_const->name = "PI";
    pi_const->type = "f64";
    pi_const->is_const = true;
    program->constants.push_back(move(pi_const));

    // const f64 E = 2.71828182845904523536
    auto e_const = make_unique<VariableDecl>();
    e_const->name = "E";
    e_const->type = "f64";
    e_const->is_const = true;
    program->constants.push_back(move(e_const));

    // const f64 INF = infinity
    auto inf_const = make_unique<VariableDecl>();
    inf_const->name = "INF";
    inf_const->type = "f64";
    inf_const->is_const = true;
    program->constants.push_back(move(inf_const));

    // const f64 NAN = NaN
    auto nan_const = make_unique<VariableDecl>();
    nan_const->name = "NAN";
    nan_const->type = "f64";
    nan_const->is_const = true;
    program->constants.push_back(move(nan_const));

    // ============================================================
    // Basic operations
    // ============================================================

    // fn abs(f64 x) -> f64
    auto abs_fn = make_unique<FunctionDef>();
    abs_fn->name = "abs";
    abs_fn->visibility = Visibility::Public;
    abs_fn->params.push_back({"f64", "x"});
    abs_fn->return_type = "f64";
    program->functions.push_back(move(abs_fn));

    // fn abs_int(int x) -> int
    auto abs_int_fn = make_unique<FunctionDef>();
    abs_int_fn->name = "abs_int";
    abs_int_fn->visibility = Visibility::Public;
    abs_int_fn->params.push_back({"int", "x"});
    abs_int_fn->return_type = "int";
    program->functions.push_back(move(abs_int_fn));

    // fn min(f64 a, f64 b) -> f64
    auto min_fn = make_unique<FunctionDef>();
    min_fn->name = "min";
    min_fn->visibility = Visibility::Public;
    min_fn->params.push_back({"f64", "a"});
    min_fn->params.push_back({"f64", "b"});
    min_fn->return_type = "f64";
    program->functions.push_back(move(min_fn));

    // fn min_int(int a, int b) -> int
    auto min_int_fn = make_unique<FunctionDef>();
    min_int_fn->name = "min_int";
    min_int_fn->visibility = Visibility::Public;
    min_int_fn->params.push_back({"int", "a"});
    min_int_fn->params.push_back({"int", "b"});
    min_int_fn->return_type = "int";
    program->functions.push_back(move(min_int_fn));

    // fn max(f64 a, f64 b) -> f64
    auto max_fn = make_unique<FunctionDef>();
    max_fn->name = "max";
    max_fn->visibility = Visibility::Public;
    max_fn->params.push_back({"f64", "a"});
    max_fn->params.push_back({"f64", "b"});
    max_fn->return_type = "f64";
    program->functions.push_back(move(max_fn));

    // fn max_int(int a, int b) -> int
    auto max_int_fn = make_unique<FunctionDef>();
    max_int_fn->name = "max_int";
    max_int_fn->visibility = Visibility::Public;
    max_int_fn->params.push_back({"int", "a"});
    max_int_fn->params.push_back({"int", "b"});
    max_int_fn->return_type = "int";
    program->functions.push_back(move(max_int_fn));

    // fn clamp(f64 x, f64 min_val, f64 max_val) -> f64
    auto clamp_fn = make_unique<FunctionDef>();
    clamp_fn->name = "clamp";
    clamp_fn->visibility = Visibility::Public;
    clamp_fn->params.push_back({"f64", "x"});
    clamp_fn->params.push_back({"f64", "min_val"});
    clamp_fn->params.push_back({"f64", "max_val"});
    clamp_fn->return_type = "f64";
    program->functions.push_back(move(clamp_fn));

    // fn clamp_int(int x, int min_val, int max_val) -> int
    auto clamp_int_fn = make_unique<FunctionDef>();
    clamp_int_fn->name = "clamp_int";
    clamp_int_fn->visibility = Visibility::Public;
    clamp_int_fn->params.push_back({"int", "x"});
    clamp_int_fn->params.push_back({"int", "min_val"});
    clamp_int_fn->params.push_back({"int", "max_val"});
    clamp_int_fn->return_type = "int";
    program->functions.push_back(move(clamp_int_fn));

    // ============================================================
    // Rounding functions
    // ============================================================

    // fn floor(f64 x) -> f64
    auto floor_fn = make_unique<FunctionDef>();
    floor_fn->name = "floor";
    floor_fn->visibility = Visibility::Public;
    floor_fn->params.push_back({"f64", "x"});
    floor_fn->return_type = "f64";
    program->functions.push_back(move(floor_fn));

    // fn ceil(f64 x) -> f64
    auto ceil_fn = make_unique<FunctionDef>();
    ceil_fn->name = "ceil";
    ceil_fn->visibility = Visibility::Public;
    ceil_fn->params.push_back({"f64", "x"});
    ceil_fn->return_type = "f64";
    program->functions.push_back(move(ceil_fn));

    // fn round(f64 x) -> f64
    auto round_fn = make_unique<FunctionDef>();
    round_fn->name = "round";
    round_fn->visibility = Visibility::Public;
    round_fn->params.push_back({"f64", "x"});
    round_fn->return_type = "f64";
    program->functions.push_back(move(round_fn));

    // fn trunc(f64 x) -> f64
    auto trunc_fn = make_unique<FunctionDef>();
    trunc_fn->name = "trunc";
    trunc_fn->visibility = Visibility::Public;
    trunc_fn->params.push_back({"f64", "x"});
    trunc_fn->return_type = "f64";
    program->functions.push_back(move(trunc_fn));

    // ============================================================
    // Power and root functions
    // ============================================================

    // fn pow(f64 base, f64 exp) -> f64
    auto pow_fn = make_unique<FunctionDef>();
    pow_fn->name = "pow";
    pow_fn->visibility = Visibility::Public;
    pow_fn->params.push_back({"f64", "base"});
    pow_fn->params.push_back({"f64", "exp"});
    pow_fn->return_type = "f64";
    program->functions.push_back(move(pow_fn));

    // fn sqrt(f64 x) -> f64
    auto sqrt_fn = make_unique<FunctionDef>();
    sqrt_fn->name = "sqrt";
    sqrt_fn->visibility = Visibility::Public;
    sqrt_fn->params.push_back({"f64", "x"});
    sqrt_fn->return_type = "f64";
    program->functions.push_back(move(sqrt_fn));

    // fn cbrt(f64 x) -> f64
    auto cbrt_fn = make_unique<FunctionDef>();
    cbrt_fn->name = "cbrt";
    cbrt_fn->visibility = Visibility::Public;
    cbrt_fn->params.push_back({"f64", "x"});
    cbrt_fn->return_type = "f64";
    program->functions.push_back(move(cbrt_fn));

    // fn exp(f64 x) -> f64
    auto exp_fn = make_unique<FunctionDef>();
    exp_fn->name = "exp";
    exp_fn->visibility = Visibility::Public;
    exp_fn->params.push_back({"f64", "x"});
    exp_fn->return_type = "f64";
    program->functions.push_back(move(exp_fn));

    // fn log(f64 x) -> f64
    auto log_fn = make_unique<FunctionDef>();
    log_fn->name = "log";
    log_fn->visibility = Visibility::Public;
    log_fn->params.push_back({"f64", "x"});
    log_fn->return_type = "f64";
    program->functions.push_back(move(log_fn));

    // fn log10(f64 x) -> f64
    auto log10_fn = make_unique<FunctionDef>();
    log10_fn->name = "log10";
    log10_fn->visibility = Visibility::Public;
    log10_fn->params.push_back({"f64", "x"});
    log10_fn->return_type = "f64";
    program->functions.push_back(move(log10_fn));

    // fn log2(f64 x) -> f64
    auto log2_fn = make_unique<FunctionDef>();
    log2_fn->name = "log2";
    log2_fn->visibility = Visibility::Public;
    log2_fn->params.push_back({"f64", "x"});
    log2_fn->return_type = "f64";
    program->functions.push_back(move(log2_fn));

    // ============================================================
    // Trigonometric functions
    // ============================================================

    // fn sin(f64 x) -> f64
    auto sin_fn = make_unique<FunctionDef>();
    sin_fn->name = "sin";
    sin_fn->visibility = Visibility::Public;
    sin_fn->params.push_back({"f64", "x"});
    sin_fn->return_type = "f64";
    program->functions.push_back(move(sin_fn));

    // fn cos(f64 x) -> f64
    auto cos_fn = make_unique<FunctionDef>();
    cos_fn->name = "cos";
    cos_fn->visibility = Visibility::Public;
    cos_fn->params.push_back({"f64", "x"});
    cos_fn->return_type = "f64";
    program->functions.push_back(move(cos_fn));

    // fn tan(f64 x) -> f64
    auto tan_fn = make_unique<FunctionDef>();
    tan_fn->name = "tan";
    tan_fn->visibility = Visibility::Public;
    tan_fn->params.push_back({"f64", "x"});
    tan_fn->return_type = "f64";
    program->functions.push_back(move(tan_fn));

    // fn asin(f64 x) -> f64
    auto asin_fn = make_unique<FunctionDef>();
    asin_fn->name = "asin";
    asin_fn->visibility = Visibility::Public;
    asin_fn->params.push_back({"f64", "x"});
    asin_fn->return_type = "f64";
    program->functions.push_back(move(asin_fn));

    // fn acos(f64 x) -> f64
    auto acos_fn = make_unique<FunctionDef>();
    acos_fn->name = "acos";
    acos_fn->visibility = Visibility::Public;
    acos_fn->params.push_back({"f64", "x"});
    acos_fn->return_type = "f64";
    program->functions.push_back(move(acos_fn));

    // fn atan(f64 x) -> f64
    auto atan_fn = make_unique<FunctionDef>();
    atan_fn->name = "atan";
    atan_fn->visibility = Visibility::Public;
    atan_fn->params.push_back({"f64", "x"});
    atan_fn->return_type = "f64";
    program->functions.push_back(move(atan_fn));

    // fn atan2(f64 y, f64 x) -> f64
    auto atan2_fn = make_unique<FunctionDef>();
    atan2_fn->name = "atan2";
    atan2_fn->visibility = Visibility::Public;
    atan2_fn->params.push_back({"f64", "y"});
    atan2_fn->params.push_back({"f64", "x"});
    atan2_fn->return_type = "f64";
    program->functions.push_back(move(atan2_fn));

    // ============================================================
    // Hyperbolic functions
    // ============================================================

    // fn sinh(f64 x) -> f64
    auto sinh_fn = make_unique<FunctionDef>();
    sinh_fn->name = "sinh";
    sinh_fn->visibility = Visibility::Public;
    sinh_fn->params.push_back({"f64", "x"});
    sinh_fn->return_type = "f64";
    program->functions.push_back(move(sinh_fn));

    // fn cosh(f64 x) -> f64
    auto cosh_fn = make_unique<FunctionDef>();
    cosh_fn->name = "cosh";
    cosh_fn->visibility = Visibility::Public;
    cosh_fn->params.push_back({"f64", "x"});
    cosh_fn->return_type = "f64";
    program->functions.push_back(move(cosh_fn));

    // fn tanh(f64 x) -> f64
    auto tanh_fn = make_unique<FunctionDef>();
    tanh_fn->name = "tanh";
    tanh_fn->visibility = Visibility::Public;
    tanh_fn->params.push_back({"f64", "x"});
    tanh_fn->return_type = "f64";
    program->functions.push_back(move(tanh_fn));

    // ============================================================
    // Utility functions
    // ============================================================

    // fn is_nan(f64 x) -> bool
    auto is_nan_fn = make_unique<FunctionDef>();
    is_nan_fn->name = "is_nan";
    is_nan_fn->visibility = Visibility::Public;
    is_nan_fn->params.push_back({"f64", "x"});
    is_nan_fn->return_type = "bool";
    program->functions.push_back(move(is_nan_fn));

    // fn is_inf(f64 x) -> bool
    auto is_inf_fn = make_unique<FunctionDef>();
    is_inf_fn->name = "is_inf";
    is_inf_fn->visibility = Visibility::Public;
    is_inf_fn->params.push_back({"f64", "x"});
    is_inf_fn->return_type = "bool";
    program->functions.push_back(move(is_inf_fn));

    // fn is_finite(f64 x) -> bool
    auto is_finite_fn = make_unique<FunctionDef>();
    is_finite_fn->name = "is_finite";
    is_finite_fn->visibility = Visibility::Public;
    is_finite_fn->params.push_back({"f64", "x"});
    is_finite_fn->return_type = "bool";
    program->functions.push_back(move(is_finite_fn));

    // fn sign(f64 x) -> int
    auto sign_fn = make_unique<FunctionDef>();
    sign_fn->name = "sign";
    sign_fn->visibility = Visibility::Public;
    sign_fn->params.push_back({"f64", "x"});
    sign_fn->return_type = "int";
    program->functions.push_back(move(sign_fn));

    // fn gcd(int a, int b) -> int
    auto gcd_fn = make_unique<FunctionDef>();
    gcd_fn->name = "gcd";
    gcd_fn->visibility = Visibility::Public;
    gcd_fn->params.push_back({"int", "a"});
    gcd_fn->params.push_back({"int", "b"});
    gcd_fn->return_type = "int";
    program->functions.push_back(move(gcd_fn));

    // fn lcm(int a, int b) -> int
    auto lcm_fn = make_unique<FunctionDef>();
    lcm_fn->name = "lcm";
    lcm_fn->visibility = Visibility::Public;
    lcm_fn->params.push_back({"int", "a"});
    lcm_fn->params.push_back({"int", "b"});
    lcm_fn->return_type = "int";
    program->functions.push_back(move(lcm_fn));

    return program;
}

/**
 * Returns empty - math.hpp is included at the top of generated code
 * for precompiled header support.
 */
string generate_math_runtime() {
    return "";
}

}  // namespace bishop::stdlib
