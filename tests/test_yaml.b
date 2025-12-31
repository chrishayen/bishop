import yaml;

fn test_parse_object() {
    data := yaml.parse("name: Alice\nage: 30") or return;
    assert_eq(true, data.is_object());
}

fn test_parse_list() {
    data := yaml.parse("- 1\n- 2\n- 3") or return;
    assert_eq(true, data.is_list());
}

fn test_parse_string() {
    data := yaml.parse("hello") or return;
    assert_eq(true, data.is_str());
    name := data.as_str() or return;
    assert_eq("hello", name);
}

fn test_parse_quoted_string() {
    data := yaml.parse("\"hello world\"") or return;
    assert_eq(true, data.is_str());
    name := data.as_str() or return;
    assert_eq("hello world", name);
}

fn test_parse_int() {
    data := yaml.parse("42") or return;
    assert_eq(true, data.is_int());
    val := data.as_int() or return;
    assert_eq(42, val);
}

fn test_parse_negative_int() {
    data := yaml.parse("-42") or return;
    assert_eq(true, data.is_int());
    val := data.as_int() or return;
    assert_eq(-42, val);
}

fn test_parse_float() {
    data := yaml.parse("3.14") or return;
    assert_eq(true, data.is_float());
}

fn test_parse_bool_true() {
    data := yaml.parse("true") or return;
    assert_eq(true, data.is_bool());
    val := data.as_bool() or return;
    assert_eq(true, val);
}

fn test_parse_bool_false() {
    data := yaml.parse("false") or return;
    val := data.as_bool() or return;
    assert_eq(false, val);
}

fn test_parse_bool_yes() {
    data := yaml.parse("yes") or return;
    assert_eq(true, data.is_bool());
    val := data.as_bool() or return;
    assert_eq(true, val);
}

fn test_parse_bool_no() {
    data := yaml.parse("no") or return;
    val := data.as_bool() or return;
    assert_eq(false, val);
}

fn test_parse_null() {
    data := yaml.parse("null") or return;
    assert_eq(true, data.is_null());
}

fn test_parse_null_tilde() {
    data := yaml.parse("~") or return;
    assert_eq(true, data.is_null());
}

fn test_get_str() {
    data := yaml.parse("name: Bob") or return;
    name := data.get_str("name") or return;
    assert_eq("Bob", name);
}

fn test_get_int() {
    data := yaml.parse("count: 42") or return;
    count := data.get_int("count") or return;
    assert_eq(42, count);
}

fn test_get_bool() {
    data := yaml.parse("active: true") or return;
    active := data.get_bool("active") or return;
    assert_eq(true, active);
}

fn test_get_list() {
    data := yaml.parse("items:\n  - 1\n  - 2\n  - 3") or return;
    items := data.get_list("items") or return;
    assert_eq(true, items.is_list());
    assert_eq(3, items.length());
}

fn test_get_object() {
    yaml_str := "user:\n  name: Charlie";
    data := yaml.parse(yaml_str) or return;
    user := data.get_object("user") or return;
    assert_eq(true, user.is_object());
    name := user.get_str("name") or return;
    assert_eq("Charlie", name);
}

fn test_path_access() {
    yaml_str := "user:\n  address:\n    city: NYC";
    data := yaml.parse(yaml_str) or return;
    city := data.path("user.address.city") or return;
    city_str := city.as_str() or return;
    assert_eq("NYC", city_str);
}

fn test_create_object() {
    obj := yaml.object();
    assert_eq(true, obj.is_object());
    assert_eq(0, obj.length());
}

fn test_create_array() {
    arr := yaml.array();
    assert_eq(true, arr.is_list());
    assert_eq(0, arr.length());
}

fn test_set_values() {
    obj := yaml.object();
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
    arr := yaml.array();
    arr.push_int(1);
    arr.push_int(2);
    arr.push_str("three");
    assert_eq(3, arr.length());
}

fn test_has_key() {
    data := yaml.parse("name: Eve") or return;
    assert_eq(true, data.has("name"));
    assert_eq(false, data.has("age"));
}

fn test_remove_key() {
    obj := yaml.object();
    obj.set_str("temp", "value");
    assert_eq(true, obj.has("temp"));
    obj.remove("temp");
    assert_eq(false, obj.has("temp"));
}

fn test_keys() {
    data := yaml.parse("a: 1\nb: 2") or return;
    key_list := data.keys();
    assert_eq(true, key_list.length() > 0);
}

fn test_stringify() {
    obj := yaml.object();
    obj.set_str("name", "Frank");
    result := yaml.stringify(obj);
    assert_eq(true, result.length() > 0);
}

fn test_roundtrip() {
    original := "name: test\nvalue: 42";
    data := yaml.parse(original) or return;
    result := yaml.stringify(data);
    data2 := yaml.parse(result) or return;
    name := data2.get_str("name") or return;
    assert_eq("test", name);
}

fn test_nested_object() {
    yaml_str := "level1:\n  level2:\n    level3: deep";
    data := yaml.parse(yaml_str) or return;
    l1 := data.get_object("level1") or return;
    l2 := l1.get_object("level2") or return;
    l3 := l2.get_str("level3") or return;
    assert_eq("deep", l3);
}

fn test_inline_list() {
    data := yaml.parse("items: [1, 2, 3]") or return;
    items := data.get_list("items") or return;
    assert_eq(3, items.length());
}

fn test_inline_object() {
    data := yaml.parse("person: {name: Alice, age: 30}") or return;
    person := data.get_object("person") or return;
    name := person.get_str("name") or return;
    assert_eq("Alice", name);
}

fn test_multiline_string() {
    yaml_str := "text: |\n  line1\n  line2";
    data := yaml.parse(yaml_str) or return;
    text := data.get_str("text") or return;
    assert_eq(true, text.length() > 0);
}

fn test_comment_handling() {
    yaml_str := "# comment\nname: test # inline comment";
    data := yaml.parse(yaml_str) or return;
    name := data.get_str("name") or return;
    assert_eq("test", name);
}

fn test_list_of_objects() {
    yaml_str := "- id: 1\n- id: 2";
    data := yaml.parse(yaml_str) or return;
    assert_eq(2, data.length());
    first := data.get("0") or return;
    id := first.get_int("id") or return;
    assert_eq(1, id);
}

fn test_empty_value() {
    yaml_str := "empty:";
    data := yaml.parse(yaml_str) or return;
    empty := data.get("empty") or return;
    assert_eq(true, empty.is_null());
}
