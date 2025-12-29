// Tests for the math module

import math;

// ============================================================================
// Constants
// ============================================================================

fn test_pi() {
    pi := math.PI();
    assert_eq(true, pi > 3.14);
    assert_eq(true, pi < 3.15);
}

fn test_e() {
    e := math.E();
    assert_eq(true, e > 2.71);
    assert_eq(true, e < 2.72);
}

fn test_inf() {
    inf := math.INF();
    assert_eq(true, math.is_inf(inf));
    assert_eq(false, math.is_finite(inf));
}

fn test_nan() {
    nan := math.NAN();
    assert_eq(true, math.is_nan(nan));
    assert_eq(false, math.is_finite(nan));
}

// ============================================================================
// Basic Operations
// ============================================================================

fn test_abs() {
    assert_eq(true, math.abs(-5.5) > 5.4);
    assert_eq(true, math.abs(-5.5) < 5.6);
    assert_eq(true, math.abs(5.5) > 5.4);
}

fn test_abs_int() {
    assert_eq(5, math.abs_int(-5));
    assert_eq(5, math.abs_int(5));
    assert_eq(0, math.abs_int(0));
}

fn test_min() {
    assert_eq(true, math.min(3.5, 2.1) > 2.0);
    assert_eq(true, math.min(3.5, 2.1) < 2.2);
    assert_eq(true, math.min(-5.0, 5.0) < -4.9);
}

fn test_min_int() {
    assert_eq(2, math.min_int(3, 2));
    assert_eq(-5, math.min_int(-5, 5));
    assert_eq(0, math.min_int(0, 0));
}

fn test_max() {
    assert_eq(true, math.max(3.5, 2.1) > 3.4);
    assert_eq(true, math.max(3.5, 2.1) < 3.6);
    assert_eq(true, math.max(-5.0, 5.0) > 4.9);
}

fn test_max_int() {
    assert_eq(3, math.max_int(3, 2));
    assert_eq(5, math.max_int(-5, 5));
    assert_eq(0, math.max_int(0, 0));
}

fn test_clamp() {
    assert_eq(true, math.clamp(15.0, 0.0, 10.0) > 9.9);
    assert_eq(true, math.clamp(15.0, 0.0, 10.0) < 10.1);
    assert_eq(true, math.clamp(-5.0, 0.0, 10.0) > -0.1);
    assert_eq(true, math.clamp(-5.0, 0.0, 10.0) < 0.1);
    assert_eq(true, math.clamp(5.0, 0.0, 10.0) > 4.9);
    assert_eq(true, math.clamp(5.0, 0.0, 10.0) < 5.1);
}

fn test_clamp_int() {
    assert_eq(10, math.clamp_int(15, 0, 10));
    assert_eq(0, math.clamp_int(-5, 0, 10));
    assert_eq(5, math.clamp_int(5, 0, 10));
}

// ============================================================================
// Rounding Functions
// ============================================================================

fn test_floor() {
    assert_eq(true, math.floor(3.7) > 2.9);
    assert_eq(true, math.floor(3.7) < 3.1);
    assert_eq(true, math.floor(-3.7) < -3.9);
    assert_eq(true, math.floor(-3.7) > -4.1);
}

fn test_ceil() {
    assert_eq(true, math.ceil(3.2) > 3.9);
    assert_eq(true, math.ceil(3.2) < 4.1);
    assert_eq(true, math.ceil(-3.2) > -3.1);
    assert_eq(true, math.ceil(-3.2) < -2.9);
}

fn test_round() {
    assert_eq(true, math.round(3.5) > 3.9);
    assert_eq(true, math.round(3.5) < 4.1);
    assert_eq(true, math.round(3.4) > 2.9);
    assert_eq(true, math.round(3.4) < 3.1);
}

fn test_trunc() {
    assert_eq(true, math.trunc(3.7) > 2.9);
    assert_eq(true, math.trunc(3.7) < 3.1);
    assert_eq(true, math.trunc(-3.7) > -3.1);
    assert_eq(true, math.trunc(-3.7) < -2.9);
}

// ============================================================================
// Power and Root Functions
// ============================================================================

fn test_pow() {
    result := math.pow(2.0, 10.0);
    assert_eq(true, result > 1023.0);
    assert_eq(true, result < 1025.0);
}

fn test_sqrt() {
    result := math.sqrt(16.0);
    assert_eq(true, result > 3.9);
    assert_eq(true, result < 4.1);
}

