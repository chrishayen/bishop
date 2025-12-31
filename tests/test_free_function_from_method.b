// Test calling free functions from within static and instance methods
// Free functions should be visible from within struct methods

Calculator :: struct {
    value int
}

// Free function (not a static method)
fn helper_add(int a, int b) -> int {
    return a + b;
}

// Free function that is fallible
fn helper_parse(str s) -> int or err {
    return 42;
}

// Static method calling a free function
Calculator :: compute(int x, int y) -> int {
    return helper_add(x, y);
}

// Static method calling a fallible free function
Calculator :: parse_and_compute(str s) -> int or err {
    val := helper_parse(s) or fail err;
    return helper_add(val, 10);
}

// Instance method calling a free function
Calculator :: add_to_value(self, int x) -> int {
    return helper_add(self.value, x);
}

fn test_static_calls_free_function() {
    result := Calculator.compute(3, 4);
    assert_eq(result, 7);
}

fn test_static_calls_fallible_free_function() -> void or err {
    result := Calculator.parse_and_compute("test") or fail err;
    assert_eq(result, 52);
}

fn test_instance_calls_free_function() {
    c := Calculator { value: 10 };
    result := c.add_to_value(5);
    assert_eq(result, 15);
}
