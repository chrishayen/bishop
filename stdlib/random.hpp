/**
 * @file random.hpp
 * @brief Built-in random module for Bishop.
 *
 * Provides random number generation functionality using std::mt19937.
 * Includes functions for random integers, floats, booleans, and
 * list operations like choice, shuffle, and sample.
 */

#pragma once
#include "parser/ast.hpp"
#include <memory>
#include <string>

namespace nog::stdlib {

/**
 * Creates the AST for the built-in random module.
 * Contains:
 * - int(min, max) -> int: Random integer in range [min, max]
 * - float() -> f64: Random float in [0, 1)
 * - float_range(min, max) -> f64: Random float in [min, max)
 * - bool() -> bool: Random boolean with 50/50 probability
 * - bool_prob(probability) -> bool: Random boolean with given probability
 * - choice(list) -> str: Random element from string list
 * - choice_int(list) -> int: Random element from int list
 * - shuffle(list): Shuffles string list in place
 * - shuffle_int(list): Shuffles int list in place
 * - sample(list, n) -> List<str>: Sample n elements from string list
 * - sample_int(list, n) -> List<int>: Sample n elements from int list
 * - seed(n): Seed the random generator for deterministic sequences
 */
std::unique_ptr<Program> create_random_module();

/**
 * Generates the C++ runtime code for the random module.
 * Returns empty since random.hpp is included via build system.
 */
std::string generate_random_runtime();

}  // namespace nog::stdlib
