/**
 * @file algo.cpp
 * @brief Built-in algo module implementation.
 *
 * Creates the AST definitions for the algo module.
 * The actual runtime is in runtime/algo/algo.hpp.
 */

/**
 * @nog_fn sort_int
 * @module algo
 * @description Sorts an integer list in ascending order (in-place).
 * @param list List<int> - The list to sort
 * @example
 * nums := [3, 1, 4, 1, 5];
 * algo.sort_int(nums);  // [1, 1, 3, 4, 5]
 */

/**
 * @nog_fn sort_desc_int
 * @module algo
 * @description Sorts an integer list in descending order (in-place).
 * @param list List<int> - The list to sort
 * @example
 * nums := [3, 1, 4, 1, 5];
 * algo.sort_desc_int(nums);  // [5, 4, 3, 1, 1]
 */

/**
 * @nog_fn sort_str
 * @module algo
 * @description Sorts a string list in ascending order (in-place).
 * @param list List<str> - The list to sort
 * @example
 * names := ["charlie", "alice", "bob"];
 * algo.sort_str(names);  // ["alice", "bob", "charlie"]
 */

/**
 * @nog_fn sort_desc_str
 * @module algo
 * @description Sorts a string list in descending order (in-place).
 * @param list List<str> - The list to sort
 * @example
 * names := ["charlie", "alice", "bob"];
 * algo.sort_desc_str(names);  // ["charlie", "bob", "alice"]
 */

/**
 * @nog_fn sort_float
 * @module algo
 * @description Sorts a float list in ascending order (in-place).
 * @param list List<f64> - The list to sort
 * @example
 * vals := [3.14, 1.41, 2.72];
 * algo.sort_float(vals);  // [1.41, 2.72, 3.14]
 */

/**
 * @nog_fn sort_desc_float
 * @module algo
 * @description Sorts a float list in descending order (in-place).
 * @param list List<f64> - The list to sort
 * @example
 * vals := [3.14, 1.41, 2.72];
 * algo.sort_desc_float(vals);  // [3.14, 2.72, 1.41]
 */

/**
 * @nog_fn min_int
 * @module algo
 * @description Returns the minimum value in an integer list.
 * @param list List<int> - The list to search
 * @returns int or err - The minimum value, or error if list is empty
 * @example
 * min_val := algo.min_int([3, 1, 4]) or return;  // 1
 */

/**
 * @nog_fn max_int
 * @module algo
 * @description Returns the maximum value in an integer list.
 * @param list List<int> - The list to search
 * @returns int or err - The maximum value, or error if list is empty
 * @example
 * max_val := algo.max_int([3, 1, 4]) or return;  // 4
 */

/**
 * @nog_fn min_float
 * @module algo
 * @description Returns the minimum value in a float list.
 * @param list List<f64> - The list to search
 * @returns f64 or err - The minimum value, or error if list is empty
 * @example
 * min_val := algo.min_float([3.14, 1.41, 2.72]) or return;  // 1.41
 */

/**
 * @nog_fn max_float
 * @module algo
 * @description Returns the maximum value in a float list.
 * @param list List<f64> - The list to search
 * @returns f64 or err - The maximum value, or error if list is empty
 * @example
 * max_val := algo.max_float([3.14, 1.41, 2.72]) or return;  // 3.14
 */

/**
 * @nog_fn min_str
 * @module algo
 * @description Returns the minimum string in a list (lexicographically).
 * @param list List<str> - The list to search
 * @returns str or err - The minimum string, or error if list is empty
 * @example
 * min_name := algo.min_str(["charlie", "alice", "bob"]) or return;  // "alice"
 */

/**
 * @nog_fn max_str
 * @module algo
 * @description Returns the maximum string in a list (lexicographically).
 * @param list List<str> - The list to search
 * @returns str or err - The maximum string, or error if list is empty
 * @example
 * max_name := algo.max_str(["charlie", "alice", "bob"]) or return;  // "charlie"
 */

/**
 * @nog_fn sum_int
 * @module algo
 * @description Returns the sum of all integers in a list.
 * @param list List<int> - The list to sum
 * @returns int - The sum (0 for empty list)
 * @example
 * total := algo.sum_int([1, 2, 3, 4, 5]);  // 15
 */

/**
 * @nog_fn sum_float
 * @module algo
 * @description Returns the sum of all floats in a list.
 * @param list List<f64> - The list to sum
 * @returns f64 - The sum (0.0 for empty list)
 * @example
 * total := algo.sum_float([1.5, 2.5, 3.0]);  // 7.0
 */

