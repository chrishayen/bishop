// Test calling static methods via self from within instance methods

Calculator :: struct {
    value int
}

// Static method - no self parameter
Calculator :: zero() -> int {
    return 0;
}

// Static method with params - no self
Calculator :: add(int a, int b) -> int {
    return a + b;
}

// Static method returning struct
Calculator :: create(int initial) -> Calculator {
    return Calculator { value: initial };
}

// Instance method that calls static method via self
Calculator :: get_or_default(self) -> int {
    if self.value == 0 {
        return self.zero();
    }
    return self.value;
}

// Instance method calling static with params via self
Calculator :: compute(self, int x) -> int {
    return self.add(self.value, x);
}

// Instance method calling static that returns struct via self
Calculator :: duplicate(self) -> Calculator {
    return self.create(self.value);
}

fn test_self_static_call_no_params() {
    c := Calculator { value: 0 };
    assert_eq(c.get_or_default(), 0);
}

fn test_self_static_call_with_value() {
    c := Calculator { value: 42 };
    assert_eq(c.get_or_default(), 42);
}

fn test_self_static_call_with_params() {
    c := Calculator { value: 10 };
    assert_eq(c.compute(5), 15);
}

fn test_self_static_call_returns_struct() {
    c := Calculator { value: 7 };
    c2 := c.duplicate();
    assert_eq(c2.value, 7);
}

fn test_direct_and_self_static_same_result() {
    direct := Calculator.add(3, 4);
    c := Calculator { value: 0 };
    via_self := c.compute(4);  // add(0, 4)
    assert_eq(direct, 7);
    assert_eq(via_self, 4);
}
