/// Tests for the math module

import math;

// ============================================================
// Tests for math constants
// ============================================================

fn test_constant_PI() {
    pi := math.PI;
    assert_eq(true, pi > 3.14);
    assert_eq(true, pi < 3.15);
}

fn test_constant_E() {
    e := math.E;
    assert_eq(true, e > 2.71);
    assert_eq(true, e < 2.72);
}

fn test_constant_INF() {
    inf := math.INF;
    assert_eq(true, math.is_inf(inf));
    assert_eq(true, inf > 1000000.0);
}

// ============================================================
// Tests for basic operations - abs
// ============================================================

fn test_abs_positive() {
    result := math.abs(5.5);
    assert_eq(true, result > 5.4);
    assert_eq(true, result < 5.6);
}

fn test_abs_negative() {
    result := math.abs(-5.5);
    assert_eq(true, result > 5.4);
    assert_eq(true, result < 5.6);
}

fn test_abs_zero() {
    result := math.abs(0.0);
    assert_eq(true, result < 0.0001);
}

fn test_abs_int_positive() {
    result := math.abs_int(5);
    assert_eq(5, result);
}

fn test_abs_int_negative() {
    result := math.abs_int(-5);
    assert_eq(5, result);
}

fn test_abs_int_zero() {
    result := math.abs_int(0);
    assert_eq(0, result);
}

// ============================================================
// Tests for basic operations - min/max
// ============================================================

fn test_min_first_smaller() {
    result := math.min(3.0, 7.0);
    assert_eq(true, result > 2.9);
    assert_eq(true, result < 3.1);
}

fn test_min_second_smaller() {
    result := math.min(7.0, 3.0);
    assert_eq(true, result > 2.9);
    assert_eq(true, result < 3.1);
}

fn test_min_equal() {
    result := math.min(5.0, 5.0);
    assert_eq(true, result > 4.9);
    assert_eq(true, result < 5.1);
}

fn test_min_negative() {
    result := math.min(-3.0, -7.0);
    assert_eq(true, result > -7.1);
    assert_eq(true, result < -6.9);
}

fn test_min_int_first_smaller() {
    result := math.min_int(3, 7);
    assert_eq(3, result);
}

fn test_min_int_second_smaller() {
    result := math.min_int(7, 3);
    assert_eq(3, result);
}

fn test_max_first_larger() {
    result := math.max(7.0, 3.0);
    assert_eq(true, result > 6.9);
    assert_eq(true, result < 7.1);
}

fn test_max_second_larger() {
    result := math.max(3.0, 7.0);
    assert_eq(true, result > 6.9);
    assert_eq(true, result < 7.1);
}

fn test_max_equal() {
    result := math.max(5.0, 5.0);
    assert_eq(true, result > 4.9);
    assert_eq(true, result < 5.1);
}

fn test_max_int_first_larger() {
    result := math.max_int(7, 3);
    assert_eq(7, result);
}

fn test_max_int_second_larger() {
    result := math.max_int(3, 7);
    assert_eq(7, result);
}

// ============================================================
// Tests for clamp
// ============================================================

fn test_clamp_within_range() {
    result := math.clamp(5.0, 0.0, 10.0);
    assert_eq(true, result > 4.9);
    assert_eq(true, result < 5.1);
}

fn test_clamp_below_min() {
    result := math.clamp(-5.0, 0.0, 10.0);
    assert_eq(true, result > -0.1);
    assert_eq(true, result < 0.1);
}

fn test_clamp_above_max() {
    result := math.clamp(15.0, 0.0, 10.0);
    assert_eq(true, result > 9.9);
    assert_eq(true, result < 10.1);
}

fn test_clamp_int_within_range() {
    result := math.clamp_int(5, 0, 10);
    assert_eq(5, result);
}

fn test_clamp_int_below_min() {
    result := math.clamp_int(-5, 0, 10);
    assert_eq(0, result);
}

fn test_clamp_int_above_max() {
    result := math.clamp_int(15, 0, 10);
    assert_eq(10, result);
}

// ============================================================
// Tests for rounding functions
// ============================================================

fn test_floor_positive() {
    result := math.floor(3.7);
    assert_eq(true, result > 2.9);
    assert_eq(true, result < 3.1);
}

