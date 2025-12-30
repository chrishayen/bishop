// Tests for default expressions with all falsy types (Issue #105)
// https://github.com/chrishayen/bishop/issues/105

// ============================================
// String default - empty string is falsy
// ============================================

fn get_empty_string() -> str {
    return "";
}

fn get_nonempty_string() -> str {
    return "value";
}

fn test_empty_string_literal_default() {
    s := "" default "fallback";
    assert_eq(s, "fallback");
}

fn test_nonempty_string_literal_default() {
    s := "hello" default "fallback";
    assert_eq(s, "hello");
}

fn test_function_empty_string_default() {
    s := get_empty_string() default "fallback";
    assert_eq(s, "fallback");
}

fn test_function_nonempty_string_default() {
    s := get_nonempty_string() default "fallback";
    assert_eq(s, "value");
}

fn test_variable_empty_string_default() {
    x := "";
    s := x default "fallback";
    assert_eq(s, "fallback");
}

fn test_variable_nonempty_string_default() {
    x := "hello";
    s := x default "fallback";
    assert_eq(s, "hello");
}

fn test_substr_empty_result_default() {
    s := "hello";
    result := s.substr(5, 0) default "fallback";
    assert_eq(result, "fallback");
}

fn test_substr_nonempty_result_default() {
    s := "hello";
    result := s.substr(0, 3) default "fallback";
    assert_eq(result, "hel");
}

// ============================================
// List default - empty list is falsy
// ============================================

fn get_empty_list() -> List<int> {
    return List<int>();
}

fn get_nonempty_list() -> List<int> {
    return [1, 2, 3];
}

fn test_empty_list_literal_default() {
    l := List<int>() default [99];
    assert_eq(l.length(), 1);
    assert_eq(l.get(0), 99);
}

fn test_nonempty_list_literal_default() {
    l := [1, 2] default [99];
    assert_eq(l.length(), 2);
    assert_eq(l.get(0), 1);
}

fn test_function_empty_list_default() {
    l := get_empty_list() default [99];
    assert_eq(l.length(), 1);
    assert_eq(l.get(0), 99);
}

fn test_function_nonempty_list_default() {
    l := get_nonempty_list() default [99];
    assert_eq(l.length(), 3);
    assert_eq(l.get(0), 1);
}

fn test_variable_empty_list_default() {
    x := List<int>();
    l := x default [99];
    assert_eq(l.length(), 1);
}

fn test_variable_nonempty_list_default() {
    x := [1, 2, 3];
    l := x default [99];
    assert_eq(l.length(), 3);
}

