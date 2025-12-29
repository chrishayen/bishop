// ============================================
// Pair Creation
// ============================================

fn test_pair_create_int() {
    p := Pair<int>(1, 2);
    assert_eq(p.first, 1);
    assert_eq(p.second, 2);
}

fn test_pair_create_str() {
    p := Pair<str>("hello", "world");
    assert_eq(p.first, "hello");
    assert_eq(p.second, "world");
}

fn test_pair_create_bool() {
    p := Pair<bool>(true, false);
    assert_eq(p.first, true);
    assert_eq(p.second, false);
}

// ============================================
// Pair Field Access
// ============================================

fn test_pair_first_access() {
    p := Pair<int>(42, 100);
    x := p.first;
    assert_eq(x, 42);
}

fn test_pair_second_access() {
    p := Pair<int>(42, 100);
    x := p.second;
    assert_eq(x, 100);
}

// ============================================
// Pair get() with default
// ============================================

fn test_pair_get_first_with_default() {
    p := Pair<int>(10, 20);
    x := p.get(0) default 0;
    assert_eq(x, 10);
}

fn test_pair_get_second_with_default() {
    p := Pair<int>(10, 20);
    x := p.get(1) default 0;
    assert_eq(x, 20);
}

fn test_pair_get_out_of_bounds_uses_default() {
    p := Pair<int>(10, 20);
    x := p.get(2) default 99;
    assert_eq(x, 99);
}

fn test_pair_get_negative_uses_default() {
    p := Pair<int>(10, 20);
    x := p.get(-1) default 99;
    assert_eq(x, 99);
}

fn test_pair_get_with_variable_index() {
    p := Pair<str>("a", "b");
    idx := 1;
    x := p.get(idx) default "z";
    assert_eq(x, "b");
}

// ============================================
// Pair Typed Declaration
// ============================================

fn test_pair_typed_decl() {
    Pair<int> p = Pair<int>(5, 10);
    assert_eq(p.first, 5);
    assert_eq(p.second, 10);
}

// ============================================
// Pair with Structs
// ============================================

Point :: struct {
    x int,
    y int
}

fn test_pair_of_structs() {
    p1 := Point { x: 1, y: 2 };
    p2 := Point { x: 3, y: 4 };
    pair := Pair<Point>(p1, p2);
    assert_eq(pair.first.x, 1);
    assert_eq(pair.second.x, 3);
}

// ============================================
// Pair as Function Return Type
// ============================================

fn divide(int a, int b) -> Pair<int> {
    quotient := a / b;
    remainder := a - quotient * b;
    return Pair<int>(quotient, remainder);
}

fn test_pair_as_return_type() {
    result := divide(17, 5);
    assert_eq(result.first, 3);
    assert_eq(result.second, 2);
}

fn test_pair_as_return_type_access() {
    result := divide(10, 3);
    quotient := result.first;
    remainder := result.second;
    assert_eq(quotient, 3);
    assert_eq(remainder, 1);
}

// ============================================
// Pair Chained Access
// ============================================

fn test_pair_chained_access() {
    p := Pair<int>(100, 200);
    sum := p.first + p.second;
    assert_eq(sum, 300);
}
