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

// Raw string literal tests (r"..." and r'...')

fn test_raw_string_basic() {
    s := r"hello";
    assert_eq(s, "hello");
}

fn test_raw_string_single_quoted() {
    s := r'hello';
    assert_eq(s, "hello");
}

fn test_raw_string_backslash_not_escaped() {
    s := r"hello\nworld";
    assert_eq(s.length(), 12);  // \n is 2 chars, not newline
    assert_eq(s.contains("\\n"), true);
}

fn test_raw_string_backslash_single_quoted() {
    s := r'path\to\file';
    assert_eq(s.length(), 12);
    assert_eq(s.contains("\\"), true);
}

fn test_raw_string_with_backslashes() {
    s := r"path\to\file";
    assert_eq(s.contains("\\"), true);
}

fn test_raw_string_regex_pattern() {
    s := r"\d+\.\d+";
    assert_eq(s.length(), 8);
}

fn test_raw_string_windows_path() {
    s := r"C:\Users\name\file.txt";
    assert_eq(s.contains("\\"), true);
    assert_eq(s.starts_with("C:"), true);
}

fn test_raw_string_empty() {
    s := r"";
    assert_eq(s.empty(), true);
}

fn test_raw_string_concat() {
    a := r"hello\n";
    b := r"world\n";
    c := a + b;
    assert_eq(c, "hello\\nworld\\n");
}
