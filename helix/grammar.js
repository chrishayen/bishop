/// <reference types="tree-sitter-cli/dsl" />
// @ts-check
/**
 * Tree-sitter grammar for the Bishop programming language
 * Bishop is a modern programming language that transpiles to C++
 *
 * Grammar based on the lexer and parser implementation in:
 * - lexer/lexer.cpp
 * - parser/parser.cpp
 */

const PREC = {
  OR: 1,
  DEFAULT: 2,
  COMPARISON: 3,
  ADDITIVE: 4,
  MULTIPLICATIVE: 5,
  UNARY: 6,
  CALL: 7,
  MEMBER: 8,
};

module.exports = grammar({
  name: 'bishop',

  extras: $ => [
    /\s/,
    $.doc_comment,  // Must come before comment to match /// first
    $.comment,
  ],

  word: $ => $.identifier,

  conflicts: $ => [
    [$.parameter_list, $.function_type],
    [$.type_expr, $.optional_type],
  ],

  rules: {
    source_file: $ => repeat($._item),

    _item: $ => choice(
      $.import_statement,
      $.using_statement,
      $.function_definition,
      $.struct_definition,
      $.method_definition,
      $.error_definition,
      $.extern_function,
      $.const_declaration,
    ),

    // ============================================
    // Comments
    // ============================================

    // Regular line comment: // followed by optional non-/ char then anything
    // This ensures /// is not matched as a regular comment
    comment: _ => token(prec(-1, seq('//', /[^\n]*/))),

    // Doc comment: /// followed by anything (higher precedence)
    doc_comment: _ => token(prec(1, seq('///', /[^\n]*/))),

    // ============================================
    // Imports and Using
    // ============================================

    import_statement: $ => seq(
      'import',
      field('module', $.identifier),
      optional(';'),
    ),

    using_statement: $ => seq(
      'using',
      field('path', $.module_path),
      optional(';'),
    ),

    module_path: $ => seq(
      $.identifier,
      repeat(seq('::', $.identifier)),
      optional(seq('::', '*')),
    ),

    // ============================================
    // Function Definitions
    // ============================================

    function_definition: $ => seq(
      optional($.visibility),
      'fn',
      field('name', $.identifier),
      field('parameters', $.parameter_list),
      optional(field('return_type', $.return_type)),
      field('body', $.block),
    ),

    visibility: $ => seq('@', 'private'),

    return_type: $ => seq(
      '->',
      choice($.type_expr, $.qualified_type),
      optional(seq('or', 'err')),
    ),

    parameter_list: $ => seq(
      '(',
      optional(commaSep($.parameter)),
      ')',
    ),

    parameter: $ => choice(
      seq(field('type', $._param_type), field('name', $.identifier)),
      'self',
    ),

    // Parameter types can include module-qualified types like http.Request
    _param_type: $ => choice(
      $.type_expr,
      $.qualified_type,
    ),

    // Module-qualified type: module.Type
    qualified_type: $ => seq(
      field('module', $.identifier),
      '.',
      field('type', $.type_identifier),
    ),

    // ============================================
    // Struct Definitions
    // ============================================

    struct_definition: $ => seq(
      optional($.visibility),
      field('name', $._type_name),
      '::',
      'struct',
      field('body', $.struct_body),
    ),

    struct_body: $ => seq(
      '{',
      optional(commaSep($.struct_field)),
      '}',
    ),

    struct_field: $ => seq(
      field('name', $.identifier),
      field('type', $.type_expr),
    ),

    // ============================================
    // Method Definitions
    // ============================================

    method_definition: $ => seq(
      optional($.visibility),
      field('struct_name', $._type_name),
      '::',
      field('method_name', $.identifier),
      field('parameters', $.parameter_list),
      optional(field('return_type', $.return_type)),
      field('body', $.block),
    ),

    // ============================================
    // Error Definitions
    // ============================================

    error_definition: $ => seq(
      optional($.visibility),
      field('name', $._type_name),
      '::',
      'err',
      optional(field('body', $.error_body)),
      optional(';'),
    ),

    error_body: $ => seq(
      '{',
      optional(commaSep($.struct_field)),
      '}',
    ),

    // ============================================
    // Extern Functions
    // ============================================

    extern_function: $ => seq(
      '@',
      'extern',
      '(',
      $.string_literal,
      ')',
      'fn',
      field('name', $.identifier),
      field('parameters', $.parameter_list),
      optional(field('return_type', $.return_type)),
      optional(';'),
    ),

    // ============================================
    // Const Declarations
    // ============================================

    const_declaration: $ => seq(
      'const',
      field('name', $.identifier),
      optional(seq(':', $.type_expr)),
      '=',
      field('value', $._expression),
      optional(';'),
    ),

    // ============================================
    // Types
    // ============================================

    // Type hierarchy to avoid ambiguity:
    // type_expr = base_type | base_type? | &base_type
    type_expr: $ => choice(
      $._base_type,
      $.optional_type,
      $.pointer_type,
    ),

    _base_type: $ => choice(
      $.primitive_type,
      $.type_identifier,
      $.generic_type,
      $.function_type,
    ),

    type_identifier: _ => /[A-Z][a-zA-Z0-9_]*/,

    // Type name can be either PascalCase (type_identifier) or lowercase (identifier)
    _type_name: $ => choice($.type_identifier, $.identifier),

    primitive_type: _ => choice(
      'int', 'str', 'bool', 'f32', 'f64', 'u32', 'u64', 'cint', 'cstr', 'void',
    ),

    generic_type: $ => seq(
      field('type', choice('Channel', 'List', 'Pair', 'Tuple', $.type_identifier)),
      '<',
      commaSep1($.type_expr),
      '>',
    ),

    // Optional types apply to base types only: int?, List<int>?, etc.
    optional_type: $ => seq($._base_type, '?'),

    function_type: $ => seq(
      'fn',
      '(',
      optional(commaSep($.type_expr)),
      ')',
      optional(seq('->', $.type_expr)),
    ),

    // Pointer types apply to base types only: &int, &List<int>, etc.
    pointer_type: $ => seq('&', $._base_type),

    // ============================================
    // Blocks and Statements
    // ============================================

    block: $ => seq(
      '{',
      repeat($._statement),
      '}',
    ),

    _statement: $ => choice(
      $.variable_declaration,
      $.return_statement,
      $.if_statement,
      $.while_statement,
      $.for_statement,
      $.select_statement,
      $.break_statement,
      $.continue_statement,
      $.expression_statement,
    ),

    variable_declaration: $ => seq(
      optional(field('type', $.type_expr)),
      field('name', $.identifier),
      choice(':=', '='),
      field('value', $._expression),
      optional(';'),
    ),

    return_statement: $ => prec.right(seq(
      'return',
      optional(field('value', $._expression)),
      optional(';'),
    )),

    break_statement: _ => seq('break', optional(';')),

    continue_statement: _ => seq('continue', optional(';')),

    if_statement: $ => prec.right(seq(
      'if',
      field('condition', $._expression),
      field('consequence', $.block),
      optional(seq('else', field('alternative', choice($.block, $.if_statement)))),
    )),

    while_statement: $ => seq(
      'while',
      field('condition', $._expression),
      field('body', $.block),
    ),

    for_statement: $ => seq(
      'for',
      field('variable', $.identifier),
      'in',
      field('iterable', $._expression),
      optional(seq('..', field('end', $._expression))),
      field('body', $.block),
    ),

    select_statement: $ => seq(
      'select',
      '{',
      repeat($.case_clause),
      optional($.default_clause),
      '}',
    ),

    case_clause: $ => seq(
      'case',
      optional(seq(field('variable', $.identifier), ':=')),
      $._expression,
      field('body', $.block),
    ),

    default_clause: $ => seq('default', field('body', $.block)),

    expression_statement: $ => seq($._expression, optional(';')),

    // ============================================
    // Expressions
    // ============================================

    _expression: $ => choice(
      $.identifier,
      $.integer_literal,
      $.float_literal,
      $.string_literal,
      $.boolean_literal,
      $.none_literal,
      $.list_literal,
      $.struct_instantiation,
      $.call_expression,
      $.member_expression,
      $.index_expression,
      $.binary_expression,
      $.unary_expression,
      $.lambda_expression,
      $.go_expression,
      $.parenthesized_expression,
      $.or_expression,
      $.match_expression,
      $.fail_expression,
    ),

    parenthesized_expression: $ => seq('(', $._expression, ')'),

    // ============================================
    // Literals
    // ============================================

    identifier: _ => /[a-z_][a-zA-Z0-9_]*/,

    integer_literal: _ => /\d+/,

    float_literal: _ => /\d+\.\d+/,

    string_literal: _ => choice(
      seq('"', /[^"]*/, '"'),
      seq("'", /[^']*/, "'"),
      seq('r"', /[^"]*/, '"'),
      seq("r'", /[^']*/, "'"),
    ),

    boolean_literal: _ => choice('true', 'false'),

    none_literal: _ => 'none',

    list_literal: $ => seq('[', optional(commaSep($._expression)), ']'),

    struct_instantiation: $ => seq(
      field('type', $.type_identifier),
      '{',
      optional(commaSep($.field_initializer)),
      '}',
    ),

    field_initializer: $ => seq(
      field('name', $.identifier),
      ':',
      field('value', $._expression),
    ),

    // ============================================
    // Operators
    // ============================================

    binary_expression: $ => choice(
      prec.left(PREC.ADDITIVE, seq($._expression, '+', $._expression)),
      prec.left(PREC.ADDITIVE, seq($._expression, '-', $._expression)),
      prec.left(PREC.MULTIPLICATIVE, seq($._expression, '*', $._expression)),
      prec.left(PREC.MULTIPLICATIVE, seq($._expression, '/', $._expression)),
      prec.left(PREC.COMPARISON, seq($._expression, '==', $._expression)),
      prec.left(PREC.COMPARISON, seq($._expression, '!=', $._expression)),
      prec.left(PREC.COMPARISON, seq($._expression, '<', $._expression)),
      prec.left(PREC.COMPARISON, seq($._expression, '>', $._expression)),
      prec.left(PREC.COMPARISON, seq($._expression, '<=', $._expression)),
      prec.left(PREC.COMPARISON, seq($._expression, '>=', $._expression)),
      prec.left(PREC.DEFAULT, seq($._expression, 'default', $._expression)),
    ),

    unary_expression: $ => prec(PREC.UNARY, seq(choice('!', '-', '&'), $._expression)),

    or_expression: $ => prec.right(PREC.OR, seq(
      $._expression,
      'or',
      choice(
        seq('return', optional($._expression)),
        seq('fail', choice('err', $._expression)),
        $.match_expression,
        $.block,
      ),
    )),

    match_expression: $ => seq(
      'match',
      optional('err'),
      '{',
      repeat($.match_arm),
      '}',
    ),

    match_arm: $ => seq(
      field('pattern', choice($.identifier, $.type_identifier, '_')),
      '=>',
      choice($._expression, seq('fail', choice('err', $._expression))),
      optional(','),
    ),

    fail_expression: $ => prec(PREC.UNARY + 1, seq('fail', choice($.string_literal, $.struct_instantiation))),

    // ============================================
    // Calls and Member Access
    // ============================================

    call_expression: $ => prec(PREC.CALL, seq(
      field('function', choice($.identifier, $.type_identifier, $.member_expression, $.generic_type)),
      field('arguments', $.argument_list),
    )),

    member_expression: $ => prec.left(PREC.MEMBER, seq(
      field('object', $._expression),
      '.',
      field('member', $.identifier),
    )),

    index_expression: $ => prec(PREC.MEMBER, seq(
      field('object', $._expression),
      '[',
      field('index', $._expression),
      ']',
    )),

    argument_list: $ => seq('(', optional(commaSep($._expression)), ')'),

    // ============================================
    // Lambda Expressions
    // ============================================

    lambda_expression: $ => seq(
      'fn',
      field('parameters', $.parameter_list),
      optional(field('return_type', $.return_type)),
      field('body', $.block),
    ),

    // ============================================
    // Concurrency
    // ============================================

    go_expression: $ => prec(PREC.CALL + 1, seq('go', choice($.call_expression, $.lambda_expression))),
  },
});

/**
 * Comma-separated list with optional trailing comma
 */
function commaSep(rule) {
  return optional(seq(rule, repeat(seq(',', rule)), optional(',')));
}

/**
 * Comma-separated list with at least one element
 */
function commaSep1(rule) {
  return seq(rule, repeat(seq(',', rule)), optional(','));
}
