// ============================================
// assert_ne - Not Equal
// ============================================

fn test_assert_ne_int() {
    assert_ne(1, 2);
}

fn test_assert_ne_str() {
    assert_ne("hello", "world");
}

fn test_assert_ne_bool() {
    assert_ne(true, false);
}

// ============================================
// assert_true / assert_false - Boolean Assertions
// ============================================

fn test_assert_true_literal() {
    assert_true(true);
}

fn test_assert_true_comparison() {
    assert_true(5 > 3);
}

fn test_assert_true_expression() {
    x := 10;
    assert_true(x == 10);
}

fn test_assert_false_literal() {
    assert_false(false);
}

fn test_assert_false_comparison() {
    assert_false(5 < 3);
}

fn test_assert_false_expression() {
    x := 10;
    assert_false(x == 5);
}

// ============================================
// assert_gt / assert_gte / assert_lt / assert_lte - Comparisons
// ============================================

fn test_assert_gt() {
    assert_gt(5, 3);
}

fn test_assert_gt_float() {
    assert_gt(3.14, 2.0);
}

fn test_assert_gte() {
    assert_gte(5, 5);
}

fn test_assert_gte_greater() {
    assert_gte(10, 5);
}

fn test_assert_lt() {
    assert_lt(3, 5);
}

fn test_assert_lt_float() {
    assert_lt(2.0, 3.14);
}

fn test_assert_lte() {
    assert_lte(3, 3);
}

fn test_assert_lte_less() {
    assert_lte(3, 5);
}

// ============================================
// assert_contains - Collection Membership (item, collection)
// ============================================

fn test_assert_contains_int_list() {
    nums := [1, 2, 3];
    assert_contains(2, nums);
}

fn test_assert_contains_str_list() {
    names := ["alice", "bob", "charlie"];
    assert_contains("bob", names);
}

// ============================================
// assert_starts_with / assert_ends_with - String Checks (prefix/suffix, string)
// ============================================

fn test_assert_starts_with() {
    s := "hello world";
    assert_starts_with("hello", s);
}

fn test_assert_starts_with_same() {
    s := "hello";
    assert_starts_with("hello", s);
}

fn test_assert_ends_with() {
    s := "hello world";
    assert_ends_with("world", s);
}

fn test_assert_ends_with_same() {
    s := "world";
    assert_ends_with("world", s);
}

// ============================================
// assert_near - Float Comparison with Epsilon
// ============================================

fn test_assert_near_exact() {
    pi := 3.14159;
    assert_near(pi, 3.14159, 0.0001);
}

fn test_assert_near_close() {
    pi := 3.14159;
    assert_near(pi, 3.14, 0.01);
}

fn test_assert_near_larger_epsilon() {
    value := 100.5;
    assert_near(value, 100.0, 1.0);
}

// ============================================
// Combining with other features
// ============================================

fn test_assertions_in_loop() {
    for i in 0..5 {
        assert_gte(i, 0);
        assert_lt(i, 5);
    }
}

fn test_assertions_with_expressions() {
    x := 10;
    y := 5;
    assert_eq(x + y, 15);
    assert_ne(x - y, 0);
    assert_gt(x * 2, y * 3);
}