fn test_floor_negative() {
    result := math.floor(-3.2);
    assert_eq(true, result > -4.1);
    assert_eq(true, result < -3.9);
}

fn test_ceil_positive() {
    result := math.ceil(3.2);
    assert_eq(true, result > 3.9);
    assert_eq(true, result < 4.1);
}

fn test_ceil_negative() {
    result := math.ceil(-3.7);
    assert_eq(true, result > -3.1);
    assert_eq(true, result < -2.9);
}

fn test_round_up() {
    result := math.round(3.6);
    assert_eq(true, result > 3.9);
    assert_eq(true, result < 4.1);
}

fn test_round_down() {
    result := math.round(3.4);
    assert_eq(true, result > 2.9);
    assert_eq(true, result < 3.1);
}

fn test_round_half() {
    result := math.round(3.5);
    assert_eq(true, result > 3.9);
    assert_eq(true, result < 4.1);
}

fn test_trunc_positive() {
    result := math.trunc(3.7);
    assert_eq(true, result > 2.9);
    assert_eq(true, result < 3.1);
}

fn test_trunc_negative() {
    result := math.trunc(-3.7);
    assert_eq(true, result > -3.1);
    assert_eq(true, result < -2.9);
}

// ============================================================
// Tests for power and root functions
// ============================================================

fn test_pow_integer_exponent() {
    result := math.pow(2.0, 10.0);
    assert_eq(true, result > 1023.9);
    assert_eq(true, result < 1024.1);
}

fn test_pow_fractional_exponent() {
    result := math.pow(4.0, 0.5);
    assert_eq(true, result > 1.9);
    assert_eq(true, result < 2.1);
}

fn test_pow_negative_exponent() {
    result := math.pow(2.0, -1.0);
    assert_eq(true, result > 0.49);
    assert_eq(true, result < 0.51);
}

fn test_sqrt_perfect_square() {
    result := math.sqrt(16.0);
    assert_eq(true, result > 3.9);
    assert_eq(true, result < 4.1);
}

fn test_sqrt_non_perfect() {
    result := math.sqrt(2.0);
    assert_eq(true, result > 1.41);
    assert_eq(true, result < 1.42);
}

fn test_cbrt_positive() {
    result := math.cbrt(27.0);
    assert_eq(true, result > 2.9);
    assert_eq(true, result < 3.1);
}

fn test_cbrt_negative() {
    result := math.cbrt(-8.0);
    assert_eq(true, result > -2.1);
    assert_eq(true, result < -1.9);
}

fn test_exp() {
    result := math.exp(1.0);
    assert_eq(true, result > 2.71);
    assert_eq(true, result < 2.72);
}

fn test_exp_zero() {
    result := math.exp(0.0);
    assert_eq(true, result > 0.99);
    assert_eq(true, result < 1.01);
}

fn test_log_e() {
    result := math.log(math.E);
    assert_eq(true, result > 0.99);
    assert_eq(true, result < 1.01);
}

fn test_log_one() {
    result := math.log(1.0);
    assert_eq(true, result > -0.01);
    assert_eq(true, result < 0.01);
}

fn test_log10_hundred() {
    result := math.log10(100.0);
    assert_eq(true, result > 1.99);
    assert_eq(true, result < 2.01);
}

fn test_log10_thousand() {
    result := math.log10(1000.0);
    assert_eq(true, result > 2.99);
    assert_eq(true, result < 3.01);
}

fn test_log2_eight() {
    result := math.log2(8.0);
    assert_eq(true, result > 2.99);
    assert_eq(true, result < 3.01);
}

fn test_log2_sixteen() {
    result := math.log2(16.0);
    assert_eq(true, result > 3.99);
    assert_eq(true, result < 4.01);
}

// ============================================================
// Tests for trigonometric functions
// ============================================================

fn test_sin_zero() {
    result := math.sin(0.0);
    assert_eq(true, result > -0.01);
    assert_eq(true, result < 0.01);
}

fn test_sin_pi_half() {
    result := math.sin(math.PI / 2.0);
    assert_eq(true, result > 0.99);
    assert_eq(true, result < 1.01);
}

fn test_sin_pi() {
    result := math.sin(math.PI);
    assert_eq(true, result > -0.01);
    assert_eq(true, result < 0.01);
}

