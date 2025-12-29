// ============================================
// Case Transformation Methods
// ============================================

fn test_str_upper() {
    s := "hello world";
    assert_eq(s.upper(), "HELLO WORLD");
}

fn test_str_upper_mixed() {
    s := "Hello World";
    assert_eq(s.upper(), "HELLO WORLD");
}

fn test_str_upper_empty() {
    s := "";
    assert_eq(s.upper(), "");
}

fn test_str_lower() {
    s := "HELLO WORLD";
    assert_eq(s.lower(), "hello world");
}

fn test_str_lower_mixed() {
    s := "Hello World";
    assert_eq(s.lower(), "hello world");
}

fn test_str_lower_empty() {
    s := "";
    assert_eq(s.lower(), "");
}

fn test_str_capitalize() {
    s := "hello world";
    assert_eq(s.capitalize(), "Hello world");
}

fn test_str_capitalize_already_upper() {
    s := "HELLO WORLD";
    assert_eq(s.capitalize(), "Hello world");
}

fn test_str_capitalize_empty() {
    s := "";
    assert_eq(s.capitalize(), "");
}

fn test_str_capitalize_single() {
    s := "h";
    assert_eq(s.capitalize(), "H");
}

fn test_str_title() {
    s := "hello world";
    assert_eq(s.title(), "Hello World");
}

fn test_str_title_multiple_spaces() {
    s := "hello  world";
    assert_eq(s.title(), "Hello  World");
}

fn test_str_title_empty() {
    s := "";
    assert_eq(s.title(), "");
}

fn test_str_title_already_title() {
    s := "Hello World";
    assert_eq(s.title(), "Hello World");
}

// ============================================
// Trimming Methods
// ============================================

fn test_str_trim() {
    s := "  hello  ";
    assert_eq(s.trim(), "hello");
}

fn test_str_trim_tabs() {
    s := "\thello\t";
    assert_eq(s.trim(), "hello");
}

fn test_str_trim_no_whitespace() {
    s := "hello";
    assert_eq(s.trim(), "hello");
}

fn test_str_trim_empty() {
    s := "";
    assert_eq(s.trim(), "");
}

fn test_str_trim_all_whitespace() {
    s := "   ";
    assert_eq(s.trim(), "");
}

fn test_str_trim_left() {
    s := "  hello  ";
    assert_eq(s.trim_left(), "hello  ");
}

fn test_str_trim_left_no_left_space() {
    s := "hello  ";
    assert_eq(s.trim_left(), "hello  ");
}

fn test_str_trim_right() {
    s := "  hello  ";
    assert_eq(s.trim_right(), "  hello");
}

fn test_str_trim_right_no_right_space() {
    s := "  hello";
    assert_eq(s.trim_right(), "  hello");
}

// ============================================
// Replace Methods
// ============================================

fn test_str_replace() {
    s := "hello world world";
    assert_eq(s.replace("world", "there"), "hello there world");
}

fn test_str_replace_not_found() {
    s := "hello world";
    assert_eq(s.replace("foo", "bar"), "hello world");
}

fn test_str_replace_empty_needle() {
    s := "hello";
    assert_eq(s.replace("", "x"), "hello");
}

fn test_str_replace_all() {
    s := "hello world world";
    assert_eq(s.replace_all("world", "there"), "hello there there");
}

fn test_str_replace_all_not_found() {
    s := "hello world";
    assert_eq(s.replace_all("foo", "bar"), "hello world");
}

fn test_str_replace_all_overlapping() {
    s := "aaa";
    assert_eq(s.replace_all("aa", "b"), "ba");
}

// ============================================
// Reverse and Repeat Methods
// ============================================

fn test_str_reverse() {
    s := "hello";
    assert_eq(s.reverse(), "olleh");
}

fn test_str_reverse_empty() {
    s := "";
    assert_eq(s.reverse(), "");
}

fn test_str_reverse_single() {
    s := "a";
    assert_eq(s.reverse(), "a");
}

fn test_str_reverse_palindrome() {
    s := "racecar";
    assert_eq(s.reverse(), "racecar");
}

fn test_str_repeat() {
    s := "ab";
    assert_eq(s.repeat(3), "ababab");
}