/**
 * @nog_fn product_int
 * @module algo
 * @description Returns the product of all integers in a list.
 * @param list List<int> - The list to multiply
 * @returns int - The product (1 for empty list)
 * @example
 * result := algo.product_int([2, 3, 4]);  // 24
 */

/**
 * @nog_fn product_float
 * @module algo
 * @description Returns the product of all floats in a list.
 * @param list List<f64> - The list to multiply
 * @returns f64 - The product (1.0 for empty list)
 * @example
 * result := algo.product_float([2.0, 3.0, 4.0]);  // 24.0
 */

/**
 * @nog_fn average_int
 * @module algo
 * @description Returns the average of all integers in a list.
 * @param list List<int> - The list to average
 * @returns f64 or err - The average, or error if list is empty
 * @example
 * avg := algo.average_int([1, 2, 3, 4, 5]) or return;  // 3.0
 */

/**
 * @nog_fn average_float
 * @module algo
 * @description Returns the average of all floats in a list.
 * @param list List<f64> - The list to average
 * @returns f64 or err - The average, or error if list is empty
 * @example
 * avg := algo.average_float([1.0, 2.0, 3.0]) or return;  // 2.0
 */

/**
 * @nog_fn all_int
 * @module algo
 * @description Returns true if all integers satisfy the predicate.
 * @param list List<int> - The list to check
 * @param predicate fn(int) -> bool - The predicate function
 * @returns bool - True if all elements satisfy the predicate
 * @example
 * all_positive := algo.all_int([1, 2, 3], fn(int x) -> bool { return x > 0; });
 */

/**
 * @nog_fn any_int
 * @module algo
 * @description Returns true if any integer satisfies the predicate.
 * @param list List<int> - The list to check
 * @param predicate fn(int) -> bool - The predicate function
 * @returns bool - True if any element satisfies the predicate
 * @example
 * has_five := algo.any_int([1, 5, 3], fn(int x) -> bool { return x == 5; });
 */

/**
 * @nog_fn none_int
 * @module algo
 * @description Returns true if no integers satisfy the predicate.
 * @param list List<int> - The list to check
 * @param predicate fn(int) -> bool - The predicate function
 * @returns bool - True if no elements satisfy the predicate
 * @example
 * no_negative := algo.none_int([1, 2, 3], fn(int x) -> bool { return x < 0; });
 */

/**
 * @nog_fn count_int
 * @module algo
 * @description Returns the count of integers that satisfy the predicate.
 * @param list List<int> - The list to count
 * @param predicate fn(int) -> bool - The predicate function
 * @returns int - The count of matching elements
 * @example
 * evens := algo.count_int([1, 2, 3, 4], fn(int x) -> bool { return x % 2 == 0; });  // 2
 */

/**
 * @nog_fn find_int
 * @module algo
 * @description Returns the first integer that satisfies the predicate.
 * @param list List<int> - The list to search
 * @param predicate fn(int) -> bool - The predicate function
 * @returns int or err - The first matching element, or error if not found
 * @example
 * found := algo.find_int([1, 2, 3], fn(int x) -> bool { return x > 1; }) or return;  // 2
 */

/**
 * @nog_fn find_index_int
 * @module algo
 * @description Returns the index of the first integer that satisfies the predicate.
 * @param list List<int> - The list to search
 * @param predicate fn(int) -> bool - The predicate function
 * @returns int - The index of the first matching element, or -1 if not found
 * @example
 * idx := algo.find_index_int([1, 2, 3], fn(int x) -> bool { return x == 2; });  // 1
 */

/**
 * @nog_fn all_str
 * @module algo
 * @description Returns true if all strings satisfy the predicate.
 * @param list List<str> - The list to check
 * @param predicate fn(str) -> bool - The predicate function
 * @returns bool - True if all elements satisfy the predicate
 * @example
 * all_short := algo.all_str(["a", "bb"], fn(str s) -> bool { return s.length() < 5; });
 */

/**
 * @nog_fn any_str
 * @module algo
 * @description Returns true if any string satisfies the predicate.
 * @param list List<str> - The list to check
 * @param predicate fn(str) -> bool - The predicate function
 * @returns bool - True if any element satisfies the predicate
 * @example
 * has_hello := algo.any_str(["hi", "hello"], fn(str s) -> bool { return s == "hello"; });
 */

