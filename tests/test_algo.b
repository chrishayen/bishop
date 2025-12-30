// ============================================
// Algo Module Tests
// ============================================

import algo;

// ============================================
// Sorting Tests
// ============================================

fn test_sort_int() {
    nums := [3, 1, 4, 1, 5, 9, 2, 6];
    algo.sort_int(nums);
    assert_eq(nums.get(0), 1);
    assert_eq(nums.get(1), 1);
    assert_eq(nums.get(2), 2);
    assert_eq(nums.get(3), 3);
    assert_eq(nums.get(7), 9);
}

fn test_sort_desc_int() {
    nums := [3, 1, 4, 1, 5];
    algo.sort_desc_int(nums);
    assert_eq(nums.get(0), 5);
    assert_eq(nums.get(1), 4);
    assert_eq(nums.get(2), 3);
    assert_eq(nums.get(4), 1);
}

fn test_sort_str() {
    names := ["charlie", "alice", "bob"];
    algo.sort_str(names);
    assert_eq(names.get(0), "alice");
    assert_eq(names.get(1), "bob");
    assert_eq(names.get(2), "charlie");
}

fn test_sort_desc_str() {
    names := ["charlie", "alice", "bob"];
    algo.sort_desc_str(names);
    assert_eq(names.get(0), "charlie");
    assert_eq(names.get(1), "bob");
    assert_eq(names.get(2), "alice");
}

fn test_sort_float() {
    vals := [3.14, 1.41, 2.72];
    algo.sort_float(vals);
    assert_eq(vals.get(0), 1.41);
    assert_eq(vals.get(1), 2.72);
    assert_eq(vals.get(2), 3.14);
}

fn test_sort_desc_float() {
    vals := [3.14, 1.41, 2.72];
    algo.sort_desc_float(vals);
    assert_eq(vals.get(0), 3.14);
    assert_eq(vals.get(1), 2.72);
    assert_eq(vals.get(2), 1.41);
}

fn test_sort_empty_list() {
    nums := List<int>();
    algo.sort_int(nums);
    assert_eq(nums.length(), 0);
}

fn test_sort_single_element() {
    nums := [42];
    algo.sort_int(nums);
    assert_eq(nums.get(0), 42);
}

// ============================================
// Min/Max Tests
// ============================================

fn test_min_int() {
    nums := [3, 1, 4, 1, 5];
    min_val := algo.min_int(nums) or return;
    assert_eq(min_val, 1);
}

fn test_max_int() {
    nums := [3, 1, 4, 1, 5];
    max_val := algo.max_int(nums) or return;
    assert_eq(max_val, 5);
}

fn test_min_float() {
    vals := [3.14, 1.41, 2.72];
    min_val := algo.min_float(vals) or return;
    assert_eq(min_val, 1.41);
}

fn test_max_float() {
    vals := [3.14, 1.41, 2.72];
    max_val := algo.max_float(vals) or return;
    assert_eq(max_val, 3.14);
}

fn test_min_str() {
    names := ["charlie", "alice", "bob"];
    min_name := algo.min_str(names) or return;
    assert_eq(min_name, "alice");
}

fn test_max_str() {
    names := ["charlie", "alice", "bob"];
    max_name := algo.max_str(names) or return;
    assert_eq(max_name, "charlie");
}

fn test_min_single_element() {
    nums := [42];
    min_val := algo.min_int(nums) or return;
    assert_eq(min_val, 42);
}

fn test_max_single_element() {
    nums := [42];
    max_val := algo.max_int(nums) or return;
    assert_eq(max_val, 42);
}

// ============================================
// Aggregation Tests
// ============================================

fn test_sum_int() {
    nums := [1, 2, 3, 4, 5];
    result := algo.sum_int(nums);
    assert_eq(result, 15);
}

fn test_sum_int_empty() {
    nums := List<int>();
    result := algo.sum_int(nums);
    assert_eq(result, 0);
}

fn test_sum_float() {
    vals := [1.5, 2.5, 3.0];
    result := algo.sum_float(vals);
    assert_eq(result, 7.0);
}

fn test_product_int() {
    nums := [2, 3, 4];
    result := algo.product_int(nums);
    assert_eq(result, 24);
}

fn test_product_int_empty() {
    nums := List<int>();
    result := algo.product_int(nums);
    assert_eq(result, 1);
}

fn test_product_float() {
    vals := [2.0, 3.0, 4.0];
    result := algo.product_float(vals);
    assert_eq(result, 24.0);
}

fn test_average_int() {
    nums := [1, 2, 3, 4, 5];
    result := algo.average_int(nums) or return;
    assert_eq(result, 3.0);
}

fn test_average_float() {
    vals := [1.0, 2.0, 3.0];
    result := algo.average_float(vals) or return;
    assert_eq(result, 2.0);
}

// ============================================
// Predicate Tests (with lambdas)
// ============================================

