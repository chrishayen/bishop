/// Test bare error type in fail statements

// Simple error with no fields
NotFound :: err;

// Error with custom fields
EmptyFile :: err {
    path str
}

// Another error with multiple fields
ValidationError :: err {
    field str,
    reason str
}

fn fail_not_found() -> void or err {
    fail NotFound;
}

fn fail_empty_file() -> void or err {
    fail EmptyFile;
}

fn fail_validation() -> void or err {
    fail ValidationError;
}

fn returns_or_fails(bool should_fail) -> int or err {
    if should_fail {
        fail NotFound;
    }
    return 42;
}

fn test_bare_error_simple() -> void or err {
    // Test that bare error types work
    result := returns_or_fails(false) or return;
    assert_eq(result, 42);
}

fn test_bare_error_fails() {
    // Test that the error is actually returned
    result := returns_or_fails(true) or match err {
        NotFound => 999,
        _ => 0
    };
    assert_eq(result, 999);
}

fn test_or_fail_bare() -> void or err {
    // Test propagating with or fail on bare error
    x := returns_or_fails(false) or fail err;
    assert_eq(x, 42);
}

fn propagate_bare() -> int or err {
    // Propagate bare error
    x := returns_or_fails(true) or fail err;
    return x;
}

fn test_propagate_bare() {
    result := propagate_bare() or match err {
        NotFound => 123,
        _ => 0
    };
    assert_eq(result, 123);
}
