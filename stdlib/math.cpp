/**
 * @file math.cpp
 * @brief Built-in math module implementation.
 *
 * Creates the AST definitions for the math module.
 * The actual runtime is in runtime/math/math.hpp and included as a header.
 */

/**
 * @nog_const PI
 * @module math
 * @description Pi - ratio of a circle's circumference to its diameter.
 * @type f64
 * @example
 * import math;
 * circumference := 2.0 * math.PI * radius;
 */

/**
 * @nog_const E
 * @module math
 * @description Euler's number - base of natural logarithms.
 * @type f64
 * @example
 * import math;
 * result := math.pow(math.E, x);
 */

/**
 * @nog_const INF
 * @module math
 * @description Positive infinity.
 * @type f64
 * @example
 * import math;
 * if math.is_inf(value) { }
 */

/**
 * @nog_const NAN
 * @module math
 * @description Not a number (NaN).
 * @type f64
 * @example
 * import math;
 * if math.is_nan(value) { }
 */

/**
 * @nog_fn abs
 * @module math
 * @description Returns the absolute value of x.
 * @param x f64 - Value to get absolute value of
 * @returns f64 - Absolute value
 * @example
 * import math;
 * result := math.abs(-5.5);  // 5.5
 */

/**
 * @nog_fn abs_int
 * @module math
 * @description Returns the absolute value of an integer.
 * @param x int - Value to get absolute value of
 * @returns int - Absolute value
 * @example
 * import math;
 * result := math.abs_int(-5);  // 5
 */

/**
 * @nog_fn min
 * @module math
 * @description Returns the smaller of two values.
 * @param a f64 - First value
 * @param b f64 - Second value
 * @returns f64 - Smaller value
 * @example
 * import math;
 * smaller := math.min(3.5, 2.1);  // 2.1
 */

/**
 * @nog_fn min_int
 * @module math
 * @description Returns the smaller of two integers.
 * @param a int - First value
 * @param b int - Second value
 * @returns int - Smaller value
 * @example
 * import math;
 * smaller := math.min_int(3, 2);  // 2
 */

/**
 * @nog_fn max
 * @module math
 * @description Returns the larger of two values.
 * @param a f64 - First value
 * @param b f64 - Second value
 * @returns f64 - Larger value
 * @example
 * import math;
 * larger := math.max(3.5, 2.1);  // 3.5
 */

/**
 * @nog_fn max_int
 * @module math
 * @description Returns the larger of two integers.
 * @param a int - First value
 * @param b int - Second value
 * @returns int - Larger value
 * @example
 * import math;
 * larger := math.max_int(3, 2);  // 3
 */

/**
 * @nog_fn clamp
 * @module math
 * @description Clamps a value to a range [min_val, max_val].
 * @param x f64 - Value to clamp
 * @param min_val f64 - Minimum value
 * @param max_val f64 - Maximum value
 * @returns f64 - Clamped value
 * @example
 * import math;
 * clamped := math.clamp(15.0, 0.0, 10.0);  // 10.0
 */

/**
 * @nog_fn clamp_int
 * @module math
 * @description Clamps an integer to a range [min_val, max_val].
 * @param x int - Value to clamp
 * @param min_val int - Minimum value
 * @param max_val int - Maximum value
 * @returns int - Clamped value
 * @example
 * import math;
 * clamped := math.clamp_int(15, 0, 10);  // 10
 */

/**
 * @nog_fn floor
 * @module math
 * @description Returns the largest integer less than or equal to x.
 * @param x f64 - Value to floor
 * @returns f64 - Floor value
 * @example
 * import math;
 * result := math.floor(3.7);  // 3.0
 */

/**
 * @nog_fn ceil
 * @module math
 * @description Returns the smallest integer greater than or equal to x.
 * @param x f64 - Value to ceil
 * @returns f64 - Ceiling value
 * @example
 * import math;
 * result := math.ceil(3.2);  // 4.0
 */

/**
 * @nog_fn round
 * @module math
 * @description Rounds x to the nearest integer.
 * @param x f64 - Value to round
 * @returns f64 - Rounded value
 * @example
 * import math;
 * result := math.round(3.5);  // 4.0
 */

/**
 * @nog_fn trunc
 * @module math
 * @description Truncates x towards zero.
 * @param x f64 - Value to truncate
 * @returns f64 - Truncated value
 * @example
 * import math;
 * result := math.trunc(-3.7);  // -3.0
 */

