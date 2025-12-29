/// Tests for or continue and or break error handling in loops

// ============================================================
// Error Definitions
// ============================================================

LoopError :: err;

// ============================================================
// Helper Functions
// ============================================================

fn might_fail(int value) -> int or err {
    if value < 0 {
        fail LoopError { message: "negative value not allowed" };
    }

    return value * 2;
}

fn always_fails() -> int or err {
    fail "always fails";
}

fn returns_value(int x) -> int or err {
    return x;
}

// ============================================================
// Test: Standalone continue and break
// ============================================================

fn test_standalone_continue() {
    sum := 0;

    for i in 0..5 {
        if i == 2 {
            continue;
        }

        sum = sum + i;
    }

    assert_eq(sum, 8);
}

fn test_standalone_break() {
    sum := 0;

    for i in 0..10 {
        if i == 3 {
            break;
        }

        sum = sum + i;
    }

    assert_eq(sum, 3);
}

fn test_continue_in_while() {
    i := 0;
    sum := 0;

    while i < 5 {
        i = i + 1;

        if i == 3 {
            continue;
        }

        sum = sum + i;
    }

    assert_eq(sum, 12);
}

fn test_break_in_while() {
    i := 0;
    sum := 0;

    while i < 10 {
        i = i + 1;

        if i == 4 {
            break;
        }

        sum = sum + i;
    }

    assert_eq(sum, 6);
}

// ============================================================
// Test: or continue
// ============================================================

fn test_or_continue_basic() {
    sum := 0;
    // Test: skip iterations where might_fail fails
    // values at indices 0, 2 will cause failure (via subtraction)
    // values at indices 1, 3, 4 succeed: might_fail(2)=4, might_fail(4)=8, might_fail(5)=10
    // Expected sum: 4 + 8 + 10 = 22
    for i in 0..5 {
        // i=0: might_fail(0-1) = might_fail(-1) -> fail, continue
        // i=1: might_fail(2) -> 4
        // i=2: might_fail(2-3) = might_fail(-1) -> fail, continue
        // i=3: might_fail(4) -> 8
        // i=4: might_fail(5) -> 10
        v := 0;

        if i == 0 {
            v = 0 - 1;
        }

        if i == 1 {
            v = 2;
        }

        if i == 2 {
            v = 0 - 3;
        }

        if i == 3 {
            v = 4;
        }

        if i == 4 {
            v = 5;
        }

        result := might_fail(v) or continue;
        sum = sum + result;
    }

    assert_eq(sum, 22);
}

fn test_or_continue_in_for_range() {
    count := 0;
    sum := 0;

    for i in 0..5 {
        val := might_fail(i - 1) or continue;
        count = count + 1;
        sum = sum + val;
    }

    assert_eq(count, 4);
    assert_eq(sum, 12);
}

fn test_or_continue_all_succeed() {
    count := 0;

    for i in 0..3 {
        val := might_fail(i) or continue;
        count = count + 1;
    }

    assert_eq(count, 3);
}

fn test_or_continue_all_fail() {
    count := 0;

    for i in 0..3 {
        val := always_fails() or continue;
        count = count + 1;
    }

    assert_eq(count, 0);
}

fn test_or_continue_in_while() {
    i := 0;
    sum := 0;

    while i < 5 {
        i = i + 1;
        result := might_fail(i - 2) or continue;
        sum = sum + result;
    }

    assert_eq(sum, 12);
}

// ============================================================
// Test: or break
// ============================================================

fn test_or_break_basic() {
    sum := 0;
    // Test: break when might_fail fails
    // values: 1, 2, 3, then -1 (fail and break), 5 never reached
    // might_fail(1)=2, might_fail(2)=4, might_fail(3)=6
    // Expected sum: 2 + 4 + 6 = 12
    for i in 0..5 {
        v := 0;

        if i == 0 {
            v = 1;
        }

        if i == 1 {
            v = 2;
        }

        if i == 2 {
            v = 3;
        }

        if i == 3 {
            v = 0 - 1;
        }

        if i == 4 {
            v = 5;
        }

        result := might_fail(v) or break;
        sum = sum + result;
    }

    assert_eq(sum, 12);
}

fn test_or_break_in_for_range() {
    sum := 0;

    for i in 0..10 {
        val := might_fail(3 - i) or break;
        sum = sum + val;
    }

    assert_eq(sum, 12);
}

