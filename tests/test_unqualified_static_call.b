// Test calling static methods without qualification from within the same struct
// Both self.method() and StructName.method() should work, but so should just method()

Calculator :: struct {
    value int
}

// Static method - no self parameter
Calculator :: zero() -> int {
    return 0;
}

// Static method with params
Calculator :: add(int a, int b) -> int {
    return a + b;
}

// Static method returning struct
Calculator :: create(int initial) -> Calculator {
    return Calculator { value: initial };
}

// Static method that calls another static method unqualified
Calculator :: double_add(int a, int b) -> int {
    return add(a, b) * 2;
}

// Instance method calling static method unqualified
Calculator :: compute(self, int x) -> int {
    return add(self.value, x);
}

// Instance method calling static method that returns struct unqualified
Calculator :: duplicate(self) -> Calculator {
    return create(self.value);
}

// Instance method calling static with no args unqualified
Calculator :: get_zero(self) -> int {
    return zero();
}

// Chained: static calls another static which calls another
Calculator :: triple_add(int a, int b) -> int {
    return add(double_add(a, b), a);
}

fn test_static_calling_static_unqualified() {
    result := Calculator.double_add(3, 4);
    assert_eq(result, 14);  // (3+4)*2
}

fn test_instance_calling_static_unqualified() {
    c := Calculator { value: 10 };
    result := c.compute(5);
    assert_eq(result, 15);
}

fn test_instance_calling_static_returns_struct_unqualified() {
    c := Calculator { value: 7 };
    c2 := c.duplicate();
    assert_eq(c2.value, 7);
}

fn test_instance_calling_static_no_args_unqualified() {
    c := Calculator { value: 42 };
    result := c.get_zero();
    assert_eq(result, 0);
}

fn test_chained_static_calls_unqualified() {
    result := Calculator.triple_add(2, 3);
    // double_add(2,3) = (2+3)*2 = 10
    // add(10, 2) = 12
    assert_eq(result, 12);
}
