fn pair_return() -> Pair<int> {
    return Pair<int>(1, 2);
}

fn test_minimal() {
    p := pair_return();
    assert_eq(p.first, 1);
}
