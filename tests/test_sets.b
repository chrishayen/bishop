// ============================================
// Set Creation
// ============================================

fn test_set_create_empty_int() {
    nums := Set<int>();
    assert_eq(nums.length(), 0);
}

fn test_set_create_empty_str() {
    names := Set<str>();
    assert_eq(names.is_empty(), true);
}

// ============================================
// Set Literals
// ============================================

fn test_set_literal_int() {
    nums := {1, 2, 3};
    assert_eq(nums.length(), 3);
}

fn test_set_literal_str() {
    names := {"a", "b", "c"};
    assert_eq(names.length(), 3);
}

fn test_set_literal_single() {
    nums := {42};
    assert_eq(nums.length(), 1);
}

fn test_set_literal_bool() {
    flags := {true, false};
    assert_eq(flags.length(), 2);
}

fn test_set_literal_duplicates() {
    nums := {1, 2, 2, 3, 3, 3};
    assert_eq(nums.length(), 3);
}

// ============================================
// Set Query Methods
// ============================================

fn test_set_length() {
    nums := {1, 2, 3, 4, 5};
    assert_eq(nums.length(), 5);
}

fn test_set_is_empty_false() {
    nums := {1};
    assert_eq(nums.is_empty(), false);
}

fn test_set_is_empty_true() {
    nums := Set<int>();
    assert_eq(nums.is_empty(), true);
}

fn test_set_contains_true() {
    nums := {1, 2, 3};
    assert_eq(nums.contains(2), true);
}

fn test_set_contains_false() {
    nums := {1, 2, 3};
    assert_eq(nums.contains(5), false);
}

fn test_set_contains_str() {
    names := {"alice", "bob"};
    assert_eq(names.contains("bob"), true);
    assert_eq(names.contains("charlie"), false);
}

// ============================================
// Set Modification Methods
// ============================================

fn test_set_add() {
    nums := {1, 2};
    nums.add(3);
    assert_eq(nums.length(), 3);
    assert_eq(nums.contains(3), true);
}

fn test_set_add_duplicate() {
    nums := {1, 2, 3};
    nums.add(2);
    assert_eq(nums.length(), 3);
}

fn test_set_remove_exists() {
    nums := {1, 2, 3};
    result := nums.remove(2);
    assert_eq(result, true);
    assert_eq(nums.length(), 2);
    assert_eq(nums.contains(2), false);
}

fn test_set_remove_not_exists() {
    nums := {1, 2, 3};
    result := nums.remove(5);
    assert_eq(result, false);
    assert_eq(nums.length(), 3);
}

fn test_set_clear() {
    nums := {1, 2, 3};
    nums.clear();
    assert_eq(nums.length(), 0);
    assert_eq(nums.is_empty(), true);
}

// ============================================
// Set Operations - Union
// ============================================

fn test_set_union() {
    a := {1, 2, 3};
    b := {3, 4, 5};
    c := a.union(b);
    assert_eq(c.length(), 5);
    assert_eq(c.contains(1), true);
    assert_eq(c.contains(5), true);
}

fn test_set_union_disjoint() {
    a := {1, 2};
    b := {3, 4};
    c := a.union(b);
    assert_eq(c.length(), 4);
}

fn test_set_union_identical() {
    a := {1, 2, 3};
    b := {1, 2, 3};
    c := a.union(b);
    assert_eq(c.length(), 3);
}

fn test_set_union_empty() {
    a := {1, 2, 3};
    b := Set<int>();
    c := a.union(b);
    assert_eq(c.length(), 3);
}

// ============================================
// Set Operations - Intersection
// ============================================

fn test_set_intersection() {
    a := {1, 2, 3};
    b := {2, 3, 4};
    c := a.intersection(b);
    assert_eq(c.length(), 2);
    assert_eq(c.contains(2), true);
    assert_eq(c.contains(3), true);
}

fn test_set_intersection_disjoint() {
    a := {1, 2};
    b := {3, 4};
    c := a.intersection(b);
    assert_eq(c.length(), 0);
}

fn test_set_intersection_identical() {
    a := {1, 2, 3};
    b := {1, 2, 3};
    c := a.intersection(b);
    assert_eq(c.length(), 3);
}

fn test_set_intersection_empty() {
    a := {1, 2, 3};
    b := Set<int>();
    c := a.intersection(b);
    assert_eq(c.length(), 0);
}

// ============================================
// Set Operations - Difference
// ============================================