fn test_cos_zero() {
    result := math.cos(0.0);
    assert_eq(true, result > 0.99);
    assert_eq(true, result < 1.01);
}

fn test_cos_pi_half() {
    result := math.cos(math.PI / 2.0);
    assert_eq(true, result > -0.01);
    assert_eq(true, result < 0.01);
}

fn test_cos_pi() {
    result := math.cos(math.PI);
    assert_eq(true, result > -1.01);
    assert_eq(true, result < -0.99);
}

fn test_tan_zero() {
    result := math.tan(0.0);
    assert_eq(true, result > -0.01);
    assert_eq(true, result < 0.01);
}

fn test_tan_pi_quarter() {
    result := math.tan(math.PI / 4.0);
    assert_eq(true, result > 0.99);
    assert_eq(true, result < 1.01);
}

fn test_asin_zero() {
    result := math.asin(0.0);
    assert_eq(true, result > -0.01);
    assert_eq(true, result < 0.01);
}

fn test_asin_one() {
    result := math.asin(1.0);
    expected := math.PI / 2.0;
    assert_eq(true, result > expected - 0.01);
    assert_eq(true, result < expected + 0.01);
}

fn test_acos_zero() {
    result := math.acos(0.0);
    expected := math.PI / 2.0;
    assert_eq(true, result > expected - 0.01);
    assert_eq(true, result < expected + 0.01);
}

fn test_acos_one() {
    result := math.acos(1.0);
    assert_eq(true, result > -0.01);
    assert_eq(true, result < 0.01);
}

fn test_atan_zero() {
    result := math.atan(0.0);
    assert_eq(true, result > -0.01);
    assert_eq(true, result < 0.01);
}

fn test_atan_one() {
    result := math.atan(1.0);
    expected := math.PI / 4.0;
    assert_eq(true, result > expected - 0.01);
    assert_eq(true, result < expected + 0.01);
}

fn test_atan2_positive_quadrant() {
    result := math.atan2(1.0, 1.0);
    expected := math.PI / 4.0;
    assert_eq(true, result > expected - 0.01);
    assert_eq(true, result < expected + 0.01);
}

fn test_atan2_negative_x() {
    result := math.atan2(1.0, -1.0);
    expected := 3.0 * math.PI / 4.0;
    assert_eq(true, result > expected - 0.01);
    assert_eq(true, result < expected + 0.01);
}

// ============================================================
// Tests for hyperbolic functions
// ============================================================

fn test_sinh_zero() {
    result := math.sinh(0.0);
    assert_eq(true, result > -0.01);
    assert_eq(true, result < 0.01);
}

fn test_sinh_one() {
    result := math.sinh(1.0);
    assert_eq(true, result > 1.17);
    assert_eq(true, result < 1.18);
}

fn test_cosh_zero() {
    result := math.cosh(0.0);
    assert_eq(true, result > 0.99);
    assert_eq(true, result < 1.01);
}

fn test_cosh_one() {
    result := math.cosh(1.0);
    assert_eq(true, result > 1.54);
    assert_eq(true, result < 1.55);
}

fn test_tanh_zero() {
    result := math.tanh(0.0);
    assert_eq(true, result > -0.01);
    assert_eq(true, result < 0.01);
}

fn test_tanh_large() {
    result := math.tanh(10.0);
    assert_eq(true, result > 0.99);
    assert_eq(true, result < 1.01);
}

// ============================================================
// Tests for utility functions
// ============================================================

fn test_is_nan_true() {
    nan := 0.0 / 0.0;
    result := math.is_nan(nan);
    assert_eq(true, result);
}

fn test_is_nan_false() {
    result := math.is_nan(5.0);
    assert_eq(false, result);
}

fn test_is_nan_inf() {
    result := math.is_nan(math.INF);
    assert_eq(false, result);
}

fn test_is_inf_positive() {
    result := math.is_inf(math.INF);
    assert_eq(true, result);
}

fn test_is_inf_negative() {
    neg_inf := -1.0 * math.INF;
    result := math.is_inf(neg_inf);
    assert_eq(true, result);
}

fn test_is_inf_finite() {
    result := math.is_inf(5.0);
    assert_eq(false, result);
}

fn test_is_finite_regular() {
    result := math.is_finite(5.0);
    assert_eq(true, result);
}