fn test_all_int() {
    nums := [2, 4, 6, 8];
    result := algo.all_int(nums, fn(int x) -> bool { return x > 0; });
    assert_eq(result, true);
}

fn test_all_int_false() {
    nums := [2, 3, 6, 0];
    result := algo.all_int(nums, fn(int x) -> bool { return x > 0; });
    assert_eq(result, false);
}

fn test_all_int_empty() {
    nums := List<int>();
    result := algo.all_int(nums, fn(int x) -> bool { return x > 0; });
    assert_eq(result, true);
}

fn test_any_int() {
    nums := [1, 3, 5, 10];
    result := algo.any_int(nums, fn(int x) -> bool { return x > 5; });
    assert_eq(result, true);
}

fn test_any_int_false() {
    nums := [1, 3, 5, 4];
    result := algo.any_int(nums, fn(int x) -> bool { return x > 10; });
    assert_eq(result, false);
}

fn test_none_int() {
    nums := [1, 3, 5, 7];
    result := algo.none_int(nums, fn(int x) -> bool { return x > 10; });
    assert_eq(result, true);
}

fn test_none_int_false() {
    nums := [1, 3, 100, 7];
    result := algo.none_int(nums, fn(int x) -> bool { return x > 10; });
    assert_eq(result, false);
}

fn test_count_int() {
    nums := [1, 20, 3, 40, 5, 60];
    result := algo.count_int(nums, fn(int x) -> bool { return x > 10; });
    assert_eq(result, 3);
}

fn test_count_int_zero() {
    nums := [1, 3, 5, 7];
    result := algo.count_int(nums, fn(int x) -> bool { return x > 10; });
    assert_eq(result, 0);
}

fn test_find_int() {
    nums := [1, 2, 3, 4, 5];
    result := algo.find_int(nums, fn(int x) -> bool { return x > 3; }) or return;
    assert_eq(result, 4);
}

fn test_find_index_int() {
    nums := [10, 20, 30, 40];
    result := algo.find_index_int(nums, fn(int x) -> bool { return x == 30; });
    assert_eq(result, 2);
}

fn test_find_index_int_not_found() {
    nums := [10, 20, 30, 40];
    result := algo.find_index_int(nums, fn(int x) -> bool { return x == 100; });
    assert_eq(result, -1);
}

fn test_all_str() {
    names := ["a", "ab", "abc"];
    result := algo.all_str(names, fn(str s) -> bool { return s.length() > 0; });
    assert_eq(result, true);
}

fn test_any_str() {
    names := ["a", "hello", "b"];
    result := algo.any_str(names, fn(str s) -> bool { return s.length() > 3; });
    assert_eq(result, true);
}

fn test_none_str() {
    names := ["a", "b", "c"];
    result := algo.none_str(names, fn(str s) -> bool { return s.length() > 1; });
    assert_eq(result, true);
}

fn test_count_str() {
    names := ["a", "abc", "ab", "abcd"];
    result := algo.count_str(names, fn(str s) -> bool { return s.length() > 2; });
    assert_eq(result, 2);
}

fn test_find_str() {
    names := ["a", "bb", "ccc"];
    result := algo.find_str(names, fn(str s) -> bool { return s.length() > 1; }) or return;
    assert_eq(result, "bb");
}

fn test_find_index_str() {
    names := ["a", "bb", "ccc"];
    result := algo.find_index_str(names, fn(str s) -> bool { return s == "ccc"; });
    assert_eq(result, 2);
}

// ============================================
// Transformation Tests
// ============================================

fn test_map_int() {
    nums := [1, 2, 3];
    result := algo.map_int(nums, fn(int x) -> int { return x * 2; });
    assert_eq(result.get(0), 2);
    assert_eq(result.get(1), 4);
    assert_eq(result.get(2), 6);
}

fn test_map_int_empty() {
    nums := List<int>();
    result := algo.map_int(nums, fn(int x) -> int { return x * 2; });
    assert_eq(result.length(), 0);
}

fn test_map_str() {
    names := ["a", "b", "c"];
    result := algo.map_str(names, fn(str s) -> str { return s + s; });
    assert_eq(result.get(0), "aa");
    assert_eq(result.get(1), "bb");
    assert_eq(result.get(2), "cc");
}

fn test_filter_int() {
    nums := [1, 20, 3, 40, 5, 60];
    result := algo.filter_int(nums, fn(int x) -> bool { return x > 10; });
    assert_eq(result.length(), 3);
    assert_eq(result.get(0), 20);
    assert_eq(result.get(1), 40);
    assert_eq(result.get(2), 60);
}

fn test_filter_int_none_match() {
    nums := [1, 3, 5, 7];
    result := algo.filter_int(nums, fn(int x) -> bool { return x > 10; });
    assert_eq(result.length(), 0);
}