/**
 * @nog_fn none_str
 * @module algo
 * @description Returns true if no strings satisfy the predicate.
 * @param list List<str> - The list to check
 * @param predicate fn(str) -> bool - The predicate function
 * @returns bool - True if no elements satisfy the predicate
 * @example
 * no_empty := algo.none_str(["a", "b"], fn(str s) -> bool { return s.empty(); });
 */

/**
 * @nog_fn count_str
 * @module algo
 * @description Returns the count of strings that satisfy the predicate.
 * @param list List<str> - The list to count
 * @param predicate fn(str) -> bool - The predicate function
 * @returns int - The count of matching elements
 * @example
 * long_names := algo.count_str(["a", "abc", "abcde"], fn(str s) -> bool { return s.length() > 2; });
 */

/**
 * @nog_fn find_str
 * @module algo
 * @description Returns the first string that satisfies the predicate.
 * @param list List<str> - The list to search
 * @param predicate fn(str) -> bool - The predicate function
 * @returns str or err - The first matching element, or error if not found
 * @example
 * found := algo.find_str(["a", "bb", "ccc"], fn(str s) -> bool { return s.length() > 1; }) or return;
 */

/**
 * @nog_fn find_index_str
 * @module algo
 * @description Returns the index of the first string that satisfies the predicate.
 * @param list List<str> - The list to search
 * @param predicate fn(str) -> bool - The predicate function
 * @returns int - The index of the first matching element, or -1 if not found
 * @example
 * idx := algo.find_index_str(["a", "bb"], fn(str s) -> bool { return s == "bb"; });  // 1
 */

/**
 * @nog_fn map_int
 * @module algo
 * @description Maps each integer to a new integer using a transform function.
 * @param list List<int> - The list to transform
 * @param transform fn(int) -> int - The transform function
 * @returns List<int> - The transformed list
 * @example
 * doubled := algo.map_int([1, 2, 3], fn(int x) -> int { return x * 2; });  // [2, 4, 6]
 */

/**
 * @nog_fn map_str
 * @module algo
 * @description Maps each string to a new string using a transform function.
 * @param list List<str> - The list to transform
 * @param transform fn(str) -> str - The transform function
 * @returns List<str> - The transformed list
 * @example
 * upper := algo.map_str(["a", "b"], fn(str s) -> str { return s.upper(); });
 */

/**
 * @nog_fn filter_int
 * @module algo
 * @description Filters integers by a predicate, keeping only matching elements.
 * @param list List<int> - The list to filter
 * @param predicate fn(int) -> bool - The predicate function
 * @returns List<int> - The filtered list
 * @example
 * evens := algo.filter_int([1, 2, 3, 4], fn(int x) -> bool { return x % 2 == 0; });  // [2, 4]
 */

/**
 * @nog_fn filter_str
 * @module algo
 * @description Filters strings by a predicate, keeping only matching elements.
 * @param list List<str> - The list to filter
 * @param predicate fn(str) -> bool - The predicate function
 * @returns List<str> - The filtered list
 * @example
 * long_names := algo.filter_str(["a", "abc", "ab"], fn(str s) -> bool { return s.length() > 1; });
 */

/**
 * @nog_fn reduce_int
 * @module algo
 * @description Reduces an integer list to a single value using an accumulator.
 * @param list List<int> - The list to reduce
 * @param accumulator fn(int, int) -> int - The accumulator function
 * @param initial int - The initial value
 * @returns int - The reduced value
 * @example
 * sum := algo.reduce_int([1, 2, 3], fn(int a, int b) -> int { return a + b; }, 0);  // 6
 */

/**
 * @nog_fn reduce_str
 * @module algo
 * @description Reduces a string list to a single string using an accumulator.
 * @param list List<str> - The list to reduce
 * @param accumulator fn(str, str) -> str - The accumulator function
 * @param initial str - The initial value
 * @returns str - The reduced value
 * @example
 * joined := algo.reduce_str(["a", "b", "c"], fn(str a, str b) -> str { return a + b; }, "");
 */

/**
 * @nog_fn reverse_int
 * @module algo
 * @description Reverses an integer list in place.
 * @param list List<int> - The list to reverse
 * @example
 * nums := [1, 2, 3];
 * algo.reverse_int(nums);  // [3, 2, 1]
 */

/**
 * @nog_fn reverse_str
 * @module algo
 * @description Reverses a string list in place.
 * @param list List<str> - The list to reverse
 * @example
 * names := ["a", "b", "c"];
 * algo.reverse_str(names);  // ["c", "b", "a"]
 */

