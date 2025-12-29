// ============================================
// List<str> Join Method
// ============================================

fn test_list_join_basic() {
    parts := ["hello", "world"];
    result := parts.join(" ");
    assert_eq(result, "hello world");
}

fn test_list_join_comma() {
    parts := ["a", "b", "c"];
    result := parts.join(", ");
    assert_eq(result, "a, b, c");
}

fn test_list_join_empty_separator() {
    parts := ["a", "b", "c"];
    result := parts.join("");
    assert_eq(result, "abc");
}

fn test_list_join_single_element() {
    parts := ["hello"];
    result := parts.join("-");
    assert_eq(result, "hello");
}

fn test_list_join_empty_list() {
    parts := List<str>();
    result := parts.join("-");
    assert_eq(result, "");
}

fn test_list_join_dash() {
    parts := ["hello", "world", "foo"];
    result := parts.join("-");
    assert_eq(result, "hello-world-foo");
}

fn test_list_join_multi_char_separator() {
    parts := ["a", "b", "c"];
    result := parts.join(" :: ");
    assert_eq(result, "a :: b :: c");
}

fn test_list_join_with_empty_strings() {
    parts := ["a", "", "c"];
    result := parts.join("-");
    assert_eq(result, "a--c");
}

fn test_split_then_join() {
    s := "hello world foo";
    parts := s.split(" ");
    result := parts.join("-");
    assert_eq(result, "hello-world-foo");
}
