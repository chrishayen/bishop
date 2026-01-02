// ============================================
// Map Creation
// ============================================

fn test_map_create_empty() {
    ages := Map<str, int>();
    assert_eq(ages.length(), 0);
}

fn test_map_create_str_str() {
    config := Map<str, str>();
    assert_eq(config.is_empty(), true);
}

// ============================================
// Map Literal Syntax
// ============================================

fn test_map_literal_str_str() {
    config := {"host": "localhost", "port": "8080"};
    assert_eq(config.length(), 2);
}

fn test_map_literal_str_int() {
    ages := {"alice": 30, "bob": 25};
    assert_eq(ages.length(), 2);
}

fn test_map_literal_single() {
    single := {"key": "value"};
    assert_eq(single.length(), 1);
}

// ============================================
// Map set() and get()
// ============================================

fn test_map_set_get() {
    ages := Map<str, int>();
    ages.set("alice", 30);
    ages.set("bob", 25);

    alice_age := ages.get("alice") default 0;
    assert_eq(alice_age, 30);

    bob_age := ages.get("bob") default 0;
    assert_eq(bob_age, 25);
}

fn test_map_get_missing_key() {
    ages := Map<str, int>();
    ages.set("alice", 30);

    charlie_age := ages.get("charlie") default 99;
    assert_eq(charlie_age, 99);
}

fn test_map_set_overwrite() {
    ages := Map<str, int>();
    ages.set("alice", 30);
    ages.set("alice", 31);

    alice_age := ages.get("alice") default 0;
    assert_eq(alice_age, 31);
}

// ============================================
// Map contains()
// ============================================

fn test_map_contains_true() {
    ages := {"alice": 30, "bob": 25};
    assert_eq(ages.contains("alice"), true);
    assert_eq(ages.contains("bob"), true);
}

fn test_map_contains_false() {
    ages := {"alice": 30};
    assert_eq(ages.contains("charlie"), false);
}

// ============================================
// Map remove()
// ============================================

fn test_map_remove() {
    ages := {"alice": 30, "bob": 25};
    ages.remove("bob");

    assert_eq(ages.length(), 1);
    assert_eq(ages.contains("bob"), false);
    assert_eq(ages.contains("alice"), true);
}

fn test_map_remove_missing_key() {
    ages := {"alice": 30};
    ages.remove("charlie");

    assert_eq(ages.length(), 1);
}

// ============================================
// Map Query Methods
// ============================================

fn test_map_length() {
    ages := {"alice": 30, "bob": 25, "charlie": 35};
    assert_eq(ages.length(), 3);
}

fn test_map_is_empty_true() {
    ages := Map<str, int>();
    assert_eq(ages.is_empty(), true);
}

fn test_map_is_empty_false() {
    ages := {"alice": 30};
    assert_eq(ages.is_empty(), false);
}

fn test_map_clear() {
    ages := {"alice": 30, "bob": 25};
    ages.clear();

    assert_eq(ages.length(), 0);
    assert_eq(ages.is_empty(), true);
}

// ============================================
// Map keys(), values(), items()
// ============================================

fn test_map_keys() {
    ages := {"alice": 30, "bob": 25};
    key_list := ages.keys();

    assert_eq(key_list.length(), 2);
    assert_eq(key_list.contains("alice"), true);
    assert_eq(key_list.contains("bob"), true);
}

fn test_map_values() {
    ages := {"alice": 30, "bob": 25};
    val_list := ages.values();

    assert_eq(val_list.length(), 2);
    assert_eq(val_list.contains(30), true);
    assert_eq(val_list.contains(25), true);
}

fn test_map_items() {
    ages := {"alice": 30, "bob": 25};
    item_list := ages.items();

    assert_eq(item_list.length(), 2);
}

// ============================================
// Map Iteration
// ============================================

fn test_map_iterate_keys() {
    ages := {"alice": 30, "bob": 25};
    count := 0;

    for key in ages.keys() {
        count = count + 1;
    }

    assert_eq(count, 2);
}

fn test_map_iterate_values() {
    ages := {"alice": 30, "bob": 25};
    total := 0;

    for val in ages.values() {
        total = total + val;
    }

    assert_eq(total, 55);
}

fn test_map_iterate_items() {
    ages := {"alice": 30, "bob": 25};
    total := 0;

    for item in ages.items() {
        total = total + item.value;
    }

    assert_eq(total, 55);
}

// ============================================
// Map with Different Types
// ============================================

fn test_map_int_str() {
    codes := Map<int, str>();
    codes.set(200, "OK");
    codes.set(404, "Not Found");

    msg := codes.get(200) default "Unknown";
    assert_eq(msg, "OK");
}

fn test_map_str_bool() {
    flags := Map<str, bool>();
    flags.set("debug", true);
    flags.set("verbose", false);

    is_debug := flags.get("debug") default false;
    assert_eq(is_debug, true);
}

// ============================================
// Typed Variable Declaration
// ============================================

fn test_map_typed_decl() {
    Map<str, int> ages = {"alice": 30};
    assert_eq(ages.length(), 1);
}

fn test_map_typed_empty() {
    Map<str, str> config = Map<str, str>();
    assert_eq(config.is_empty(), true);
}
