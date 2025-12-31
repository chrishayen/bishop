; indents.scm - Auto-indentation queries for Bishop language

; ============================================
; Indent triggers
; ============================================

; Blocks and bodies increase indent
[
  (block)
  (struct_body)
  (error_body)
] @indent

; ============================================
; Outdent triggers
; ============================================

; Closing braces decrease indent
"}" @outdent

; ============================================
; Special indent cases
; ============================================

; Select/case statements need indent for their clauses
(case_clause) @indent
(default_clause) @indent

; Match arms need indent
(match_arm) @indent

; ============================================
; Extend behavior
; ============================================

; Multi-line constructs that should extend
(struct_instantiation) @extend
(list_literal) @extend
(argument_list) @extend
