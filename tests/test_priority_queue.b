// Test PriorityQueue<T> collection

// Test max heap with integers (default)
fn test_max_heap_int() {
    pq := PriorityQueue<int>();

    pq.push(5);
    pq.push(10);
    pq.push(3);
    pq.push(8);

    assert_eq(4, pq.length());
    assert_eq(10, pq.top());  // Max should be at top

    val := pq.pop();
    assert_eq(10, val);

    val = pq.pop();
    assert_eq(8, val);

    val = pq.pop();
    assert_eq(5, val);

    val = pq.pop();
    assert_eq(3, val);

    assert_true(pq.is_empty());
}

// Test min heap with integers
fn test_min_heap_int() {
    pq := PriorityQueue<int>.min();

    pq.push(5);
    pq.push(10);
    pq.push(3);
    pq.push(8);

    assert_eq(4, pq.length());
    assert_eq(3, pq.top());  // Min should be at top

    val := pq.pop();
    assert_eq(3, val);

    val = pq.pop();
    assert_eq(5, val);

    val = pq.pop();
    assert_eq(8, val);

    val = pq.pop();
    assert_eq(10, val);

    assert_true(pq.is_empty());
}

// Test length and is_empty
fn test_length_empty() {
    pq := PriorityQueue<str>();

    assert_true(pq.is_empty());
    assert_eq(0, pq.length());

    pq.push("hello");
    assert_false(pq.is_empty());
    assert_eq(1, pq.length());

    pq.push("world");
    assert_eq(2, pq.length());

    pq.pop();
    assert_eq(1, pq.length());

    pq.pop();
    assert_true(pq.is_empty());
}

// Struct with less_than method for custom comparison
struct Task {
    name: str
    priority: int
}

impl Task {
    // Lower priority value = higher priority (like nice values)
    fn less_than(self, other: Task) -> bool {
        return self.priority < other.priority;
    }
}

// Test max heap with custom struct using less_than
fn test_max_heap_custom() {
    pq := PriorityQueue<Task>();

    pq.push(Task { name: "low", priority: 10 });
    pq.push(Task { name: "high", priority: 1 });
    pq.push(Task { name: "medium", priority: 5 });

    // Max heap: highest priority value first (10)
    task := pq.pop();
    assert_eq("low", task.name);

    task = pq.pop();
    assert_eq("medium", task.name);

    task = pq.pop();
    assert_eq("high", task.name);
}

// Test min heap with custom struct using less_than
fn test_min_heap_custom() {
    pq := PriorityQueue<Task>.min();

    pq.push(Task { name: "low", priority: 10 });
    pq.push(Task { name: "high", priority: 1 });
    pq.push(Task { name: "medium", priority: 5 });

    // Min heap: lowest priority value first (1)
    task := pq.pop();
    assert_eq("high", task.name);

    task = pq.pop();
    assert_eq("medium", task.name);

    task = pq.pop();
    assert_eq("low", task.name);
}

// Test with float values
fn test_float_heap() {
    max_pq := PriorityQueue<f64>();
    max_pq.push(3.14);
    max_pq.push(2.71);
    max_pq.push(1.41);

    assert_eq(3.14, max_pq.top());

    min_pq := PriorityQueue<f64>.min();
    min_pq.push(3.14);
    min_pq.push(2.71);
    min_pq.push(1.41);

    assert_eq(1.41, min_pq.top());
}

// Test top does not remove element
fn test_top_no_remove() {
    pq := PriorityQueue<int>();
    pq.push(42);

    assert_eq(42, pq.top());
    assert_eq(1, pq.length());  // Still has the element

    assert_eq(42, pq.top());
    assert_eq(1, pq.length());  // Still has the element
}
