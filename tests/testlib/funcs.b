// Module-level constants for cross-module testing
const int TESTLIB_VALUE = 42;
const str TESTLIB_NAME = "testlib";

Point :: struct { x int, y int }

fn greet() {
    print("hello from testlib");
}

fn add(int a, int b) -> int {
    return a + b;
}

fn make_point(int x, int y) -> Point {
    return Point { x: x, y: y };
}