/**
 * @nog_fn reversed_int
 * @module algo
 * @description Returns a reversed copy of an integer list.
 * @param list List<int> - The list to reverse
 * @returns List<int> - The reversed copy
 * @example
 * rev := algo.reversed_int([1, 2, 3]);  // [3, 2, 1]
 */

/**
 * @nog_fn reversed_str
 * @module algo
 * @description Returns a reversed copy of a string list.
 * @param list List<str> - The list to reverse
 * @returns List<str> - The reversed copy
 * @example
 * rev := algo.reversed_str(["a", "b", "c"]);  // ["c", "b", "a"]
 */

/**
 * @nog_fn rotate_int
 * @module algo
 * @description Rotates an integer list left by n positions (in-place).
 * @param list List<int> - The list to rotate
 * @param n int - Number of positions to rotate (positive = left, negative = right)
 * @example
 * nums := [1, 2, 3, 4, 5];
 * algo.rotate_int(nums, 2);  // [3, 4, 5, 1, 2]
 */

/**
 * @nog_fn rotate_str
 * @module algo
 * @description Rotates a string list left by n positions (in-place).
 * @param list List<str> - The list to rotate
 * @param n int - Number of positions to rotate
 * @example
 * names := ["a", "b", "c", "d"];
 * algo.rotate_str(names, 1);  // ["b", "c", "d", "a"]
 */

/**
 * @nog_fn unique_int
 * @module algo
 * @description Returns a new list with unique integers (order preserved).
 * @param list List<int> - The list to deduplicate
 * @returns List<int> - The deduplicated list
 * @example
 * uniq := algo.unique_int([1, 2, 1, 3, 2]);  // [1, 2, 3]
 */

/**
 * @nog_fn unique_str
 * @module algo
 * @description Returns a new list with unique strings (order preserved).
 * @param list List<str> - The list to deduplicate
 * @returns List<str> - The deduplicated list
 * @example
 * uniq := algo.unique_str(["a", "b", "a"]);  // ["a", "b"]
 */

#include "algo.hpp"

using namespace std;

