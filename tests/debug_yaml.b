import yaml;

fn test_negative_int() {
    data := yaml.parse("-42") or return;
    print(data.is_int());
    print(data.is_str());
}

fn test_list() {
    data := yaml.parse("items:\n  - 1\n  - 2\n  - 3") or return;
    print(data.is_object());
    items := data.get_list("items") or return;
    print(items.is_list());
    print(items.length());
}