fn test_is_finite_inf() {
    result := math.is_finite(math.INF);
    assert_eq(false, result);
}

fn test_is_finite_nan() {
    nan := 0.0 / 0.0;
    result := math.is_finite(nan);
    assert_eq(false, result);
}

fn test_sign_positive() {
    result := math.sign(5.0);
    assert_eq(1, result);
}

fn test_sign_negative() {
    result := math.sign(-5.0);
    assert_eq(-1, result);
}

fn test_sign_zero() {
    result := math.sign(0.0);
    assert_eq(0, result);
}

fn test_gcd_common_divisor() {
    result := math.gcd(12, 8);
    assert_eq(4, result);
}

fn test_gcd_coprime() {
    result := math.gcd(7, 11);
    assert_eq(1, result);
}

fn test_gcd_same() {
    result := math.gcd(5, 5);
    assert_eq(5, result);
}

fn test_gcd_one_zero() {
    result := math.gcd(5, 0);
    assert_eq(5, result);
}

fn test_gcd_negative() {
    result := math.gcd(-12, 8);
    assert_eq(4, result);
}

fn test_lcm_common_multiple() {
    result := math.lcm(4, 6);
    assert_eq(12, result);
}

fn test_lcm_coprime() {
    result := math.lcm(7, 11);
    assert_eq(77, result);
}

fn test_lcm_same() {
    result := math.lcm(5, 5);
    assert_eq(5, result);
}

fn test_lcm_one() {
    result := math.lcm(1, 5);
    assert_eq(5, result);
}

fn test_lcm_negative() {
    result := math.lcm(-4, 6);
    assert_eq(12, result);
}

// ============================================================
// Edge case tests
// ============================================================

fn test_sqrt_zero() {
    result := math.sqrt(0.0);
    assert_eq(true, result > -0.01);
    assert_eq(true, result < 0.01);
}

fn test_log_very_small() {
    result := math.log(0.001);
    assert_eq(true, result < -6.9);
}

fn test_exp_negative() {
    result := math.exp(-1.0);
    assert_eq(true, result > 0.36);
    assert_eq(true, result < 0.37);
}

fn test_pow_zero_exponent() {
    result := math.pow(5.0, 0.0);
    assert_eq(true, result > 0.99);
    assert_eq(true, result < 1.01);
}

fn test_pow_one_base() {
    result := math.pow(1.0, 100.0);
    assert_eq(true, result > 0.99);
    assert_eq(true, result < 1.01);
}

// ============================================================
// Integration test from issue #32 example
// ============================================================

fn test_issue_32_example() {
    // Constants
    pi := math.PI;
    e := math.E;
    assert_eq(true, pi > 3.14);
    assert_eq(true, e > 2.71);

    // Basic ops
    x := math.abs(-5.0);
    assert_eq(true, x > 4.9);
    assert_eq(true, x < 5.1);

    y := math.min(3.0, 7.0);
    assert_eq(true, y > 2.9);
    assert_eq(true, y < 3.1);

    z := math.max(3.0, 7.0);
    assert_eq(true, z > 6.9);
    assert_eq(true, z < 7.1);

    c := math.clamp(15.0, 0.0, 10.0);
    assert_eq(true, c > 9.9);
    assert_eq(true, c < 10.1);

    // Rounding
    f := math.floor(3.7);
    assert_eq(true, f > 2.9);
    assert_eq(true, f < 3.1);

    r := math.round(3.5);
    assert_eq(true, r > 3.9);
    assert_eq(true, r < 4.1);

    // Power/roots
    p := math.pow(2.0, 10.0);
    assert_eq(true, p > 1023.9);
    assert_eq(true, p < 1024.1);

    s := math.sqrt(16.0);
    assert_eq(true, s > 3.9);
    assert_eq(true, s < 4.1);

    // Trig
    sin_val := math.sin(math.PI / 2.0);
    assert_eq(true, sin_val > 0.99);
    assert_eq(true, sin_val < 1.01);

    // Utilities
    g := math.gcd(12, 8);
    assert_eq(4, g);

    l := math.lcm(4, 6);
    assert_eq(12, l);

    // NaN detection
    nan := 0.0 / 0.0;
    assert_eq(true, math.is_nan(nan));
}
