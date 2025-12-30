import tests.testlib;
using testlib.greet, testlib.add;
using testlib.TESTLIB_VALUE, testlib.TESTLIB_NAME;

// Test using with selective function imports
fn test_using_function_call() {
    greet();
    assert_eq(1, 1);
}

fn test_using_function_call_with_return() {
    int result = add(2, 3);
    assert_eq(result, 5);
}

// Test using with selective constant imports
fn test_using_const_int() {
    assert_eq(TESTLIB_VALUE, 42);
}

fn test_using_const_str() {
    assert_eq(TESTLIB_NAME, "testlib");
}

fn test_using_const_in_expression() {
    result := TESTLIB_VALUE * 2;
    assert_eq(result, 84);
}

// Test that qualified access still works alongside using
fn test_using_with_qualified_access() {
    greet();  // using alias
    testlib.greet();  // qualified access
    assert_eq(1, 1);
}
