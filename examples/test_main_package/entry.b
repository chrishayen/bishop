// Test: Root directory as implicit 'main' package
// All .b files in the same directory should share scope

fn main() {
    // Helper is defined in helper.b - should be visible here
    h := Helper { value: 10 };

    if h.value != 10 {
        print("FAIL: struct literal");
        return;
    }

    h2 := Helper.create(20);

    if h2.value != 20 {
        print("FAIL: static method");
        return;
    }

    h3 := Helper { value: 30 };

    if h3.get_value() != 30 {
        print("FAIL: instance method");
        return;
    }

    // helper_function is defined in helper.b
    result := helper_function();

    if result != 42 {
        print("FAIL: sibling function");
        return;
    }

    print("PASS: main package merging works");
}
