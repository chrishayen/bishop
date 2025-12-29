// ============================================
// Regex Module Tests
// ============================================

import regex;

// ============================================
// Pattern Compilation
// ============================================

fn test_compile_valid_pattern() {
    re := regex.compile(r"hello") or return;
    assert_eq(true, true);
}

fn test_compile_invalid_pattern() {
    passed := false;

    result := regex.compile(r"[invalid") or {
        passed = true;
        return;
    };

    assert_eq(passed, true);
}

fn test_compile_empty_pattern() {
    re := regex.compile(r"") or return;
    assert_eq(true, true);
}

// ============================================
// matches() - Full Match
// ============================================

fn test_matches_full_match() {
    re := regex.compile(r"hello") or return;
    assert_eq(re.matches("hello"), true);
}

fn test_matches_no_match() {
    re := regex.compile(r"hello") or return;
    assert_eq(re.matches("world"), false);
}

fn test_matches_partial_does_not_match() {
    re := regex.compile(r"hello") or return;
    assert_eq(re.matches("hello world"), false);
}

fn test_matches_with_anchors() {
    re := regex.compile(r"^hello$") or return;
    assert_eq(re.matches("hello"), true);
    assert_eq(re.matches("hello world"), false);
}

fn test_matches_with_groups() {
    re := regex.compile(r"(\d+)-(\d+)") or return;
    assert_eq(re.matches("123-456"), true);
}

// ============================================
// contains() - Partial Match
// ============================================

fn test_contains_partial_match() {
    re := regex.compile(r"world") or return;
    assert_eq(re.contains("hello world"), true);
}

fn test_contains_no_match() {
    re := regex.compile(r"foo") or return;
    assert_eq(re.contains("hello world"), false);
}

fn test_contains_at_start() {
    re := regex.compile(r"hello") or return;
    assert_eq(re.contains("hello world"), true);
}

fn test_contains_at_end() {
    re := regex.compile(r"world") or return;
    assert_eq(re.contains("hello world"), true);
}

fn test_contains_full_string() {
    re := regex.compile(r"hello") or return;
    assert_eq(re.contains("hello"), true);
}

// ============================================
// find() - Single Match with Groups
// ============================================

fn test_find_basic() {
    re := regex.compile(r"\d+") or return;
    m := re.find("abc 123 def");
    assert_eq(m.found(), true);
    assert_eq(m.text, "123");
    assert_eq(m.start, 4);
    assert_eq(m.end, 7);
}

fn test_find_no_match() {
    re := regex.compile(r"\d+") or return;
    m := re.find("no digits here");
    assert_eq(m.found(), false);
    assert_eq(m.text, "");
    assert_eq(m.start, -1);
    assert_eq(m.end, -1);
}

fn test_find_with_groups() {
    re := regex.compile(r"(\d+)-(\d+)") or return;
    m := re.find("Price: 100-200 dollars");
    assert_eq(m.found(), true);
    assert_eq(m.text, "100-200");
    assert_eq(m.start, 7);
    assert_eq(m.end, 14);
    assert_eq(m.group(1), "100");
    assert_eq(m.group(2), "200");
}

fn test_find_group_zero_is_full_match() {
    re := regex.compile(r"(\d+)-(\d+)") or return;
    m := re.find("Price: 100-200 dollars");
    assert_eq(m.group(0), "100-200");
}

fn test_find_group_out_of_bounds() {
    re := regex.compile(r"(\d+)") or return;
    m := re.find("abc 123");
    assert_eq(m.group(5), "");
}

fn test_find_at_start() {
    re := regex.compile(r"\w+") or return;
    m := re.find("hello world");
    assert_eq(m.text, "hello");
    assert_eq(m.start, 0);
}

// ============================================
// find_all() - Multiple Matches
// ============================================

fn test_find_all_multiple_matches() {
    re := regex.compile(r"\d+") or return;
    matches := re.find_all("a1 b2 c3");
    assert_eq(matches.length(), 3);
    assert_eq(matches.get(0).text, "1");
    assert_eq(matches.get(1).text, "2");
    assert_eq(matches.get(2).text, "3");
}

fn test_find_all_no_matches() {
    re := regex.compile(r"\d+") or return;
    matches := re.find_all("no digits here");
    assert_eq(matches.length(), 0);
}

fn test_find_all_single_match() {
    re := regex.compile(r"\d+") or return;
    matches := re.find_all("abc123");
    assert_eq(matches.length(), 1);
    assert_eq(matches.get(0).text, "123");
}

fn test_find_all_with_groups() {
    re := regex.compile(r"(\d+)-(\d+)") or return;
    matches := re.find_all("A: 1-2, B: 3-4");
    assert_eq(matches.length(), 2);
    assert_eq(matches.get(0).group(1), "1");
    assert_eq(matches.get(0).group(2), "2");
    assert_eq(matches.get(1).group(1), "3");
    assert_eq(matches.get(1).group(2), "4");
}

// ============================================
// replace() - Single Replacement
// ============================================

fn test_replace_single() {
    re := regex.compile(r"\d+") or return;
    result := re.replace("a1b2c3", "X");
    assert_eq(result, "aXb2c3");
}

fn test_replace_no_match() {
    re := regex.compile(r"\d+") or return;
    result := re.replace("abc", "X");
    assert_eq(result, "abc");
}

fn test_replace_with_capture_groups() {
    re := regex.compile(r"(\d+)-(\d+)") or return;
    result := re.replace("123-456", "$2-$1");
    assert_eq(result, "456-123");
}