fn test_str_repeat_zero() {
    s := "hello";
    assert_eq(s.repeat(0), "");
}

fn test_str_repeat_one() {
    s := "hello";
    assert_eq(s.repeat(1), "hello");
}

fn test_str_repeat_empty() {
    s := "";
    assert_eq(s.repeat(5), "");
}

fn test_str_repeat_variable() {
    s := "x";
    n := 4;
    assert_eq(s.repeat(n), "xxxx");
}

// ============================================
// Split Methods
// ============================================

fn test_str_split() {
    s := "hello world foo";
    parts := s.split(" ");
    assert_eq(parts.length(), 3);
    assert_eq(parts.get(0), "hello");
    assert_eq(parts.get(1), "world");
    assert_eq(parts.get(2), "foo");
}

fn test_str_split_no_delimiter() {
    s := "hello";
    parts := s.split(" ");
    assert_eq(parts.length(), 1);
    assert_eq(parts.get(0), "hello");
}

fn test_str_split_empty_string() {
    s := "";
    parts := s.split(" ");
    assert_eq(parts.length(), 1);
    assert_eq(parts.get(0), "");
}

fn test_str_split_multiple_chars() {
    s := "hello::world::foo";
    parts := s.split("::");
    assert_eq(parts.length(), 3);
    assert_eq(parts.get(0), "hello");
    assert_eq(parts.get(1), "world");
    assert_eq(parts.get(2), "foo");
}

fn test_str_split_lines() {
    s := "line1\nline2\nline3";
    lines := s.split_lines();
    assert_eq(lines.length(), 3);
    assert_eq(lines.get(0), "line1");
    assert_eq(lines.get(1), "line2");
    assert_eq(lines.get(2), "line3");
}

fn test_str_split_lines_empty() {
    s := "";
    lines := s.split_lines();
    assert_eq(lines.length(), 1);
    assert_eq(lines.get(0), "");
}

fn test_str_split_lines_single() {
    s := "single line";
    lines := s.split_lines();
    assert_eq(lines.length(), 1);
    assert_eq(lines.get(0), "single line");
}

// ============================================
// Padding Methods
// ============================================

fn test_str_pad_left() {
    s := "hi";
    assert_eq(s.pad_left(5), "   hi");
}

fn test_str_pad_left_already_long() {
    s := "hello";
    assert_eq(s.pad_left(3), "hello");
}

fn test_str_pad_left_exact() {
    s := "hi";
    assert_eq(s.pad_left(2), "hi");
}

fn test_str_pad_left_fill() {
    s := "hi";
    assert_eq(s.pad_left(5, "0"), "000hi");
}

fn test_str_pad_right() {
    s := "hi";
    assert_eq(s.pad_right(5), "hi   ");
}

fn test_str_pad_right_already_long() {
    s := "hello";
    assert_eq(s.pad_right(3), "hello");
}

fn test_str_pad_right_fill() {
    s := "hi";
    assert_eq(s.pad_right(5, "."), "hi...");
}

fn test_str_center() {
    s := "hi";
    assert_eq(s.center(6), "  hi  ");
}

fn test_str_center_odd() {
    s := "hi";
    assert_eq(s.center(5), " hi  ");
}

fn test_str_center_already_long() {
    s := "hello";
    assert_eq(s.center(3), "hello");
}

fn test_str_center_fill() {
    s := "hi";
    assert_eq(s.center(6, "-"), "--hi--");
}

// ============================================
// Conversion Methods
// ============================================

fn test_str_to_int() {
    s := "42";
    val := s.to_int();
    assert_eq(val, 42);
}

fn test_str_to_int_negative() {
    s := "-123";
    val := s.to_int();
    assert_eq(val, -123);
}

fn test_str_to_int_zero() {
    s := "0";
    val := s.to_int();
    assert_eq(val, 0);
}

fn test_str_to_float() {
    s := "3.14";
    val := s.to_float();
    assert_eq(val, 3.14);
}

fn test_str_to_float_negative() {
    s := "-2.5";
    val := s.to_float();
    assert_eq(val, -2.5);
}

fn test_str_to_float_integer() {
    s := "42";
    val := s.to_float();
    assert_eq(val, 42.0);
}
