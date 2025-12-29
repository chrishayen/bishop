# Bishop

Bishop is a modern programming language that compiles to C++. It takes the performance and capabilities of C++ and wraps them in clean, intuitive syntax inspired by Python and Go.

## Features

- Clean, readable syntax with type inference
- Structs with methods
- First-class functions
- Goroutines and channels for concurrency
- Comprehensive error handling with pattern matching
- HTTP and filesystem standard library
- FFI for calling C libraries
- Resource management with `with` blocks

## Quick Start

```bishop
fn main() {
    print("Hello, World!");
}
```

Build and run:

```bash
make build
./bishop run hello.b
```

## Language Reference

### Types

#### Primitive Types

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

#### Optional Types

```bishop
int? maybe_num = none;
int? value = 42;

if value is none { }
if value { }  // truthy check for non-none
```

### Variables

#### Explicit Type Declaration

```bishop
int x = 42;
str name = "Chris";
bool flag = true;
```

#### Type Inference

```bishop
x := 100;        // inferred as int
name := "Hello"; // inferred as str
```

### Functions

#### Basic Function

```bishop
fn add(int a, int b) -> int {
    return a + b;
}
```

#### Void Function

```bishop
fn greet() {
    print("Hello");
}
```

#### Function References

Functions can be passed as arguments:

```bishop
fn apply_op(int x, int y, fn(int, int) -> int op) -> int {
    return op(x, y);
}

result := apply_op(3, 4, add);
```

### Structs

#### Definition

```bishop
Person :: struct {
    name str,
    age int
}
```

#### Instantiation

```bishop
p := Person { name: "Chris", age: 32 };
```

#### Field Access

```bishop
p.name;
p.age = 33;
```

#### Pass by Reference

Structs can be passed by reference using pointer syntax:

```bishop
fn process(Person *p) {
    p.name = "updated";    // auto-deref, always mutable
    p.set_age(30);         // auto-deref for method calls
}

bob := Person { name: "Bob", age: 25 };
process(&bob);             // pass by reference
```

Note: Pointers are only for pass-by-reference. They cannot be stored in variables.

### Methods

#### Definition

```bishop
Person :: get_name(self) -> str {
    return self.name;
}

Person :: greet(self, str greeting) -> str {
    return greeting;
}
```

#### Calling Methods

```bishop
p.get_name();
p.greet("Hello");
```

### Control Flow

#### If/Else

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

#### While Loop

```bishop
while i < 5 {
    i = i + 1;
}
```

#### For Loops

Range-based (exclusive end):

```bishop
for i in 0..10 {
    print(i);  // prints 0 through 9
}
```

Foreach:

```bishop
nums := [1, 2, 3];

for n in nums {
    print(n);
}
```

### Operators

| Category   | Operators                    |
|------------|------------------------------|
| Arithmetic | `+`, `-`, `*`, `/`           |
| Comparison | `==`, `!=`, `<`, `>`, `<=`, `>=` |
| Logical    | `!` (negation)               |

### Lists

#### Creation

```bishop
nums := List<int>();           // empty list
nums := [1, 2, 3];             // list literal
List<int> nums = [1, 2, 3];    // typed declaration
```

#### Methods

```bishop
nums.length();           // -> int: number of elements
nums.is_empty();         // -> bool: true if no elements
nums.contains(x);        // -> bool: true if x is in list
nums.get(i);             // -> T: element at index
nums.first();            // -> T: first element
nums.last();             // -> T: last element
nums.append(x);          // add element to end
nums.pop();              // remove last element
nums.set(i, x);          // replace element at index
nums.clear();            // remove all elements
nums.insert(i, x);       // insert element at index
nums.remove(i);          // remove element at index
```

### String Methods

```bishop
s.length();              // -> int
s.empty();               // -> bool
s.contains("x");         // -> bool
s.starts_with("x");      // -> bool
s.ends_with("x");        // -> bool
s.substr(start, len);    // -> str
s.at(index);             // -> char
s.find("x");             // -> int
```

### Concurrency

#### Channels

```bishop
ch := Channel<int>();
ch.send(42);
val := ch.recv();
```

#### Goroutines

```bishop
fn worker(Channel<int> ch, int val) {
    ch.send(val);
}

go worker(ch, 42);
```

#### Select

```bishop
select {
    case val := ch1.recv() {
        // handle ch1
    }
    case val := ch2.recv() {
        // handle ch2
    }
}
```

### Error Handling

#### Error Types

