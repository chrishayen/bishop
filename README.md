# Bishop

Bishop is a programming language that transpiles to C++, combining the performance of C++ with clean, intuitive syntax inspired by Python and Go.

## Quick Start

```bishop
fn main() {
    print("Hello, World!");
}
```

Save as `hello.b` and compile:

```bash
make
./bishop hello.b -o hello
./hello
```

## Types

### Primitive Types

| Type | Description |
|------|-------------|
| `int` | Integer |
| `str` | String |
| `bool` | Boolean (`true`/`false`) |
| `char` | Single character |
| `f32` | 32-bit float |
| `f64` | 64-bit float |
| `u32` | Unsigned 32-bit integer |
| `u64` | Unsigned 64-bit integer |

### Optional Types

```bishop
int? maybe_num = none;
int? value = 42;

if value is none { }
if value { }  // truthy check for non-none
```

## Variables

### Explicit Type Declaration

```bishop
int x = 42;
str name = "Chris";
bool flag = true;
```

### Type Inference

```bishop
x := 100;        // inferred as int
name := "Hello"; // inferred as str
```

## Functions

### Basic Function

```bishop
fn add(int a, int b) -> int {
    return a + b;
}
```

### Void Function

```bishop
fn greet() {
    print("Hello");
}
```

### Function References

```bishop
fn apply_op(int x, int y, fn(int, int) -> int op) -> int {
    return op(x, y);
}

result := apply_op(3, 4, add);
```

## Structs

### Definition and Instantiation

```bishop
Person :: struct {
    name str,
    age int
}

p := Person { name: "Chris", age: 32 };
p.name;
p.age = 33;
```

### Pass by Reference

Structs can be passed by reference using pointer syntax:

```bishop
fn process(Person *p) {
    p.name = "updated";    // auto-deref, always mutable
    p.set_age(30);         // auto-deref for method calls
}

bob := Person { name: "Bob", age: 25 };
process(&bob);             // pass by reference
```

**Not supported:** storing pointers in variables, primitive pointers, dereference syntax, pointer arithmetic.

## Methods

```bishop
Person :: get_name(self) -> str {
    return self.name;
}

Person :: greet(self, str greeting) -> str {
    return greeting;
}

p.get_name();
p.greet("Hello");
```

## Control Flow

### If/Else

```bishop
if condition {
    // then
} else {
    // else
}
```

### While Loop

```bishop
while i < 5 {
    i = i + 1;
}
```

### For Loops

```bishop
// Range-based (exclusive end, like Python)
for i in 0..10 {
    print(i);  // prints 0 through 9
}

// Foreach
nums := [1, 2, 3];
for n in nums {
    print(n);
}
```

## Operators

| Category | Operators |
|----------|-----------|
| Arithmetic | `+`, `-`, `*`, `/` |
| Comparison | `==`, `!=`, `<`, `>`, `<=`, `>=` |
| Logical | `!` (negation) |

## Lists

### Creation

```bishop
nums := List<int>();           // empty list
nums := [1, 2, 3];             // list literal
List<int> nums = [1, 2, 3];    // typed declaration
```

### Methods

```bishop
nums.length();       // -> int
nums.is_empty();     // -> bool
nums.contains(x);    // -> bool
nums.get(i);         // -> T (bounds-checked)
nums.first();        // -> T
nums.last();         // -> T
nums.append(x);      // add to end
nums.pop();          // remove last
nums.set(i, x);      // replace at index
nums.clear();        // remove all
nums.insert(i, x);   // insert at index
nums.remove(i);      // remove at index
```

## String Methods

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

## Error Handling

### Error Types

```bishop
// Simple error
ParseError :: err;

// Error with fields
IOError :: err {
    code int,
    path str
}
```

All errors have: `message str`, `cause err?`, `root_cause`.

### Fallible Functions

```bishop
fn read_config(str path) -> Config or err {
    if !fs.exists(path) {
        fail IOError { message: "not found", code: 404, path: path };
    }
    return parse(content) or fail ParseError { message: "invalid", cause: err };
}
```

### Error Handling with `or`

```bishop
x := fallible() or return;                    // early return
x := fallible() or return default_value;      // return with value
x := fallible() or fail err;                  // propagate error
x := fallible() or fail WrapperError { cause: err };  // wrap and propagate

// Pattern match on error type
x := fallible() or match err {
    IOError    => default_config,
    ParseError => fail err,
    _          => fail ConfigError { cause: err }
};
```

### Default Values

```bishop
count := get_count() default 1;
name := get_name() default "unknown";

// Combine with error handling
x := fetch() default 1 or fail err;
```

## Resource Management

The `with` statement provides automatic cleanup:

```bishop
Resource :: struct { name str }
Resource :: close(self) { print("Closing"); }

with create_resource("file") as res {
    print(res.name);
}  // res.close() called automatically
```

## Modules

### HTTP Module

```bishop
import http;

fn handle(http.Request req) -> http.Response {
    return http.text("Hello");
}

http.serve(8080, handle);

// Response helpers
http.text("Hello");     // 200 OK, text/plain
http.json("{...}");     // 200 OK, application/json
http.not_found();       // 404 Not Found

// App-based routing
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

## Import System

```bishop
import http;
import tests.testlib;

testlib.greet();
result := testlib.add(2, 3);
```

## Visibility

```bishop
@private fn internal_function() { }
@private MyStruct :: struct { }
```

## FFI (Foreign Function Interface)

### C Types

| Type | Description |
|------|-------------|
| `cint` | C int |
| `cstr` | C string (const char*) |
| `void` | void return type |

### External Functions

```bishop
@extern("c") fn puts(cstr s) -> cint;
@extern("m") fn sqrt(f64 x) -> f64;

fn main() {
    puts("Hello from C!");  // str auto-converts to cstr
}
```

Library name maps to `-llib` when linking: `"c"` (libc), `"m"` (libm), `"pthread"`.

## Documentation Comments

```bishop
/// Doc comment for struct
Person :: struct {
    /// Doc comment for field
    name str
}

/// Doc comment for function
fn add(int a, int b) -> int { }
```

## Built-in Functions

```bishop
print("Hello");
print("Multiple", "args");
assert_eq(a, b);  // test mode only
sleep(100);       // milliseconds
```

## Keywords

`fn`, `return`, `struct`, `if`, `else`, `while`, `for`, `in`, `true`, `false`, `none`, `is`, `import`, `select`, `case`, `Channel`, `List`, `extern`, `go`, `sleep`, `err`, `fail`, `or`, `match`, `default`, `with`, `as`

## File Extension

`.b`

## Building

```bash
make        # Build the compiler
make test   # Run tests
```

## License

See LICENSE file for details.
