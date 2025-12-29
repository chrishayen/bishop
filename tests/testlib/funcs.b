// Module-level constants for cross-module testing
const int TESTLIB_VALUE = 42;
const str TESTLIB_NAME = "testlib";

fn greet() {
    print("hello from testlib");
}

fn add(int a, int b) -> int {
    return a + b;
}