```bishop
// Simple error
ParseError :: err;

// Error with fields
IOError :: err {
    code int,
    path str
}
```

All errors have: `message str`, `cause err?`, and `root_cause`.

#### Fallible Functions

```bishop
fn read_config(str path) -> Config or err {
    if !fs.exists(path) {
        fail IOError { message: "not found", code: 404, path: path };
    }
    return config;
}
```

#### The `or` Keyword

```bishop
x := fallible() or return;                    // return early
x := fallible() or return default_value;      // return with value
x := fallible() or fail err;                  // propagate error
x := fallible() or fail Wrapper { cause: err }; // wrap and propagate

x := fallible() or match err {
    IOError    => default_config,
    ParseError => fail err,
    _          => fail err
};
```

#### The `default` Keyword

Handle falsy values (0, "", false, none):

```bishop
count := get_count() default 1;
name := get_name() default "unknown";
```

### Resource Management

The `with` statement provides automatic cleanup:

```bishop
Resource :: struct { name str }

Resource :: close(self) {
    print("Closing");
}

with create_resource("file") as res {
    print(res.name);
}  // res.close() called automatically
```

### Modules

#### Import

```bishop
import http;
import fs;
import tests.testlib;
```

#### Visibility

```bishop
@private fn internal_function() { }
@private MyStruct :: struct { }
```

### HTTP Module

```bishop
import http;

fn handle(http.Request req) -> http.Response {
    return http.text("Hello");
}

fn main() {
    http.serve(8080, handle);
}
```

#### Response Helpers

```bishop
http.text("Hello");     // 200 OK, text/plain
http.json("{...}");     // 200 OK, application/json
http.not_found();       // 404 Not Found
```

#### App-based Routing

```bishop
app := http.App {};
app.get("/", home);
app.post("/submit", handler);
app.listen(8080);
```

### Filesystem Module

```bishop
import fs;

fs.read_file("path");    // -> str
fs.exists("path");       // -> bool
fs.is_dir("path");       // -> bool
fs.read_dir("path");     // -> str (newline-separated)
```

### FFI (Foreign Function Interface)

#### C Types

- `cint` - C int
- `cstr` - C string (const char*)
- `void` - void return type

#### Declaring External Functions

```bishop
@extern("c") fn puts(cstr s) -> cint;
@extern("m") fn sqrt(f64 x) -> f64;
```

The library name maps to `-llib` when linking.

#### Type Compatibility

- `str` converts to `cstr` automatically
- `int` is compatible with `cint`
- `f64` works with C `double`

### Documentation Comments

```bishop
/// Doc comment for the struct
Person :: struct {
    /// Doc comment for field
    name str
}

/// Doc comment for function
fn add(int a, int b) -> int { }
```

### Built-in Functions

```bishop
print("Hello");
print("Multiple", "args");
assert_eq(a, b);  // test mode only
sleep(100);       // sleep for 100 milliseconds
```

### Keywords

`fn`, `return`, `struct`, `if`, `else`, `while`, `for`, `in`, `true`, `false`, `none`, `is`, `import`, `select`, `case`, `Channel`, `List`, `extern`, `go`, `sleep`, `err`, `fail`, `or`, `match`, `default`, `with`, `as`

## Examples

### Hello World

```bishop
fn main() {
    print("Hello, World!");
}
```

### Static File Server

```bishop
import http;
import fs;

fn handle(http.Request req) -> http.Response {
    path := "." + req.path;

    if !fs.exists(path) {
        return http.not_found();
    }

    content := fs.read_file(path);
    return http.text(content);
}

fn main() {
    http.serve(8080, handle);
}
```

### Error Handling

```bishop
IOError :: err {
    code int,
    path str
}

fn read_config(str path) -> str or err {
    if !fs.exists(path) {
        fail IOError { message: "not found", code: 404, path: path };
    }
    return fs.read_file(path);
}

fn main() {
    config := read_config("app.conf") or match err {
        IOError => {
            print("File error");
            return;
        }
        _ => {
            print("Unknown error");
            return;
        }
    };

    print(config);
}
```

### Concurrency

```bishop
fn worker(Channel<int> ch, int id) {
    ch.send(id * 10);
}

fn main() {
    ch := Channel<int>();

    go worker(ch, 1);
    go worker(ch, 2);

    a := ch.recv();
    b := ch.recv();

    print(a + b);
}
```

## File Extension

Bishop source files use the `.b` extension.

## Building

```bash
make build      # Build the compiler
make test       # Run tests
make clean      # Clean build artifacts
```

## License

See LICENSE file for details.
