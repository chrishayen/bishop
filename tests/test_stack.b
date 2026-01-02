// ============================================
// Stack Creation
// ============================================

fn test_stack_create_empty_int() {
    s := Stack<int>();
    assert_eq(s.length(), 0);
}

fn test_stack_create_empty_str() {
    s := Stack<str>();
    assert_eq(s.is_empty(), true);
}

// ============================================
// Stack Push
// ============================================

fn test_stack_push() {
    s := Stack<int>();
    s.push(1);
    s.push(2);
    s.push(3);
    assert_eq(s.length(), 3);
}

fn test_stack_push_str() {
    s := Stack<str>();
    s.push("hello");
    s.push("world");
    assert_eq(s.length(), 2);
}

// ============================================
// Stack Pop (LIFO)
// ============================================

fn test_stack_pop() {
    s := Stack<int>();
    s.push(1);
    s.push(2);
    s.push(3);
    val := s.pop();
    assert_eq(val, 3);
    assert_eq(s.length(), 2);
}

fn test_stack_pop_multiple() {
    s := Stack<int>();
    s.push(1);
    s.push(2);
    s.push(3);
    first := s.pop();
    second := s.pop();
    third := s.pop();
    assert_eq(first, 3);
    assert_eq(second, 2);
    assert_eq(third, 1);
}

fn test_stack_lifo_order() {
    s := Stack<str>();
    s.push("a");
    s.push("b");
    s.push("c");
    assert_eq(s.pop(), "c");
    assert_eq(s.pop(), "b");
    assert_eq(s.pop(), "a");
}

// ============================================
// Stack Top
// ============================================

fn test_stack_top() {
    s := Stack<int>();
    s.push(10);
    s.push(20);
    s.push(30);
    assert_eq(s.top(), 30);
    assert_eq(s.length(), 3);
}

fn test_stack_top_after_pop() {
    s := Stack<int>();
    s.push(1);
    s.push(2);
    s.push(3);
    s.pop();
    assert_eq(s.top(), 2);
}

// ============================================
// Stack Query Methods
// ============================================

fn test_stack_length() {
    s := Stack<int>();
    s.push(1);
    s.push(2);
    s.push(3);
    assert_eq(s.length(), 3);
}

fn test_stack_is_empty_true() {
    s := Stack<int>();
    assert_eq(s.is_empty(), true);
}

fn test_stack_is_empty_false() {
    s := Stack<int>();
    s.push(1);
    assert_eq(s.is_empty(), false);
}

fn test_stack_is_empty_after_pop() {
    s := Stack<int>();
    s.push(1);
    s.pop();
    assert_eq(s.is_empty(), true);
}

// ============================================
// Typed Variable Declaration
// ============================================

fn test_stack_typed_decl() {
    Stack<int> s = Stack<int>();
    s.push(1);
    assert_eq(s.length(), 1);
}

// ============================================
// Stack with bool
// ============================================

fn test_stack_bool() {
    s := Stack<bool>();
    s.push(true);
    s.push(false);
    assert_eq(s.pop(), false);
    assert_eq(s.pop(), true);
}
