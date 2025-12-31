; locals.scm - Scope and local variable queries for Bishop language
; Used for scope-aware highlighting and reference resolution

; ============================================
; Scopes
; ============================================

; Function bodies create scopes
(function_definition
  body: (block) @local.scope)

; Method bodies create scopes
(method_definition
  body: (block) @local.scope)

; Lambda bodies create scopes
(lambda_expression
  body: (block) @local.scope)

; Blocks create scopes
(block) @local.scope

; If statements create scopes
(if_statement
  consequence: (block) @local.scope)

(if_statement
  alternative: (block) @local.scope)

; While loops create scopes
(while_statement
  body: (block) @local.scope)

; For loops create scopes
(for_statement
  body: (block) @local.scope)

; Select/case statements create scopes
(select_statement) @local.scope
(case_clause
  body: (block) @local.scope)

; Match expressions create scopes
(match_expression) @local.scope

; ============================================
; Definitions
; ============================================

; Function parameters are definitions
(parameter
  name: (identifier) @local.definition)

; Variable declarations are definitions
(variable_declaration
  name: (identifier) @local.definition)

; For loop variable is a definition
(for_statement
  variable: (identifier) @local.definition)

; Case clause variable (e.g., case val := ch.recv())
(case_clause
  variable: (identifier) @local.definition)

; Const declarations
(const_declaration
  name: (identifier) @local.definition)

; ============================================
; References
; ============================================

; Identifiers are references
(identifier) @local.reference