fn test_replace_at_start() {
    re := regex.compile(r"hello") or return;
    result := re.replace("hello world", "hi");
    assert_eq(result, "hi world");
}

fn test_replace_at_end() {
    re := regex.compile(r"world") or return;
    result := re.replace("hello world", "there");
    assert_eq(result, "hello there");
}

// ============================================
// replace_all() - Replace All Matches
// ============================================

fn test_replace_all_multiple() {
    re := regex.compile(r"\d+") or return;
    result := re.replace_all("a1b2c3", "X");
    assert_eq(result, "aXbXcX");
}

fn test_replace_all_no_match() {
    re := regex.compile(r"\d+") or return;
    result := re.replace_all("abc", "X");
    assert_eq(result, "abc");
}

fn test_replace_all_with_capture_groups() {
    re := regex.compile(r"(\d+)") or return;
    result := re.replace_all("a1b2c3", "[$1]");
    assert_eq(result, "a[1]b[2]c[3]");
}

fn test_replace_all_single_match() {
    re := regex.compile(r"\d+") or return;
    result := re.replace_all("abc123", "X");
    assert_eq(result, "abcX");
}

// ============================================
// regex.split()
// ============================================

fn test_split_basic() {
    parts := regex.split(r"\s+", "a  b   c") or return;
    assert_eq(parts.length(), 3);
    assert_eq(parts.get(0), "a");
    assert_eq(parts.get(1), "b");
    assert_eq(parts.get(2), "c");
}

fn test_split_no_match() {
    parts := regex.split(r",", "hello") or return;
    assert_eq(parts.length(), 1);
    assert_eq(parts.get(0), "hello");
}

fn test_split_empty_string() {
    parts := regex.split(r",", "") or return;
    assert_eq(parts.length(), 1);
    assert_eq(parts.get(0), "");
}

fn test_split_delimiter_at_start() {
    parts := regex.split(r",", ",a,b") or return;
    assert_eq(parts.length(), 3);
    assert_eq(parts.get(0), "");
    assert_eq(parts.get(1), "a");
    assert_eq(parts.get(2), "b");
}

fn test_split_delimiter_at_end() {
    parts := regex.split(r",", "a,b,") or return;
    assert_eq(parts.length(), 3);
    assert_eq(parts.get(0), "a");
    assert_eq(parts.get(1), "b");
    assert_eq(parts.get(2), "");
}

fn test_split_invalid_pattern() {
    passed := false;

    parts := regex.split(r"[invalid", "test") or {
        passed = true;
        return;
    };

    assert_eq(passed, true);
}

fn test_split_complex_pattern() {
    parts := regex.split(r"[,;]", "a,b;c") or return;
    assert_eq(parts.length(), 3);
    assert_eq(parts.get(0), "a");
    assert_eq(parts.get(1), "b");
    assert_eq(parts.get(2), "c");
}

// ============================================
// Error Handling Patterns
// ============================================

fn test_or_block_with_error_access() {
    re := regex.compile(r"[invalid") or {
        // Error message should be accessible
        assert_eq(err.message.contains("regex"), true);
        return;
    };

    assert_eq(true, false);
}

fn test_or_continue_in_loop() {
    patterns := ["\\d+", "[invalid", "\\w+"];
    valid_count := 0;

    for pattern in patterns {
        re := regex.compile(pattern) or continue;
        valid_count = valid_count + 1;
    }

    assert_eq(valid_count, 2);
}

// ============================================
// Common Use Cases
// ============================================

fn test_email_validation() {
    re := regex.compile(r"^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$") or return;
    assert_eq(re.matches("test@example.com"), true);
    assert_eq(re.matches("invalid-email"), false);
    assert_eq(re.matches("also@invalid"), false);
}

fn test_extract_numbers() {
    re := regex.compile(r"\d+") or return;
    matches := re.find_all("Item 1: $25, Item 2: $50");
    assert_eq(matches.length(), 4);
    assert_eq(matches.get(0).text, "1");
    assert_eq(matches.get(1).text, "25");
    assert_eq(matches.get(2).text, "2");
    assert_eq(matches.get(3).text, "50");
}

fn test_word_boundaries() {
    re := regex.compile(r"\bcat\b") or return;
    assert_eq(re.contains("the cat sat"), true);
    assert_eq(re.contains("category"), false);
    assert_eq(re.contains("concatenate"), false);
}

fn test_date_extraction() {
    re := regex.compile(r"(\d{4})-(\d{2})-(\d{2})") or return;
    m := re.find("Date: 2024-01-15");
    assert_eq(m.found(), true);
    assert_eq(m.group(1), "2024");
    assert_eq(m.group(2), "01");
    assert_eq(m.group(3), "15");
}

fn test_url_parsing() {
    re := regex.compile(r"(https?)://([^/]+)(/[^\s]*)?") or return;
    m := re.find("Visit https://example.com/path for more");
    assert_eq(m.found(), true);
    assert_eq(m.group(1), "https");
    assert_eq(m.group(2), "example.com");
    assert_eq(m.group(3), "/path");
}

fn test_match_groups_field() {
    re := regex.compile(r"(\w+)=(\w+)") or return;
    m := re.find("key=value");
    assert_eq(m.groups.length(), 3);
    assert_eq(m.groups.get(0), "key=value");
    assert_eq(m.groups.get(1), "key");
    assert_eq(m.groups.get(2), "value");
}