/**
 * @nog_fn pow
 * @module math
 * @description Returns base raised to the power of exp.
 * @param base f64 - Base value
 * @param exp f64 - Exponent
 * @returns f64 - Result of base^exp
 * @example
 * import math;
 * result := math.pow(2.0, 10.0);  // 1024.0
 */

/**
 * @nog_fn sqrt
 * @module math
 * @description Returns the square root of x.
 * @param x f64 - Value to get square root of
 * @returns f64 - Square root
 * @example
 * import math;
 * result := math.sqrt(16.0);  // 4.0
 */

/**
 * @nog_fn cbrt
 * @module math
 * @description Returns the cube root of x.
 * @param x f64 - Value to get cube root of
 * @returns f64 - Cube root
 * @example
 * import math;
 * result := math.cbrt(27.0);  // 3.0
 */

/**
 * @nog_fn exp
 * @module math
 * @description Returns e raised to the power of x.
 * @param x f64 - Exponent
 * @returns f64 - e^x
 * @example
 * import math;
 * result := math.exp(1.0);  // ~2.718
 */

/**
 * @nog_fn log
 * @module math
 * @description Returns the natural logarithm of x.
 * @param x f64 - Value
 * @returns f64 - Natural log of x
 * @example
 * import math;
 * result := math.log(math.E);  // 1.0
 */

/**
 * @nog_fn log10
 * @module math
 * @description Returns the base-10 logarithm of x.
 * @param x f64 - Value
 * @returns f64 - Base-10 log of x
 * @example
 * import math;
 * result := math.log10(100.0);  // 2.0
 */

/**
 * @nog_fn log2
 * @module math
 * @description Returns the base-2 logarithm of x.
 * @param x f64 - Value
 * @returns f64 - Base-2 log of x
 * @example
 * import math;
 * result := math.log2(8.0);  // 3.0
 */

/**
 * @nog_fn sin
 * @module math
 * @description Returns the sine of x (x in radians).
 * @param x f64 - Angle in radians
 * @returns f64 - Sine of x
 * @example
 * import math;
 * result := math.sin(math.PI / 2.0);  // 1.0
 */

/**
 * @nog_fn cos
 * @module math
 * @description Returns the cosine of x (x in radians).
 * @param x f64 - Angle in radians
 * @returns f64 - Cosine of x
 * @example
 * import math;
 * result := math.cos(0.0);  // 1.0
 */

/**
 * @nog_fn tan
 * @module math
 * @description Returns the tangent of x (x in radians).
 * @param x f64 - Angle in radians
 * @returns f64 - Tangent of x
 * @example
 * import math;
 * result := math.tan(math.PI / 4.0);  // ~1.0
 */

/**
 * @nog_fn asin
 * @module math
 * @description Returns the arc sine of x.
 * @param x f64 - Value in range [-1, 1]
 * @returns f64 - Arc sine in radians
 * @example
 * import math;
 * result := math.asin(1.0);  // PI/2
 */

/**
 * @nog_fn acos
 * @module math
 * @description Returns the arc cosine of x.
 * @param x f64 - Value in range [-1, 1]
 * @returns f64 - Arc cosine in radians
 * @example
 * import math;
 * result := math.acos(1.0);  // 0.0
 */

/**
 * @nog_fn atan
 * @module math
 * @description Returns the arc tangent of x.
 * @param x f64 - Value
 * @returns f64 - Arc tangent in radians
 * @example
 * import math;
 * result := math.atan(1.0);  // PI/4
 */

/**
 * @nog_fn atan2
 * @module math
 * @description Returns the arc tangent of y/x, using signs to determine quadrant.
 * @param y f64 - Y coordinate
 * @param x f64 - X coordinate
 * @returns f64 - Arc tangent in radians
 * @example
 * import math;
 * angle := math.atan2(1.0, 1.0);  // PI/4
 */

/**
 * @nog_fn sinh
 * @module math
 * @description Returns the hyperbolic sine of x.
 * @param x f64 - Value
 * @returns f64 - Hyperbolic sine
 * @example
 * import math;
 * result := math.sinh(0.0);  // 0.0
 */

/**
 * @nog_fn cosh
 * @module math
 * @description Returns the hyperbolic cosine of x.
 * @param x f64 - Value
 * @returns f64 - Hyperbolic cosine
 * @example
 * import math;
 * result := math.cosh(0.0);  // 1.0
 */

