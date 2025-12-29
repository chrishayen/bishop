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
| `char` | Single character         |
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
char c = 'a';
f64 pi = 3.14159;
```

### Type Inference

Use `:=` for type inference:

```bishop
x := 100;        // inferred as int
name := "Hello"; // inferred as str
pi := 3.14;      // inferred as f64
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

### String Methods

```bishop
s := "hello world";

s.length();              // -> int: 11
s.empty();               // -> bool: false
s.contains("world");     // -> bool: true
s.starts_with("hello");  // -> bool: true
s.ends_with("world");    // -> bool: true
s.substr(0, 5);          // -> str: "hello"
s.at(0);                 // -> char: 'h'
s.find("world");         // -> int: index of substring
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

```bishop
fs.read_file("path");    // -> str (file contents, empty if not found)
fs.exists("path");       // -> bool (true if file or dir exists)
fs.is_dir("path");       // -> bool (true if path is a directory)
fs.read_dir("path");     // -> str (newline-separated filenames)
```

### Crypto Module

Cryptographic utilities for hashing, encoding, and UUID generation. Uses OpenSSL.

```bishop
import crypto;
```

#### Hashing Functions

All hash functions return lowercase hex strings:

```bishop
crypto.md5("hello");     // -> "5d41402abc4b2a76b9719d911017c592"
crypto.sha1("hello");    // -> "aaf4c61ddcc5e8a2dabede0f3b482cd9aea9434d"
crypto.sha256("hello");  // -> "2cf24dba5fb0a30e26e83b2ac5b9e29e..."
crypto.sha512("hello");  // -> "9b71d224bd62f3785d96d46ad3ea3d73..."
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
hex := crypto.hex_encode("hello");       // -> "68656c6c6f"
decoded := crypto.hex_decode("68656c6c6f");  // -> "hello"
```

#### UUID Generation

```bishop
// Random UUID v4
id := crypto.uuid();
// e.g., "550e8400-e29b-41d4-a716-446655440000"

// Deterministic UUID v5 (namespace + name based)
id := crypto.uuid_v5("namespace", "name");
// Same inputs always produce same UUID
```

#### Random Bytes

```bishop
bytes := crypto.random_bytes(32);  // -> List<u8> with 32 random bytes
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
// Get environment variable (returns empty string if not found)
home := process.env("HOME");

// Set environment variable
process.set_env("MY_VAR", "value");
```

#### Working Directory

```bishop
// Get current working directory
print(process.cwd());

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

## Import System

Import modules using dot notation:

```bishop
import http;
import fs;
import tests.testlib;

testlib.greet();
result := testlib.add(2, 3);
```

## Visibility

Use `@private` to restrict visibility to the current file:

```bishop
@private fn internal_helper() -> int {
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
assert_eq(a, b);  // test mode only
sleep(100);       // sleep for 100 milliseconds
```

## Keywords

`fn`, `return`, `struct`, `if`, `else`, `while`, `for`, `in`, `true`, `false`, `none`, `is`, `import`, `select`, `case`, `Channel`, `List`, `extern`, `go`, `sleep`, `err`, `fail`, `or`, `match`, `default`, `with`, `as`
