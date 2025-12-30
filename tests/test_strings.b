fn test_string_equality() {
    assert_eq("hello", "hello");
}

fn test_another() {
    assert_eq("world", "world");
}

fn test_str_length() {
    s := "hello";
    assert_eq(s.length(), 5);
}

fn test_str_length_empty() {
    s := "";
    assert_eq(s.length(), 0);
}

fn test_str_empty_true() {
    s := "";
    assert_eq(s.empty(), true);
}

fn test_str_empty_false() {
    s := "hello";
    assert_eq(s.empty(), false);
}

fn test_str_contains_true() {
    s := "hello world";
    assert_eq(s.contains("world"), true);
}

fn test_str_contains_false() {
    s := "hello world";
    assert_eq(s.contains("foo"), false);
}

fn test_str_starts_with_true() {
    s := "hello world";
    assert_eq(s.starts_with("hello"), true);
}

fn test_str_starts_with_false() {
    s := "hello world";
    assert_eq(s.starts_with("world"), false);
}

fn test_str_ends_with_true() {
    s := "hello world";
    assert_eq(s.ends_with("world"), true);
}

fn test_str_ends_with_false() {
    s := "hello world";
    assert_eq(s.ends_with("hello"), false);
}

fn test_str_substr() {
    s := "hello world";
    assert_eq(s.substr(0, 5), "hello");
}

fn test_str_substr_middle() {
    s := "hello world";
    assert_eq(s.substr(6, 5), "world");
}

fn test_str_method_on_literal() {
    assert_eq("hello".length(), 5);
}

fn test_str_method_chaining() {
    s := "hello world";
    sub := s.substr(0, 5);
    assert_eq(sub.length(), 5);
}

fn test_str_concat() {
    s := "hello" + " " + "world";
    assert_eq(s, "hello world");
}

fn test_str_concat_variables() {
    a := "foo";
    b := "bar";
    c := a + b;
    assert_eq(c, "foobar");
}

fn test_single_quoted_string() {
    s := 'hello';
    assert_eq(s, "hello");
}

fn test_single_quoted_string_with_double_quotes() {
    s := '{"name": "Alice"}';
    assert_eq(s.contains("name"), true);
}

fn test_single_quoted_string_concat() {
    a := 'foo';
    b := 'bar';
    c := a + b;
    assert_eq(c, "foobar");
}

fn test_single_quoted_string_methods() {
    s := 'hello world';
    assert_eq(s.length(), 11);
    assert_eq(s.contains("world"), true);
}

fn test_single_and_double_quoted_mixed() {
    a := "hello";
    b := ' world';
    c := a + b;
    assert_eq(c, "hello world");
}

fn test_single_quoted_empty() {
    s := '';
    assert_eq(s.empty(), true);
}

fn test_double_quoted_string_with_single_quotes() {
    s := "can't";
    assert_eq(s.contains("'"), true);
    assert_eq(s.length(), 5);
}

fn test_single_char_single_quoted_is_string() {
    s := 'a';
    assert_eq(s, "a");
    assert_eq(s.length(), 1);
}

fn test_str_at_returns_string() {
    s := "hello";
    c := s.at(0);
    assert_eq(c, "h");
    assert_eq(c.length(), 1);
}

fn test_escape_sequence_newline() {
    s := "hello\nworld";
    assert_eq(s.length(), 11);
    assert_eq(s.contains("\n"), true);
}

fn test_escape_sequence_tab() {
    s := "hello\tworld";
    assert_eq(s.length(), 11);
    assert_eq(s.contains("\t"), true);
}

fn test_escape_sequence_carriage_return() {
    s := "hello\rworld";
    assert_eq(s.length(), 11);
}

fn test_escape_sequence_backslash() {
    s := "path\\to\\file";
    assert_eq(s.length(), 12);
}

fn test_escape_sequence_double_quote_via_single_quoted() {
    s := 'say "hello"';
    dq := '"';
    assert_eq(s.contains(dq), true);
}

fn test_escape_sequence_single_quote_in_double_quoted() {
    s := "it's";
    assert_eq(s.length(), 4);
}

fn test_escape_sequence_backslash_then_newline() {
    // \\ + \n in source = backslash + newline (2 chars in result)
    s := "\\\n";
    assert_eq(s.length(), 2);
}

fn test_escape_sequence_backslash_then_tab() {
    // \\ + \t in source = backslash + tab (2 chars in result)
    s := "\\\t";
    assert_eq(s.length(), 2);
}

fn test_escape_sequence_double_backslash() {
    // \\ + \\ in source = two backslashes (2 chars in result)
    s := "\\\\";
    assert_eq(s.length(), 2);
}

fn test_escape_sequence_multiple_newlines() {
    // Three newlines
    s := "\n\n\n";
    assert_eq(s.length(), 3);
}

fn test_escape_sequence_backslash_followed_by_letter_n() {
    // \\ + n (literal) in source = backslash + letter n (2 chars)
    // This is different from \n which is a newline
    s := "\\n";
    assert_eq(s.length(), 2);
    assert_eq(s.contains("\n"), false);
}

fn test_escape_sequence_backslash_followed_by_letter_t() {
    // \\ + t (literal) in source = backslash + letter t (2 chars)
    s := "\\t";
    assert_eq(s.length(), 2);
    assert_eq(s.contains("\t"), false);
}
