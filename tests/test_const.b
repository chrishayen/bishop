// Tests for const keyword

fn test_const_int_explicit() {
    const int MAX_SIZE = 100;
    assert_eq(MAX_SIZE, 100);
}

fn test_const_str_explicit() {
    const str NAME = "Bishop";
    assert_eq(NAME, "Bishop");
}

fn test_const_bool_explicit() {
    const bool FLAG = true;
    assert_eq(FLAG, true);
}

fn test_const_f64_explicit() {
    const f64 PI = 3.14159;
    assert_eq(PI, 3.14159);
}

fn test_const_inferred_int() {
    const MAX := 50;
    assert_eq(MAX, 50);
}

fn test_const_inferred_str() {
    const GREETING := "Hello";
    assert_eq(GREETING, "Hello");
}

fn test_const_inferred_bool() {
    const ENABLED := false;
    assert_eq(ENABLED, false);
}

fn test_const_inferred_float() {
    const E := 2.71828;
    assert_eq(E, 2.71828);
}

fn test_const_char_explicit() {
    const char LETTER = 'A';
    assert_eq(LETTER, 'A');
}

fn test_const_with_expression() {
    const int DOUBLED = 21 * 2;
    assert_eq(DOUBLED, 42);
}

fn test_const_with_string_concat() {
    const str FULL_NAME = "John" + " " + "Doe";
    assert_eq(FULL_NAME, "John Doe");
}

fn test_const_in_function_scope() {
    const int LOCAL_CONST = 123;
    assert_eq(LOCAL_CONST, 123);
}

fn helper_const_usage() -> int {
    const int VALUE = 42;
    return VALUE;
}

fn test_const_return_value() {
    result := helper_const_usage();
    assert_eq(result, 42);
}

fn test_const_in_condition() {
    const int THRESHOLD = 10;
    x := 15;

    if x > THRESHOLD {
        assert_eq(true, true);
    } else {
        assert_eq(true, false);
    }
}

fn test_const_in_loop() {
    const int LIMIT = 5;
    sum := 0;

    for i in 0..LIMIT {
        sum = sum + i;
    }

    assert_eq(sum, 10);
}
