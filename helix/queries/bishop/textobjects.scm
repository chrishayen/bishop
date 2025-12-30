; textobjects.scm - Code navigation queries for Bishop language
; Defines text objects for selecting functions, classes, parameters, etc.

; ============================================
; Functions
; ============================================

; Function definitions
(function_definition) @function.outer
(function_definition
  body: (block) @function.inner)

; Method definitions
(method_definition) @function.outer
(method_definition
  body: (block) @function.inner)

; Lambda expressions
(lambda_expression) @function.outer
(lambda_expression
  body: (block) @function.inner)

; ============================================
; Classes/Types
; ============================================

; Struct definitions
(struct_definition) @class.outer
(struct_definition
  body: (struct_body) @class.inner)

; Error definitions
(error_definition) @class.outer
(error_definition
  body: (error_body) @class.inner)

; ============================================
; Parameters
; ============================================

; Function parameters
(parameter_list) @parameter.outer
(parameter) @parameter.inner

; ============================================
; Comments
; ============================================

(comment) @comment.outer
(comment) @comment.inner

(doc_comment) @comment.outer
(doc_comment) @comment.inner

; ============================================
; Arguments
; ============================================

(argument_list) @argument.outer

; ============================================
; Blocks
; ============================================

(block) @block.outer

; If statement blocks
(if_statement) @conditional.outer
(if_statement
  consequence: (block) @conditional.inner)

; ============================================
; Loops
; ============================================

; While loops
(while_statement) @loop.outer
(while_statement
  body: (block) @loop.inner)

; For loops
(for_statement) @loop.outer
(for_statement
  body: (block) @loop.inner)

; ============================================
; Statements
; ============================================

[
  (variable_declaration)
  (return_statement)
  (expression_statement)
  (break_statement)
  (continue_statement)
] @statement.outer

; ============================================
; Entries (struct fields, list items, etc.)
; ============================================

; Struct fields
(struct_field) @entry.outer
(field_initializer) @entry.outer
