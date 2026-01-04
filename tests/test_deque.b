// ============================================
// Deque Creation
// ============================================

fn test_deque_create_empty_int() {
    dq := Deque<int>();
    assert_eq(dq.length(), 0);
}

fn test_deque_create_empty_str() {
    dq := Deque<str>();
    assert_eq(dq.is_empty(), true);
}

// ============================================
// Deque Push Operations
// ============================================

fn test_deque_push_front() {
    dq := Deque<int>();
    dq.push_front(1);
    dq.push_front(2);
    dq.push_front(3);
    assert_eq(dq.length(), 3);
    assert_eq(dq.front(), 3);
}

fn test_deque_push_back() {
    dq := Deque<int>();
    dq.push_back(1);
    dq.push_back(2);
    dq.push_back(3);
    assert_eq(dq.length(), 3);
    assert_eq(dq.back(), 3);
}

fn test_deque_push_front_and_back() {
    dq := Deque<int>();
    dq.push_back(2);
    dq.push_front(1);
    dq.push_back(3);
    assert_eq(dq.front(), 1);
    assert_eq(dq.back(), 3);
    assert_eq(dq.length(), 3);
}

// ============================================
// Deque Pop Operations
// ============================================

fn test_deque_pop_front() {
    dq := Deque<int>();
    dq.push_back(1);
    dq.push_back(2);
    dq.push_back(3);
    val := dq.pop_front();
    assert_eq(val, 1);
    assert_eq(dq.length(), 2);
}

fn test_deque_pop_back() {
    dq := Deque<int>();
    dq.push_back(1);
    dq.push_back(2);
    dq.push_back(3);
    val := dq.pop_back();
    assert_eq(val, 3);
    assert_eq(dq.length(), 2);
}

fn test_deque_pop_front_and_back() {
    dq := Deque<int>();
    dq.push_back(1);
    dq.push_back(2);
    dq.push_back(3);
    front := dq.pop_front();
    back := dq.pop_back();
    assert_eq(front, 1);
    assert_eq(back, 3);
    assert_eq(dq.length(), 1);
    assert_eq(dq.front(), 2);
}

// ============================================
// Deque Access Methods
// ============================================

fn test_deque_front() {
    dq := Deque<int>();
    dq.push_back(10);
    dq.push_back(20);
    assert_eq(dq.front(), 10);
}

fn test_deque_back() {
    dq := Deque<int>();
    dq.push_back(10);
    dq.push_back(20);
    assert_eq(dq.back(), 20);
}

fn test_deque_get() {
    dq := Deque<int>();
    dq.push_back(10);
    dq.push_back(20);
    dq.push_back(30);
    assert_eq(dq.get(0), 10);
    assert_eq(dq.get(1), 20);
    assert_eq(dq.get(2), 30);
}

// ============================================
// Deque Query Methods
// ============================================

fn test_deque_length() {
    dq := Deque<int>();
    dq.push_back(1);
    dq.push_back(2);
    dq.push_back(3);
    assert_eq(dq.length(), 3);
}

fn test_deque_is_empty_true() {
    dq := Deque<int>();
    assert_eq(dq.is_empty(), true);
}

fn test_deque_is_empty_false() {
    dq := Deque<int>();
    dq.push_back(1);
    assert_eq(dq.is_empty(), false);
}

// ============================================
// Deque Clear
// ============================================

fn test_deque_clear() {
    dq := Deque<int>();
    dq.push_back(1);
    dq.push_back(2);
    dq.push_back(3);
    dq.clear();
    assert_eq(dq.length(), 0);
    assert_eq(dq.is_empty(), true);
}

// ============================================
// Deque with Strings
// ============================================

fn test_deque_str_push_front() {
    dq := Deque<str>();
    dq.push_front("a");
    dq.push_front("b");
    assert_eq(dq.front(), "b");
    assert_eq(dq.back(), "a");
}

fn test_deque_str_push_back() {
    dq := Deque<str>();
    dq.push_back("hello");
    dq.push_back("world");
    assert_eq(dq.front(), "hello");
    assert_eq(dq.back(), "world");
}

// ============================================
// Typed Variable Declaration
// ============================================

fn test_deque_typed_decl() {
    Deque<int> dq = Deque<int>();
    dq.push_back(1);
    assert_eq(dq.length(), 1);
}
