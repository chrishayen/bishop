; indents.scm - Auto-indentation queries for Bishop language

; ============================================
; Indent triggers
; ============================================

; Blocks increase indent
[
  (block)
  (struct_body)
  (error_body)
  (select_statement)
  (match_expression)
] @indent

; ============================================
; Outdent triggers
; ============================================

; Closing braces decrease indent
"}" @outdent

; ============================================
; Indent on specific constructs
; ============================================

; Function definitions
(function_definition
  body: (block) @indent)

; Method definitions
(method_definition
  body: (block) @indent)

; Struct definitions
(struct_definition
  body: (struct_body) @indent)

; If statements
(if_statement
  consequence: (block) @indent)

(if_statement
  alternative: (block) @indent)

; While loops
(while_statement
  body: (block) @indent)

; For loops
(for_statement
  body: (block) @indent)

; Lambda expressions
(lambda_expression
  body: (block) @indent)

; Select/case statements
(select_statement) @indent
(case_clause) @indent
(default_clause) @indent

; Match expressions
(match_expression) @indent

; ============================================
; Extend behavior
; ============================================

; Multi-line constructs that should extend
(struct_instantiation) @extend
(list_literal) @extend
(argument_list) @extend
