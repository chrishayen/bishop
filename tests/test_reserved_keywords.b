/// Test that C++ reserved keywords can be used as variable names in Bishop

fn test_unsigned_variable() {
    u32 unsigned = 100;
    assert_eq(unsigned, 100);
}

fn test_class_variable() {
    str class = "MyClass";
    assert_eq(class, "MyClass");
}

fn test_namespace_variable() {
    int namespace = 42;
    assert_eq(namespace, 42);
}

fn test_template_variable() {
    int template = 99;
    assert_eq(template, 99);
}

fn test_auto_variable() {
    int auto = 10;
    assert_eq(auto, 10);
}

fn test_register_variable() {
    int register = 5;
    assert_eq(register, 5);
}