fn test_set_difference() {
    a := {1, 2, 3};
    b := {2, 3, 4};
    c := a.difference(b);
    assert_eq(c.length(), 1);
    assert_eq(c.contains(1), true);
}

fn test_set_difference_disjoint() {
    a := {1, 2};
    b := {3, 4};
    c := a.difference(b);
    assert_eq(c.length(), 2);
}

fn test_set_difference_identical() {
    a := {1, 2, 3};
    b := {1, 2, 3};
    c := a.difference(b);
    assert_eq(c.length(), 0);
}

fn test_set_difference_empty_other() {
    a := {1, 2, 3};
    b := Set<int>();
    c := a.difference(b);
    assert_eq(c.length(), 3);
}

// ============================================
// Set Operations - Symmetric Difference
// ============================================

fn test_set_symmetric_difference() {
    a := {1, 2, 3};
    b := {2, 3, 4};
    c := a.symmetric_difference(b);
    assert_eq(c.length(), 2);
    assert_eq(c.contains(1), true);
    assert_eq(c.contains(4), true);
    assert_eq(c.contains(2), false);
}

fn test_set_symmetric_difference_disjoint() {
    a := {1, 2};
    b := {3, 4};
    c := a.symmetric_difference(b);
    assert_eq(c.length(), 4);
}

fn test_set_symmetric_difference_identical() {
    a := {1, 2, 3};
    b := {1, 2, 3};
    c := a.symmetric_difference(b);
    assert_eq(c.length(), 0);
}

// ============================================
// Set Predicates - Subset/Superset
// ============================================

fn test_set_is_subset_true() {
    a := {1, 2};
    b := {1, 2, 3};
    assert_eq(a.is_subset(b), true);
}

fn test_set_is_subset_false() {
    a := {1, 2, 4};
    b := {1, 2, 3};
    assert_eq(a.is_subset(b), false);
}

fn test_set_is_subset_equal() {
    a := {1, 2, 3};
    b := {1, 2, 3};
    assert_eq(a.is_subset(b), true);
}

fn test_set_is_subset_empty() {
    a := Set<int>();
    b := {1, 2, 3};
    assert_eq(a.is_subset(b), true);
}

fn test_set_is_superset_true() {
    a := {1, 2, 3};
    b := {1, 2};
    assert_eq(a.is_superset(b), true);
}

fn test_set_is_superset_false() {
    a := {1, 2, 3};
    b := {1, 2, 4};
    assert_eq(a.is_superset(b), false);
}

fn test_set_is_superset_equal() {
    a := {1, 2, 3};
    b := {1, 2, 3};
    assert_eq(a.is_superset(b), true);
}

fn test_set_is_superset_of_empty() {
    a := {1, 2, 3};
    b := Set<int>();
    assert_eq(a.is_superset(b), true);
}

// ============================================
// Set Iteration
// ============================================

fn test_set_iteration() {
    nums := {1, 2, 3};
    count := 0;

    for n in nums {
        count = count + 1;
    }

    assert_eq(count, 3);
}

fn test_set_iteration_empty() {
    nums := Set<int>();
    count := 0;

    for n in nums {
        count = count + 1;
    }

    assert_eq(count, 0);
}

fn test_set_iteration_str() {
    names := {"alice", "bob", "charlie"};
    count := 0;

    for name in names {
        count = count + 1;
    }

    assert_eq(count, 3);
}

// ============================================
// Typed Variable Declaration
// ============================================

fn test_set_typed_decl() {
    Set<int> nums = {1, 2, 3};
    assert_eq(nums.length(), 3);
}

fn test_set_typed_empty() {
    Set<str> names = Set<str>();
    assert_eq(names.is_empty(), true);
}

// ============================================
// Set with Structs
// ============================================

Point :: struct {
    x int,
    y int
}

fn test_set_contains_after_add() {
    nums := Set<int>();
    nums.add(10);
    nums.add(20);
    nums.add(30);
    assert_eq(nums.contains(20), true);
    assert_eq(nums.contains(40), false);
}

// ============================================
// Chained Operations
// ============================================

fn test_set_chained_add() {
    nums := Set<int>();
    nums.add(1);
    nums.add(2);
    nums.add(3);
    assert_eq(nums.length(), 3);
    assert_eq(nums.contains(1), true);
    assert_eq(nums.contains(3), true);
}

fn test_set_chained_operations() {
    a := {1, 2, 3, 4, 5};
    b := {4, 5, 6, 7};
    common := a.intersection(b);
    assert_eq(common.length(), 2);
    assert_eq(common.contains(4), true);
    assert_eq(common.contains(5), true);
}
