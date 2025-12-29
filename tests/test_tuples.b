// ============================================
// Tuple Creation (2 elements)
// ============================================

fn test_tuple_create_2_int() {
    t := Tuple<int>(1, 2);
    x := t.get(0) default 0;
    y := t.get(1) default 0;
    assert_eq(x, 1);
    assert_eq(y, 2);
}

fn test_tuple_create_2_str() {
    t := Tuple<str>("a", "b");
    x := t.get(0) default "";
    y := t.get(1) default "";
    assert_eq(x, "a");
    assert_eq(y, "b");
}

// ============================================
// Tuple Creation (3 elements)
// ============================================

fn test_tuple_create_3() {
    t := Tuple<int>(10, 20, 30);
    assert_eq(t.get(0) default 0, 10);
    assert_eq(t.get(1) default 0, 20);
    assert_eq(t.get(2) default 0, 30);
}

// ============================================
// Tuple Creation (4 elements)
// ============================================

fn test_tuple_create_4() {
    t := Tuple<int>(1, 2, 3, 4);
    assert_eq(t.get(0) default 0, 1);
    assert_eq(t.get(1) default 0, 2);
    assert_eq(t.get(2) default 0, 3);
    assert_eq(t.get(3) default 0, 4);
}

// ============================================
// Tuple Creation (5 elements - max)
// ============================================

fn test_tuple_create_5() {
    t := Tuple<int>(1, 2, 3, 4, 5);
    assert_eq(t.get(0) default 0, 1);
    assert_eq(t.get(1) default 0, 2);
    assert_eq(t.get(2) default 0, 3);
    assert_eq(t.get(3) default 0, 4);
    assert_eq(t.get(4) default 0, 5);
}

// ============================================
// Tuple get() with default - out of bounds
// ============================================

fn test_tuple_get_out_of_bounds() {
    t := Tuple<int>(1, 2, 3);
    x := t.get(10) default 99;
    assert_eq(x, 99);
}

fn test_tuple_get_negative_uses_default() {
    t := Tuple<int>(1, 2, 3);
    x := t.get(-1) default 99;
    assert_eq(x, 99);
}

// ============================================
// Tuple get() with variable index
// ============================================

fn test_tuple_get_with_variable_index() {
    t := Tuple<str>("a", "b", "c");
    idx := 2;
    x := t.get(idx) default "z";
    assert_eq(x, "c");
}

fn test_tuple_get_variable_out_of_bounds() {
    t := Tuple<str>("a", "b", "c");
    idx := 5;
    x := t.get(idx) default "fallback";
    assert_eq(x, "fallback");
}

// ============================================
// Tuple Typed Declaration
// ============================================

fn test_tuple_typed_decl() {
    Tuple<int> t = Tuple<int>(10, 20, 30);
    assert_eq(t.get(0) default 0, 10);
}

// ============================================
// Tuple with Different Types
// ============================================

fn test_tuple_bool() {
    t := Tuple<bool>(true, false, true);
    assert_eq(t.get(0) default false, true);
    assert_eq(t.get(1) default true, false);
    assert_eq(t.get(2) default false, true);
}

// ============================================
// Tuple with Structs
// ============================================

Coord :: struct {
    x int,
    y int
}

fn test_tuple_of_structs() {
    c1 := Coord { x: 1, y: 2 };
    c2 := Coord { x: 3, y: 4 };
    c3 := Coord { x: 5, y: 6 };
    t := Tuple<Coord>(c1, c2, c3);
    first := t.get(0) default Coord { x: 0, y: 0 };
    assert_eq(first.x, 1);
}

// ============================================
// Tuple as Function Return Type
// ============================================

fn get_coords() -> Tuple<int> {
    return Tuple<int>(10, 20, 30);
}

fn test_tuple_as_return_type() {
    coords := get_coords();
    assert_eq(coords.get(0) default 0, 10);
    assert_eq(coords.get(1) default 0, 20);
    assert_eq(coords.get(2) default 0, 30);
}

// ============================================
// Tuple in Loop
// ============================================

fn test_tuple_in_loop() {
    t := Tuple<int>(1, 2, 3, 4, 5);
    sum := 0;

    for i in 0..5 {
        sum = sum + (t.get(i) default 0);
    }

    assert_eq(sum, 15);
}
