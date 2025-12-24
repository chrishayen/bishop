# Pointers, Handles, and FFI Implementation Plan

## Overview

Leverage C++ best practices for memory safety and FFI:
- References for pass-by-reference (non-null, non-rebindable)
- RAII handles for resource management
- Clean FFI with automatic type conversions

---

## Phase 1: Refine Pointer System to References

**Goal:** `&` is only for pass-by-reference to functions, not storable in variables.

### Current Behavior
```nog
Person p = &bob;     // storing pointer in variable - REMOVE
p := &bob;           // inferred pointer variable - REMOVE
fn process(Person *p) // pointer parameter - KEEP
process(&bob);        // call site - KEEP
```

### New Behavior (Go-style)
```nog
fn process(Person *p) {    // pointer parameter (like Go)
    p.age = 30;            // auto-deref, always mutable
}

bob := Person { name: "Bob", age: 30 };
process(&bob);             // explicit & at call site (like Go)

// These become errors:
Person p = &bob;           // ERROR: cannot store pointer
p := &bob;                 // ERROR: cannot store pointer
```

### Design Decisions
- Parameter syntax: `Person *p` (like Go)
- Call site: explicit `&bob` required (like Go)
- Mutability: always mutable (like Go)
- No pointer storage in variables - only pass-by-reference

### Tasks
- [x] Update typechecker: disallow AddressOf in variable declarations
- [x] Update typechecker: only allow AddressOf as function argument
- [x] Update tests/test_pointers.n - remove variable storage tests
- [x] Add error test for storing pointers
- [x] Update CLAUDE.md reference

---

## Phase 2: Handle Types (RAII Wrappers)

**Goal:** Automatic resource management like C++ smart pointers.

### Syntax Options

#### Option A: Built-in unique/shared
```nog
file := unique<File>(fs.open("data.txt"));
// automatically closed when out of scope

shared := shared<Connection>(db.connect());
// reference counted, closed when last ref drops
```

#### Option B: Handle keyword
```nog
handle file := fs.open("data.txt");
// compiler tracks ownership, auto-cleanup
```

#### Option C: Drop trait pattern
```nog
File :: struct {
    fd int
}

File :: drop(self) {
    close(self.fd);
}

// any struct with drop() is auto-cleaned
```

### Tasks
- [ ] Decide on syntax (A, B, or C)
- [ ] Implement ownership tracking in typechecker
- [ ] Generate destructor calls in codegen
- [ ] Handle move semantics (transfer ownership)
- [ ] Add compile error for use-after-move
- [ ] Create tests/test_handles.n
- [ ] Update CLAUDE.md reference

---

## Phase 3: Expand FFI

**Goal:** Full interop with C libraries.

### 3.1 Type Mapping (existing)
```nog
@extern("c") fn puts(cstr s) -> cint;
@extern("m") fn sqrt(f64 x) -> f64;
```

### 3.2 Struct Passing to C
```nog
// C-compatible struct layout
@repr("c")
Point :: struct {
    x f64,
    y f64
}

@extern("mylib") fn transform(Point p) -> Point;
@extern("mylib") fn transform_ptr(Point &p);
```

### 3.3 Callbacks (function pointers to C)
```nog
@extern("c") fn qsort(
    cptr base,
    csize count,
    csize size,
    fn(cptr, cptr) -> cint compare
);
```

### 3.4 Opaque Handles from C
```nog
// C returns opaque pointer we wrap
FileHandle :: extern_handle;

@extern("c") fn fopen(cstr path, cstr mode) -> FileHandle;
@extern("c") fn fclose(FileHandle f) -> cint;
@extern("c") fn fread(cptr buf, csize size, csize count, FileHandle f) -> csize;
```

### Tasks
- [ ] Add `@repr("c")` for C-layout structs
- [ ] Implement struct-by-value passing to extern functions
- [ ] Implement struct-by-reference passing to extern functions
- [ ] Add function pointer types for callbacks
- [ ] Generate proper callback wrappers
- [ ] Add `extern_handle` type for opaque C pointers
- [ ] Add more C types: `cptr`, `csize`, `cvoid`
- [ ] Create tests/test_ffi_advanced.n
- [ ] Update CLAUDE.md reference

---

## Implementation Order

1. **Phase 1** - Clean up pointer semantics first (breaking change, do it now)
2. **Phase 2** - Add handles for safe resource management
3. **Phase 3** - Expand FFI capabilities

---

## Open Questions

1. Should references be nullable? (`Person &? p`)
2. Should we support `const &` for read-only references?
3. For handles, prefer built-in types or user-defined drop?
4. Should extern_handle auto-wrap in RAII?

---

## Progress

| Phase | Status | Notes |
|-------|--------|-------|
| Phase 1: References | Complete | Go-style pass-by-reference only |
| Phase 2: Handles | Not Started | |
| Phase 3: FFI | Partial | Basic @extern works |
