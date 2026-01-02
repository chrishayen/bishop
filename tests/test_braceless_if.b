/// Test braceless if/else syntax

NotFound :: err;
EmptyValue :: err;

fn test_braceless_if_simple() {
    x := 5;
    result := 0;

    if x > 3 result = 10;
    else result = 20;

    assert_eq(result, 10);
}

fn test_braceless_if_else_chain() {
    x := 2;
    result := 0;

    if x == 1 result = 1;
    else if x == 2 result = 2;
    else result = 3;

    assert_eq(result, 2);
}

fn test_braceless_if_with_fail() -> void or err {
    x := 5;

    if x < 0 fail NotFound;

    assert_eq(x, 5);
}

fn helper_braceless(int x) -> int or err {
    if x < 0 fail NotFound;
    else return x * 2;
}

fn test_braceless_if_else_with_fail() -> void or err {
    result := helper_braceless(5) or fail err;
    assert_eq(result, 10);
}

fn test_braceless_if_else_returns() {
    result := helper_braceless(-1) or match err {
        NotFound => 999,
        _ => 0
    };
    assert_eq(result, 999);
}

fn get_value(bool empty) -> int or err {
    if empty fail EmptyValue;
    else return 42;
}

fn test_braceless_return_value() -> void or err {
    v := get_value(false) or fail err;
    assert_eq(v, 42);
}

fn test_braceless_single_statement() {
    count := 0;

    if true count = count + 1;
    assert_eq(count, 1);

    if false count = count + 10;
    assert_eq(count, 1);
}

fn test_mixed_braced_and_braceless() {
    x := 10;
    result := 0;

    if x > 5 {
        result = 100;
    } else result = 50;

    assert_eq(result, 100);
}
