// Error test: or handler on non-falsy type
// Structs are not falsy, only bool, int, float, str, list, and optional

Point :: struct { x int, y int }

fn get_point() -> Point {
    return Point { x: 1, y: 2 };
}

fn main() {
    p := get_point() or return;
}