namespace bishop::stdlib {

/**
 * Creates the AST for the built-in algo module.
 */
unique_ptr<Program> create_algo_module() {
    auto program = make_unique<Program>();

    // ============================================================
    // Sorting (in-place)
    // ============================================================

    // fn sort_int(List<int> list)
    auto sort_int_fn = make_unique<FunctionDef>();
    sort_int_fn->name = "sort_int";
    sort_int_fn->visibility = Visibility::Public;
    sort_int_fn->params.push_back({"List<int>", "list"});
    sort_int_fn->return_type = "";
    program->functions.push_back(move(sort_int_fn));

    // fn sort_desc_int(List<int> list)
    auto sort_desc_int_fn = make_unique<FunctionDef>();
    sort_desc_int_fn->name = "sort_desc_int";
    sort_desc_int_fn->visibility = Visibility::Public;
    sort_desc_int_fn->params.push_back({"List<int>", "list"});
    sort_desc_int_fn->return_type = "";
    program->functions.push_back(move(sort_desc_int_fn));

    // fn sort_str(List<str> list)
    auto sort_str_fn = make_unique<FunctionDef>();
    sort_str_fn->name = "sort_str";
    sort_str_fn->visibility = Visibility::Public;
    sort_str_fn->params.push_back({"List<str>", "list"});
    sort_str_fn->return_type = "";
    program->functions.push_back(move(sort_str_fn));

    // fn sort_desc_str(List<str> list)
    auto sort_desc_str_fn = make_unique<FunctionDef>();
    sort_desc_str_fn->name = "sort_desc_str";
    sort_desc_str_fn->visibility = Visibility::Public;
    sort_desc_str_fn->params.push_back({"List<str>", "list"});
    sort_desc_str_fn->return_type = "";
    program->functions.push_back(move(sort_desc_str_fn));

    // fn sort_float(List<f64> list)
    auto sort_float_fn = make_unique<FunctionDef>();
    sort_float_fn->name = "sort_float";
    sort_float_fn->visibility = Visibility::Public;
    sort_float_fn->params.push_back({"List<f64>", "list"});
    sort_float_fn->return_type = "";
    program->functions.push_back(move(sort_float_fn));

    // fn sort_desc_float(List<f64> list)
    auto sort_desc_float_fn = make_unique<FunctionDef>();
    sort_desc_float_fn->name = "sort_desc_float";
    sort_desc_float_fn->visibility = Visibility::Public;
    sort_desc_float_fn->params.push_back({"List<f64>", "list"});
    sort_desc_float_fn->return_type = "";
    program->functions.push_back(move(sort_desc_float_fn));

    // ============================================================
    // Min/Max
    // ============================================================

    // fn min_int(List<int> list) -> int or err
    auto min_int_fn = make_unique<FunctionDef>();
    min_int_fn->name = "min_int";
    min_int_fn->visibility = Visibility::Public;
    min_int_fn->params.push_back({"List<int>", "list"});
    min_int_fn->return_type = "int";
    min_int_fn->error_type = "err";
    program->functions.push_back(move(min_int_fn));

    // fn max_int(List<int> list) -> int or err
    auto max_int_fn = make_unique<FunctionDef>();
    max_int_fn->name = "max_int";
    max_int_fn->visibility = Visibility::Public;
    max_int_fn->params.push_back({"List<int>", "list"});
    max_int_fn->return_type = "int";
    max_int_fn->error_type = "err";
    program->functions.push_back(move(max_int_fn));

    // fn min_float(List<f64> list) -> f64 or err
    auto min_float_fn = make_unique<FunctionDef>();
    min_float_fn->name = "min_float";
    min_float_fn->visibility = Visibility::Public;
    min_float_fn->params.push_back({"List<f64>", "list"});
    min_float_fn->return_type = "f64";
    min_float_fn->error_type = "err";
    program->functions.push_back(move(min_float_fn));

    // fn max_float(List<f64> list) -> f64 or err
    auto max_float_fn = make_unique<FunctionDef>();
    max_float_fn->name = "max_float";
    max_float_fn->visibility = Visibility::Public;
    max_float_fn->params.push_back({"List<f64>", "list"});
    max_float_fn->return_type = "f64";
    max_float_fn->error_type = "err";
    program->functions.push_back(move(max_float_fn));

    // fn min_str(List<str> list) -> str or err
    auto min_str_fn = make_unique<FunctionDef>();
    min_str_fn->name = "min_str";
    min_str_fn->visibility = Visibility::Public;
    min_str_fn->params.push_back({"List<str>", "list"});
    min_str_fn->return_type = "str";
    min_str_fn->error_type = "err";
    program->functions.push_back(move(min_str_fn));

    // fn max_str(List<str> list) -> str or err
    auto max_str_fn = make_unique<FunctionDef>();
    max_str_fn->name = "max_str";
    max_str_fn->visibility = Visibility::Public;
    max_str_fn->params.push_back({"List<str>", "list"});
    max_str_fn->return_type = "str";
    max_str_fn->error_type = "err";
    program->functions.push_back(move(max_str_fn));

    // ============================================================
    // Aggregation
    // ============================================================

    // fn sum_int(List<int> list) -> int
    auto sum_int_fn = make_unique<FunctionDef>();
    sum_int_fn->name = "sum_int";
    sum_int_fn->visibility = Visibility::Public;
    sum_int_fn->params.push_back({"List<int>", "list"});
    sum_int_fn->return_type = "int";
    program->functions.push_back(move(sum_int_fn));

    // fn sum_float(List<f64> list) -> f64
    auto sum_float_fn = make_unique<FunctionDef>();
    sum_float_fn->name = "sum_float";
    sum_float_fn->visibility = Visibility::Public;
    sum_float_fn->params.push_back({"List<f64>", "list"});
    sum_float_fn->return_type = "f64";
    program->functions.push_back(move(sum_float_fn));

    // fn product_int(List<int> list) -> int
    auto product_int_fn = make_unique<FunctionDef>();
    product_int_fn->name = "product_int";
    product_int_fn->visibility = Visibility::Public;
    product_int_fn->params.push_back({"List<int>", "list"});
    product_int_fn->return_type = "int";
    program->functions.push_back(move(product_int_fn));

    // fn product_float(List<f64> list) -> f64
    auto product_float_fn = make_unique<FunctionDef>();
    product_float_fn->name = "product_float";
    product_float_fn->visibility = Visibility::Public;
    product_float_fn->params.push_back({"List<f64>", "list"});
    product_float_fn->return_type = "f64";
    program->functions.push_back(move(product_float_fn));

    // fn average_int(List<int> list) -> f64 or err
    auto average_int_fn = make_unique<FunctionDef>();
    average_int_fn->name = "average_int";
    average_int_fn->visibility = Visibility::Public;
    average_int_fn->params.push_back({"List<int>", "list"});
    average_int_fn->return_type = "f64";
    average_int_fn->error_type = "err";
    program->functions.push_back(move(average_int_fn));

    // fn average_float(List<f64> list) -> f64 or err
    auto average_float_fn = make_unique<FunctionDef>();
    average_float_fn->name = "average_float";
    average_float_fn->visibility = Visibility::Public;
    average_float_fn->params.push_back({"List<f64>", "list"});
    average_float_fn->return_type = "f64";
    average_float_fn->error_type = "err";
    program->functions.push_back(move(average_float_fn));

    // ============================================================
    // Predicates (with lambda callbacks)
    // ============================================================

    // fn all_int(List<int> list, fn(int) -> bool predicate) -> bool
    auto all_int_fn = make_unique<FunctionDef>();
    all_int_fn->name = "all_int";
    all_int_fn->visibility = Visibility::Public;
    all_int_fn->params.push_back({"List<int>", "list"});
    all_int_fn->params.push_back({"fn(int) -> bool", "predicate"});
    all_int_fn->return_type = "bool";
    program->functions.push_back(move(all_int_fn));

    // fn any_int(List<int> list, fn(int) -> bool predicate) -> bool
    auto any_int_fn = make_unique<FunctionDef>();
    any_int_fn->name = "any_int";
    any_int_fn->visibility = Visibility::Public;
    any_int_fn->params.push_back({"List<int>", "list"});
    any_int_fn->params.push_back({"fn(int) -> bool", "predicate"});
    any_int_fn->return_type = "bool";
    program->functions.push_back(move(any_int_fn));

    // fn none_int(List<int> list, fn(int) -> bool predicate) -> bool
    auto none_int_fn = make_unique<FunctionDef>();
    none_int_fn->name = "none_int";
    none_int_fn->visibility = Visibility::Public;
    none_int_fn->params.push_back({"List<int>", "list"});
    none_int_fn->params.push_back({"fn(int) -> bool", "predicate"});
    none_int_fn->return_type = "bool";
    program->functions.push_back(move(none_int_fn));

    // fn count_int(List<int> list, fn(int) -> bool predicate) -> int
    auto count_int_fn = make_unique<FunctionDef>();
    count_int_fn->name = "count_int";
    count_int_fn->visibility = Visibility::Public;
    count_int_fn->params.push_back({"List<int>", "list"});
    count_int_fn->params.push_back({"fn(int) -> bool", "predicate"});
    count_int_fn->return_type = "int";
    program->functions.push_back(move(count_int_fn));

    // fn find_int(List<int> list, fn(int) -> bool predicate) -> int or err
    auto find_int_fn = make_unique<FunctionDef>();
    find_int_fn->name = "find_int";
    find_int_fn->visibility = Visibility::Public;
    find_int_fn->params.push_back({"List<int>", "list"});
    find_int_fn->params.push_back({"fn(int) -> bool", "predicate"});
    find_int_fn->return_type = "int";
    find_int_fn->error_type = "err";
    program->functions.push_back(move(find_int_fn));

    // fn find_index_int(List<int> list, fn(int) -> bool predicate) -> int
    auto find_index_int_fn = make_unique<FunctionDef>();
    find_index_int_fn->name = "find_index_int";
    find_index_int_fn->visibility = Visibility::Public;
    find_index_int_fn->params.push_back({"List<int>", "list"});
    find_index_int_fn->params.push_back({"fn(int) -> bool", "predicate"});
    find_index_int_fn->return_type = "int";
    program->functions.push_back(move(find_index_int_fn));

    // fn all_str(List<str> list, fn(str) -> bool predicate) -> bool
    auto all_str_fn = make_unique<FunctionDef>();
    all_str_fn->name = "all_str";
    all_str_fn->visibility = Visibility::Public;
    all_str_fn->params.push_back({"List<str>", "list"});
    all_str_fn->params.push_back({"fn(str) -> bool", "predicate"});
    all_str_fn->return_type = "bool";
    program->functions.push_back(move(all_str_fn));

    // fn any_str(List<str> list, fn(str) -> bool predicate) -> bool
    auto any_str_fn = make_unique<FunctionDef>();
    any_str_fn->name = "any_str";
    any_str_fn->visibility = Visibility::Public;
    any_str_fn->params.push_back({"List<str>", "list"});
    any_str_fn->params.push_back({"fn(str) -> bool", "predicate"});
    any_str_fn->return_type = "bool";
    program->functions.push_back(move(any_str_fn));

    // fn none_str(List<str> list, fn(str) -> bool predicate) -> bool
    auto none_str_fn = make_unique<FunctionDef>();
    none_str_fn->name = "none_str";
    none_str_fn->visibility = Visibility::Public;
    none_str_fn->params.push_back({"List<str>", "list"});
    none_str_fn->params.push_back({"fn(str) -> bool", "predicate"});
    none_str_fn->return_type = "bool";
    program->functions.push_back(move(none_str_fn));

    // fn count_str(List<str> list, fn(str) -> bool predicate) -> int
    auto count_str_fn = make_unique<FunctionDef>();
    count_str_fn->name = "count_str";
    count_str_fn->visibility = Visibility::Public;
    count_str_fn->params.push_back({"List<str>", "list"});
    count_str_fn->params.push_back({"fn(str) -> bool", "predicate"});
    count_str_fn->return_type = "int";
    program->functions.push_back(move(count_str_fn));

    // fn find_str(List<str> list, fn(str) -> bool predicate) -> str or err
    auto find_str_fn = make_unique<FunctionDef>();
    find_str_fn->name = "find_str";
    find_str_fn->visibility = Visibility::Public;
    find_str_fn->params.push_back({"List<str>", "list"});
    find_str_fn->params.push_back({"fn(str) -> bool", "predicate"});
    find_str_fn->return_type = "str";
    find_str_fn->error_type = "err";
    program->functions.push_back(move(find_str_fn));

    // fn find_index_str(List<str> list, fn(str) -> bool predicate) -> int
    auto find_index_str_fn = make_unique<FunctionDef>();
    find_index_str_fn->name = "find_index_str";
    find_index_str_fn->visibility = Visibility::Public;
    find_index_str_fn->params.push_back({"List<str>", "list"});
    find_index_str_fn->params.push_back({"fn(str) -> bool", "predicate"});
    find_index_str_fn->return_type = "int";
    program->functions.push_back(move(find_index_str_fn));

    // ============================================================
    // Transformations
    // ============================================================

    // fn map_int(List<int> list, fn(int) -> int transform) -> List<int>
    auto map_int_fn = make_unique<FunctionDef>();
    map_int_fn->name = "map_int";
    map_int_fn->visibility = Visibility::Public;
    map_int_fn->params.push_back({"List<int>", "list"});
    map_int_fn->params.push_back({"fn(int) -> int", "transform"});
    map_int_fn->return_type = "List<int>";
    program->functions.push_back(move(map_int_fn));

    // fn map_str(List<str> list, fn(str) -> str transform) -> List<str>
    auto map_str_fn = make_unique<FunctionDef>();
    map_str_fn->name = "map_str";
    map_str_fn->visibility = Visibility::Public;
    map_str_fn->params.push_back({"List<str>", "list"});
    map_str_fn->params.push_back({"fn(str) -> str", "transform"});
    map_str_fn->return_type = "List<str>";
    program->functions.push_back(move(map_str_fn));

    // fn filter_int(List<int> list, fn(int) -> bool predicate) -> List<int>
    auto filter_int_fn = make_unique<FunctionDef>();
    filter_int_fn->name = "filter_int";
    filter_int_fn->visibility = Visibility::Public;
    filter_int_fn->params.push_back({"List<int>", "list"});
    filter_int_fn->params.push_back({"fn(int) -> bool", "predicate"});
    filter_int_fn->return_type = "List<int>";
    program->functions.push_back(move(filter_int_fn));

    // fn filter_str(List<str> list, fn(str) -> bool predicate) -> List<str>
    auto filter_str_fn = make_unique<FunctionDef>();
    filter_str_fn->name = "filter_str";
    filter_str_fn->visibility = Visibility::Public;
    filter_str_fn->params.push_back({"List<str>", "list"});
    filter_str_fn->params.push_back({"fn(str) -> bool", "predicate"});
    filter_str_fn->return_type = "List<str>";
    program->functions.push_back(move(filter_str_fn));

    // fn reduce_int(List<int> list, fn(int, int) -> int accumulator, int initial) -> int
    auto reduce_int_fn = make_unique<FunctionDef>();
    reduce_int_fn->name = "reduce_int";
    reduce_int_fn->visibility = Visibility::Public;
    reduce_int_fn->params.push_back({"List<int>", "list"});
    reduce_int_fn->params.push_back({"fn(int, int) -> int", "accumulator"});
    reduce_int_fn->params.push_back({"int", "initial"});
    reduce_int_fn->return_type = "int";
    program->functions.push_back(move(reduce_int_fn));

    // fn reduce_str(List<str> list, fn(str, str) -> str accumulator, str initial) -> str
    auto reduce_str_fn = make_unique<FunctionDef>();
    reduce_str_fn->name = "reduce_str";
    reduce_str_fn->visibility = Visibility::Public;
    reduce_str_fn->params.push_back({"List<str>", "list"});
    reduce_str_fn->params.push_back({"fn(str, str) -> str", "accumulator"});
    reduce_str_fn->params.push_back({"str", "initial"});
    reduce_str_fn->return_type = "str";
    program->functions.push_back(move(reduce_str_fn));

    // ============================================================
    // Reordering
    // ============================================================

    // fn reverse_int(List<int> list)
    auto reverse_int_fn = make_unique<FunctionDef>();
    reverse_int_fn->name = "reverse_int";
    reverse_int_fn->visibility = Visibility::Public;
    reverse_int_fn->params.push_back({"List<int>", "list"});
    reverse_int_fn->return_type = "";
    program->functions.push_back(move(reverse_int_fn));

    // fn reverse_str(List<str> list)
    auto reverse_str_fn = make_unique<FunctionDef>();
    reverse_str_fn->name = "reverse_str";
    reverse_str_fn->visibility = Visibility::Public;
    reverse_str_fn->params.push_back({"List<str>", "list"});
    reverse_str_fn->return_type = "";
    program->functions.push_back(move(reverse_str_fn));

    // fn reversed_int(List<int> list) -> List<int>
    auto reversed_int_fn = make_unique<FunctionDef>();
    reversed_int_fn->name = "reversed_int";
    reversed_int_fn->visibility = Visibility::Public;
    reversed_int_fn->params.push_back({"List<int>", "list"});
    reversed_int_fn->return_type = "List<int>";
    program->functions.push_back(move(reversed_int_fn));

    // fn reversed_str(List<str> list) -> List<str>
    auto reversed_str_fn = make_unique<FunctionDef>();
    reversed_str_fn->name = "reversed_str";
    reversed_str_fn->visibility = Visibility::Public;
    reversed_str_fn->params.push_back({"List<str>", "list"});
    reversed_str_fn->return_type = "List<str>";
    program->functions.push_back(move(reversed_str_fn));

    // fn rotate_int(List<int> list, int n)
    auto rotate_int_fn = make_unique<FunctionDef>();
    rotate_int_fn->name = "rotate_int";
    rotate_int_fn->visibility = Visibility::Public;
    rotate_int_fn->params.push_back({"List<int>", "list"});
    rotate_int_fn->params.push_back({"int", "n"});
    rotate_int_fn->return_type = "";
    program->functions.push_back(move(rotate_int_fn));

    // fn rotate_str(List<str> list, int n)
    auto rotate_str_fn = make_unique<FunctionDef>();
    rotate_str_fn->name = "rotate_str";
    rotate_str_fn->visibility = Visibility::Public;
    rotate_str_fn->params.push_back({"List<str>", "list"});
    rotate_str_fn->params.push_back({"int", "n"});
    rotate_str_fn->return_type = "";
    program->functions.push_back(move(rotate_str_fn));

    // fn unique_int(List<int> list) -> List<int>
    auto unique_int_fn = make_unique<FunctionDef>();
    unique_int_fn->name = "unique_int";
    unique_int_fn->visibility = Visibility::Public;
    unique_int_fn->params.push_back({"List<int>", "list"});
    unique_int_fn->return_type = "List<int>";
    program->functions.push_back(move(unique_int_fn));

    // fn unique_str(List<str> list) -> List<str>
    auto unique_str_fn = make_unique<FunctionDef>();
    unique_str_fn->name = "unique_str";
    unique_str_fn->visibility = Visibility::Public;
    unique_str_fn->params.push_back({"List<str>", "list"});
    unique_str_fn->return_type = "List<str>";
    program->functions.push_back(move(unique_str_fn));

    return program;
}

/**
 * Returns empty - algo.hpp is included at the top of generated code
 * for precompiled header support.
 */
string generate_algo_runtime() {
    return "";
}

}  // namespace bishop::stdlib
