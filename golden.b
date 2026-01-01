/// Golden file demonstrating all Bishop language features
/// This file should compile and run successfully

// ============================================================
// Imports
// ============================================================

import fs;
import crypto;
import math;
import time;
import random;
import algo;
import json;
import log;
import regex;
import process;

// ============================================================
// Constants
// ============================================================

const int MAX_SIZE = 100;
const str APP_NAME = "Bishop";
const MAX := 100;
const NAME := "Golden";

// ============================================================
// Structs
// ============================================================

Person :: struct {
    name str,
    age int
}

Point :: struct {
    x int,
    y int
}

Counter :: struct {
    value int
}

Calculator :: struct {
    value int
}

// ============================================================
// Instance Methods
// ============================================================

Person :: get_name(self) -> str {
    return self.name;
}

Person :: set_age(self, int new_age) {
    self.age = new_age;
}

Person :: is_adult(self) -> bool {
    if self.age >= 18 {
        return true;
    }

    return false;
}

Point :: sum(self) -> int {
    return self.x + self.y;
}

// ============================================================
// Static Methods
// ============================================================

Counter :: create() -> Counter {
    return Counter { value: 0 };
}

Counter :: add(int a, int b) -> int {
    return a + b;
}

Calculator :: add(int a, int b) -> int {
    return a + b;
}

Calculator :: compute(self, int x) -> int {
    return add(self.value, x);
}

Calculator :: double_add(int a, int b) -> int {
    return add(a, b) * 2;
}

Calculator :: triple_add(int a, int b) -> int {
    return Calculator.add(a, b) * 3;
}

// ============================================================
// Pass by Reference
// ============================================================

fn set_age(Person *p, int new_age) {
    p.age = new_age;
}

// ============================================================
// Functions
// ============================================================

fn add(int a, int b) -> int {
    return a + b;
}

fn multiply(int a, int b) -> int {
    return a * b;
}

fn greet() {
    print("Hello");
}

fn apply_op(int x, int y, fn(int, int) -> int op) -> int {
    return op(x, y);
}

// ============================================================
// Error Types
// ============================================================

ParseError :: err;

IOError :: err {
    code int,
    path str
}

ConfigError :: err;

// ============================================================
// Fallible Functions
// ============================================================

fn always_succeeds() -> int or err {
    return 42;
}

fn always_fails() -> int or err {
    fail "always fails";
}

fn fails_with_io() -> int or err {
    fail IOError { message: "not found", code: 404, path: "/missing" };
}

fn conditional_fail(bool should_fail) -> int or err {
    if should_fail {
        fail "conditional failure";
    }

    return 100;
}

// ============================================================
// Resource Management
// ============================================================

Resource :: struct {
    name str
}

Resource :: close(self) {
    print("Closing resource");
}

fn create_resource(str name) -> Resource {
    return Resource { name: name };
}

// ============================================================
// Main Entry Point
// ============================================================

