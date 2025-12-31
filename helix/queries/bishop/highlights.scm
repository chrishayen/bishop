; highlights.scm - Syntax highlighting queries for Bishop language
; Bishop is a modern programming language that transpiles to C++

; ============================================
; Keywords
; ============================================

; Control flow keywords
[
  "if"
  "else"
  "while"
  "for"
  "in"
  "return"
  "break"
  "continue"
  "match"
  "select"
  "case"
  "default"
] @keyword.control.flow

; Function and struct keywords
[
  "fn"
  "struct"
] @keyword.function

; Storage/declaration keywords
[
  "const"
  "import"
  "using"
  "extern"
] @keyword.storage

; Visibility
"@" @punctuation.special
"private" @keyword.storage.modifier

; Error handling keywords
[
  "err"
  "fail"
  "or"
] @keyword.control.exception

; Concurrency keywords
"go" @keyword.coroutine

; ============================================
; Types
; ============================================

; Primitive types
(primitive_type) @type.builtin

; Built-in generic types in generic_type
(generic_type
  type: "Channel" @type.builtin)
(generic_type
  type: "List" @type.builtin)
(generic_type
  type: "Pair" @type.builtin)
(generic_type
  type: "Tuple" @type.builtin)

; Type identifiers (PascalCase)
(type_identifier) @type

; Qualified types (module.Type)
(qualified_type
  module: (identifier) @module
  type: (type_identifier) @type)

; Optional type marker
(optional_type "?" @type)

; Pointer type
(pointer_type "&" @type)

; Function type
(function_type "fn" @type.builtin)

; ============================================
; Functions and Methods
; ============================================

; Function definitions
(function_definition
  name: (identifier) @function)

; Method definitions
(method_definition
  struct_name: (identifier) @type
  method_name: (identifier) @function.method)

; Struct definitions
(struct_definition
  name: (identifier) @type)

; Error definitions
(error_definition
  name: (identifier) @type)

; Extern function definitions
(extern_function
  name: (identifier) @function)

; Function calls
(call_expression
  function: (identifier) @function.call)

; Method calls
(call_expression
  function: (member_expression
    member: (identifier) @function.method.call))

; Generic type calls (Channel<int>(), List<str>())
(call_expression
  function: (generic_type) @function.call)

; ============================================
; Variables and Parameters
; ============================================

; Function parameters
(parameter
  name: (identifier) @variable.parameter)

; Self parameter
"self" @variable.builtin

; Variable declarations
(variable_declaration
  name: (identifier) @variable)

; Struct field definitions
(struct_field
  name: (identifier) @variable.other.member)

; Struct field initializers
(field_initializer
  name: (identifier) @variable.other.member)

; Const declarations
(const_declaration
  name: (identifier) @constant)

; For loop variable
(for_statement
  variable: (identifier) @variable)

; Case clause variable
(case_clause
  variable: (identifier) @variable)

; ============================================
; Literals
; ============================================

; Strings
(string_literal) @string

; Numbers
(integer_literal) @constant.numeric.integer
(float_literal) @constant.numeric.float

; Booleans
(boolean_literal) @constant.builtin.boolean

; None/null
(none_literal) @constant.builtin

; ============================================
; Operators
; ============================================

; Arithmetic operators
[
  "+"
  "-"
  "*"
  "/"
] @operator

; Comparison operators
[
  "=="
  "!="
  "<"
  ">"
  "<="
  ">="
] @operator

; Assignment operators
[
  "="
  ":="
] @operator

; Logical operators
"!" @operator

; Type operators
[
  "->"
  "::"
  ".."
] @operator

; Default operator
"default" @keyword.operator

; ============================================
; Punctuation
; ============================================

; Delimiters
[
  ","
  "."
  ":"
  ";"
] @punctuation.delimiter

; Brackets
[
  "("
  ")"
  "{"
  "}"
  "["
  "]"
] @punctuation.bracket

; Angle brackets only in generic type contexts (not comparison operators)
(generic_type
  "<" @punctuation.bracket
  ">" @punctuation.bracket)

; ============================================
; Comments
; ============================================

(comment) @comment.line
(doc_comment) @comment.documentation

; ============================================
; Imports and Modules
; ============================================

(import_statement
  module: (identifier) @module)

(module_path
  (identifier) @module)

(using_statement
  path: (module_path) @module)

; ============================================
; Match Expressions
; ============================================

(match_arm
  pattern: (identifier) @type)

(match_arm
  pattern: (type_identifier) @type)

; Wildcard pattern
(match_arm
  pattern: "_" @variable.builtin)

"=>" @operator

; ============================================
; Special Identifiers
; ============================================

; Common built-in function calls
((identifier) @function.builtin
  (#any-of? @function.builtin
    "assert"
    "assert_eq"
    "print"
    "println"
    "len"
    "sleep"))

; Member access on identifier
(member_expression
  object: (identifier) @variable
  member: (identifier) @variable.other.member)
