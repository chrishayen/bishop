/// Tests for the random module

import random;

// ============================================================
// Tests for random.int
// ============================================================

fn test_int_returns_value_in_range() {
    i := 0;

    while i < 100 {
        val := random.int(1, 6);
        assert_eq(true, val >= 1);
        assert_eq(true, val <= 6);
        i = i + 1;
    }
}

fn test_int_min_equals_max() {
    val := random.int(5, 5);
    assert_eq(5, val);
}

fn test_int_negative_range() {
    i := 0;

    while i < 100 {
        val := random.int(-10, -5);
        assert_eq(true, val >= -10);
        assert_eq(true, val <= -5);
        i = i + 1;
    }
}

fn test_int_crossing_zero() {
    i := 0;

    while i < 100 {
        val := random.int(-5, 5);
        assert_eq(true, val >= -5);
        assert_eq(true, val <= 5);
        i = i + 1;
    }
}

// ============================================================
// Tests for random.float
// ============================================================

fn test_float_no_args() {
    i := 0;

    while i < 100 {
        val := random.float();
        assert_eq(true, val >= 0.0);
        assert_eq(true, val < 1.0);
        i = i + 1;
    }
}

fn test_float_with_range() {
    i := 0;

    while i < 100 {
        val := random.float_range(-10.0, 40.0);
        assert_eq(true, val >= -10.0);
        assert_eq(true, val < 40.0);
        i = i + 1;
    }
}

fn test_float_negative_range() {
    i := 0;

    while i < 100 {
        val := random.float_range(-100.0, -50.0);
        assert_eq(true, val >= -100.0);
        assert_eq(true, val < -50.0);
        i = i + 1;
    }
}

// ============================================================
// Tests for random.bool
// ============================================================

fn test_bool_no_args() {
    true_count := 0;
    false_count := 0;
    i := 0;

    while i < 1000 {
        val := random.bool();

        if val {
            true_count = true_count + 1;
        } else {
            false_count = false_count + 1;
        }

        i = i + 1;
    }

    // With 1000 samples, should have at least some of each
    assert_eq(true, true_count > 100);
    assert_eq(true, false_count > 100);
}

fn test_bool_with_probability_zero() {
    i := 0;

    while i < 100 {
        val := random.bool_prob(0.0);
        assert_eq(false, val);
        i = i + 1;
    }
}

fn test_bool_with_probability_one() {
    i := 0;

    while i < 100 {
        val := random.bool_prob(1.0);
        assert_eq(true, val);
        i = i + 1;
    }
}

fn test_bool_with_low_probability() {
    true_count := 0;
    i := 0;

    while i < 1000 {
        val := random.bool_prob(0.1);

        if val {
            true_count = true_count + 1;
        }

        i = i + 1;
    }

    // With 10% probability over 1000 samples, expect roughly 100
    // Allow range 20-250 to account for randomness
    assert_eq(true, true_count < 250);
}

// ============================================================
// Tests for random.choice
// ============================================================

fn test_choice_returns_element_from_list() {
    items := ["a", "b", "c"];
    i := 0;

    while i < 100 {
        pick := random.choice(items) or return;
        assert_eq(true, items.contains(pick));
        i = i + 1;
    }
}

fn test_choice_single_element() {
    items := ["only"];
    pick := random.choice(items) or return;
    assert_eq("only", pick);
}

fn test_choice_empty_list_fails() {
    items := List<str>();
    passed := false;

    result := random.choice(items) or {
        passed = true;
        return;
    };

    assert_eq(passed, true);
}

fn test_choice_int_list() {
    items := [10, 20, 30, 40, 50];
    i := 0;

    while i < 100 {
        pick := random.choice_int(items) or return;
        assert_eq(true, items.contains(pick));
        i = i + 1;
    }
}

fn test_choice_int_empty_list_fails() {
    items := List<int>();
    passed := false;

    result := random.choice_int(items) or {
        passed = true;
        return;
    };

    assert_eq(passed, true);
}

// ============================================================
// Tests for random.shuffle
// ============================================================

fn test_shuffle_preserves_length() {
    items := ["a", "b", "c", "d", "e"];
    random.shuffle(items);
    assert_eq(5, items.length());
}

fn test_shuffle_preserves_elements() {
    items := ["a", "b", "c"];
    random.shuffle(items);
    assert_eq(true, items.contains("a"));
    assert_eq(true, items.contains("b"));
    assert_eq(true, items.contains("c"));
}

fn test_shuffle_int_list() {
    items := [1, 2, 3, 4, 5];
    random.shuffle_int(items);
    assert_eq(5, items.length());
    assert_eq(true, items.contains(1));
    assert_eq(true, items.contains(5));
}

// ============================================================
// Tests for random.sample
// ============================================================

fn test_sample_returns_correct_count() {
    items := ["a", "b", "c", "d", "e"];
    sampled := random.sample(items, 3);
    assert_eq(3, sampled.length());
}

fn test_sample_returns_unique_elements() {
    items := ["a", "b", "c", "d", "e"];
    sampled := random.sample(items, 3);

    // Each sampled element should be from original list
    i := 0;

    while i < sampled.length() {
        assert_eq(true, items.contains(sampled.get(i)));
        i = i + 1;
    }
}

fn test_sample_full_list() {
    items := ["a", "b", "c"];
    sampled := random.sample(items, 3);
    assert_eq(3, sampled.length());
}

fn test_sample_int_list() {
    items := [1, 2, 3, 4, 5];
    sampled := random.sample_int(items, 2);
    assert_eq(2, sampled.length());
}

fn test_sample_count_exceeds_list_size() {
    items := ["a", "b", "c"];
    sampled := random.sample(items, 10);
    // Should clamp to list size
    assert_eq(3, sampled.length());
}

fn test_sample_zero_count() {
    items := ["a", "b", "c"];
    sampled := random.sample(items, 0);
    assert_eq(0, sampled.length());
}

fn test_sample_negative_count() {
    items := ["a", "b", "c"];
    sampled := random.sample(items, -5);
    assert_eq(0, sampled.length());
}

fn test_sample_empty_list() {
    items := List<str>();
    sampled := random.sample(items, 3);
    assert_eq(0, sampled.length());
}

fn test_sample_int_count_exceeds_list_size() {
    items := [1, 2, 3];
    sampled := random.sample_int(items, 10);
    assert_eq(3, sampled.length());
}

// ============================================================
// Tests for random.seed
// ============================================================

fn test_seed_produces_deterministic_sequence() {
    random.seed(42);
    first1 := random.int(1, 100);
    second1 := random.int(1, 100);
    third1 := random.int(1, 100);

    random.seed(42);
    first2 := random.int(1, 100);
    second2 := random.int(1, 100);
    third2 := random.int(1, 100);

    assert_eq(first1, first2);
    assert_eq(second1, second2);
    assert_eq(third1, third2);
}

fn test_different_seeds_different_sequences() {
    random.seed(1);
    val1 := random.int(1, 1000000);

    random.seed(2);
    val2 := random.int(1, 1000000);

    // Very unlikely to be the same with different seeds
    assert_eq(true, val1 != val2);
}

fn test_seed_affects_float() {
    random.seed(123);
    f1 := random.float();

    random.seed(123);
    f2 := random.float();

    assert_eq(f1, f2);
}

fn test_seed_affects_bool() {
    random.seed(456);
    results1 := List<bool>();
    i := 0;

    while i < 10 {
        results1.append(random.bool());
        i = i + 1;
    }

    random.seed(456);
    i = 0;

    while i < 10 {
        assert_eq(results1.get(i), random.bool());
        i = i + 1;
    }
}
