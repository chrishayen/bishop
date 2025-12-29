// ============================================
// Nested Generic Types
// ============================================
// Tests for nested generic type handling like:
// - Pair<List<int>>
// - List<Pair<int>>
// - Tuple<List<str>>

// ============================================
// Pair of Lists
// ============================================

fn test_pair_of_lists_int() {
    list1 := [1, 2, 3];
    list2 := [4, 5, 6];
    p := Pair<List<int>>(list1, list2);
    assert_eq(p.first.length(), 3);
    assert_eq(p.second.length(), 3);
    assert_eq(p.first.get(0), 1);
    assert_eq(p.second.get(0), 4);
}

fn test_pair_of_lists_str() {
    list1 := ["a", "b"];
    list2 := ["c", "d"];
    p := Pair<List<str>>(list1, list2);
    assert_eq(p.first.length(), 2);
    assert_eq(p.second.length(), 2);
    assert_eq(p.first.get(0), "a");
    assert_eq(p.second.get(1), "d");
}

fn test_pair_of_lists_field_access() {
    list1 := [10, 20];
    list2 := [30, 40];
    p := Pair<List<int>>(list1, list2);
    first_list := p.first;
    second_list := p.second;
    assert_eq(first_list.get(0), 10);
    assert_eq(second_list.get(1), 40);
}

fn test_pair_of_lists_get_method() {
    list1 := [100];
    list2 := [200];
    p := Pair<List<int>>(list1, list2);
    first := p.get(0) default List<int>();
    assert_eq(first.length(), 1);
    assert_eq(first.get(0), 100);
}

// ============================================
// List of Pairs
// ============================================

fn test_list_of_pairs() {
    p1 := Pair<int>(1, 2);
    p2 := Pair<int>(3, 4);
    list := List<Pair<int>>();
    list.append(p1);
    list.append(p2);
    assert_eq(list.length(), 2);
    assert_eq(list.get(0).first, 1);
    assert_eq(list.get(1).second, 4);
}

fn test_list_of_pairs_literal() {
    p1 := Pair<int>(10, 20);
    p2 := Pair<int>(30, 40);
    list := [p1, p2];
    assert_eq(list.length(), 2);
    assert_eq(list.first().first, 10);
    assert_eq(list.last().second, 40);
}

// ============================================
// Tuple of Lists
// ============================================

fn test_tuple_of_lists() {
    list1 := [1, 2];
    list2 := [3, 4];
    list3 := [5, 6];
    t := Tuple<List<int>>(list1, list2, list3);
    first := t.get(0) default List<int>();
    second := t.get(1) default List<int>();
    third := t.get(2) default List<int>();
    assert_eq(first.length(), 2);
    assert_eq(second.get(0), 3);
    assert_eq(third.get(1), 6);
}

fn test_tuple_of_lists_str() {
    list1 := ["a"];
    list2 := ["b"];
    t := Tuple<List<str>>(list1, list2);
    first := t.get(0) default List<str>();
    assert_eq(first.get(0), "a");
}

// ============================================
// List of Tuples
// ============================================

fn test_list_of_tuples() {
    t1 := Tuple<int>(1, 2, 3);
    t2 := Tuple<int>(4, 5, 6);
    list := List<Tuple<int>>();
    list.append(t1);
    list.append(t2);
    assert_eq(list.length(), 2);
}

// ============================================
// Function Return Types with Nested Generics
// ============================================

fn create_pair_of_lists() -> Pair<List<int>> {
    list1 := [1, 2, 3];
    list2 := [4, 5, 6];
    return Pair<List<int>>(list1, list2);
}

fn test_nested_generic_return_type() {
    p := create_pair_of_lists();
    assert_eq(p.first.length(), 3);
    assert_eq(p.second.get(2), 6);
}

fn create_list_of_pairs() -> List<Pair<int>> {
    list := List<Pair<int>>();
    list.append(Pair<int>(1, 2));
    list.append(Pair<int>(3, 4));
    return list;
}

fn test_list_of_pairs_return_type() {
    list := create_list_of_pairs();
    assert_eq(list.length(), 2);
    assert_eq(list.get(0).first, 1);
}

// ============================================
// Typed Declarations with Nested Generics
// ============================================

fn test_typed_decl_pair_of_lists() {
    Pair<List<int>> p = Pair<List<int>>([1, 2], [3, 4]);
    assert_eq(p.first.length(), 2);
    assert_eq(p.second.get(0), 3);
}

fn test_typed_decl_list_of_pairs() {
    List<Pair<int>> list = List<Pair<int>>();
    list.append(Pair<int>(10, 20));
    assert_eq(list.length(), 1);
}

// ============================================
// Deeply Nested Generics
// ============================================

fn test_pair_of_pair_of_int() {
    inner1 := Pair<int>(1, 2);
    inner2 := Pair<int>(3, 4);
    outer := Pair<Pair<int>>(inner1, inner2);
    assert_eq(outer.first.first, 1);
    assert_eq(outer.first.second, 2);
    assert_eq(outer.second.first, 3);
    assert_eq(outer.second.second, 4);
}

fn test_list_of_list_of_int() {
    inner1 := [1, 2];
    inner2 := [3, 4];
    outer := [inner1, inner2];
    assert_eq(outer.length(), 2);
    assert_eq(outer.get(0).length(), 2);
    assert_eq(outer.get(0).get(0), 1);
    assert_eq(outer.get(1).get(1), 4);
}

// ============================================
// Channel with Nested Generic Types
// ============================================

fn list_sender(Channel<List<int>> ch, List<int> val) {
    ch.send(val);
}

fn test_channel_of_list_int() {
    ch := Channel<List<int>>();
    list := [1, 2, 3];

    go list_sender(ch, list);

    received := ch.recv();
    assert_eq(received.length(), 3);
    assert_eq(received.get(0), 1);
    assert_eq(received.get(2), 3);
}

fn pair_sender(Channel<Pair<int>> ch, Pair<int> val) {
    ch.send(val);
}

fn test_channel_of_pair_int() {
    ch := Channel<Pair<int>>();
    p := Pair<int>(10, 20);

    go pair_sender(ch, p);

    received := ch.recv();
    assert_eq(received.first, 10);
    assert_eq(received.second, 20);
}

fn tuple_sender(Channel<Tuple<int>> ch, Tuple<int> val) {
    ch.send(val);
}

fn test_channel_of_tuple_int() {
    ch := Channel<Tuple<int>>();
    t := Tuple<int>(100, 200, 300);

    go tuple_sender(ch, t);

    received := ch.recv();
    first := received.get(0) default 0;
    second := received.get(1) default 0;
    third := received.get(2) default 0;
    assert_eq(first, 100);
    assert_eq(second, 200);
    assert_eq(third, 300);
}

fn test_channel_of_list_str() {
    ch := Channel<List<str>>();
    list := ["hello", "world"];

    go fn() {
        ch.send(list);
    }();

    received := ch.recv();
    assert_eq(received.length(), 2);
    assert_eq(received.get(0), "hello");
    assert_eq(received.get(1), "world");
}

// Test typed declaration for Channel with nested generic
fn test_typed_channel_of_list() {
    Channel<List<int>> ch = Channel<List<int>>();
    list := [42];

    go fn() {
        ch.send(list);
    }();

    received := ch.recv();
    assert_eq(received.get(0), 42);
}