fn test_filter_str() {
    names := ["a", "abc", "ab"];
    result := algo.filter_str(names, fn(str s) -> bool { return s.length() > 1; });
    assert_eq(result.length(), 2);
    assert_eq(result.get(0), "abc");
    assert_eq(result.get(1), "ab");
}

fn test_reduce_int() {
    nums := [1, 2, 3, 4, 5];
    result := algo.reduce_int(nums, fn(int a, int b) -> int { return a + b; }, 0);
    assert_eq(result, 15);
}

fn test_reduce_int_with_initial() {
    nums := [1, 2, 3];
    result := algo.reduce_int(nums, fn(int a, int b) -> int { return a + b; }, 10);
    assert_eq(result, 16);
}

fn test_reduce_int_product() {
    nums := [2, 3, 4];
    result := algo.reduce_int(nums, fn(int a, int b) -> int { return a * b; }, 1);
    assert_eq(result, 24);
}

fn test_reduce_str() {
    names := ["a", "b", "c"];
    result := algo.reduce_str(names, fn(str a, str b) -> str { return a + b; }, "");
    assert_eq(result, "abc");
}

// ============================================
// Reordering Tests
// ============================================

fn test_reverse_int() {
    nums := [1, 2, 3, 4, 5];
    algo.reverse_int(nums);
    assert_eq(nums.get(0), 5);
    assert_eq(nums.get(1), 4);
    assert_eq(nums.get(2), 3);
    assert_eq(nums.get(3), 2);
    assert_eq(nums.get(4), 1);
}

fn test_reverse_str() {
    names := ["a", "b", "c"];
    algo.reverse_str(names);
    assert_eq(names.get(0), "c");
    assert_eq(names.get(1), "b");
    assert_eq(names.get(2), "a");
}

fn test_reverse_empty() {
    nums := List<int>();
    algo.reverse_int(nums);
    assert_eq(nums.length(), 0);
}

fn test_reverse_single() {
    nums := [42];
    algo.reverse_int(nums);
    assert_eq(nums.get(0), 42);
}

fn test_reversed_int() {
    nums := [1, 2, 3];
    result := algo.reversed_int(nums);
    assert_eq(result.get(0), 3);
    assert_eq(result.get(1), 2);
    assert_eq(result.get(2), 1);
    // Original unchanged
    assert_eq(nums.get(0), 1);
}

fn test_reversed_str() {
    names := ["a", "b", "c"];
    result := algo.reversed_str(names);
    assert_eq(result.get(0), "c");
    assert_eq(result.get(1), "b");
    assert_eq(result.get(2), "a");
    // Original unchanged
    assert_eq(names.get(0), "a");
}

fn test_rotate_int() {
    nums := [1, 2, 3, 4, 5];
    algo.rotate_int(nums, 2);
    assert_eq(nums.get(0), 3);
    assert_eq(nums.get(1), 4);
    assert_eq(nums.get(2), 5);
    assert_eq(nums.get(3), 1);
    assert_eq(nums.get(4), 2);
}

fn test_rotate_int_negative() {
    nums := [1, 2, 3, 4, 5];
    algo.rotate_int(nums, -1);
    // Negative rotation should wrap correctly
    assert_eq(nums.get(0), 5);
    assert_eq(nums.get(4), 4);
}

fn test_rotate_str() {
    names := ["a", "b", "c", "d"];
    algo.rotate_str(names, 1);
    assert_eq(names.get(0), "b");
    assert_eq(names.get(1), "c");
    assert_eq(names.get(2), "d");
    assert_eq(names.get(3), "a");
}

fn test_rotate_empty() {
    nums := List<int>();
    algo.rotate_int(nums, 5);
    assert_eq(nums.length(), 0);
}

fn test_unique_int() {
    nums := [1, 2, 1, 3, 2, 4, 1];
    result := algo.unique_int(nums);
    assert_eq(result.length(), 4);
    assert_eq(result.get(0), 1);
    assert_eq(result.get(1), 2);
    assert_eq(result.get(2), 3);
    assert_eq(result.get(3), 4);
}

fn test_unique_int_no_duplicates() {
    nums := [1, 2, 3, 4];
    result := algo.unique_int(nums);
    assert_eq(result.length(), 4);
}

fn test_unique_int_all_same() {
    nums := [5, 5, 5, 5];
    result := algo.unique_int(nums);
    assert_eq(result.length(), 1);
    assert_eq(result.get(0), 5);
}

fn test_unique_str() {
    names := ["a", "b", "a", "c", "b"];
    result := algo.unique_str(names);
    assert_eq(result.length(), 3);
    assert_eq(result.get(0), "a");
    assert_eq(result.get(1), "b");
    assert_eq(result.get(2), "c");
}

fn test_unique_empty() {
    nums := List<int>();
    result := algo.unique_int(nums);
    assert_eq(result.length(), 0);
}
