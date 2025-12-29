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

with server {
    while true {
        conn := server.accept() or continue;

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

`fn`, `return`, `struct`, `if`, `else`, `while`, `for`, `in`, `true`, `false`, `none`, `is`, `import`, `select`, `case`, `Channel`, `List`, `Pair`, `Tuple`, `extern`, `go`, `sleep`, `err`, `fail`, `or`, `match`, `default`, `with`, `as`, `const`, `continue`, `break`
