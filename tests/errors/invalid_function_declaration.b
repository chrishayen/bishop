/// Test that fn name() or err { } is rejected without explicit return type
/// Correct syntax: fn name() -> void or err { }

fn broken() or err {
    fail "should not parse";
}
