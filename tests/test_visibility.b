// Test @private decorator on same line as fn
@private fn internal_helper_same_line() -> int {
    return 42;
}

// Test @private decorator on separate line from fn
@private
fn internal_helper_newline() -> int {
    return 43;
}

fn test_private_function_same_line() {
    result := internal_helper_same_line();
    assert_eq(result, 42);
}

fn test_private_function_newline() {
    result := internal_helper_newline();
    assert_eq(result, 43);
}

// Test @private decorator on same line as struct
@private PrivateStructSameLine :: struct {
    value int
}

// Test @private decorator on separate line from struct
@private
PrivateStructNewline :: struct {
    value int
}

fn test_private_struct_same_line() {
    p := PrivateStructSameLine { value: 100 };
    assert_eq(p.value, 100);
}

fn test_private_struct_newline() {
    p := PrivateStructNewline { value: 200 };
    assert_eq(p.value, 200);
}
