- We're building a compiler for a new language called bishop. It's more of a c++ transpiler because we will be lexing, parsing, but our features will be implemented in c++, but the user uses the nicer, more elegant bishop syntax.
- We're basically building an asbtraction over C++. We're taking the best things from c++ and giving it nice syntax.
- We're writing production quality code
- Use the Makefile in the root of the project for build and test
- before every action it is required to read tests/ so that we know each variability of the language so that we implement each feature correctly. Every possible combination of features must be implemented and tested.
- when you write if or while statments, put a line of space on each side that has other logic
- all c++ code requires docstrings
- language features should be logically separated by purpose. (e.g. src/typechecker/strings.cpp)
- we wrote pythonic code and our language and standard libraries should be easy and intuitive to the user
- readability is of utmost importance
- when adding or modifying language features, update the Bishop Language Reference section below

---

# Bishop Language Reference

## Types

### Primitive Types
- `int` - Integer
- `str` - String
- `bool` - Boolean (`true`/`false`)
- `char` - Single character
- `f32` - 32-bit float
- `f64` - 64-bit float
- `u32` - Unsigned 32-bit integer
- `u64` - Unsigned 64-bit integer

### Optional Types
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

### Void Function (no return type)
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
p.name;
p.age = 33;
```

### Pass by Reference (Go-style)
Structs can be passed by reference to functions using pointer syntax. Pointers cannot be stored in variables - only used for pass-by-reference.

Function parameters use `*` to take a reference:
```bishop
fn process(Person *p) {
    p.name = "updated";    // auto-deref, always mutable
    p.set_age(30);         // auto-deref for method calls
}
```

Call site uses `&` to pass by reference:
```bishop
bob := Person { name: "Bob", age: 25 };
process(&bob);             // pass by reference
assert_eq(bob.name, "updated");  // mutation visible
```

What's NOT supported:
- `Person p = &bob` - no storing pointers in variables
- `p := &bob` - no inferred pointer variables
- `int *p` - no primitive pointers
- `*p = value` - no dereference syntax
- Pointer arithmetic

## Methods

### Definition
```bishop
Person :: get_name(self) -> str {
    return self.name;
}

Person :: greet(self, str greeting) -> str {
    return greeting;
}
```

### Calling Methods
```bishop
p.get_name();
p.greet("Hello");
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
while i < 5 {
    i = i + 1;
}
```

### For Loops

#### Range-based For
```bishop
for i in 0..10 {
    print(i);  // prints 0 through 9
}
```

Range is inclusive start, exclusive end (like Python's `range()`).

#### Foreach
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

## HTTP Module

### Import
```bishop
import http;
```

### Request/Response
```bishop
http.Request { method: "GET", path: "/test", body: "" }
http.Response { status: 200, content_type: "text/plain", body: "Hello" }
```

### Response Helpers
```bishop
http.text("Hello");     // 200 OK, text/plain
http.json("{...}");     // 200 OK, application/json
http.not_found();       // 404 Not Found
```

### App-based Routing
```bishop
fn home(http.Request req) -> http.Response {
    return http.text("Home");
}

app := http.App {};
app.get("/", home);
app.post("/submit", handler);
app.listen(8080);
```

### Handler Function Pattern
```bishop
fn handle(http.Request req) -> http.Response {
    return http.text("Hello");
}

