/// Test that 'or fail ErrorType;' captures the original error as cause

BaseError :: err;
WrapperError :: err;
OuterError :: err;

fn fail_base() -> int or err {
    fail BaseError { message: "base failure" };
}

fn wrap_error() -> int or err {
    x := fail_base() or fail WrapperError;
    return x;
}

fn double_wrap() -> int or err {
    x := wrap_error() or fail OuterError;
    return x;
}

fn test_cause_is_set() {
    result := wrap_error() or {
        // err should be WrapperError with BaseError as cause
        assert_eq(err.message, "WrapperError");
        assert_eq(err.cause.message, "base failure");
        return;
    };
}

fn test_root_cause() {
    result := double_wrap() or {
        // err is OuterError
        // err.root_cause should be BaseError (the original error)
        assert_eq(err.message, "OuterError");
        assert_eq(err.root_cause.message, "base failure");
        return;
    };
}

fn test_cause_chain_length() {
    result := double_wrap() or {
        // Check full chain: OuterError -> WrapperError -> BaseError
        assert_eq(err.message, "OuterError");
        assert_eq(err.cause.message, "WrapperError");
        assert_eq(err.cause.cause.message, "base failure");
        return;
    };
}
