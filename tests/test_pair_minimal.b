fn pair_return(int a, int b) -> Pair<int> {
    return Pair<int>(a, b);
}

fn test_minimal() {
    p := pair_return(1, 2);
    assert_eq(p.first, 1);
}