fn test_cbrt() {
    result := math.cbrt(27.0);
    assert_eq(true, result > 2.9);
    assert_eq(true, result < 3.1);
}

fn test_exp() {
    result := math.exp(1.0);
    e := math.E();
    assert_eq(true, result > e - 0.01);
    assert_eq(true, result < e + 0.01);
}

fn test_log() {
    e := math.E();
    result := math.log(e);
    assert_eq(true, result > 0.99);
    assert_eq(true, result < 1.01);
}

fn test_log10() {
    result := math.log10(100.0);
    assert_eq(true, result > 1.99);
    assert_eq(true, result < 2.01);
}

fn test_log2() {
    result := math.log2(8.0);
    assert_eq(true, result > 2.99);
    assert_eq(true, result < 3.01);
}

// ============================================================================
// Trigonometric Functions
// ============================================================================

fn test_sin() {
    pi := math.PI();
    result := math.sin(pi / 2.0);
    assert_eq(true, result > 0.99);
    assert_eq(true, result < 1.01);
}

fn test_cos() {
    result := math.cos(0.0);
    assert_eq(true, result > 0.99);
    assert_eq(true, result < 1.01);
}

fn test_tan() {
    pi := math.PI();
    result := math.tan(pi / 4.0);
    assert_eq(true, result > 0.99);
    assert_eq(true, result < 1.01);
}

fn test_asin() {
    pi := math.PI();
    result := math.asin(1.0);
    expected := pi / 2.0;
    assert_eq(true, result > expected - 0.01);
    assert_eq(true, result < expected + 0.01);
}

fn test_acos() {
    result := math.acos(1.0);
    assert_eq(true, result > -0.01);
    assert_eq(true, result < 0.01);
}

fn test_atan() {
    pi := math.PI();
    result := math.atan(1.0);
    expected := pi / 4.0;
    assert_eq(true, result > expected - 0.01);
    assert_eq(true, result < expected + 0.01);
}

fn test_atan2() {
    pi := math.PI();
    result := math.atan2(1.0, 1.0);
    expected := pi / 4.0;
    assert_eq(true, result > expected - 0.01);
    assert_eq(true, result < expected + 0.01);
}

// ============================================================================
// Hyperbolic Functions
// ============================================================================

fn test_sinh() {
    result := math.sinh(0.0);
    assert_eq(true, result > -0.01);
    assert_eq(true, result < 0.01);
}

fn test_cosh() {
    result := math.cosh(0.0);
    assert_eq(true, result > 0.99);
    assert_eq(true, result < 1.01);
}

fn test_tanh() {
    result := math.tanh(0.0);
    assert_eq(true, result > -0.01);
    assert_eq(true, result < 0.01);
}

// ============================================================================
// Utility Functions
// ============================================================================

fn test_is_nan() {
    nan := math.NAN();
    assert_eq(true, math.is_nan(nan));
    assert_eq(false, math.is_nan(0.0));
    assert_eq(false, math.is_nan(1.0));
}

fn test_is_inf() {
    inf := math.INF();
    assert_eq(true, math.is_inf(inf));
    neg_inf := 0.0 - inf;
    assert_eq(true, math.is_inf(neg_inf));
    assert_eq(false, math.is_inf(0.0));
    assert_eq(false, math.is_inf(1.0));
}

fn test_is_finite() {
    assert_eq(true, math.is_finite(0.0));
    assert_eq(true, math.is_finite(1.0));
    assert_eq(true, math.is_finite(-1.0));

    inf := math.INF();
    assert_eq(false, math.is_finite(inf));

    nan := math.NAN();
    assert_eq(false, math.is_finite(nan));
}

fn test_sign() {
    assert_eq(1, math.sign(5.5));
    assert_eq(-1, math.sign(-5.5));
    assert_eq(0, math.sign(0.0));
}

fn test_sign_int() {
    assert_eq(1, math.sign_int(5));
    assert_eq(-1, math.sign_int(-5));
    assert_eq(0, math.sign_int(0));
}

fn test_gcd() {
    assert_eq(4, math.gcd(12, 8));
    assert_eq(1, math.gcd(7, 5));
    assert_eq(5, math.gcd(0, 5));
    assert_eq(6, math.gcd(6, 0));
}

fn test_lcm() {
    assert_eq(12, math.lcm(4, 6));
    assert_eq(35, math.lcm(7, 5));
    assert_eq(0, math.lcm(0, 5));
}