fn main() {
    print("Running Bishop Golden File");
    print("==========================");

    // Primitive types
    int x = 42;
    str name = "Chris";
    bool flag = true;
    f64 pi = 3.14159;
    f32 small = 1.5;
    u32 unsigned = 100;
    u64 big_unsigned = 1000000;

    // Type inference
    inferred_int := 100;
    inferred_str := "Hello";
    inferred_float := 3.14;
    print("Primitives: OK");

    // Constants
    c := MAX_SIZE + MAX;
    print("Constants: OK");

    // Optionals
    int? maybe_num = none;
    int? value = 42;

    if value is none {
        print("value is none");
    }

    if value {
        print("Optionals: OK");
    }

    // Structs and methods
    p := Person { name: "Chris", age: 32 };
    p.get_name();
    p.is_adult();
    p.set_age(33);

    // Static methods
    counter := Counter.create();
    Counter.add(10, 20);

    // Unqualified static calls
    calc := Calculator { value: 5 };
    calc.compute(10);
    Calculator.double_add(3, 4);
    Calculator.triple_add(3, 4);
    print("Structs: OK");

    // Pointers
    bob := Person { name: "Bob", age: 25 };
    set_age(&bob, 26);
    print("Pointers: OK");

    // Functions and function references
    add(3, 4);
    multiply(3, 4);
    result := apply_op(3, 4, add);
    print("Functions: OK");

    // Lambdas
    doubler := fn(int x) -> int { return x * 2; };
    result = doubler(21);

    multiplier := 10;
    scale := fn(int x) -> int { return x * multiplier; };
    result = scale(4);
    print("Lambdas: OK");

    // Control flow
    if x > 5 {
        print("If/Else: OK");
    }

    // Loops
    i := 0;
    sum := 0;

    while i < 5 {
        sum = sum + i;
        i = i + 1;
    }

    sum = 0;

    for i in 0..5 {
        sum = sum + i;
    }

    nums := [1, 2, 3, 4, 5];
    sum = 0;

    for n in nums {
        sum = sum + n;
    }

    print("Loops: OK");

    // Strings
    str greeting = "Hello, World!";
    str_len := greeting.length();
    has_world := greeting.contains("World");
    starts_hello := greeting.starts_with("Hello");
    ends_bang := greeting.ends_with("!");
    upper_greeting := greeting.upper();
    lower_greeting := greeting.lower();
    sub_greeting := greeting.substr(0, 5);

    csv := "a,b,c";
    parts := csv.split(",");
    joined := parts.join("-");

    hello_twice := "hello hello";
    replaced := hello_twice.replace("hello", "world");
    replaced_all := hello_twice.replace_all("hello", "world");

    spaced := "  spaces  ";
    trimmed := spaced.trim();

    // Raw strings
    pattern := r"\d+\.\d+";
    path := r"C:\Users\name";
    print("Strings: OK");

    // Lists
    list_nums := List<int>();
    names := ["a", "b", "c"];

    list_nums.append(42);
    list_nums.append(10);
    list_nums.append(20);
    list_len := list_nums.length();
    list_get := list_nums.get(0);
    list_first := list_nums.first();
    list_last := list_nums.last();
    list_contains := list_nums.contains(42);
    popped := list_nums.pop();
    names_joined := names.join(" ");
    print("Lists: OK");

    // Pairs
    pair := Pair<int>(10, 20);
    pair_x := pair.first;
    pair_y := pair.second;
    pair_val := pair.get(0) default 0;
    pair_oob := pair.get(5) default 99;
    print("Pairs: OK");

    // Tuples
    t := Tuple<int>(10, 20, 30);
    t0 := t.get(0) default 0;
    t1 := t.get(1) default 0;
    t2 := t.get(2) default 0;
    print("Tuples: OK");

    // Error handling - or return
    val := always_succeeds() or return;
    print("Or Return: OK");

    // Error handling - or match
    matched_val := fails_with_io() or match err {
        IOError => 100,
        ParseError => 200,
        _ => 0
    };
    print("Or Match: OK");

    // Error handling - or continue
    items := [1, 2, 3];
    sum = 0;

    for item in items {
        res := conditional_fail(item == 2) or continue;
        sum = sum + res;
    }

    print("Or Continue: OK");

    // Default keyword
    zero := 0 default 100;
    nonzero := 42 default 100;
    b := false default true;
    print("Default: OK");

    // Math
    math.abs(-5.5);
    math.sqrt(16.0);
    math.pow(2.0, 3.0);
    math.floor(3.7);
    math.ceil(3.2);
    math.min(3.0, 7.0);
    math.max(3.0, 7.0);
    pi_const := math.PI;
    e_const := math.E;
    print("Math: OK");

    // Crypto
    encoded := crypto.base64_encode("Hello!");
    decoded := crypto.base64_decode(encoded) or return;
    hash := crypto.sha256("hello") or return;
    md5_hash := crypto.md5("hello") or return;
    id := crypto.uuid() or return;
    print("Crypto: OK");

    // Time
    d := time.seconds(90);
    d = time.minutes(5);
    d = time.hours(2);
    now := time.now();
    year := now.year;
    month := now.month;
    day := now.day;
    future := now + time.days(7);
    formatted := now.format("%Y-%m-%d");
    print("Time: OK");

    // Random
    random.seed(42);
    dice := random.int(1, 6);
    chance := random.float();
    coin := random.bool();
    rand_items := ["a", "b", "c"];
    pick := random.choice(rand_items) or return;
    print("Random: OK");

    // Algo
    algo_nums := [3, 1, 4, 1, 5];
    algo.sum_int(algo_nums);
    algo.min_int(algo_nums) or return;
    doubled := algo.map_int(algo_nums, fn(int x) -> int { return x * 2; });
    large := algo.filter_int(algo_nums, fn(int x) -> bool { return x > 2; });
    algo.all_int(algo_nums, fn(int x) -> bool { return x > 0; });
    algo.any_int(algo_nums, fn(int x) -> bool { return x > 3; });
    print("Algo: OK");

    // JSON
    data := json.parse('{"name": "Alice", "age": 30}') or return;
    json_name := data.get_str("name") or return;

    obj := json.object();
    obj.set_str("name", "Charlie");
    obj.set_int("age", 25);
    json_str := json.stringify(obj);
    pretty := json.stringify_pretty(obj);
    print("JSON: OK");

    // Regex
    re := regex.compile(r"(\d+)-(\d+)") or return;
    re.contains("abc123-456def");

    m := re.find("Price: 100-200");

    if m.found() {
        match_text := m.text;
        match_group := m.group(1);
    }

    re.replace("123-456", "$2-$1");
    print("Regex: OK");

    // Log
    log.set_level(log.INFO);
    log.info("Golden file running");
    print("Log: OK");

    // With statement
    with create_resource("myfile") as res {
        res_name := res.name;
    }

    print("With: OK");

    print("==========================");
    print("All features passed!");
}
