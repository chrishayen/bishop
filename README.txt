Deez Programming Language
===========================

Deez is a modern programming language that transpiles to C++. It provides
elegant, Pythonic syntax while leveraging C++ performance and memory management.

Key Features
------------

- Clean, readable syntax inspired by Python and Go
- Strong static typing with type inference
- First-class functions and closures
- Struct-based object system with methods
- Automatic resource management with 'with' blocks
- Comprehensive error handling with 'or' and 'fail'
- Built-in HTTP server and filesystem modules
- Foreign Function Interface (FFI) for C library integration
- Generic collections (List<T>)

Quick Example
-------------

    fn greet(str name) -> str {
        return "Hello, " + name + "!";
    }

    fn main() {
        message := greet("World");
        print(message);
    }

Types
-----

Primitives: int, str, bool, char, f32, f64, u32, u64
Optionals:  int?, str? (can be 'none')
Collections: List<T>

Error Handling
--------------

    ParseError :: err;

    fn parse(str input) -> int or err {
        if input.empty() {
            fail ParseError { message: "empty input" };
        }
        return 42;
    }

    fn main() {
        result := parse("test") or {
            print("Error:", err.message);
            return;
        };
    }

File Extension
--------------

Deez source files use the .b extension.

Building
--------

Use the Makefile in the root of the project:

    make        # Build the compiler
    make test   # Run tests
