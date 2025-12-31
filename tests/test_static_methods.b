// Test file for @static decorator functionality
// Tests static methods without self parameter, called as TypeName.method()

Counter :: struct {
    value int
}

@static
Counter :: make(int initial) -> Counter {
    return Counter { value: initial };
}

@static
Counter :: create() -> Counter {
    return Counter { value: 0 };
}

@static
Counter :: from_value(int val) -> Counter {
    return Counter { value: val };
}

@static
Counter :: add(int a, int b) -> int {
    return a + b;
}

@static
Counter :: zero() -> int {
    return 0;
}

Counter :: get(self) -> int {
    return self.value;
}

Counter :: increment(self) {
    self.value = self.value + 1;
}

// Test basic static method call with no parameters
fn test_static_method_no_params() {
    c := Counter.create();
    assert_eq(c.value, 0);
}

// Test static method with one parameter
fn test_static_method_one_param() {
    c := Counter.from_value(42);
    assert_eq(c.value, 42);
}

// Test static method with multiple parameters
fn test_static_method_multiple_params() {
    result := Counter.add(10, 20);
    assert_eq(result, 30);
}

// Test static method returning primitive
fn test_static_method_returns_primitive() {
    z := Counter.zero();
    assert_eq(z, 0);
}

// Test mixing static and instance methods
fn test_static_and_instance_methods() {
    c := Counter.from_value(5);
    assert_eq(c.get(), 5);
    c.increment();
    assert_eq(c.get(), 6);
}

// Test chaining static method result with instance method
fn test_static_then_instance_call() {
    result := Counter.from_value(100).get();
    assert_eq(result, 100);
}

// Test multiple static method calls
fn test_multiple_static_calls() {
    c1 := Counter.create();
    c2 := Counter.from_value(10);
    c3 := Counter.from_value(20);
    assert_eq(c1.value, 0);
    assert_eq(c2.value, 10);
    assert_eq(c3.value, 20);
}

// Test static method on different struct
Point :: struct {
    x int,
    y int
}

@static
Point :: origin() -> Point {
    return Point { x: 0, y: 0 };
}

@static
Point :: at(int x, int y) -> Point {
    return Point { x: x, y: y };
}

Point :: sum(self) -> int {
    return self.x + self.y;
}

fn test_static_method_different_struct() {
    p := Point.origin();
    assert_eq(p.x, 0);
    assert_eq(p.y, 0);
}

fn test_static_factory_method() {
    p := Point.at(3, 4);
    assert_eq(p.sum(), 7);
}
