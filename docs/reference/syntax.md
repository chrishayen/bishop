# Nog Language Reference

## Types

### int

Integer type for whole numbers.

**Syntax:**
```
int
```

**Example:**
```bishop
int x = 42;
```

### str

String type for text.

**Syntax:**
```
str
```

**Example:**
```bishop
str name = "Chris";
```

### bool

Boolean type with values true or false.

**Syntax:**
```
bool
```

**Example:**
```bishop
bool flag = true;
```

### char

Single character type.

**Syntax:**
```
char
```

**Example:**
```bishop
char c = 'a';
```

### f32

32-bit floating point number.

**Syntax:**
```
f32
```

**Example:**
```bishop
f32 pi = 3.14;
```

### f64

64-bit floating point number.

**Syntax:**
```
f64
```

**Example:**
```bishop
f64 precise = 3.14159265359;
```

### u32

Unsigned 32-bit integer.

**Syntax:**
```
u32
```

**Example:**
```bishop
u32 count = 100;
```

### u64

Unsigned 64-bit integer.

**Syntax:**
```
u64
```

**Example:**
```bishop
u64 big_num = 9999999999;
```

### Optional Types

Optional type that can hold a value or none.

**Syntax:**
```
T?
```

**Example:**
```bishop
int? maybe_num = none;
int? value = 42;
if value is none {
    print("No value");
}
```

> Use `is none` to check for none, or use a truthy check

## Variables

### Explicit Declaration

Declare a variable with an explicit type.

**Syntax:**
```
type name = expr;
```

**Example:**
```bishop
int x = 42;
str name = "Chris";
bool flag = true;
```

### Type Inference

Declare a variable with inferred type using :=.

**Syntax:**
```
name := expr;
```

**Example:**
```bishop
x := 100;
name := "Hello";
pi := 3.14;
```

## Functions

### Function Declaration

Declare a function with parameters and return type.

**Syntax:**
```
fn name(type param, ...) -> return_type { }
```

**Example:**
```bishop
fn add(int a, int b) -> int {
    return a + b;
}
```

### Void Function

Function with no return type (void).

**Syntax:**
```
fn name(params) { }
```

**Example:**
```bishop
fn greet(str name) {
    print("Hello, " + name);
}
```

### Function References

Pass functions as arguments to other functions.

**Syntax:**
```
fn(param_types) -> return_type
```

**Example:**
```bishop
fn apply_op(int x, int y, fn(int, int) -> int op) -> int {
    return op(x, y);
}
result := apply_op(3, 4, add);
```

## Structs

### Struct Definition

Define a custom data structure.

**Syntax:**
```
Name :: struct { field type, ... }
```

**Example:**
```bishop
Person :: struct {
    name str,
    age int
}
```

### Struct Instantiation

Create an instance of a struct.

**Syntax:**
```
TypeName { field: value, field: value }
```

**Example:**
```bishop
p := Person { name: "Chris", age: 32 };
req := http.Request { method: "GET", path: "/", body: "" };
```

### Field Access

Access or modify struct fields using dot notation.

**Syntax:**
```
instance.field
```

**Example:**
```bishop
name := p.name;
p.age = 33;
```

## Methods

### Method Definition

Define a method on a struct type.

**Syntax:**
```
Type :: name(self, params) -> return_type { }
```

**Example:**
```bishop
Person :: get_name(self) -> str {
    return self.name;
}

Person :: greet(self, str greeting) -> str {
    return greeting + ", " + self.name;
}
```

### Method Call

Call a method on a struct instance.

**Syntax:**
```
instance.method(args)
```

**Example:**
```bishop
name := p.get_name();
msg := p.greet("Hello");
```

## Control Flow

### if

Conditional branching with if and optional else.

**Syntax:**
```
if condition { ... } else { ... }
```

**Example:**
```bishop
if x > 10 {
    print("big");
} else {
    print("small");
}
```

### while

Loop while a condition is true.

**Syntax:**
```
while condition { ... }
```

**Example:**
```bishop
i := 0;
while i < 5 {
    print(i);
    i = i + 1;
}
```

## Operators

### Arithmetic Operators

Mathematical operations.

**Syntax:**
```
+ - * /
```

**Example:**
```bishop
sum := a + b;
diff := a - b;
prod := a * b;
quot := a / b;
```

> Precedence: `*` and `/` bind tighter than `+` and `-` (left-associative within each level).

### Parentheses

Group expressions to override precedence and enable postfix access on expression results.

**Syntax:**
```
(expr)
```

**Example:**
```bishop
result := (1 + 2) * 3;
len := ("hi" + "there").length();
```

### Comparison Operators

Compare values.

**Syntax:**
```
== != < > <= >=
```

**Example:**
```bishop
if x == y { }
if x != y { }
if x < y { }
if x >= y { }
```

### String Concatenation

Join strings with the + operator.

**Syntax:**
```
str + str
```

**Example:**
```bishop
msg := "Hello, " + name + "!";
```

## Imports

### import

Import a module to use its types and functions.

**Syntax:**
```
import module_name;
```

**Example:**
```bishop
import http;
import myproject.utils;
```

> Use dot notation for nested modules

### Qualified Access

Access imported module members with dot notation.

**Syntax:**
```
module.member
```

**Example:**
```bishop
resp := http.text("Hello");
result := utils.helper();
```

## Visibility

### @private

Mark a function or struct as private to its module.

**Syntax:**
```
@private fn/struct
```

> Private members are not exported when the module is imported

### Doc Comments

Document functions, structs, and fields with /// comments.

**Syntax:**
```
/// description
```

**Example:**
```bishop
/// This is a doc comment for the function
fn add(int a, int b) -> int {
    return a + b;
}

/// Doc comment for struct
Person :: struct {
    /// Doc comment for field
    name str
}
```
