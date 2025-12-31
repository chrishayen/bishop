/// Tests for 'or' keyword with falsy expressions (not just fallible functions)
/// This tests the feature requested in issue #110

// ============================================================
// Helper functions (not run directly by test runner)
// ============================================================

// These are helpers that are expected to fail - called by wrapper tests
fn helper_bool_or_fail_false() -> int or err {
    b := false;
    b or fail "expected failure";
    return 1;
}

fn helper_bool_or_fail_true() -> int or err {
    b := true;
    b or fail "should not fail";
    return 1;
}

fn helper_int_zero_or_fail() -> int or err {
    x := 0;
    x or fail "zero is falsy";
    return 1;
}

fn helper_int_nonzero_or_fail() -> int or err {
    x := 42;
    x or fail "should not fail";
    return 1;
}

fn helper_str_empty_or_fail() -> int or err {
    s := "";
    s or fail "empty string is falsy";
    return 1;
}

fn helper_str_nonempty_or_fail() -> int or err {
    s := "hello";
    s or fail "should not fail";
    return 1;
}

fn helper_guard_example(bool exists, bool valid) -> str or err {
    exists or fail "not found";
    valid or fail "not valid";
    return "success";
}

// ============================================================
// Test: Bool expressions with or fail
// ============================================================

fn test_bool_false_triggers_fail() {
    // false should trigger or fail
    result := helper_bool_or_fail_false() or match err {
        _ => 0
    };
    assert_eq(result, 0);
}

fn test_bool_true_does_not_fail() or err {
    // true should not trigger or fail
    result := helper_bool_or_fail_true() or fail err;
    assert_eq(result, 1);
}

// ============================================================
// Test: Bool expressions with or return
// ============================================================

fn helper_bool_or_return_true() -> int {
    b := true;
    b or return 0;
    return 1;
}

fn helper_bool_or_return_false() -> int {
    b := false;
    b or return 0;
    return 1;
}

fn test_bool_or_return() {
    assert_eq(helper_bool_or_return_true(), 1);
    assert_eq(helper_bool_or_return_false(), 0);
}

// ============================================================
// Test: Int expressions with or fail
// ============================================================

fn test_int_zero_triggers_fail() {
    // 0 should trigger or fail
    result := helper_int_zero_or_fail() or match err {
        _ => 0
    };
    assert_eq(result, 0);
}

fn test_int_nonzero_does_not_fail() or err {
    // non-zero should not trigger or fail
    result := helper_int_nonzero_or_fail() or fail err;
    assert_eq(result, 1);
}

// ============================================================
// Test: String expressions with or fail
// ============================================================

fn test_str_empty_triggers_fail() {
    // empty string should trigger or fail
    result := helper_str_empty_or_fail() or match err {
        _ => 0
    };
    assert_eq(result, 0);
}

fn test_str_nonempty_does_not_fail() or err {
    // non-empty string should not trigger or fail
    result := helper_str_nonempty_or_fail() or fail err;
    assert_eq(result, 1);
}

// ============================================================
// Test: Guard clause patterns (main use case from issue)
// ============================================================

fn test_guard_all_pass() or err {
    result := helper_guard_example(true, true) or fail err;
    assert_eq(result, "success");
}

fn test_guard_first_fails() {
    result := helper_guard_example(false, true) or match err {
        _ => "failed"
    };
    assert_eq(result, "failed");
}

fn test_guard_second_fails() {
    result := helper_guard_example(true, false) or match err {
        _ => "failed"
    };
    assert_eq(result, "failed");
}

// ============================================================
// Test: or continue in loop
// ============================================================

fn test_or_continue() {
    count := 0;
    for i in 0..5 {
        // Skip when i is 0 (falsy)
        i or continue;
        count = count + 1;
    }
    // i=0 skipped, i=1,2,3,4 processed = 4
    assert_eq(count, 4);
}

// ============================================================
// Test: or break in loop
// ============================================================

fn test_or_break() {
    count := 0;
    values := [1, 2, 0, 4, 5];  // 0 is falsy, should break
    for v in values {
        v or break;
        count = count + 1;
    }
    // Processes 1, 2, then breaks at 0
    assert_eq(count, 2);
}
