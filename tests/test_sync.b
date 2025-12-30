// Tests for the sync module

import sync;

// ============================================
// Mutex Tests
// ============================================

fn test_mutex_create() {
    mtx := sync.mutex_create();
}

fn test_mutex_lock_unlock() {
    mtx := sync.mutex_create();
    sync.mutex_lock(mtx);
    sync.mutex_unlock(mtx);
}

fn test_mutex_try_lock() {
    mtx := sync.mutex_create();

    // First try_lock should succeed
    result := sync.mutex_try_lock(mtx);
    assert_eq(true, result);

    // Unlock for cleanup
    sync.mutex_unlock(mtx);
}

fn test_mutex_relock() {
    mtx := sync.mutex_create();

    // Lock, unlock, lock again
    sync.mutex_lock(mtx);
    sync.mutex_unlock(mtx);
    sync.mutex_lock(mtx);
    sync.mutex_unlock(mtx);
}

// ============================================
// WaitGroup Tests
// ============================================

fn test_waitgroup_create() {
    wg := sync.waitgroup_create();
}

fn test_waitgroup_add_done() {
    wg := sync.waitgroup_create();
    sync.waitgroup_add(wg, 1);
    sync.waitgroup_done(wg);
}

fn test_waitgroup_multiple() {
    wg := sync.waitgroup_create();
    sync.waitgroup_add(wg, 3);
    sync.waitgroup_done(wg);
    sync.waitgroup_done(wg);
    sync.waitgroup_done(wg);
}

fn test_waitgroup_wait_immediate() {
    // Wait on an empty wait group should return immediately
    wg := sync.waitgroup_create();
    sync.waitgroup_wait(wg);
}

fn test_waitgroup_add_wait() {
    wg := sync.waitgroup_create();
    sync.waitgroup_add(wg, 1);
    sync.waitgroup_done(wg);
    sync.waitgroup_wait(wg);
}

// ============================================
// Once Tests
// ============================================

fn test_once_create() {
    once := sync.once_create();
}

// ============================================
// AtomicInt Tests
// ============================================

fn test_atomic_create() {
    counter := sync.atomic_int_create(0);
}

fn test_atomic_create_with_value() {
    counter := sync.atomic_int_create(42);
    val := sync.atomic_int_load(counter);
    assert_eq(42, val);
}

fn test_atomic_load_store() {
    counter := sync.atomic_int_create(0);
    sync.atomic_int_store(counter, 100);
    val := sync.atomic_int_load(counter);
    assert_eq(100, val);
}

fn test_atomic_add() {
    counter := sync.atomic_int_create(10);

    // add returns the previous value
    prev := sync.atomic_int_add(counter, 5);
    assert_eq(10, prev);

    // current value should be 15
    current := sync.atomic_int_load(counter);
    assert_eq(15, current);
}

fn test_atomic_add_negative() {
    counter := sync.atomic_int_create(100);
    prev := sync.atomic_int_add(counter, -30);
    assert_eq(100, prev);

    current := sync.atomic_int_load(counter);
    assert_eq(70, current);
}

fn test_atomic_swap() {
    counter := sync.atomic_int_create(42);

    // swap returns old value
    old := sync.atomic_int_swap(counter, 100);
    assert_eq(42, old);

    // current should be new value
    current := sync.atomic_int_load(counter);
    assert_eq(100, current);
}

fn test_atomic_compare_swap_success() {
    counter := sync.atomic_int_create(10);

    // CAS should succeed when expected matches
    success := sync.atomic_int_compare_swap(counter, 10, 20);
    assert_eq(true, success);

    val := sync.atomic_int_load(counter);
    assert_eq(20, val);
}

fn test_atomic_compare_swap_fail() {
    counter := sync.atomic_int_create(10);

    // CAS should fail when expected doesn't match
    success := sync.atomic_int_compare_swap(counter, 5, 20);
    assert_eq(false, success);

    // Value should remain unchanged
    val := sync.atomic_int_load(counter);
    assert_eq(10, val);
}

fn test_atomic_increment_pattern() {
    counter := sync.atomic_int_create(0);

    // Increment by 1 multiple times
    sync.atomic_int_add(counter, 1);
    sync.atomic_int_add(counter, 1);
    sync.atomic_int_add(counter, 1);

    val := sync.atomic_int_load(counter);
    assert_eq(3, val);
}
