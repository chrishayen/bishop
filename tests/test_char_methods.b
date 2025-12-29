// ============================================
// Char Type Checking Methods
// ============================================

fn test_char_is_alpha_upper() {
    c := 'A';
    assert_eq(c.is_alpha(), true);
}

fn test_char_is_alpha_lower() {
    c := 'z';
    assert_eq(c.is_alpha(), true);
}

fn test_char_is_alpha_digit() {
    c := '5';
    assert_eq(c.is_alpha(), false);
}

fn test_char_is_alpha_symbol() {
    c := '@';
    assert_eq(c.is_alpha(), false);
}

fn test_char_is_digit_true() {
    c := '7';
    assert_eq(c.is_digit(), true);
}

fn test_char_is_digit_false() {
    c := 'x';
    assert_eq(c.is_digit(), false);
}

fn test_char_is_digit_zero() {
    c := '0';
    assert_eq(c.is_digit(), true);
}

fn test_char_is_digit_nine() {
    c := '9';
    assert_eq(c.is_digit(), true);
}

fn test_char_is_whitespace_space() {
    c := ' ';
    assert_eq(c.is_whitespace(), true);
}

fn test_char_is_whitespace_tab() {
    c := '\t';
    assert_eq(c.is_whitespace(), true);
}

fn test_char_is_whitespace_newline() {
    c := '\n';
    assert_eq(c.is_whitespace(), true);
}

fn test_char_is_whitespace_letter() {
    c := 'a';
    assert_eq(c.is_whitespace(), false);
}

// ============================================
// Char Case Checking Methods
// ============================================

fn test_char_is_upper_true() {
    c := 'A';
    assert_eq(c.is_upper(), true);
}

fn test_char_is_upper_false() {
    c := 'a';
    assert_eq(c.is_upper(), false);
}

fn test_char_is_upper_digit() {
    c := '5';
    assert_eq(c.is_upper(), false);
}

fn test_char_is_lower_true() {
    c := 'z';
    assert_eq(c.is_lower(), true);
}

fn test_char_is_lower_false() {
    c := 'Z';
    assert_eq(c.is_lower(), false);
}

fn test_char_is_lower_digit() {
    c := '3';
    assert_eq(c.is_lower(), false);
}

// ============================================
// Char Case Conversion Methods
// ============================================

fn test_char_to_upper_lower() {
    c := 'a';
    assert_eq(c.to_upper(), 'A');
}

fn test_char_to_upper_already_upper() {
    c := 'A';
    assert_eq(c.to_upper(), 'A');
}

fn test_char_to_upper_digit() {
    c := '5';
    assert_eq(c.to_upper(), '5');
}

fn test_char_to_lower_upper() {
    c := 'A';
    assert_eq(c.to_lower(), 'a');
}

fn test_char_to_lower_already_lower() {
    c := 'a';
    assert_eq(c.to_lower(), 'a');
}

fn test_char_to_lower_digit() {
    c := '5';
    assert_eq(c.to_lower(), '5');
}
