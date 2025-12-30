import tests.testlib;
using testlib.*;

// Test using wildcard imports - all public members available

fn test_wildcard_function_call() {
    greet();
    assert_eq(1, 1);
}

fn test_wildcard_function_with_return() {
    int result = add(2, 3);
    assert_eq(result, 5);
}

fn test_wildcard_const_int() {
    assert_eq(TESTLIB_VALUE, 42);
}

fn test_wildcard_const_str() {
    assert_eq(TESTLIB_NAME, "testlib");
}

fn test_wildcard_struct() {
    p := make_point(10, 20);
    assert_eq(p.x, 10);
    assert_eq(p.y, 20);
}

fn test_wildcard_struct_literal() {
    p := Point { x: 5, y: 15 };
    assert_eq(p.x, 5);
    assert_eq(p.y, 15);
}
