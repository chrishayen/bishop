// Tests for the json module

import json;

fn test_parse_object() {
    data := json.parse('{"name": "Alice", "age": 30}') or return;
    assert_eq(true, data.is_object());
}

fn test_parse_array() {
    data := json.parse('[1, 2, 3]') or return;
    assert_eq(true, data.is_list());
}

fn test_parse_string() {
    data := json.parse('"hello"') or return;
    assert_eq(true, data.is_str());
    name := data.as_str() or return;
    assert_eq("hello", name);
}

fn test_parse_int() {
    data := json.parse('42') or return;
    assert_eq(true, data.is_int());
    val := data.as_int() or return;
    assert_eq(42, val);
}

fn test_parse_float() {
    data := json.parse('3.14') or return;
    assert_eq(true, data.is_float());
}

fn test_parse_bool_true() {
    data := json.parse('true') or return;
    assert_eq(true, data.is_bool());
    val := data.as_bool() or return;
    assert_eq(true, val);
}

fn test_parse_bool_false() {
    data := json.parse('false') or return;
    val := data.as_bool() or return;
    assert_eq(false, val);
}

fn test_parse_null() {
    data := json.parse('null') or return;
    assert_eq(true, data.is_null());
}

fn test_parse_invalid_json() {
    // This should fail to parse
    result := json.parse('{invalid}') or {
        // Expected error
        assert_eq(true, true);
        return;
    };
    // Should not reach here
    assert_eq(true, false);
}

fn test_get_str() {
    data := json.parse('{"name": "Bob"}') or return;
    name := data.get_str("name") or return;
    assert_eq("Bob", name);
}

fn test_get_int() {
    data := json.parse('{"count": 42}') or return;
    count := data.get_int("count") or return;
    assert_eq(42, count);
}

fn test_get_bool() {
    data := json.parse('{"active": true}') or return;
    active := data.get_bool("active") or return;
    assert_eq(true, active);
}

fn test_get_list() {
    data := json.parse('{"items": [1, 2, 3]}') or return;
    items := data.get_list("items") or return;
    assert_eq(true, items.is_list());
    assert_eq(3, items.length());
}

fn test_get_object() {
    data := json.parse('{"user": {"name": "Charlie"}}') or return;
    user := data.get_object("user") or return;
    assert_eq(true, user.is_object());
    name := user.get_str("name") or return;
    assert_eq("Charlie", name);
}

fn test_path_access() {
    data := json.parse('{"user": {"address": {"city": "NYC"}}}') or return;
    city := data.path("user.address.city") or return;
    city_str := city.as_str() or return;
    assert_eq("NYC", city_str);
}

fn test_create_object() {
    obj := json.object();
    assert_eq(true, obj.is_object());
    assert_eq(0, obj.length());
}

fn test_create_array() {
    arr := json.array();
    assert_eq(true, arr.is_list());
    assert_eq(0, arr.length());
}

fn test_set_values() {
    obj := json.object();
    obj.set_str("name", "Dave");
    obj.set_int("age", 25);
    obj.set_bool("active", true);
    obj.set_float("score", 95.5);

    assert_eq(4, obj.length());

    name := obj.get_str("name") or return;
    assert_eq("Dave", name);

    age := obj.get_int("age") or return;
    assert_eq(25, age);
}

fn test_push_values() {
    arr := json.array();
    arr.push_int(1);
    arr.push_int(2);
    arr.push_str("three");

    assert_eq(3, arr.length());
}

fn test_has_key() {
    data := json.parse('{"name": "Eve"}') or return;
    assert_eq(true, data.has("name"));
    assert_eq(false, data.has("age"));
}

fn test_remove_key() {
    obj := json.object();
    obj.set_str("temp", "value");
    assert_eq(true, obj.has("temp"));
    obj.remove("temp");
    assert_eq(false, obj.has("temp"));
}

fn test_keys() {
    data := json.parse('{"a": 1, "b": 2}') or return;
    key_list := data.keys();
    assert_eq(2, key_list.length());

    // Keys are sorted alphabetically in JSON objects
    assert_eq("a", key_list.get(0));
    assert_eq("b", key_list.get(1));
}

fn test_keys_empty_object() {
    data := json.parse('{}') or return;
    key_list := data.keys();
    assert_eq(0, key_list.length());
}

fn test_keys_iteration() {
    data := json.parse('{"x": 1, "y": 2, "z": 3}') or return;
    key_list := data.keys();
    count := 0;

    for k in key_list {
        count = count + 1;
    }

    assert_eq(3, count);
}

fn test_stringify_compact() {
    obj := json.object();
    obj.set_str("name", "Frank");
    result := json.stringify(obj);
    assert_eq(true, result.length() > 0);
}

fn test_stringify_pretty() {
    obj := json.object();
    obj.set_str("name", "Grace");
    result := json.stringify_pretty(obj);
    assert_eq(true, result.length() > 0);
}

fn test_roundtrip() {
    original := '{"name":"test","value":42}';
    data := json.parse(original) or return;
    result := json.stringify(data);
    // Parse again to verify
    data2 := json.parse(result) or return;
    name := data2.get_str("name") or return;
    assert_eq("test", name);
}

fn test_nested_object() {
    json_str := '{"level1": {"level2": {"level3": "deep"}}}';
    data := json.parse(json_str) or return;
    l1 := data.get_object("level1") or return;
    l2 := l1.get_object("level2") or return;
    l3 := l2.get_str("level3") or return;
    assert_eq("deep", l3);
}

fn test_array_of_objects() {
    json_str := '[{"id": 1}, {"id": 2}]';
    data := json.parse(json_str) or return;
    assert_eq(2, data.length());
    first := data.get("0") or return;
    id := first.get_int("id") or return;
    assert_eq(1, id);
}

fn test_escape_sequences() {
    json_str := '{"text": "line1\\nline2\\ttab"}';
    data := json.parse(json_str) or return;
    text := data.get_str("text") or return;
    assert_eq(true, text.length() > 0);
}

fn test_null_value() {
    data := json.parse('{"value": null}') or return;
    val := data.get("value") or return;
    assert_eq(true, val.is_null());
}

fn test_set_null() {
    obj := json.object();
    obj.set_null("empty");
    val := obj.get("empty") or return;
    assert_eq(true, val.is_null());
}

fn test_push_null() {
    arr := json.array();
    arr.push_null();
    assert_eq(1, arr.length());
    val := arr.get("0") or return;
    assert_eq(true, val.is_null());
}

fn test_negative_numbers() {
    data := json.parse('-42') or return;
    val := data.as_int() or return;
    assert_eq(-42, val);
}

fn test_scientific_notation() {
    data := json.parse('1.5e10') or return;
    assert_eq(true, data.is_float());
}

fn test_empty_object() {
    data := json.parse('{}') or return;
    assert_eq(true, data.is_object());
    assert_eq(0, data.length());
}

fn test_empty_array() {
    data := json.parse('[]') or return;
    assert_eq(true, data.is_list());
    assert_eq(0, data.length());
}