/**
 * @nog_fn tanh
 * @module math
 * @description Returns the hyperbolic tangent of x.
 * @param x f64 - Value
 * @returns f64 - Hyperbolic tangent
 * @example
 * import math;
 * result := math.tanh(0.0);  // 0.0
 */

/**
 * @nog_fn is_nan
 * @module math
 * @description Returns true if x is NaN.
 * @param x f64 - Value to check
 * @returns bool - True if NaN
 * @example
 * import math;
 * if math.is_nan(value) { }
 */

/**
 * @nog_fn is_inf
 * @module math
 * @description Returns true if x is positive or negative infinity.
 * @param x f64 - Value to check
 * @returns bool - True if infinity
 * @example
 * import math;
 * if math.is_inf(value) { }
 */

/**
 * @nog_fn is_finite
 * @module math
 * @description Returns true if x is finite (not NaN or infinity).
 * @param x f64 - Value to check
 * @returns bool - True if finite
 * @example
 * import math;
 * if math.is_finite(value) { }
 */

/**
 * @nog_fn sign
 * @module math
 * @description Returns the sign of x: -1, 0, or 1.
 * @param x f64 - Value
 * @returns int - Sign (-1, 0, or 1)
 * @example
 * import math;
 * s := math.sign(-5.5);  // -1
 */

/**
 * @nog_fn sign_int
 * @module math
 * @description Returns the sign of an integer: -1, 0, or 1.
 * @param x int - Value
 * @returns int - Sign (-1, 0, or 1)
 * @example
 * import math;
 * s := math.sign_int(-5);  // -1
 */

/**
 * @nog_fn gcd
 * @module math
 * @description Returns the greatest common divisor of a and b.
 * @param a int - First value
 * @param b int - Second value
 * @returns int - GCD
 * @example
 * import math;
 * result := math.gcd(12, 8);  // 4
 */

/**
 * @nog_fn lcm
 * @module math
 * @description Returns the least common multiple of a and b.
 * @param a int - First value
 * @param b int - Second value
 * @returns int - LCM
 * @example
 * import math;
 * result := math.lcm(4, 6);  // 12
 */

#include "math.hpp"

using namespace std;

namespace nog::stdlib {

/**
 * Creates the AST for the built-in math module.
 */
unique_ptr<Program> create_math_module() {
    auto program = make_unique<Program>();

    // ========================================================================
    // Constants (represented as zero-param functions for now)
    // ========================================================================

    // fn PI() -> f64
    auto pi_fn = make_unique<FunctionDef>();
    pi_fn->name = "PI";
    pi_fn->visibility = Visibility::Public;
    pi_fn->return_type = "f64";
    program->functions.push_back(move(pi_fn));

    // fn E() -> f64
    auto e_fn = make_unique<FunctionDef>();
    e_fn->name = "E";
    e_fn->visibility = Visibility::Public;
    e_fn->return_type = "f64";
    program->functions.push_back(move(e_fn));

    // fn INF() -> f64
    auto inf_fn = make_unique<FunctionDef>();
    inf_fn->name = "INF";
    inf_fn->visibility = Visibility::Public;
    inf_fn->return_type = "f64";
    program->functions.push_back(move(inf_fn));

    // fn NAN() -> f64
    auto nan_fn = make_unique<FunctionDef>();
    nan_fn->name = "NAN";
    nan_fn->visibility = Visibility::Public;
    nan_fn->return_type = "f64";
    program->functions.push_back(move(nan_fn));

    // ========================================================================
    // Basic Operations
    // ========================================================================

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

    // ========================================================================
    // Rounding Functions
    // ========================================================================

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

    // ========================================================================
    // Power and Root Functions
    // ========================================================================

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

    // ========================================================================
    // Trigonometric Functions
    // ========================================================================

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

    // ========================================================================
    // Hyperbolic Functions
    // ========================================================================

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

    // ========================================================================
    // Utility Functions
    // ========================================================================

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

    // fn sign_int(int x) -> int
    auto sign_int_fn = make_unique<FunctionDef>();
    sign_int_fn->name = "sign_int";
    sign_int_fn->visibility = Visibility::Public;
    sign_int_fn->params.push_back({"int", "x"});
    sign_int_fn->return_type = "int";
    program->functions.push_back(move(sign_int_fn));

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

}  // namespace nog::stdlib
