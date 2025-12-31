// Helper struct defined in a sibling file
// Should be visible to entry.b without explicit import

Helper :: struct {
    value int
}

@static
Helper :: create(int val) -> Helper {
    return Helper { value: val };
}

Helper :: get_value(self) -> int {
    return self.value;
}

fn helper_function() -> int {
    return 42;
}
