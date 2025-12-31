# Bishop

Bishop is a modern programming language that transpiles to C++. It combines the power of C++ with an elegant, Python-inspired syntax designed for readability and ease of use.

## Quick Start

```bash
# Build the compiler
make

# Run a Bishop program
./bishop run examples/serve.b

# Run tests
./bishop test tests/
```

## File Extension

Bishop source files use the `.b` extension.

---

# Language Reference

## Types

### Primitive Types

| Type   | Description              |
|--------|--------------------------|
| `int`  | Integer                  |
| `str`  | String                   |
| `bool` | Boolean (`true`/`false`) |
| `f32`  | 32-bit float             |
| `f64`  | 64-bit float             |
| `u32`  | Unsigned 32-bit integer  |
| `u64`  | Unsigned 64-bit integer  |

### Optional Types

Any type can be made optional by appending `?`:

```bishop
int? maybe_num = none;
int? value = 42;
```

Check for none:

```bishop
if value is none { }
if value { }  // truthy check for non-none
```

## Variables

### Explicit Type Declaration

```bishop
int x = 42;
str name = "Chris";
bool flag = true;
f64 pi = 3.14159;
```

### Type Inference

Use `:=` for type inference:

```bishop
x := 100;        // inferred as int
name := "Hello"; // inferred as str
pi := 3.14;      // inferred as f64
```

### Constants

Use `const` to declare immutable values:

```bishop
const int MAX_SIZE = 100;
const str APP_NAME = "MyApp";
const f64 PI = 3.14159;
```

Constants with type inference:

```bishop
const MAX := 100;           // inferred as int
const NAME := "Bishop";     // inferred as str
const RATE := 0.05;         // inferred as f64
```

Constants cannot be reassigned:

```bishop
const int X = 42;
X = 100;  // ERROR: cannot assign to const variable 'X'
```

### Module-Level Constants

Constants can be declared at the module level (outside of functions):

```bishop
// config.b
const int MAX_CONNECTIONS = 100;
const str VERSION = "1.0.0";

fn get_max_connections() -> int {
    return MAX_CONNECTIONS;  // access module-level const
}
```

Module-level constants can be accessed from other modules using qualified names:

```bishop
// main.b
import config;

fn main() {
    print(config.MAX_CONNECTIONS);  // 100
    print(config.VERSION);          // "1.0.0"
}
```

## Functions

### Basic Function

```bishop
fn add(int a, int b) -> int {
    return a + b;
}
```

### Void Function

Functions without a return type return nothing:

```bishop
fn greet() {
    print("Hello");
}
```

### Function References

Functions can be passed as arguments:

```bishop
fn add(int a, int b) -> int {
    return a + b;
}

fn apply_op(int x, int y, fn(int, int) -> int op) -> int {
    return op(x, y);
}

result := apply_op(3, 4, add);  // result = 7
```

### Anonymous Functions (Lambdas)

Anonymous functions can be defined inline using `fn` without a name:

```bishop
// Basic anonymous function
doubler := fn(int x) -> int { return x * 2; };
result := doubler(21);  // 42

// No parameters
get_value := fn() -> int { return 42; };

// Void return (no return type)
action := fn() { print("Hello"); };

// Multiple parameters
add := fn(int a, int b) -> int { return a + b; };
```

#### Lambdas as Arguments

Pass lambdas directly to higher-order functions:

```bishop
fn apply_op(int x, int y, fn(int, int) -> int op) -> int {
    return op(x, y);
}

result := apply_op(3, 4, fn(int a, int b) -> int { return a + b; });  // 7
```

#### Closures

Lambdas capture variables from their enclosing scope:

```bishop
multiplier := 10;
scale := fn(int x) -> int { return x * multiplier; };
result := scale(4);  // 40
```

#### Lambdas with Goroutines

Use lambdas with `go` for concurrent execution:

```bishop
ch := Channel<int>();

go fn() {
    ch.send(42);
}();

val := ch.recv();  // 42
```

## Structs

### Definition

```bishop
Person :: struct {
    name str,
    age int
}
```

### Instantiation

```bishop
p := Person { name: "Chris", age: 32 };
```

### Field Access

```bishop
print(p.name);
p.age = 33;
```

### Pass by Reference

Structs can be passed by reference using pointer syntax. Pointers are only for pass-by-reference and cannot be stored in variables.

Function parameters use `*` to take a reference:

```bishop
fn set_age(Person *p, int new_age) {
    p.age = new_age;  // auto-deref, always mutable
}
```

Call site uses `&` to pass by reference:

```bishop
bob := Person { name: "Bob", age: 25 };
set_age(&bob, 26);
assert_eq(bob.age, 26);  // mutation visible
```

What's NOT supported:
- `Person p = &bob` - no storing pointers in variables
- `p := &bob` - no inferred pointer variables
- `int *p` - no primitive pointers
- `*p = value` - no dereference syntax
- Pointer arithmetic

## Methods

### Definition

Methods are defined with `StructName :: method_name(self, ...)`:

```bishop
Person :: get_name(self) -> str {
    return self.name;
}

Person :: set_age(self, int new_age) {
    self.age = new_age;
}

Person :: birth_year(self, int current_year) -> int {
    return current_year - self.age;
}
```

### Calling Methods

```bishop
p := Person { name: "Chris", age: 32 };
p.get_name();           // "Chris"
p.set_age(33);          // mutates p.age
p.birth_year(2025);     // 1993
```

### Static Methods

Static methods belong to the type itself rather than an instance. Define them with the `@static` decorator:

```bishop
Counter :: struct {
    value int
}

@static
Counter :: create() -> Counter {
    return Counter { value: 0 };
}

@static
Counter :: from_value(int val) -> Counter {
    return Counter { value: val };
}

@static
Counter :: add(int a, int b) -> int {
    return a + b;
}
```

Call static methods on the type name:

```bishop
c := Counter.create();           // Counter { value: 0 }
c := Counter.from_value(42);     // Counter { value: 42 }
result := Counter.add(10, 20);   // 30
```

Static methods have no `self` parameter and cannot access instance fields. They're useful for:
- Factory methods that create instances
- Utility functions related to a type
- Namespace grouping of related functions

## Control Flow

### If/Else

```bishop
if condition {
    // then
}

if condition {
    // then
} else {
    // else
}
```

### While Loop

```bishop
i := 0;

while i < 5 {
    print(i);
    i = i + 1;
}
```

### For Loops

#### Range-based For

