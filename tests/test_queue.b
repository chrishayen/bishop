// ============================================
// Queue Creation
// ============================================

fn test_queue_create_empty_int() {
    q := Queue<int>();
    assert_eq(q.length(), 0);
}

fn test_queue_create_empty_str() {
    q := Queue<str>();
    assert_eq(q.is_empty(), true);
}

// ============================================
// Queue Push
// ============================================

fn test_queue_push() {
    q := Queue<int>();
    q.push(1);
    q.push(2);
    q.push(3);
    assert_eq(q.length(), 3);
}

fn test_queue_push_str() {
    q := Queue<str>();
    q.push("hello");
    q.push("world");
    assert_eq(q.length(), 2);
}

// ============================================
// Queue Pop (FIFO)
// ============================================

fn test_queue_pop() {
    q := Queue<int>();
    q.push(1);
    q.push(2);
    q.push(3);
    val := q.pop();
    assert_eq(val, 1);
    assert_eq(q.length(), 2);
}

fn test_queue_pop_multiple() {
    q := Queue<int>();
    q.push(1);
    q.push(2);
    q.push(3);
    first := q.pop();
    second := q.pop();
    third := q.pop();
    assert_eq(first, 1);
    assert_eq(second, 2);
    assert_eq(third, 3);
}

fn test_queue_fifo_order() {
    q := Queue<str>();
    q.push("a");
    q.push("b");
    q.push("c");
    assert_eq(q.pop(), "a");
    assert_eq(q.pop(), "b");
    assert_eq(q.pop(), "c");
}

// ============================================
// Queue Front and Back
// ============================================

fn test_queue_front() {
    q := Queue<int>();
    q.push(10);
    q.push(20);
    q.push(30);
    assert_eq(q.front(), 10);
}

fn test_queue_back() {
    q := Queue<int>();
    q.push(10);
    q.push(20);
    q.push(30);
    assert_eq(q.back(), 30);
}

fn test_queue_front_after_pop() {
    q := Queue<int>();
    q.push(1);
    q.push(2);
    q.push(3);
    q.pop();
    assert_eq(q.front(), 2);
}

// ============================================
// Queue Query Methods
// ============================================

fn test_queue_length() {
    q := Queue<int>();
    q.push(1);
    q.push(2);
    q.push(3);
    assert_eq(q.length(), 3);
}

fn test_queue_is_empty_true() {
    q := Queue<int>();
    assert_eq(q.is_empty(), true);
}

fn test_queue_is_empty_false() {
    q := Queue<int>();
    q.push(1);
    assert_eq(q.is_empty(), false);
}

fn test_queue_is_empty_after_pop() {
    q := Queue<int>();
    q.push(1);
    q.pop();
    assert_eq(q.is_empty(), true);
}

// ============================================
// Typed Variable Declaration
// ============================================

fn test_queue_typed_decl() {
    Queue<int> q = Queue<int>();
    q.push(1);
    assert_eq(q.length(), 1);
}

// ============================================
// Queue with bool
// ============================================

fn test_queue_bool() {
    q := Queue<bool>();
    q.push(true);
    q.push(false);
    assert_eq(q.pop(), true);
    assert_eq(q.pop(), false);
}