http.serve(8080, handle);
```

## Filesystem Module

### Import
```bishop
import fs;
```

### Functions
```bishop
fs.read_file("path");    // -> str (file contents, empty if not found)
fs.exists("path");       // -> bool (true if file or dir exists)
fs.is_dir("path");       // -> bool (true if path is a directory)
fs.read_dir("path");     // -> str (newline-separated filenames)
```

### Example: Static File Server
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

## Math Module

### Import
```bishop
import math;
```

### Constants
```bishop
math.PI();      // -> f64: 3.14159265358979...
math.E();       // -> f64: 2.71828182845904...
math.INF();     // -> f64: positive infinity
math.NAN();     // -> f64: not a number
```

### Basic Operations
```bishop
math.abs(x);              // -> f64: absolute value
math.abs_int(x);          // -> int: absolute value of integer
math.min(a, b);           // -> f64: smaller of two values
math.min_int(a, b);       // -> int: smaller of two integers
math.max(a, b);           // -> f64: larger of two values
math.max_int(a, b);       // -> int: larger of two integers
math.clamp(x, min, max);  // -> f64: clamp to range
math.clamp_int(x, min, max); // -> int: clamp integer to range
```

### Rounding Functions
```bishop
math.floor(x);   // -> f64: largest integer <= x
math.ceil(x);    // -> f64: smallest integer >= x
math.round(x);   // -> f64: round to nearest integer
math.trunc(x);   // -> f64: truncate towards zero
```

### Power and Root Functions
```bishop
math.pow(base, exp);   // -> f64: base^exp
math.sqrt(x);          // -> f64: square root
math.cbrt(x);          // -> f64: cube root
math.exp(x);           // -> f64: e^x
math.log(x);           // -> f64: natural logarithm
math.log10(x);         // -> f64: base-10 logarithm
math.log2(x);          // -> f64: base-2 logarithm
```

### Trigonometric Functions
```bishop
math.sin(x);           // -> f64: sine (x in radians)
math.cos(x);           // -> f64: cosine (x in radians)
math.tan(x);           // -> f64: tangent (x in radians)
math.asin(x);          // -> f64: arc sine
math.acos(x);          // -> f64: arc cosine
math.atan(x);          // -> f64: arc tangent
math.atan2(y, x);      // -> f64: arc tangent of y/x
```

### Hyperbolic Functions
```bishop
math.sinh(x);          // -> f64: hyperbolic sine
math.cosh(x);          // -> f64: hyperbolic cosine
math.tanh(x);          // -> f64: hyperbolic tangent
```

### Utility Functions
```bishop
math.is_nan(x);        // -> bool: true if NaN
math.is_inf(x);        // -> bool: true if infinity
math.is_finite(x);     // -> bool: true if finite
math.sign(x);          // -> int: -1, 0, or 1
math.sign_int(x);      // -> int: -1, 0, or 1 for integer
math.gcd(a, b);        // -> int: greatest common divisor
math.lcm(a, b);        // -> int: least common multiple
```

### Example
```bishop
import math;

fn calculate_circle_area(f64 radius) -> f64 {
    return math.PI() * math.pow(radius, 2.0);
}

fn main() {
    area := calculate_circle_area(5.0);
    print("Area:", area);

    // Clamp a value to a valid range
    value := math.clamp(150.0, 0.0, 100.0);
    print("Clamped:", value);  // 100.0

    // Check for special values
    result := math.sqrt(-1.0);

    if math.is_nan(result) {
        print("Invalid result");
    }
}
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
nums.length();           // -> int: number of elements
nums.is_empty();         // -> bool: true if no elements
nums.contains(x);        // -> bool: true if x is in list
nums.get(i);             // -> T: element at index (bounds-checked)
nums.first();            // -> T: first element
nums.last();             // -> T: last element
nums.append(x);          // add element to end
nums.pop();              // remove last element
nums.set(i, x);          // replace element at index
nums.clear();            // remove all elements
nums.insert(i, x);       // insert element at index
nums.remove(i);          // remove element at index
```

### Example
```bishop
fn main() {
    nums := [10, 20, 30];
    nums.append(40);

    if nums.contains(20) {
        print("Found 20");
    }

    for n in nums {
        print(n);
    }
}
```

## Resource Management (with blocks)

The `with` statement provides automatic resource cleanup. When the block exits, `close()` is called on the resource.

### Syntax
```bishop
with expression as name {
    // use name here
}  // name.close() called automatically
```

### Example
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
    }  // res.close() called here
}
```

### Nested with blocks
```bishop
with create_outer() as outer {
    with create_inner() as inner {
        // both outer and inner available
    }  // inner.close() called
}  // outer.close() called
```

