/**
 * @file algo.hpp
 * @brief Bishop algo runtime library.
 *
 * Provides algorithm functions for sorting, searching, aggregation,
 * predicates, and transformations on lists.
 * This header is included when programs import the algo module.
 */

#pragma once

#include <bishop/std.hpp>
#include <bishop/error.hpp>
#include <algorithm>
#include <functional>
#include <numeric>
#include <vector>
#include <string>
#include <unordered_set>

namespace algo {

// ============================================================
// Sorting (in-place)
// ============================================================

/**
 * Sorts an integer list in ascending order.
 */
inline void sort_int(std::vector<int>& list) {
    std::sort(list.begin(), list.end());
}

/**
 * Sorts an integer list in descending order.
 */
inline void sort_desc_int(std::vector<int>& list) {
    std::sort(list.begin(), list.end(), std::greater<int>());
}

/**
 * Sorts a string list in ascending order.
 */
inline void sort_str(std::vector<std::string>& list) {
    std::sort(list.begin(), list.end());
}

/**
 * Sorts a string list in descending order.
 */
inline void sort_desc_str(std::vector<std::string>& list) {
    std::sort(list.begin(), list.end(), std::greater<std::string>());
}

/**
 * Sorts a float list in ascending order.
 */
inline void sort_float(std::vector<double>& list) {
    std::sort(list.begin(), list.end());
}

/**
 * Sorts a float list in descending order.
 */
inline void sort_desc_float(std::vector<double>& list) {
    std::sort(list.begin(), list.end(), std::greater<double>());
}

// ============================================================
// Min/Max
// ============================================================

/**
 * Returns the minimum value in an integer list.
 */
inline bishop::rt::Result<int> min_int(const std::vector<int>& list) {
    if (list.empty()) {
        return bishop::rt::make_error<int>("cannot find min of empty list");
    }

    return *std::min_element(list.begin(), list.end());
}

/**
 * Returns the maximum value in an integer list.
 */
inline bishop::rt::Result<int> max_int(const std::vector<int>& list) {
    if (list.empty()) {
        return bishop::rt::make_error<int>("cannot find max of empty list");
    }

    return *std::max_element(list.begin(), list.end());
}

/**
 * Returns the minimum value in a float list.
 */
inline bishop::rt::Result<double> min_float(const std::vector<double>& list) {
    if (list.empty()) {
        return bishop::rt::make_error<double>("cannot find min of empty list");
    }

    return *std::min_element(list.begin(), list.end());
}

/**
 * Returns the maximum value in a float list.
 */
inline bishop::rt::Result<double> max_float(const std::vector<double>& list) {
    if (list.empty()) {
        return bishop::rt::make_error<double>("cannot find max of empty list");
    }

    return *std::max_element(list.begin(), list.end());
}

/**
 * Returns the minimum string in a list.
 */
inline bishop::rt::Result<std::string> min_str(const std::vector<std::string>& list) {
    if (list.empty()) {
        return bishop::rt::make_error<std::string>("cannot find min of empty list");
    }

    return *std::min_element(list.begin(), list.end());
}

/**
 * Returns the maximum string in a list.
 */
inline bishop::rt::Result<std::string> max_str(const std::vector<std::string>& list) {
    if (list.empty()) {
        return bishop::rt::make_error<std::string>("cannot find max of empty list");
    }

    return *std::max_element(list.begin(), list.end());
}

// ============================================================
// Aggregation
// ============================================================

/**
 * Returns the sum of all integers in a list.
 */
inline int sum_int(const std::vector<int>& list) {
    return std::accumulate(list.begin(), list.end(), static_cast<int>(0));
}

/**
 * Returns the sum of all floats in a list.
 */
inline double sum_float(const std::vector<double>& list) {
    return std::accumulate(list.begin(), list.end(), 0.0);
}

/**
 * Returns the product of all integers in a list.
 */
inline int product_int(const std::vector<int>& list) {
    if (list.empty()) {
        return 1;
    }

    return std::accumulate(
        list.begin(), list.end(), static_cast<int>(1),
        std::multiplies<int>()
    );
}

/**
 * Returns the product of all floats in a list.
 */
inline double product_float(const std::vector<double>& list) {
    if (list.empty()) {
        return 1.0;
    }

    return std::accumulate(
        list.begin(), list.end(), 1.0, std::multiplies<double>()
    );
}

/**
 * Returns the average of all integers in a list.
 */
inline bishop::rt::Result<double> average_int(const std::vector<int>& list) {
    if (list.empty()) {
        return bishop::rt::make_error<double>("cannot compute average of empty list");
    }

    return static_cast<double>(sum_int(list)) / static_cast<double>(list.size());
}

/**
 * Returns the average of all floats in a list.
 */
inline bishop::rt::Result<double> average_float(const std::vector<double>& list) {
    if (list.empty()) {
        return bishop::rt::make_error<double>("cannot compute average of empty list");
    }

    return sum_float(list) / static_cast<double>(list.size());
}

// ============================================================
// Predicates (with lambda callbacks)
// ============================================================

/**
 * Returns true if all elements satisfy the predicate.
 */
inline bool all_int(
    const std::vector<int>& list,
    const std::function<bool(int)>& predicate
) {
    return std::all_of(list.begin(), list.end(), predicate);
}

/**
 * Returns true if any element satisfies the predicate.
 */
inline bool any_int(
    const std::vector<int>& list,
    const std::function<bool(int)>& predicate
) {
    return std::any_of(list.begin(), list.end(), predicate);
}

/**
 * Returns true if no elements satisfy the predicate.
 */
inline bool none_int(
    const std::vector<int>& list,
    const std::function<bool(int)>& predicate
) {
    return std::none_of(list.begin(), list.end(), predicate);
}

/**
 * Returns the count of elements that satisfy the predicate.
 */
inline int count_int(
    const std::vector<int>& list,
    const std::function<bool(int)>& predicate
) {
    return static_cast<int>(std::count_if(list.begin(), list.end(), predicate));
}

/**
 * Returns the first element that satisfies the predicate, or error if not found.
 */
inline bishop::rt::Result<int> find_int(
    const std::vector<int>& list,
    const std::function<bool(int)>& predicate
) {
    auto it = std::find_if(list.begin(), list.end(), predicate);

    if (it == list.end()) {
        return bishop::rt::make_error<int>("element not found");
    }

    return *it;
}

/**
 * Returns the index of the first element that satisfies the predicate, or -1.
 */
inline int find_index_int(
    const std::vector<int>& list,
    const std::function<bool(int)>& predicate
) {
    auto it = std::find_if(list.begin(), list.end(), predicate);

    if (it == list.end()) {
        return -1;
    }

    return static_cast<int>(std::distance(list.begin(), it));
}

/**
 * Returns true if all string elements satisfy the predicate.
 */
inline bool all_str(
    const std::vector<std::string>& list,
    const std::function<bool(std::string)>& predicate
) {
    return std::all_of(list.begin(), list.end(), predicate);
}

/**
 * Returns true if any string element satisfies the predicate.
 */
inline bool any_str(
    const std::vector<std::string>& list,
    const std::function<bool(std::string)>& predicate
) {
    return std::any_of(list.begin(), list.end(), predicate);
}

/**
 * Returns true if no string elements satisfy the predicate.
 */
inline bool none_str(
    const std::vector<std::string>& list,
    const std::function<bool(std::string)>& predicate
) {
    return std::none_of(list.begin(), list.end(), predicate);
}

/**
 * Returns the count of string elements that satisfy the predicate.
 */
inline int count_str(
    const std::vector<std::string>& list,
    const std::function<bool(std::string)>& predicate
) {
    return static_cast<int>(std::count_if(list.begin(), list.end(), predicate));
}

/**
 * Returns the first string element that satisfies the predicate, or error.
 */
inline bishop::rt::Result<std::string> find_str(
    const std::vector<std::string>& list,
    const std::function<bool(std::string)>& predicate
) {
    auto it = std::find_if(list.begin(), list.end(), predicate);

    if (it == list.end()) {
        return bishop::rt::make_error<std::string>("element not found");
    }

    return *it;
}

/**
 * Returns the index of the first string element that satisfies the predicate.
 */
inline int find_index_str(
    const std::vector<std::string>& list,
    const std::function<bool(std::string)>& predicate
) {
    auto it = std::find_if(list.begin(), list.end(), predicate);

    if (it == list.end()) {
        return -1;
    }

    return static_cast<int>(std::distance(list.begin(), it));
}

// ============================================================
// Transformations
// ============================================================

/**
 * Maps each integer to a new integer using a transform function.
 */
inline std::vector<int> map_int(
    const std::vector<int>& list,
    const std::function<int(int)>& transform
) {
    std::vector<int> result;
    result.reserve(list.size());

    for (const auto& item : list) {
        result.push_back(transform(item));
    }

    return result;
}

/**
 * Maps each string to a new string using a transform function.
 */
inline std::vector<std::string> map_str(
    const std::vector<std::string>& list,
    const std::function<std::string(std::string)>& transform
) {
    std::vector<std::string> result;
    result.reserve(list.size());

    for (const auto& item : list) {
        result.push_back(transform(item));
    }

    return result;
}

/**
 * Filters integers by a predicate, keeping only matching elements.
 */
inline std::vector<int> filter_int(
    const std::vector<int>& list,
    const std::function<bool(int)>& predicate
) {
    std::vector<int> result;

    for (const auto& item : list) {
        if (predicate(item)) {
            result.push_back(item);
        }
    }

    return result;
}

/**
 * Filters strings by a predicate, keeping only matching elements.
 */
inline std::vector<std::string> filter_str(
    const std::vector<std::string>& list,
    const std::function<bool(std::string)>& predicate
) {
    std::vector<std::string> result;

    for (const auto& item : list) {
        if (predicate(item)) {
            result.push_back(item);
        }
    }

    return result;
}

/**
 * Reduces an integer list to a single value using an accumulator function.
 */
inline int reduce_int(
    const std::vector<int>& list,
    const std::function<int(int, int)>& accumulator,
    int initial
) {
    int result = initial;

    for (const auto& item : list) {
        result = accumulator(result, item);
    }

    return result;
}

/**
 * Reduces a string list to a single string using an accumulator function.
 */
inline std::string reduce_str(
    const std::vector<std::string>& list,
    const std::function<std::string(std::string, std::string)>& accumulator,
    const std::string& initial
) {
    std::string result = initial;

    for (const auto& item : list) {
        result = accumulator(result, item);
    }

    return result;
}

// ============================================================
// Reordering
// ============================================================

/**
 * Reverses an integer list in place.
 */
inline void reverse_int(std::vector<int>& list) {
    std::reverse(list.begin(), list.end());
}

/**
 * Reverses a string list in place.
 */
inline void reverse_str(std::vector<std::string>& list) {
    std::reverse(list.begin(), list.end());
}

/**
 * Returns a reversed copy of an integer list.
 */
inline std::vector<int> reversed_int(const std::vector<int>& list) {
    std::vector<int> result(list.rbegin(), list.rend());
    return result;
}

/**
 * Returns a reversed copy of a string list.
 */
inline std::vector<std::string> reversed_str(const std::vector<std::string>& list) {
    std::vector<std::string> result(list.rbegin(), list.rend());
    return result;
}

/**
 * Rotates an integer list left by n positions.
 * Positive n rotates left, negative n rotates right.
 */
inline void rotate_int(std::vector<int>& list, int n) {
    if (list.empty()) {
        return;
    }

    int size = static_cast<int>(list.size());
    n = ((n % size) + size) % size;

    std::rotate(list.begin(), list.begin() + n, list.end());
}

/**
 * Rotates a string list left by n positions.
 */
inline void rotate_str(std::vector<std::string>& list, int n) {
    if (list.empty()) {
        return;
    }

    int size = static_cast<int>(list.size());
    n = ((n % size) + size) % size;

    std::rotate(list.begin(), list.begin() + n, list.end());
}

/**
 * Returns a new list with unique elements from an integer list (order preserved).
 */
inline std::vector<int> unique_int(const std::vector<int>& list) {
    std::vector<int> result;
    std::unordered_set<int> seen;

    for (const auto& item : list) {
        if (seen.insert(item).second) {
            result.push_back(item);
        }
    }

    return result;
}

/**
 * Returns a new list with unique elements from a string list (order preserved).
 */
inline std::vector<std::string> unique_str(const std::vector<std::string>& list) {
    std::vector<std::string> result;
    std::unordered_set<std::string> seen;

    for (const auto& item : list) {
        if (seen.insert(item).second) {
            result.push_back(item);
        }
    }

    return result;
}

// ============================================================
// Zip and Enumerate (using Pair)
// ============================================================

// Note: zip and enumerate require Pair<T> type support.
// They are implemented as template functions that work with the Pair type.

}  // namespace algo
