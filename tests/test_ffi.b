// Test @extern decorator on same line as fn
@extern("c") fn puts(cstr s) -> cint;
@extern("m") fn sqrt(f64 x) -> f64;

// Test @extern decorator on separate line from fn
@extern("m")
fn floor(f64 x) -> f64;

@extern("m")
fn ceil(f64 x) -> f64;

fn test_puts() {
    puts("Hello from C FFI!");
}

fn test_sqrt_same_line() {
    result := sqrt(16.0);
    floored := floor(result);
    assert_eq(floored, 4.0);
}

fn test_floor_newline() {
    result := floor(3.7);
    assert_eq(result, 3.0);
}

fn test_ceil_newline() {
    result := ceil(3.2);
    assert_eq(result, 4.0);
}
