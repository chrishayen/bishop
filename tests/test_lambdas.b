// ============================================
// Anonymous Function (Lambda) Tests
// ============================================

// Helper function for higher-order functions
fn apply_op(int x, int y, fn(int, int) -> int op) -> int {
    return op(x, y);
}

fn apply_unary(int x, fn(int) -> int op) -> int {
    return op(x);
}

fn apply_void(fn() action) {
    action();
}

// ============================================
// Basic Lambda Syntax
// ============================================

fn test_basic_lambda() {
    // Lambda assigned to variable
    double := fn(int x) -> int { return x * 2; };
    result := double(21);
    assert_eq(result, 42);
}

fn test_lambda_no_params() {
    // Lambda with no parameters
    get_value := fn() -> int { return 42; };
    assert_eq(get_value(), 42);
}

fn test_lambda_void_return() {
    // Lambda with void return (no return type)
    counter := 0;
    increment := fn() { counter = counter + 1; };
    increment();
    assert_eq(counter, 1);
}

fn test_lambda_multiple_params() {
    // Lambda with multiple parameters
    add := fn(int a, int b) -> int { return a + b; };
    assert_eq(add(3, 4), 7);
}

// ============================================
// Lambdas as Arguments
// ============================================

fn test_lambda_as_argument() {
    // Lambda passed directly to higher-order function
    result := apply_op(3, 4, fn(int a, int b) -> int { return a + b; });
    assert_eq(result, 7);
}

fn test_lambda_as_argument_multiply() {
    result := apply_op(3, 4, fn(int a, int b) -> int { return a * b; });
    assert_eq(result, 12);
}

fn test_lambda_as_argument_unary() {
    result := apply_unary(5, fn(int x) -> int { return x * x; });
    assert_eq(result, 25);
}

// ============================================
// Closures (Capturing Variables)
// ============================================

fn test_closure_capture() {
    // Lambda capturing outer variable
    multiplier := 10;
    scale := fn(int x) -> int { return x * multiplier; };
    assert_eq(scale(4), 40);
}

fn test_closure_multiple_captures() {
    // Lambda capturing multiple variables
    a := 2;
    b := 3;
    combine := fn(int x) -> int { return x * a + b; };
    assert_eq(combine(5), 13);
}

// ============================================
// Lambda Type Compatibility
// ============================================

fn test_lambda_stored_in_fn_type() {
    // Lambda stored in explicitly typed variable
    fn(int, int) -> int op = fn(int a, int b) -> int { return a - b; };
    result := op(10, 3);
    assert_eq(result, 7);
}

// ============================================
// Lambdas with go (Goroutines)
// ============================================

fn test_lambda_with_go() {
    ch := Channel<int>();

    // Using lambda with go spawn
    go fn() {
        ch.send(42);
    }();

    val := ch.recv();
    assert_eq(val, 42);
}

fn test_lambda_with_go_capture() {
    ch := Channel<int>();
    value := 100;

    // Lambda capturing and using in goroutine
    go fn() {
        ch.send(value);
    }();

    val := ch.recv();
    assert_eq(val, 100);
}
