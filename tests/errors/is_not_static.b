Counter :: struct {
    value int
}

Counter :: get(self) -> int {
    return self.value;
}

fn main() {
    // This should fail - get() is an instance method, not static
    x := Counter.get();
}