fn test_or_break_first_fails() {
    count := 0;

    for i in 0..5 {
        val := always_fails() or break;
        count = count + 1;
    }

    assert_eq(count, 0);
}

fn test_or_break_none_fail() {
    count := 0;

    for i in 0..3 {
        val := returns_value(i) or break;
        count = count + 1;
    }

    assert_eq(count, 3);
}

fn test_or_break_in_while() {
    i := 0;
    sum := 0;

    while i < 10 {
        i = i + 1;
        result := might_fail(3 - i) or break;
        sum = sum + result;
    }

    assert_eq(sum, 6);
}

// ============================================================
// Test: Chained or handlers in loops
// ============================================================

fn test_chained_or_continue() {
    sum := 0;
    // i=0: might_fail(-1) fails, continue
    // i=1: might_fail(0)=0, might_fail(1)=2, sum += 0+2 = 2
    // i=2: might_fail(1)=2, might_fail(2)=4, sum += 2+4 = 6 -> total 8
    // i=3: might_fail(2)=4, might_fail(3)=6, sum += 4+6 = 10 -> total 18
    // i=4: might_fail(3)=6, might_fail(4)=8, sum += 6+8 = 14 -> total 32
    for i in 0..5 {
        a := might_fail(i - 1) or continue;
        b := might_fail(i) or continue;
        sum = sum + a + b;
    }

    assert_eq(sum, 32);
}

fn test_mixed_or_continue_and_break() {
    count := 0;
    sum := 0;

    for i in 0..10 {
        val := might_fail(i - 2) or continue;
        count = count + 1;

        if count > 3 {
            break;
        }

        sum = sum + val;
    }

    assert_eq(count, 4);
    assert_eq(sum, 6);
}

// ============================================================
// Test: or continue/break with struct values
// ============================================================

Result :: struct {
    value int,
    status str
}

fn get_result(int x) -> Result or err {
    if x < 0 {
        fail "negative not allowed";
    }

    return Result { value: x * 10, status: "ok" };
}

fn test_or_continue_with_struct() {
    sum := 0;
    // Test: skip iterations where get_result fails
    // values: -1 (fail), 2 (success), -3 (fail), 4 (success)
    // get_result(2).value = 20, get_result(4).value = 40
    // Expected sum: 20 + 40 = 60
    for i in 0..4 {
        v := 0;

        if i == 0 {
            v = 0 - 1;
        }

        if i == 1 {
            v = 2;
        }

        if i == 2 {
            v = 0 - 3;
        }

        if i == 3 {
            v = 4;
        }

        res := get_result(v) or continue;
        sum = sum + res.value;
    }

    assert_eq(sum, 60);
}

fn test_or_break_with_struct() {
    sum := 0;
    // Test: break when get_result fails
    // values: 1 (success), 2 (success), -3 (fail, break), 4 (never reached)
    // get_result(1).value = 10, get_result(2).value = 20
    // Expected sum: 10 + 20 = 30
    for i in 0..4 {
        v := 0;

        if i == 0 {
            v = 1;
        }

        if i == 1 {
            v = 2;
        }

        if i == 2 {
            v = 0 - 3;
        }

        if i == 3 {
            v = 4;
        }

        res := get_result(v) or break;
        sum = sum + res.value;
    }

    assert_eq(sum, 30);
}

// ============================================================
// Test: Nested loops with or continue/break
// ============================================================

fn test_or_continue_inner_loop() {
    sum := 0;

    for i in 0..3 {
        for j in 0..5 {
            val := might_fail(j - 1) or continue;
            sum = sum + val;
        }
    }

    assert_eq(sum, 36);
}

fn test_or_break_inner_loop() {
    count := 0;
    // For each outer loop iteration (i=0,1,2):
    //   j=0: might_fail(2-0)=might_fail(2)=4, count++
    //   j=1: might_fail(2-1)=might_fail(1)=2, count++
    //   j=2: might_fail(2-2)=might_fail(0)=0, count++
    //   j=3: might_fail(2-3)=might_fail(-1) -> fail, break
    // Each outer iteration counts 3, total = 3 * 3 = 9
    for i in 0..3 {
        for j in 0..5 {
            val := might_fail(2 - j) or break;
            count = count + 1;
        }
    }

    assert_eq(count, 9);
}
