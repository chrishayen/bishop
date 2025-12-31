/// Tests for standalone 'or' statements (without assignment)
/// This tests the feature requested in issue #122

// ============================================================
// Error Definitions
// ============================================================

ValidationError :: err;

// ============================================================
// Helper Functions
// ============================================================

fn check_exists(bool exists) -> bool or err {
    if !exists {
        fail ValidationError { message: "not found" };
    }

    return true;
}

fn check_valid(bool valid) -> bool or err {
    if !valid {
        fail ValidationError { message: "not valid" };
    }

    return true;
}

fn get_content(str path) -> str or err {
    if path == "" {
        fail "empty path";
    }

    return "file content";
}

// ============================================================
// Test: Standalone function call with or fail
// ============================================================

fn test_standalone_func_or_fail_success() or err {
    // Should not fail when condition is true
    check_exists(true) or fail err;
    check_valid(true) or fail err;
    return;
}

fn helper_standalone_func_fail() -> int or err {
    // Should fail and propagate the error
    check_exists(false) or fail err;
    return 1;  // Never reached
}

fn test_standalone_func_or_fail_propagates() {
    result := helper_standalone_func_fail() or match err {
        _ => 0
    };
    assert_eq(result, 0);
}

// ============================================================
// Test: Standalone variable with or fail
// ============================================================

fn helper_var_or_fail_false() -> int or err {
    exists := false;
    exists or fail "not found";
    return 1;
}

fn helper_var_or_fail_true() -> int or err {
    exists := true;
    exists or fail "should not fail";
    return 1;
}

fn test_standalone_var_or_fail_false() {
    result := helper_var_or_fail_false() or match err {
        _ => 0
    };
    assert_eq(result, 0);
}

fn test_standalone_var_or_fail_true() or err {
    result := helper_var_or_fail_true() or fail err;
    assert_eq(result, 1);
}

// ============================================================
// Test: Standalone or return
// ============================================================

fn helper_or_return_void(bool flag) {
    flag or return;
    print("reached");
}

fn helper_or_return_value(bool flag) -> int {
    flag or return 0;
    return 1;
}

fn test_standalone_or_return_void() {
    // Should return early when flag is false
    helper_or_return_void(false);
    helper_or_return_void(true);
}

fn test_standalone_or_return_value() {
    assert_eq(helper_or_return_value(false), 0);
    assert_eq(helper_or_return_value(true), 1);
}

// ============================================================
// Test: Standalone or continue in loop
// ============================================================

fn test_standalone_or_continue() {
    count := 0;

    for i in 0..5 {
        flag := i > 0;
        flag or continue;
        count = count + 1;
    }

    // i=0: false, continue; i=1,2,3,4: true, count++
    assert_eq(count, 4);
}

fn test_standalone_func_or_continue() {
    count := 0;

    for i in 0..5 {
        check_exists(i > 0) or continue;
        count = count + 1;
    }

    assert_eq(count, 4);
}

// ============================================================
// Test: Standalone or break in loop
// ============================================================

fn test_standalone_or_break() {
    count := 0;

    for i in 0..10 {
        flag := i < 3;
        flag or break;
        count = count + 1;
    }

    // i=0,1,2: true, count++; i=3: false, break
    assert_eq(count, 3);
}

fn test_standalone_func_or_break() {
    count := 0;

    for i in 0..10 {
        check_exists(i < 3) or break;
        count = count + 1;
    }

    assert_eq(count, 3);
}

// ============================================================
// Test: Standalone or block
// ============================================================

fn test_standalone_or_block() {
    handled := false;
    exists := false;

    exists or {
        handled = true;
    };

    assert_true(handled);
}

fn helper_or_block_with_return(bool flag) -> int {
    flag or {
        return 0;
    };
    return 1;
}

fn test_standalone_or_block_with_return() {
    assert_eq(helper_or_block_with_return(false), 0);
    assert_eq(helper_or_block_with_return(true), 1);
}

// ============================================================
// Test: Guard clause pattern (main use case from issue)
// ============================================================

fn load_data(bool exists, bool valid) -> str or err {
    // Guard clauses without dummy assignment
    check_exists(exists) or fail err;
    check_valid(valid) or fail err;
    return "data loaded";
}

fn test_guard_all_pass() or err {
    result := load_data(true, true) or fail err;
    assert_eq(result, "data loaded");
}

fn test_guard_first_fails() {
    result := load_data(false, true) or match err {
        _ => "failed"
    };
    assert_eq(result, "failed");
}

fn test_guard_second_fails() {
    result := load_data(true, false) or match err {
        _ => "failed"
    };
    assert_eq(result, "failed");
}

// ============================================================
// Test: Multiple standalone or statements in sequence
// ============================================================

fn validate_all(bool a, bool b, bool c) -> str or err {
    a or fail "a failed";
    b or fail "b failed";
    c or fail "c failed";
    return "all valid";
}

fn test_multiple_standalone_all_pass() or err {
    result := validate_all(true, true, true) or fail err;
    assert_eq(result, "all valid");
}

fn test_multiple_standalone_first_fails() {
    result := validate_all(false, true, true) or match err {
        _ => "failed"
    };
    assert_eq(result, "failed");
}

fn test_multiple_standalone_last_fails() {
    result := validate_all(true, true, false) or match err {
        _ => "failed"
    };
    assert_eq(result, "failed");
}

// ============================================================
// Test: Comparison expressions with or fail
// ============================================================

fn helper_comparison_or_fail_pass() -> int or err {
    x := 5;
    x >= 3 or fail "x should be at least 3";
    return 1;
}

fn helper_comparison_or_fail_fail() -> int or err {
    x := 2;
    x >= 3 or fail "x should be at least 3";
    return 1;
}

fn test_comparison_or_fail_pass() or err {
    result := helper_comparison_or_fail_pass() or fail err;
    assert_eq(result, 1);
}

fn test_comparison_or_fail_fail() {
    result := helper_comparison_or_fail_fail() or match err {
        _ => 0
    };
    assert_eq(result, 0);
}

fn helper_method_comparison_or_fail_pass() -> int or err {
    parts := ["a", "b", "c"];
    parts.length() >= 3 or fail "need at least 3 parts";
    return 1;
}

fn helper_method_comparison_or_fail_fail() -> int or err {
    parts := ["a", "b"];
    parts.length() >= 3 or fail "need at least 3 parts";
    return 1;
}

fn test_method_comparison_or_fail_pass() or err {
    result := helper_method_comparison_or_fail_pass() or fail err;
    assert_eq(result, 1);
}

fn test_method_comparison_or_fail_fail() {
    result := helper_method_comparison_or_fail_fail() or match err {
        _ => 0
    };
    assert_eq(result, 0);
}

fn helper_complex_expr_or_fail() -> int or err {
    items := [1, 2, 3, 4, 5];
    items.length() + 1 > 5 or fail "should have more than 5 items after adding 1";
    return 1;
}

fn test_complex_expr_or_fail() or err {
    result := helper_complex_expr_or_fail() or fail err;
    assert_eq(result, 1);
}