Range is inclusive start, exclusive end (like Python's `range()`):

```bishop
for i in 0..10 {
    print(i);  // prints 0 through 9
}

// With variable bounds
n := 5;

for i in 0..n {
    print(i);
}
```

#### Foreach

Iterate over lists:

```bishop
nums := [1, 2, 3];

for n in nums {
    print(n);
}
```

Works with any `List<T>`.

## Operators

### Arithmetic

`+`, `-`, `*`, `/`

### Comparison

`==`, `!=`, `<`, `>`, `<=`, `>=`

### Logical

`!` (negation)

```bishop
if !flag { }
if !fs.exists(path) { }
```

### String Concatenation

```bishop
s := "hello" + " " + "world";
```

## Strings

### String Literals

Strings can be defined using either double quotes or single quotes:

```bishop
str greeting = "Hello, World!";
str name = 'Alice';
```

Single-quoted strings are useful when the string contains double quotes:

```bishop
// JSON with embedded quotes - no escaping needed
json := '{"name": "Alice", "age": 30}';

// Equivalent using double quotes (requires escaping)
json := "{\"name\": \"Alice\", \"age\": 30}";
```

Both quote styles produce the same `str` type.

### Raw String Literals

Raw strings are prefixed with `r` and do not process escape sequences. Backslashes are kept as literal characters, making them ideal for regex patterns and file paths:

```bishop
// Raw double-quoted
pattern := r"\d+\.\d+";     // Contains literal backslash-d, backslash-dot
path := r"C:\Users\name";   // Windows path with literal backslashes

// Raw single-quoted
regex := r'\w+@\w+\.\w+';   // Email pattern
```

Raw strings are especially useful with the regex module:

```bishop
import regex;
re := regex.compile(r"(\d{4})-(\d{2})-(\d{2})") or return;  // Date pattern
```

### String Methods

```bishop
s := "hello world";

// Query methods
s.length();              // -> int: 11
s.empty();               // -> bool: false
s.contains("world");     // -> bool: true
s.starts_with("hello");  // -> bool: true
s.ends_with("world");    // -> bool: true
s.substr(0, 5);          // -> str: "hello"
s.at(0);                 // -> str: "h"
s.find("world");         // -> int: index of substring

// Case transformation
s.upper();               // -> str: "HELLO WORLD"
s.lower();               // -> str: "hello world"
s.capitalize();          // -> str: "Hello world"
s.title();               // -> str: "Hello World"

// Trimming
padded := "  hello  ";
padded.trim();           // -> str: "hello"
padded.trim_left();      // -> str: "hello  "
padded.trim_right();     // -> str: "  hello"

// Replace
s.replace("world", "there");      // -> str: "hello there" (first only)
s.replace_all("l", "L");          // -> str: "heLLo worLd" (all)

// Reverse and repeat
s.reverse();             // -> str: "dlrow olleh"
"ab".repeat(3);          // -> str: "ababab"

// Split
s.split(" ");            // -> List<str>: ["hello", "world"]
"a\nb\nc".split_lines(); // -> List<str>: ["a", "b", "c"]

// Padding
"hi".pad_left(5);        // -> str: "   hi"
"hi".pad_left(5, "0");   // -> str: "000hi"
"hi".pad_right(5);       // -> str: "hi   "
"hi".pad_right(5, ".");  // -> str: "hi..."
"hi".center(6);          // -> str: "  hi  "
"hi".center(6, "-");     // -> str: "--hi--"

// Conversions
"42".to_int();           // -> int: 42
"3.14".to_float();       // -> f64: 3.14
```

## Lists

### List Creation

```bishop
nums := List<int>();           // empty list
names := List<str>();          // empty string list
points := List<Point>();       // list of structs
```

### List Literals

```bishop
nums := [1, 2, 3];             // inferred as List<int>
names := ["a", "b", "c"];      // inferred as List<str>
flags := [true, false];        // inferred as List<bool>
```

### Typed Declaration

```bishop
List<int> nums = [1, 2, 3];
List<str> names = List<str>();
```

### List Methods

```bishop
nums := [10, 20, 30];

// Query methods
nums.length();           // -> int: 3
nums.is_empty();         // -> bool: false
nums.contains(20);       // -> bool: true
nums.get(0);             // -> int: 10 (bounds-checked)
nums.first();            // -> int: 10
nums.last();             // -> int: 30

// Modification methods
nums.append(40);         // add element to end
nums.pop();              // remove last element
nums.set(1, 99);         // replace element at index
nums.clear();            // remove all elements
nums.insert(1, 15);      // insert element at index
nums.remove(1);          // remove element at index

// String list methods (List<str> only)
parts := ["hello", "world"];
parts.join(" ");         // -> str: "hello world"
parts.join("-");         // -> str: "hello-world"
```

## Pairs

Pairs hold exactly two values of the same type.

### Pair Creation

```bishop
p := Pair<int>(1, 2);
names := Pair<str>("hello", "world");
```

### Pair Field Access

```bishop
p := Pair<int>(10, 20);
x := p.first;   // 10
y := p.second;  // 20
```

### Pair get() with default

```bishop
p := Pair<int>(10, 20);
x := p.get(0) default 0;   // 10 (first element)
y := p.get(1) default 0;   // 20 (second element)
z := p.get(2) default 99;  // 99 (out of bounds, uses default)
```

### Pair as Return Type

```bishop
fn divide(int a, int b) -> Pair<int> {
    quotient := a / b;
    remainder := a - quotient * b;
    return Pair<int>(quotient, remainder);
}

result := divide(17, 5);
quotient := result.first;   // 3
remainder := result.second; // 2
```

## Tuples

Tuples hold 2-5 values of the same type.

### Tuple Creation

```bishop
t2 := Tuple<int>(1, 2);
t3 := Tuple<str>("a", "b", "c");
t5 := Tuple<int>(1, 2, 3, 4, 5);
```

### Tuple Access with get()

```bishop
t := Tuple<int>(10, 20, 30);
x := t.get(0) default 0;   // 10
y := t.get(1) default 0;   // 20
z := t.get(2) default 0;   // 30
out := t.get(10) default 99;  // 99 (out of bounds)
```

### Tuple with Variable Index

```bishop
t := Tuple<str>("a", "b", "c");
idx := 2;
val := t.get(idx) default "fallback";  // "c"
```

### Tuple in Loops

```bishop
t := Tuple<int>(1, 2, 3, 4, 5);
sum := 0;

for i in 0..5 {
    sum = sum + (t.get(i) default 0);
}
// sum is 15
```

## Error Handling

### Error Types

Define custom error types using `:: err` syntax:

```bishop
// Simple error (message only)
ParseError :: err;
NetworkError :: err;

// Error with custom fields
IOError :: err {
    code int,
    path str
}
```

All errors automatically have:
- `message str` - Error message (always present)
- `cause err?` - Optional wrapped error for chaining
- `root_cause` - Getter that returns the bottom-most error in a chain

### Fallible Functions

Functions that can fail use `-> T or err` return syntax:

```bishop
fn read_config(str path) -> Config or err {
    if !fs.exists(path) {
        fail IOError { message: "not found", code: 404, path: path };
    }

    content := fs.read_file(path);
    return parse(content) or fail ParseError { message: "invalid", cause: err };
}
```

### The `fail` Keyword

Use `fail` to return an error:

```bishop
fail "simple error message";
fail ParseError { message: "bad format" };
fail IOError { message: "not found", code: 404, path: path };
```

### The `or` Keyword

Handle errors using `or`:

```bishop
// Return early (void function)
x := fallible() or return;

// Return early with value
x := fallible() or return default_value;

// Propagate error as-is
x := fallible() or fail err;

// Block with error access
x := fallible() or {
    print("Error:", err.message);
    return;
};

// Pattern match on error type
x := fallible() or match err {
    IOError    => default_config,
    ParseError => fail err,
    _          => fail ConfigError { message: "unknown", cause: err }
};

// Continue to next loop iteration on error
for item in items {
    result := process(item) or continue;
    handle(result);
}

// Break out of loop on error
while running {
    data := fetch() or break;
    process(data);
}
```

#### Standalone `or` Statements

The `or` keyword can be used as a standalone statement without assignment. This enables clean guard clause patterns:

```bishop
fn load(str path) -> Data or err {
    // Guard clauses without dummy assignment
    fs.exists(path) or fail "not found";
    validate(path) or fail err;

    content := fs.read_file(path);
    content or fail "empty file";

    return parse(content);
}
```

Standalone `or` works with function calls, method calls, and variables:

### Using `or` with Falsy Values

The `or` keyword also works with falsy expressions (not just fallible functions). Falsy values are: `false`, `0`, and empty string `""`.

```bishop
// Guard clauses with booleans
fn validate(bool exists, bool valid) -> str or err {
    exists or fail "not found";
    valid or fail "not valid";
    return "success";
}

// or return with booleans
fn process(bool flag) -> int {
    flag or return 0;
    return 1;
}

// or continue/break with integers
for i in 0..5 {
    i or continue;    // skip when i is 0
    print(i);
}

values := [1, 2, 0, 4, 5];
for v in values {
    v or break;       // stop at first 0
    print(v);
}
```

This enables guard clause patterns similar to:
```bishop
fn handle_request(bool authenticated, bool authorized) -> Response or err {
    authenticated or fail AuthError { message: "not authenticated" };
    authorized or fail AuthError { message: "not authorized" };
    return process_request();
}
```

### Loop Control: `continue` and `break`

Use `continue` to skip to the next iteration and `break` to exit a loop:

```bishop
for i in 0..10 {
    if i == 3 {
        continue;  // skip 3
    }

    if i == 7 {
        break;     // stop at 7
    }

    print(i);
}
```

Combined with `or` for error handling in loops:

```bishop
// Skip items that fail processing
for item in items {
    result := process(item) or continue;
    save(result);
}

// Stop on first error
while running {
    conn := server.accept() or break;
    handle(conn);
}
```

### The `default` Keyword

Handle falsy values (0, "", false, none):

```bishop
count := get_count() default 1;
name := get_name() default "unknown";
```

### Combining `default` and `or`

Handle both falsy values and errors:

```bishop
x := fetch() default 1 or fail err;
x := fetch() default 1 or return;
```

Evaluation order:
1. Call the function
2. If **error** -> trigger `or` handler
3. If **success but falsy** -> use `default` value
4. If **success and truthy** -> use the value

### Error Chaining

Wrap errors to add context:

```bishop
fn load_config(str path) -> Config or err {
    content := fs.read_file(path)
        or fail ConfigError { message: "can't read " + path, cause: err };

    return parse(content)
        or fail ConfigError { message: "invalid config", cause: err };
}
```

Access the error chain:

```bishop
config := load_config("app.conf") or {
    print(err.message);              // "can't read app.conf"
    print(err.cause.message);        // underlying error
    print(err.root_cause.message);   // original error
    return;
};
```

## Resource Management

The `with` statement provides automatic resource cleanup. When the block exits, `close()` is called on the resource.

```bishop
Resource :: struct {
    name str
}

Resource :: close(self) {
    print("Closing resource");
}

fn create_resource(str name) -> Resource {
    return Resource { name: name };
}

fn main() {
    with create_resource("myfile") as res {
        print(res.name);
    }  // res.close() called automatically
}
```

Nested with blocks:

```bishop
with create_outer() as outer {
    with create_inner() as inner {
        // both available
    }  // inner.close() called
}  // outer.close() called
```

Notes:
- Types used with `with` must have a `close()` method
- `close()` is called even on early return

## Concurrency

### Channels

Create typed channels for communication between goroutines:

```bishop
ch := Channel<int>();
ch_str := Channel<str>();
ch_bool := Channel<bool>();
```

### Goroutines

Spawn concurrent execution with `go`:

```bishop
fn sender(Channel<int> ch, int val) {
    ch.send(val);
}

fn main() {
    ch := Channel<int>();
    go sender(ch, 42);       // spawn goroutine
    val := ch.recv();        // blocks until value available
    print(val);              // 42
}
```

### Select Statement

Wait on multiple channel operations:

```bishop
ch1 := Channel<int>();
ch2 := Channel<int>();

go sender(ch1, 41);

select {
    case val := ch1.recv() {
        print("received from ch1:", val);
    }
    case val := ch2.recv() {
        print("received from ch2:", val);
    }
}
```

## Standard Library

### HTTP Module

```bishop
import http;
```

#### Request/Response Types

```bishop
http.Request { method: "GET", path: "/test", body: "" }
http.Response { status: 200, content_type: "text/plain", body: "Hello" }
```

#### Response Helpers

```bishop
http.text("Hello");     // 200 OK, text/plain
http.json("{...}");     // 200 OK, application/json
http.not_found();       // 404 Not Found
```

#### Handler Pattern

```bishop
fn handle(http.Request req) -> http.Response {
    return http.text("Hello");
}

fn main() {
    http.serve(8080, handle);
}
```

#### App-based Routing

```bishop
fn home(http.Request req) -> http.Response {
    return http.text("Home");
}

fn about(http.Request req) -> http.Response {
    return http.text("About");
}

fn main() {
    app := http.App {};
    app.get("/", home);
    app.get("/about", about);
    app.listen(8080);
}
```

### Filesystem Module

```bishop
import fs;
```

#### Basic File Operations

```bishop
// Reading files
fs.read_file("path");              // -> str (file contents, empty if not found)
data := fs.read_bytes("path") or fail err;  // -> str or err (binary content)

// Writing files
_ := fs.write_file("path", "content") or fail err;   // -> bool or err
_ := fs.append_file("path", "more") or fail err;     // -> bool or err
_ := fs.write_bytes("path", data) or fail err;       // -> bool or err

// File/directory checks
fs.exists("path");                 // -> bool (true if file or dir exists)
fs.is_dir("path");                 // -> bool (true if path is a directory)
fs.is_file("path");                // -> bool (true if path is a regular file)
```

#### File Handle (with statement)

```bishop
// Open file and use with statement for automatic cleanup
f := fs.open("data.txt", "r") or fail err;

with f as file {
    content := file.read_all() or fail err;
    print(content);
}  // file.close() called automatically

// Modes: "r" (read), "w" (write), "a" (append), "rw" (read+write)
```

File methods:
- `f.read_line()` -> str or err
- `f.read_lines()` -> List<str> or err
- `f.read_all()` -> str or err
- `f.write(data)` -> bool or err
- `f.write_line(data)` -> bool or err
- `f.close()` -> void

#### Directory Operations

```bishop
fs.read_dir("path");               // -> str (newline-separated filenames)
files := fs.list_dir("path") or fail err;   // -> List<str> or err

_ := fs.mkdir("dir") or fail err;            // -> bool or err
_ := fs.mkdir_all("a/b/c") or fail err;      // -> bool or err

_ := fs.remove("file.txt") or fail err;      // -> bool or err (single file)
_ := fs.remove_dir("empty_dir") or fail err; // -> bool or err (empty directory)
_ := fs.remove_all("dir") or fail err;       // -> bool or err (recursive)
```

#### File Operations

```bishop
_ := fs.rename("old.txt", "new.txt") or fail err;  // -> bool or err
_ := fs.copy("src.txt", "dst.txt") or fail err;    // -> bool or err
_ := fs.copy_dir("src_dir", "dst_dir") or fail err; // -> bool or err (recursive)
```

#### Path Operations

```bishop
fs.join("dir", "file.txt");        // -> str ("dir/file.txt")
fs.dirname("/home/user/file.txt"); // -> str ("/home/user")
fs.basename("/home/user/file.txt");// -> str ("file.txt")
fs.extension("file.txt");          // -> str ("txt")
fs.stem("file.txt");               // -> str ("file")
fs.is_absolute("/home/user");      // -> bool (true)
abs := fs.absolute("rel/path") or fail err;   // -> str or err
canon := fs.canonical("path") or fail err;    // -> str or err
```

#### File Info

```bishop
info := fs.stat("file.txt") or fail err;
print(info.size);        // int (file size in bytes)
print(info.modified);    // int (Unix timestamp)
print(info.is_file);     // bool
print(info.is_dir);      // bool
print(info.is_symlink);  // bool

size := fs.file_size("file.txt") or fail err; // -> int or err
```

#### Directory Walking

```bishop
entries := fs.walk("src") or fail err;
for entry in entries {
    if !entry.is_dir {
        print(entry.path);   // full path
        print(entry.name);   // filename only
    }
}
```

#### Temp Files

```bishop
path := fs.temp_file() or fail err;   // -> str or err (unique temp file path)
dir := fs.temp_dir() or fail err;     // -> str or err (creates temp directory)
```

### Crypto Module

Cryptographic utilities for hashing, encoding, and UUID generation. Uses OpenSSL.

```bishop
import crypto;
```

#### Hashing Functions

All hash functions return `str or err` with lowercase hex strings.

```bishop
hash := crypto.md5("hello") or return;     // -> "5d41402abc4b2a76b9719d911017c592"
hash := crypto.sha1("hello") or return;    // -> "aaf4c61ddcc5e8a2dabede0f3b482cd9aea9434d"
hash := crypto.sha256("hello") or return;  // -> "2cf24dba5fb0a30e26e83b2ac5b9e29e..."
hash := crypto.sha512("hello") or return;  // -> "9b71d224bd62f3785d96d46ad3ea3d73..."
```

#### HMAC

```bishop
crypto.hmac_sha256("secret_key", "data");  // -> hex string
```

#### Base64 Encoding/Decoding

```bishop
// Encoding (always succeeds)
encoded := crypto.base64_encode("Hello, World!");
// encoded == "SGVsbG8sIFdvcmxkIQ=="

// Decoding (can fail on invalid input)
decoded := crypto.base64_decode("SGVsbG8h") or return;
```

#### Hex Encoding/Decoding

```bishop
hex := crypto.hex_encode("hello");               // -> "68656c6c6f"
decoded := crypto.hex_decode("68656c6c6f") or return;  // -> "hello"
```

#### UUID Generation

```bishop
// Random UUID v4
id := crypto.uuid() or return;
// e.g., "550e8400-e29b-41d4-a716-446655440000"

// Deterministic UUID v5 (namespace + name based)
id := crypto.uuid_v5("namespace", "name") or return;
// Same inputs always produce same UUID
```

#### Random Bytes

```bishop
bytes := crypto.random_bytes(32) or return;  // -> List<u8> with 32 random bytes
```

### Example: Static File Server

```bishop
import http;
import fs;

fn handle(http.Request req) -> http.Response {
    path := "." + req.path;

    if path == "./" {
        path = "./index.html";
    }

    if !fs.exists(path) {
        return http.not_found();
    }

    if fs.is_dir(path) {
        return http.not_found();
    }

    content := fs.read_file(path);
    return http.text(content);
}

fn main() {
    print("Serving on http://localhost:8080");
    http.serve(8080, handle);
}
```

### Networking Module

```bishop
import net;
```

#### TCP Server

```bishop
server := net.listen("127.0.0.1", 8080) or {
    print("Failed to listen:", err.message);
    return;
};

with server as s {
    while true {
        conn := s.accept() or continue;

        go fn() {
            data := conn.read(1024) or return;
            conn.write("HTTP/1.1 200 OK\r\n\r\nHello") or return;
            conn.close();
        }();
    }
}
```

#### TCP Client

```bishop
conn := net.connect("example.com", 80) or {
    print("Connection failed");
    return;
};
conn.write("GET / HTTP/1.1\r\nHost: example.com\r\n\r\n");
response := conn.read(4096);
print(response);
conn.close();
```

#### UDP

```bishop
// Bind and receive
sock := net.udp_bind("0.0.0.0", 9000) or return;
pkt := sock.recv_from(1024) or return;
print("From:", pkt.addr, ":", pkt.port);
print(pkt.data);

// Send to specific address
sock.send_to("hello", "192.168.1.1", 9000) or return;
sock.close();

// Connected UDP
conn_sock := net.udp_connect("192.168.1.1", 9000) or return;
conn_sock.send("hello") or return;
data := conn_sock.recv(1024) or return;
conn_sock.close();
```

#### DNS

```bishop
// Forward lookup
addrs := net.resolve("example.com") or return;

for addr in addrs {
    print(addr);
}

// Reverse lookup
hostname := net.reverse_lookup("93.184.216.34") or return;
print(hostname);
```

#### net.TcpListener Methods

| Method | Description |
|--------|-------------|
| `accept() -> net.TcpStream or err` | Accept incoming connection |
| `close()` | Close the listener |

#### net.TcpStream Methods

| Method | Description |
|--------|-------------|
| `read(int n) -> str or err` | Read up to n bytes |
| `read_exact(int n) -> str or err` | Read exactly n bytes |
| `read_line() -> str or err` | Read a line (up to newline) |
| `write(str data) -> int or err` | Write data, returns bytes written |
| `flush() -> bool or err` | Flush buffered data |
| `close()` | Close the connection |
| `set_timeout(int ms)` | Set read/write timeout in milliseconds |

#### net.UdpSocket Methods

| Method | Description |
|--------|-------------|
| `send_to(str data, str host, int port) -> int or err` | Send to specific address |
| `recv_from(int n) -> net.UdpPacket or err` | Receive with sender info |
| `send(str data) -> int or err` | Send to connected address |
| `recv(int n) -> str or err` | Receive from connected address |
| `close()` | Close the socket |

#### net.UdpPacket Fields

| Field | Type | Description |
|-------|------|-------------|
| `data` | `str` | Received data |
| `addr` | `str` | Sender IP address |
| `port` | `int` | Sender port |

### Process Module

```bishop
import process;
```

#### ProcessResult Struct

```bishop
process.ProcessResult {
    output str,      // Standard output from the process
    error str,       // Standard error from the process
    exit_code int,   // Exit code of the process
    success bool     // True if exit code is 0
}
```

#### Command Execution

```bishop
// Execute a command with arguments
result := process.run("ls", ["-la"]) or {
    print("Command failed:", err.message);
    return;
};
print(result.output);
print("Exit code:", result.exit_code);

if result.success {
    print("Command succeeded");
}

// Execute a shell command (supports pipes and redirects)
result := process.shell("ls -la | grep txt") or return;
print(result.output);
```

#### Environment Variables

```bishop
// Get environment variable (fails if not found)
home := process.env("HOME") or fail err;

// Get environment variable with default fallback
token := process.env("API_TOKEN") or "default_value";

// Require environment variable with graceful failure
token := process.env("GH_TOKEN") or {
    print("Error: GH_TOKEN environment variable is not set");
    return;
};

// Set environment variable (fails if name is empty or system error)
process.set_env("MY_VAR", "value") or fail err;
```

#### Working Directory

```bishop
// Get current working directory (fails if directory was deleted or permission denied)
dir := process.cwd() or fail err;
print(dir);

// Change working directory
process.chdir("/new/dir") or {
    print("Failed to change directory");
    return;
};
```

#### Command Line Arguments

```bishop
// Get command line arguments as List<str>
args := process.args();

for arg in args {
    print(arg);
}
```

### Regex Module

Regular expression support for pattern matching, searching, and text replacement.

```bishop
import regex;
```

#### Compiling Patterns

```bishop
// Compile a regex pattern (fallible - pattern could be invalid)
re := regex.compile(r"(\d+)-(\d+)") or {
    print("Invalid pattern:", err.message);
    return;
};
```

#### Matching

```bishop
// Full match - entire string must match pattern
re := regex.compile(r"\d+") or return;
re.matches("123");       // true
re.matches("abc123");    // false (not full match)

// Partial match - pattern found anywhere in string
re.contains("abc123");   // true
re.contains("abc");      // false
```

#### Finding Matches

```bishop
re := regex.compile(r"(\d+)-(\d+)") or return;

// Find first match
m := re.find("Price: 100-200 dollars");
if m.found() {
    print(m.text);       // "100-200"
    print(m.start);      // 7
    print(m.end);        // 14
    print(m.group(1));   // "100"
    print(m.group(2));   // "200"
}

// Find all matches
matches := re.find_all("A: 1-2, B: 3-4");
for m in matches {
    print(m.text);       // "1-2", then "3-4"
}
```

#### Replacement

Replacement strings support capture group references:
- `$0` - Full match
- `$1` to `$99` - Capture groups
- `$$` - Literal dollar sign

```bishop
re := regex.compile(r"(\d+)-(\d+)") or return;

// Replace first match (supports $0-$99 capture group references)
re.replace("123-456", "$2-$1");      // "456-123"
re.replace("123-456", "[$0]");       // "[123-456]"

// Replace all matches
re := regex.compile(r"\d+") or return;
re.replace_all("a1b2c3", "X");       // "aXbXcX"

// Use $$ for literal dollar sign
re.replace("price: 100", "$$");      // "price: $"
```

#### Splitting

```bishop
// Split by regex pattern (fallible - pattern could be invalid)
parts := regex.split(r"\s+", "a  b   c") or return;
// parts == ["a", "b", "c"]

// Split by character class
parts := regex.split(r"[,;]", "a,b;c") or return;
// parts == ["a", "b", "c"]
```

#### regex.Match Fields and Methods

| Field/Method | Type | Description |
|--------------|------|-------------|
| `text` | `str` | Matched text (empty if no match) |
| `start` | `int` | Start index (-1 if no match) |
| `end` | `int` | End index (-1 if no match) |
| `groups` | `List<str>` | All capture groups (index 0 is full match) |
| `found()` | `bool` | Returns true if match was found |
| `group(int n)` | `str` | Get capture group by index (empty if out of bounds) |

#### regex.Regex Methods

| Method | Description |
|--------|-------------|
| `matches(str) -> bool` | True if entire string matches pattern |
| `contains(str) -> bool` | True if pattern found anywhere |
| `find(str) -> regex.Match` | First match (or empty Match) |
| `find_all(str) -> List<regex.Match>` | All matches |
| `replace(str, str) -> str` | Replace first match |
| `replace_all(str, str) -> str` | Replace all matches |

#### Module Functions

| Function | Description |
|----------|-------------|
| `regex.compile(str) -> regex.Regex or err` | Compile pattern |
| `regex.split(str, str) -> List<str> or err` | Split text by pattern |

### Math Module

```bishop
import math;
```

#### Constants

| Constant | Type | Description |
|----------|------|-------------|
| `math.PI` | `f64` | Pi (3.14159265358979...) |
| `math.E` | `f64` | Euler's number (2.71828182845904...) |
| `math.INF` | `f64` | Positive infinity |
| `math.NAN` | `f64` | Not a Number |

#### Basic Operations

```bishop
// Absolute value
x := math.abs(-5.5);           // 5.5
y := math.abs_int(-5);         // 5

// Min/max
smaller := math.min(3.0, 7.0);         // 3.0
smaller_int := math.min_int(3, 7);     // 3
larger := math.max(3.0, 7.0);          // 7.0
larger_int := math.max_int(3, 7);      // 7

// Clamp (constrain value to range)
c := math.clamp(15.0, 0.0, 10.0);          // 10.0
c_int := math.clamp_int(15, 0, 10);        // 10
```

#### Rounding Functions

```bishop
f := math.floor(3.7);      // 3.0 (round down)
c := math.ceil(3.2);       // 4.0 (round up)
r := math.round(3.5);      // 4.0 (round to nearest)
t := math.trunc(-3.7);     // -3.0 (round toward zero)
```

#### Power and Root Functions

```bishop
p := math.pow(2.0, 10.0);      // 1024.0 (exponentiation)
s := math.sqrt(16.0);          // 4.0 (square root)
cb := math.cbrt(27.0);         // 3.0 (cube root)
e := math.exp(1.0);            // 2.718... (e^x)
ln := math.log(math.E);        // 1.0 (natural log)
log10_val := math.log10(100.0); // 2.0 (base-10 log)
log2_val := math.log2(8.0);    // 3.0 (base-2 log)
```

#### Trigonometric Functions

```bishop
// All angles in radians
s := math.sin(math.PI / 2.0);  // 1.0
c := math.cos(0.0);            // 1.0
t := math.tan(math.PI / 4.0);  // 1.0

// Inverse trig functions
asin_val := math.asin(1.0);           // PI/2
acos_val := math.acos(1.0);           // 0.0
atan_val := math.atan(1.0);           // PI/4
atan2_val := math.atan2(1.0, 1.0);    // PI/4 (y/x with quadrant)
```

#### Hyperbolic Functions

```bishop
s := math.sinh(0.0);   // 0.0
c := math.cosh(0.0);   // 1.0
t := math.tanh(0.0);   // 0.0
```

#### Utility Functions

```bishop
// Check for special values
if math.is_nan(result) { print("Invalid"); }
if math.is_inf(result) { print("Infinite"); }
if math.is_finite(result) { print("Valid number"); }

// Sign of a number (-1, 0, or 1)
s_neg := math.sign(-5.0);   // -1
s_zero := math.sign(0.0);   // 0
s_pos := math.sign(5.0);    // 1

// Greatest common divisor
g := math.gcd(12, 8);  // 4

// Least common multiple
l := math.lcm(4, 6);   // 12
```

#### Example: Circle Calculations

```bishop
import math;

fn circle_area(f64 radius) -> f64 {
    return math.PI * radius * radius;
}

fn circle_circumference(f64 radius) -> f64 {
    return 2.0 * math.PI * radius;
}

fn main() {
    radius := 5.0;
    print("Area:", circle_area(radius));
    print("Circumference:", circle_circumference(radius));
}
```

### Time Module

```bishop
import time;
```

#### Duration

Duration represents a time span. Create durations using module functions:

```bishop
// Duration construction
d := time.millis(5000);    // 5000 milliseconds
d := time.seconds(90);     // 90 seconds
d := time.minutes(5);      // 5 minutes
d := time.hours(2);        // 2 hours
d := time.days(1);         // 1 day

// Conversion methods
print(d.as_millis());      // Duration in milliseconds
print(d.as_seconds());     // Duration in seconds
print(d.as_minutes());     // Duration in minutes
print(d.as_hours());       // Duration in hours
print(d.as_days());        // Duration in days

// Duration arithmetic
total := time.hours(1) + time.minutes(30);  // 90 minutes
diff := time.hours(2) - time.minutes(30);   // 90 minutes

// Duration comparison
if time.hours(1) > time.minutes(30) {
    print("1 hour is longer than 30 minutes");
}
d1 := time.seconds(60);
d2 := time.minutes(1);
assert_eq(true, d1 == d2);  // same duration
```

#### Timestamp

Timestamp represents a point in time with broken-down components:

```bishop
// Current time
now := time.now();          // Local time
utc := time.now_utc();      // UTC time

// Access components
print(now.year);            // e.g., 2024
print(now.month);           // 1-12
print(now.day);             // 1-31
print(now.hour);            // 0-23
print(now.minute);          // 0-59
print(now.second);          // 0-59
print(now.millisecond);     // 0-999
print(now.weekday);         // 0=Sunday, 6=Saturday

// Unix timestamps
print(now.unix());          // Seconds since epoch
print(now.unix_millis());   // Milliseconds since epoch
```

#### Timestamp Arithmetic

```bishop
// Add duration to timestamp
future := now + time.days(7);
past := now - time.hours(2);

// Subtract timestamps to get duration
elapsed := future - now;
print(elapsed.as_days());   // 7

// Timestamp comparison
if now < future {
    print("now is before future");
}
assert_eq(true, now != future);
```

#### Formatting and Parsing

```bishop
// Format timestamp (strftime specifiers)
formatted := now.format("%Y-%m-%d %H:%M:%S");
print(formatted);  // "2024-01-15 14:30:00"

// Parse timestamp
ts := time.parse("2024-01-15", "%Y-%m-%d") or fail err;
ts := time.parse("2024-01-15 14:30:00", "%Y-%m-%d %H:%M:%S") or fail err;
```

#### Measuring Elapsed Time

```bishop
// Measure execution time
start := time.now();
// ... work ...
elapsed := time.since(start);
print(elapsed.as_millis());

// Alternative using subtraction
elapsed := time.now() - start;
print(elapsed.as_millis());
```

#### time.Duration Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `as_millis()` | `int` | Duration in milliseconds |
| `as_seconds()` | `int` | Duration in seconds |
| `as_minutes()` | `int` | Duration in minutes |
| `as_hours()` | `int` | Duration in hours |
| `as_days()` | `int` | Duration in days |

#### time.Timestamp Fields

| Field | Type | Description |
|-------|------|-------------|
| `year` | `int` | Year (e.g., 2024) |
| `month` | `int` | Month (1-12) |
| `day` | `int` | Day of month (1-31) |
| `hour` | `int` | Hour (0-23) |
| `minute` | `int` | Minute (0-59) |
| `second` | `int` | Second (0-59) |
| `millisecond` | `int` | Millisecond (0-999) |
| `weekday` | `int` | Day of week (0=Sunday, 6=Saturday) |

#### time.Timestamp Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `unix()` | `int` | Unix timestamp in seconds |
| `unix_millis()` | `int` | Unix timestamp in milliseconds |
| `format(str fmt)` | `str` | Format using strftime specifiers |

#### Module Functions

| Function | Returns | Description |
|----------|---------|-------------|
| `time.millis(int)` | `Duration` | Create duration from milliseconds |
| `time.seconds(int)` | `Duration` | Create duration from seconds |
| `time.minutes(int)` | `Duration` | Create duration from minutes |
| `time.hours(int)` | `Duration` | Create duration from hours |
| `time.days(int)` | `Duration` | Create duration from days |
| `time.now()` | `Timestamp` | Current local time |
| `time.now_utc()` | `Timestamp` | Current UTC time |
| `time.since(Timestamp)` | `Duration` | Elapsed time since timestamp |
| `time.parse(str, str)` | `Timestamp or err` | Parse timestamp from string |

### Random Module

```bishop
import random;
```

#### Random Integers

```bishop
// Random integer in inclusive range [min, max]
dice := random.int(1, 6);
print(dice);  // 1-6
```

#### Random Floats

```bishop
// Random float in [0.0, 1.0)
chance := random.float();

// Random float in [min, max)
temp := random.float_range(-10.0, 40.0);
```

#### Random Booleans

```bishop
// Random boolean with 50/50 probability
coin := random.bool();

// Random boolean with specified probability (10% true)
rare := random.bool_prob(0.1);
```

#### List Operations

```bishop
items := ["a", "b", "c", "d", "e"];

// Random element from string list (fails on empty list)
pick := random.choice(items) or return;

// Random element from int list (fails on empty list)
nums := [10, 20, 30];
num := random.choice_int(nums) or return;

// Shuffle in place
random.shuffle(items);
random.shuffle_int(nums);

// Sample n unique elements
sampled := random.sample(items, 2);
sampled_nums := random.sample_int(nums, 2);
```

#### Seeding for Reproducibility

```bishop
// Seed for deterministic sequences
random.seed(42);
print(random.int(1, 100));  // always same sequence with seed 42
```

#### Random Module Functions

| Function | Description |
|----------|-------------|
| `int(min, max) -> int` | Random integer in [min, max] inclusive |
| `float() -> f64` | Random float in [0.0, 1.0) |
| `float_range(min, max) -> f64` | Random float in [min, max) |
| `bool() -> bool` | Random boolean (50/50) |
| `bool_prob(p) -> bool` | Random boolean with probability p |
| `choice(list) -> str or err` | Random element from string list |
| `choice_int(list) -> int or err` | Random element from int list |
| `shuffle(list)` | Shuffle string list in place |
| `shuffle_int(list)` | Shuffle int list in place |
| `sample(list, n) -> List<str>` | Sample n elements from string list |
| `sample_int(list, n) -> List<int>` | Sample n elements from int list |
| `seed(n)` | Seed generator for deterministic sequences |

### Algo Module

```bishop
import algo;
```

Provides algorithms for sorting, searching, aggregation, predicates, and transformations on lists.

#### Sorting (in-place)

```bishop
nums := [3, 1, 4, 1, 5];
algo.sort_int(nums);       // [1, 1, 3, 4, 5]
algo.sort_desc_int(nums);  // [5, 4, 3, 1, 1]

names := ["charlie", "alice", "bob"];
algo.sort_str(names);      // ["alice", "bob", "charlie"]
algo.sort_desc_str(names); // ["charlie", "bob", "alice"]

vals := [3.14, 1.41, 2.72];
algo.sort_float(vals);       // [1.41, 2.72, 3.14]
algo.sort_desc_float(vals);  // [3.14, 2.72, 1.41]
```

#### Min/Max

```bishop
nums := [3, 1, 4, 1, 5];
min_val := algo.min_int(nums) or return;  // 1
max_val := algo.max_int(nums) or return;  // 5

vals := [3.14, 1.41, 2.72];
min_f := algo.min_float(vals) or return;  // 1.41
max_f := algo.max_float(vals) or return;  // 3.14

names := ["charlie", "alice", "bob"];
min_s := algo.min_str(names) or return;  // "alice"
max_s := algo.max_str(names) or return;  // "charlie"
```

#### Aggregation

```bishop
nums := [1, 2, 3, 4, 5];
total := algo.sum_int(nums);      // 15
prod := algo.product_int(nums);   // 120
avg := algo.average_int(nums) or return;  // 3.0

vals := [1.5, 2.5, 3.0];
total_f := algo.sum_float(vals);      // 7.0
prod_f := algo.product_float(vals);   // 11.25
avg_f := algo.average_float(vals) or return;  // 2.333...
```

#### Predicates (with lambdas)

```bishop
nums := [1, 2, 3, 4, 5];

// Check all elements
all_positive := algo.all_int(nums, fn(int x) -> bool { return x > 0; });  // true

// Check any element
has_large := algo.any_int(nums, fn(int x) -> bool { return x > 10; });  // false

// Check no elements match
no_negative := algo.none_int(nums, fn(int x) -> bool { return x < 0; });  // true

// Count matching elements
count := algo.count_int(nums, fn(int x) -> bool { return x > 2; });  // 3

// Find first matching element
found := algo.find_int(nums, fn(int x) -> bool { return x > 3; }) or return;  // 4

// Find index of first match
idx := algo.find_index_int(nums, fn(int x) -> bool { return x == 3; });  // 2

// String predicates
names := ["alice", "bob", "charlie"];
has_long := algo.any_str(names, fn(str s) -> bool { return s.length() > 5; });  // true
```

#### Transformations

```bishop
nums := [1, 2, 3];

// Map: transform each element
doubled := algo.map_int(nums, fn(int x) -> int { return x * 2; });  // [2, 4, 6]

// Filter: keep matching elements
large := algo.filter_int(nums, fn(int x) -> bool { return x > 1; });  // [2, 3]

// Reduce: accumulate to single value
sum := algo.reduce_int(nums, fn(int a, int b) -> int { return a + b; }, 0);  // 6

// String transformations
names := ["a", "b", "c"];
upper := algo.map_str(names, fn(str s) -> str { return s.upper(); });  // ["A", "B", "C"]
joined := algo.reduce_str(names, fn(str a, str b) -> str { return a + b; }, "");  // "abc"
```

#### Reordering

```bishop
nums := [1, 2, 3, 4, 5];

// Reverse in place
algo.reverse_int(nums);  // [5, 4, 3, 2, 1]

// Get reversed copy
rev := algo.reversed_int(nums);  // returns reversed copy, original unchanged

// Rotate left by n positions
nums := [1, 2, 3, 4, 5];
algo.rotate_int(nums, 2);  // [3, 4, 5, 1, 2]

// Get unique elements (preserves order)
dups := [1, 2, 1, 3, 2];
uniq := algo.unique_int(dups);  // [1, 2, 3]
```

#### Algo Module Functions

| Function | Description |
|----------|-------------|
| `sort_int(list)` | Sort integer list ascending (in-place) |
| `sort_desc_int(list)` | Sort integer list descending (in-place) |
| `sort_str(list)` | Sort string list ascending (in-place) |
| `sort_desc_str(list)` | Sort string list descending (in-place) |
| `sort_float(list)` | Sort float list ascending (in-place) |
| `sort_desc_float(list)` | Sort float list descending (in-place) |
| `min_int(list) -> int or err` | Minimum value in integer list |
| `max_int(list) -> int or err` | Maximum value in integer list |
| `min_float(list) -> f64 or err` | Minimum value in float list |
| `max_float(list) -> f64 or err` | Maximum value in float list |
| `min_str(list) -> str or err` | Minimum string (lexicographic) |
| `max_str(list) -> str or err` | Maximum string (lexicographic) |
| `sum_int(list) -> int` | Sum of integers (0 for empty) |
| `sum_float(list) -> f64` | Sum of floats (0.0 for empty) |
| `product_int(list) -> int` | Product of integers (1 for empty) |
| `product_float(list) -> f64` | Product of floats (1.0 for empty) |
| `average_int(list) -> f64 or err` | Average of integers |
| `average_float(list) -> f64 or err` | Average of floats |
| `all_int(list, fn) -> bool` | True if all satisfy predicate |
| `any_int(list, fn) -> bool` | True if any satisfies predicate |
| `none_int(list, fn) -> bool` | True if none satisfy predicate |
| `count_int(list, fn) -> int` | Count of matching elements |
| `find_int(list, fn) -> int or err` | First matching element |
| `find_index_int(list, fn) -> int` | Index of first match (-1 if none) |
| `all_str(list, fn) -> bool` | True if all strings satisfy predicate |
| `any_str(list, fn) -> bool` | True if any string satisfies predicate |
| `none_str(list, fn) -> bool` | True if no strings satisfy predicate |
| `count_str(list, fn) -> int` | Count of matching strings |
| `find_str(list, fn) -> str or err` | First matching string |
| `find_index_str(list, fn) -> int` | Index of first matching string |
| `map_int(list, fn) -> List<int>` | Transform each integer |
| `map_str(list, fn) -> List<str>` | Transform each string |
| `filter_int(list, fn) -> List<int>` | Keep matching integers |
| `filter_str(list, fn) -> List<str>` | Keep matching strings |
| `reduce_int(list, fn, init) -> int` | Reduce integers to single value |
| `reduce_str(list, fn, init) -> str` | Reduce strings to single value |
| `reverse_int(list)` | Reverse integer list (in-place) |
| `reverse_str(list)` | Reverse string list (in-place) |
| `reversed_int(list) -> List<int>` | Reversed copy of integers |
| `reversed_str(list) -> List<str>` | Reversed copy of strings |
| `rotate_int(list, n)` | Rotate integers left by n (in-place) |
| `rotate_str(list, n)` | Rotate strings left by n (in-place) |
| `unique_int(list) -> List<int>` | Unique integers (order preserved) |
| `unique_str(list) -> List<str>` | Unique strings (order preserved) |

### JSON Module

```bishop
import json;
```

Parse, create, and manipulate JSON data.

#### Parsing JSON

```bishop
// Parse JSON string (fallible)
data := json.parse('{"name": "Alice", "age": 30}') or return;

// Check types
data.is_object();   // true
data.is_list();     // false
data.is_str();      // false
data.is_int();      // false
data.is_float();    // false
data.is_bool();     // false
data.is_null();     // false
```

#### Accessing Values

```bishop
data := json.parse('{"user": {"name": "Bob", "scores": [95, 87, 92]}}') or return;

// Direct field access
name := data.get_str("user") or return;       // fails - user is object
user := data.get_object("user") or return;    // json.Value object
name := user.get_str("name") or return;       // "Bob"

// Typed getters
data.get_str("key") or return;      // -> str or err
data.get_int("key") or return;      // -> int or err
data.get_bool("key") or return;     // -> bool or err
data.get_list("key") or return;     // -> json.Value or err
data.get_object("key") or return;   // -> json.Value or err

// Path access for nested values
city := data.path("user.address.city") or return;
city_str := city.as_str() or return;

// Convert value to native type
val.as_str() or return;    // -> str or err
val.as_int() or return;    // -> int or err
val.as_bool() or return;   // -> bool or err

// Check for key existence
data.has("name");   // -> bool
data.length();      // -> int (object keys or array elements)
data.keys();        // -> List<str> (object keys)
```

#### Creating JSON

```bishop
// Create empty structures
obj := json.object();
arr := json.array();

// Set values on objects
obj.set_str("name", "Charlie");
obj.set_int("age", 25);
obj.set_bool("active", true);
obj.set_float("score", 95.5);
obj.set_null("empty");

// Push values to arrays
arr.push_int(1);
arr.push_str("hello");
arr.push_bool(true);
arr.push_null();

// Remove keys
obj.remove("temp");
```

#### Serialization

```bishop
// Compact JSON string
json_str := json.stringify(obj);

// Pretty-printed JSON
json_str := json.stringify_pretty(obj);
```

#### json.Value Methods

| Method | Description |
|--------|-------------|
| `is_object() -> bool` | Check if value is an object |
| `is_list() -> bool` | Check if value is an array |
| `is_str() -> bool` | Check if value is a string |
| `is_int() -> bool` | Check if value is an integer |
| `is_float() -> bool` | Check if value is a float |
| `is_bool() -> bool` | Check if value is a boolean |
| `is_null() -> bool` | Check if value is null |
| `as_str() -> str or err` | Convert to string |
| `as_int() -> int or err` | Convert to integer |
| `as_bool() -> bool or err` | Convert to boolean |
| `get(key) -> json.Value or err` | Get value by key or index |
| `get_str(key) -> str or err` | Get string field |
| `get_int(key) -> int or err` | Get integer field |
| `get_bool(key) -> bool or err` | Get boolean field |
| `get_list(key) -> json.Value or err` | Get array field |
| `get_object(key) -> json.Value or err` | Get object field |
| `path(str) -> json.Value or err` | Access nested value by dot path |
| `has(key) -> bool` | Check if key exists |
| `keys() -> List<str>` | Get all keys (objects only) |
| `length() -> int` | Number of keys or elements |
| `set_str(key, val)` | Set string field |
| `set_int(key, val)` | Set integer field |
| `set_bool(key, val)` | Set boolean field |
| `set_float(key, val)` | Set float field |
| `set_null(key)` | Set null field |
| `push_str(val)` | Append string to array |
| `push_int(val)` | Append integer to array |
| `push_bool(val)` | Append boolean to array |
| `push_null()` | Append null to array |
| `remove(key)` | Remove key from object |

#### Module Functions

| Function | Description |
|----------|-------------|
| `json.parse(str) -> json.Value or err` | Parse JSON string |
| `json.object() -> json.Value` | Create empty object |
| `json.array() -> json.Value` | Create empty array |
| `json.stringify(val) -> str` | Serialize to compact JSON |
| `json.stringify_pretty(val) -> str` | Serialize to formatted JSON |

### Log Module

```bishop
import log;
```

Structured logging with configurable levels and outputs.

#### Log Levels

```bishop
// Level constants (in order of severity)
log.DEBUG   // detailed debugging info
log.INFO    // general information
log.WARN    // warning conditions
log.ERROR   // error conditions
```

#### Basic Logging

```bishop
log.debug("Debugging details");
log.info("Application started");
log.warn("High memory usage");
log.error("Connection failed");
```

#### Logging with Key-Value Pairs

```bishop
log.info_kv("User logged in", "user_id", "12345");
log.debug_kv("Request received", "method", "GET");
log.warn_kv("High memory usage", "percent", "95");
log.error_kv("Connection failed", "host", "example.com");
```

#### Configuration

```bishop
// Set minimum log level (messages below this level are suppressed)
log.set_level(log.INFO);   // suppresses DEBUG
log.set_level(log.WARN);   // suppresses DEBUG and INFO
log.set_level(log.ERROR);  // only shows ERROR

// Set custom format (strftime specifiers)
log.set_format("[%Y-%m-%d %H:%M:%S] [%l] %v");
```

#### File Output

```bishop
// Add file output (receives all logged messages)
log.add_file("/var/log/app.log");

// Add file output with specific level filter
log.add_file_level("/var/log/errors.log", log.ERROR);
```

#### Log Module Functions

| Function | Description |
|----------|-------------|
| `log.debug(msg)` | Log at DEBUG level |
| `log.info(msg)` | Log at INFO level |
| `log.warn(msg)` | Log at WARN level |
| `log.error(msg)` | Log at ERROR level |
| `log.debug_kv(msg, key, val)` | Log DEBUG with key-value |
| `log.info_kv(msg, key, val)` | Log INFO with key-value |
| `log.warn_kv(msg, key, val)` | Log WARN with key-value |
| `log.error_kv(msg, key, val)` | Log ERROR with key-value |
| `log.set_level(level)` | Set minimum log level |
| `log.set_format(fmt)` | Set output format |
| `log.add_file(path)` | Add file output |
| `log.add_file_level(path, level)` | Add file output with level filter |

### Sync Module

```bishop
import sync;
```

Synchronization primitives for concurrent programming.

#### Mutex

```bishop
// Create a mutex
mtx := sync.mutex_create();

// Lock and unlock
sync.mutex_lock(mtx);
// ... critical section ...
sync.mutex_unlock(mtx);

// Try to acquire lock (non-blocking)
if sync.mutex_try_lock(mtx) {
    // acquired lock
    sync.mutex_unlock(mtx);
}
```

#### WaitGroup

Coordinate multiple goroutines:

```bishop
wg := sync.waitgroup_create();

// Add count before spawning
sync.waitgroup_add(wg, 3);

go fn() {
    // ... work ...
    sync.waitgroup_done(wg);
}();

go fn() {
    // ... work ...
    sync.waitgroup_done(wg);
}();

go fn() {
    // ... work ...
    sync.waitgroup_done(wg);
}();

// Wait for all to complete
sync.waitgroup_wait(wg);
```

#### Once

Execute a function exactly once:

```bishop
once := sync.once_create();

// Only the first call executes
sync.once_do(once, fn() {
    print("This runs once");
});
```

#### AtomicInt

Thread-safe integer operations:

```bishop
// Create with initial value
counter := sync.atomic_int_create(0);

// Load current value
val := sync.atomic_int_load(counter);

// Store new value
sync.atomic_int_store(counter, 100);

// Add and return previous value
prev := sync.atomic_int_add(counter, 5);

// Swap and return old value
old := sync.atomic_int_swap(counter, 42);

// Compare and swap (returns bool)
success := sync.atomic_int_compare_swap(counter, 42, 100);
```

#### Sync Module Functions

| Function | Description |
|----------|-------------|
| `sync.mutex_create() -> Mutex` | Create a mutex |
| `sync.mutex_lock(mtx)` | Acquire lock (blocking) |
| `sync.mutex_unlock(mtx)` | Release lock |
| `sync.mutex_try_lock(mtx) -> bool` | Try to acquire lock |
| `sync.waitgroup_create() -> WaitGroup` | Create a wait group |
| `sync.waitgroup_add(wg, n)` | Add n to counter |
| `sync.waitgroup_done(wg)` | Decrement counter by 1 |
| `sync.waitgroup_wait(wg)` | Block until counter is 0 |
| `sync.once_create() -> Once` | Create a once guard |
| `sync.once_do(once, fn)` | Execute fn exactly once |
| `sync.atomic_int_create(n) -> AtomicInt` | Create atomic integer |
| `sync.atomic_int_load(a) -> int` | Load current value |
| `sync.atomic_int_store(a, n)` | Store new value |
| `sync.atomic_int_add(a, n) -> int` | Add and return previous |
| `sync.atomic_int_swap(a, n) -> int` | Swap and return old |
| `sync.atomic_int_compare_swap(a, expected, new) -> bool` | CAS operation |

### YAML Module

```bishop
import yaml;
```

Parse, create, and manipulate YAML data. The API mirrors the JSON module.

#### Parsing YAML

```bishop
// Parse YAML string (fallible)
data := yaml.parse("name: Alice\nage: 30") or return;

// Parse lists
list := yaml.parse("- 1\n- 2\n- 3") or return;

// Check types (same as JSON)
data.is_object();
data.is_list();
data.is_str();
data.is_int();
data.is_float();
data.is_bool();
data.is_null();
```

#### YAML-Specific Parsing

```bishop
// Boolean variations
yaml.parse("yes") or return;    // true
yaml.parse("no") or return;     // false
yaml.parse("true") or return;   // true
yaml.parse("false") or return;  // false

// Null variations
yaml.parse("null") or return;   // null
yaml.parse("~") or return;      // null

// Inline syntax
yaml.parse("items: [1, 2, 3]") or return;
yaml.parse("person: {name: Alice, age: 30}") or return;

// Multiline strings
yaml.parse("text: |\n  line1\n  line2") or return;

// Comments are ignored
yaml.parse("# comment\nname: test") or return;
```

#### Accessing Values

```bishop
data := yaml.parse("user:\n  name: Bob\n  city: NYC") or return;

// Typed getters
name := data.get_str("key") or return;
count := data.get_int("key") or return;
flag := data.get_bool("key") or return;
list := data.get_list("key") or return;
obj := data.get_object("key") or return;

// Path access
city := data.path("user.city") or return;
city_str := city.as_str() or return;   // "NYC"

// Key operations
data.has("name");   // -> bool
data.keys();        // -> List<str>
data.length();      // -> int
```

#### Creating YAML

```bishop
// Create structures
obj := yaml.object();
arr := yaml.array();

// Set values
obj.set_str("name", "Charlie");
obj.set_int("age", 25);
obj.set_bool("active", true);
obj.set_float("score", 95.5);

// Push to arrays
arr.push_int(1);
arr.push_str("hello");
arr.push_bool(true);

// Remove keys
obj.remove("temp");
```

#### Serialization

```bishop
yaml_str := yaml.stringify(obj);
```

#### Module Functions

| Function | Description |
|----------|-------------|
| `yaml.parse(str) -> yaml.Value or err` | Parse YAML string |
| `yaml.object() -> yaml.Value` | Create empty object |
| `yaml.array() -> yaml.Value` | Create empty array |
| `yaml.stringify(val) -> str` | Serialize to YAML |

### Markdown Module

```bishop
import markdown;
```

Parse and render Markdown documents.

#### Converting Markdown to HTML

```bishop
// Direct conversion
html := markdown.to_html("# Title\n\nParagraph with **bold**.");
// <h1>Title</h1>\n<p>Paragraph with <strong>bold</strong>.</p>

// Supports common syntax:
// - Headings: # ## ### etc.
// - Bold: **text**
// - Italic: *text*
// - Code: `inline` and ```blocks```
// - Links: [text](url)
// - Images: ![alt](url)
// - Lists: - item or 1. item
```

#### Extracting Plain Text

```bishop
text := markdown.to_text("# Hello **World**");
// "Hello World"
```

#### Document Parsing

```bishop
// Parse to document object
doc := markdown.parse("# Title\n\nContent") or return;

// Convert document to HTML
html := doc.to_html();

// Serialize back to Markdown
md := markdown.stringify(doc);
```

#### Module Functions

| Function | Description |
|----------|-------------|
| `markdown.to_html(str) -> str` | Convert Markdown to HTML |
| `markdown.to_text(str) -> str` | Extract plain text from Markdown |
| `markdown.parse(str) -> markdown.Document or err` | Parse to document |
| `markdown.stringify(doc) -> str` | Serialize document to Markdown |

#### markdown.Document Methods

| Method | Description |
|--------|-------------|
| `to_html() -> str` | Render document as HTML |

## Import System

Import modules using dot notation:

```bishop
import http;
import fs;
import tests.testlib;

testlib.greet();
result := testlib.add(2, 3);
```

### Using

The `using` keyword brings module members into the local namespace, allowing unqualified access:

#### Selective Imports

```bishop
import log;
using log.info, log.debug, log.warn, log.error;

fn main() {
    info("Application started");
    warn("This is a warning");
    error("Something went wrong");
}
```

#### Wildcard Imports

```bishop
import math;
using math.*;

fn main() {
    x := sin(PI / 2.0);  // all math members available
    y := sqrt(16.0);
}
```

#### With Structs

```bishop
import mymodule;
using mymodule.Point, mymodule.make_point;

fn main() {
    p := Point { x: 10, y: 20 };  // struct literal works
    q := make_point(5, 15);       // function call works
}
```

Notes:
- `using` statements must appear after `import` statements
- Wildcard imports (`using module.*`) bring all public members into scope
- Qualified access (`module.member`) still works alongside `using`
- Using aliases resolve at compile time with no runtime overhead

## Visibility

Use `@private` to restrict visibility to the current file:

```bishop
@private
fn internal_helper() -> int {
    return 42;
}

@private MyStruct :: struct {
    value int
}
```

## FFI (Foreign Function Interface)

### C Types

| Type   | Description          |
|--------|----------------------|
| `cint` | C int                |
| `cstr` | C string (const char*) |
| `void` | void return type     |

### Declaring External Functions

```bishop
@extern("c") fn puts(cstr s) -> cint;
@extern("m") fn sqrt(f64 x) -> f64;
@extern("m") fn floor(f64 x) -> f64;
```

The library name in `@extern("lib")` maps to `-llib` when linking:
- `"c"` - libc (implicit, no flag needed)
- `"m"` - libm (math library)
- `"pthread"` - libpthread

### Calling External Functions

```bishop
@extern("c") fn puts(cstr s) -> cint;

fn main() {
    puts("Hello from C!");  // str automatically converts to cstr
}
```

### Type Compatibility

- `str` can be passed where `cstr` is expected (automatic `.c_str()` conversion)
- `int` can be passed where `cint` is expected
- `f64` works directly with C `double`

## Documentation Comments

Use `///` for documentation comments:

```bishop
/// This is a doc comment for the struct
Person :: struct {
    /// Doc comment for field
    name str
}

/// Doc comment for function
fn add(int a, int b) -> int {
    return a + b;
}
```

## Built-in Functions

```bishop
print("Hello");
print("Multiple", "args");
sleep(100);       // sleep for 100 milliseconds
```

## Test Assertions

Test assertions are available only in test mode (`./bishop test`). They provide informative error messages with line numbers on failure.

### Equality Assertions

```bishop
assert_eq(a, b);   // a == b
assert_ne(a, b);   // a != b
```

### Boolean Assertions

```bishop
assert_true(condition);   // condition is true
assert_false(condition);  // condition is false
```

### Comparison Assertions

```bishop
assert_gt(a, b);   // a > b
assert_gte(a, b);  // a >= b
assert_lt(a, b);   // a < b
assert_lte(a, b);  // a <= b
```

### Collection Assertions

```bishop
nums := [1, 2, 3];
assert_contains(2, nums);  // 2 is in nums
```

### String Assertions

```bishop
s := "hello world";
assert_starts_with("hello", s);  // s starts with "hello"
assert_ends_with("world", s);    // s ends with "world"
```

### Float Comparison

```bishop
pi := 3.14159;
assert_near(pi, 3.14, 0.01);  // pi is within 0.01 of 3.14
```

### Example Test

```bishop
fn test_math() {
    assert_eq(1 + 1, 2);
    assert_ne(1, 2);
    assert_true(5 > 3);
    assert_gt(10, 5);
}
```

## Keywords

`fn`, `return`, `struct`, `if`, `else`, `while`, `for`, `in`, `true`, `false`, `none`, `is`, `import`, `using`, `select`, `case`, `Channel`, `List`, `Pair`, `Tuple`, `extern`, `go`, `sleep`, `err`, `fail`, `or`, `match`, `default`, `with`, `as`, `const`, `continue`, `break`

## Decorators

`@static`, `@private`, `@extern`
