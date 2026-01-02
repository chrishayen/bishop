/// Test that Type :: method() or err { } is rejected without explicit return type
/// Correct syntax: Type :: method() -> void or err { }

Example :: struct {}

Example :: broken(self) or err {
    fail "should not parse";
}