### Notes
- Types used with `with` must have a `close()` method
- `close()` is called even on early return

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
- `cint` - C int
- `cstr` - C string (const char*)
- `void` - void return type

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
```bishop
/// This is a doc comment for the struct
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
sleep(100);       // sleep for 100 milliseconds
```

## Keywords
`fn`, `return`, `struct`, `if`, `else`, `while`, `for`, `in`, `true`, `false`, `none`, `is`, `import`, `select`, `case`, `Channel`, `List`, `extern`, `go`, `sleep`, `err`, `fail`, `or`, `match`, `default`, `with`, `as`

## Error Handling

### Error Types

Errors are defined similarly to structs, using `:: err` syntax:

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

### Built-in Error Fields

All errors automatically have:
- `message str` - Error message (always present)
- `cause err?` - Optional wrapped error for chaining
- `root_cause` - Getter that returns the bottom-most error in a chain

### Fallible Functions

Functions that can fail use `-> T or err` return syntax:

```bishop
fn read_config(str path) -> Config or err {
    if !fs.exists(path) {
        fail IOError { message: "file not found", code: 404, path: path };
    }

    content := fs.read_file(path);
    return parse(content) or fail ParseError { message: "invalid config", cause: err };
}
```

### The `fail` Keyword

Use `fail` to return an error:

```bishop
fail "simple error message";
fail ParseError { message: "bad format" };
fail IOError { message: "not found", code: 404, path: path };
fail WrapperError { message: "context", cause: err };
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

// Wrap error and propagate
x := fallible() or fail WrapperError { message: "context", cause: err };

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

Handle falsy values (0, "", false, none) using `default`:

```bishop
count := get_count() default 1;
name := get_name() default "unknown";
```

### Combining `default` and `or`

Handle both falsy values and errors in one expression:

```bishop
// default handles falsy, or handles errors
x := fetch() default 1 or fail err;
x := fetch() default 1 or return;
x := fetch() default "n/a" or fail FetchError { cause: err };
```

Evaluation order:
1. Call the function
2. If **error** → trigger `or` handler
3. If **success but falsy** → use `default` value
4. If **success and truthy** → use the value

### Error Chaining

Wrap errors to add context:

```bishop
ConfigError :: err;

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
    print(err.cause.message);        // "permission denied"
    print(err.root_cause.message);   // original error
    return;
};
```

### Pattern Matching Errors

```bishop
config := read_config("app.conf") or match err {
    IOError    => default_config,
    ParseError => fail err,
    _          => fail ConfigError { message: "unexpected", cause: err }
};
```

### Rules

1. Functions returning `T or err` must use `fail` or return a value
2. Callers of fallible functions must use `or` to handle errors
3. Nested `or` is not allowed (no `or { ... or ... }`)
4. `err` is the universal error type - all custom errors are compatible
5. Use `err is ErrorType` to check error type
6. `or` triggers on errors, `default` triggers on falsy values

### Complete Example

```bishop
ParseError :: err;
IOError :: err {
    code int,
    path str
}
ConfigError :: err;

fn read_file(str path) -> str or err {
    if !fs.exists(path) {
        fail IOError { message: "not found", code: 404, path: path };
    }
    return fs.read_file(path);
}

fn parse_config(str content) -> Config or err {
    if content.empty() {
        fail ParseError { message: "empty config" };
    }
    // ... parse logic
    return config;
}

fn load_config(str path) -> Config or err {
    content := read_file(path)
        or fail ConfigError { message: "can't read config", cause: err };

    return parse_config(content)
        or fail ConfigError { message: "invalid config", cause: err };
}

fn main() {
    config := load_config("app.conf") or match err {
        IOError => {
            print("File error:", err.path);
            return;
        }
        ParseError => {
            print("Parse error:", err.message);
            return;
        }
        _ => {
            print("Error:", err.root_cause.message);
            return;
        }
    };

    print("Config loaded:", config.name);
}
```

## File Extension
`.b`
- AST nodes require the line number to be set
