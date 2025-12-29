import tests.testlib;

fn test_qualified_call() {
    testlib.greet();
    assert_eq(1, 1);
}

fn test_qualified_call_with_return() {
    int result = testlib.add(2, 3);
    assert_eq(result, 5);
}

// Cross-module const tests
fn test_cross_module_const_int() {
    assert_eq(testlib.TESTLIB_VALUE, 42);
}

fn test_cross_module_const_str() {
    assert_eq(testlib.TESTLIB_NAME, "testlib");
}

fn test_cross_module_const_in_expression() {
    result := testlib.TESTLIB_VALUE * 2;
    assert_eq(result, 84);
}
