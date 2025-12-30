/**
 * @file algo.hpp
 * @brief Built-in algo module header.
 *
 * Declares the AST creation functions for the algo module.
 * The actual runtime is in runtime/algo/algo.hpp.
 */

#pragma once

#include "parser/ast.hpp"
#include <memory>
#include <string>

namespace nog::stdlib {

/**
 * Creates the AST for the built-in algo module.
 * Contains:
 * - Sorting: sort_int, sort_desc_int, sort_str, sort_desc_str
 * - Min/Max: min_int, max_int, min_str, max_str, min_float, max_float
 * - Aggregation: sum_int, sum_float, product_int, product_float, average_int, average_float
 * - Predicates: all_int, any_int, none_int, count_int, find_int, find_index_int, etc.
 * - Transformations: map_int, map_str, filter_int, filter_str, reduce_int, reduce_str
 * - Reordering: reverse_int, reverse_str, reversed_int, reversed_str, rotate_int, rotate_str, unique_int, unique_str
 */
std::unique_ptr<Program> create_algo_module();

/**
 * Generates the algo runtime code (empty - uses precompiled header).
 */
std::string generate_algo_runtime();

}  // namespace nog::stdlib
