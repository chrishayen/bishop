fn pair_return(int a, int b) -> Pair<int> {
    return Pair<int>(a / b, a - (a / b) * b);
}

fn test_minimal() {
    p := pair_return(17, 5);
    assert_eq(p.first, 3);
    assert_eq(p.second, 2);
}
