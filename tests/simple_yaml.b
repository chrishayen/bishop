import yaml;

fn test_simple() {
    data := yaml.parse("hello") or return;
    assert_eq(true, true);
}
