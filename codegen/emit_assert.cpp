/**
 * @file emit_assert.cpp
 * @brief Assert statement emission for the Bishop code generator.
 */

#include "codegen.hpp"
#include <fmt/format.h>

using namespace std;

namespace codegen {

/**
 * Emits a call to the _assert_eq test helper with line number for errors.
 */
string assert_eq(const string& a, const string& b, int line) {
    return fmt::format("_assert_eq({}, {}, {})", a, b, line);
}

/**
 * Emits a call to the _assert_ne test helper with line number for errors.
 */
string assert_ne(const string& a, const string& b, int line) {
    return fmt::format("_assert_ne({}, {}, {})", a, b, line);
}

/**
 * Emits a call to the _assert_true test helper with line number for errors.
 */
string assert_true(const string& condition, int line) {
    return fmt::format("_assert_true({}, {})", condition, line);
}

/**
 * Emits a call to the _assert_false test helper with line number for errors.
 */
string assert_false(const string& condition, int line) {
    return fmt::format("_assert_false({}, {})", condition, line);
}

/**
 * Emits a call to the _assert_gt test helper with line number for errors.
 */
string assert_gt(const string& a, const string& b, int line) {
    return fmt::format("_assert_gt({}, {}, {})", a, b, line);
}

/**
 * Emits a call to the _assert_gte test helper with line number for errors.
 */
string assert_gte(const string& a, const string& b, int line) {
    return fmt::format("_assert_gte({}, {}, {})", a, b, line);
}

/**
 * Emits a call to the _assert_lt test helper with line number for errors.
 */
string assert_lt(const string& a, const string& b, int line) {
    return fmt::format("_assert_lt({}, {}, {})", a, b, line);
}

/**
 * Emits a call to the _assert_lte test helper with line number for errors.
 */
string assert_lte(const string& a, const string& b, int line) {
    return fmt::format("_assert_lte({}, {}, {})", a, b, line);
}

/**
 * Emits a call to the _assert_contains test helper with line number for errors.
 */
string assert_contains(const string& item, const string& collection, int line) {
    return fmt::format("_assert_contains({}, {}, {})", item, collection, line);
}

/**
 * Emits a call to the _assert_starts_with test helper with line number for errors.
 */
string assert_starts_with(const string& prefix, const string& str, int line) {
    return fmt::format("_assert_starts_with({}, {}, {})", prefix, str, line);
}

/**
 * Emits a call to the _assert_ends_with test helper with line number for errors.
 */
string assert_ends_with(const string& suffix, const string& str, int line) {
    return fmt::format("_assert_ends_with({}, {}, {})", suffix, str, line);
}

/**
 * Emits a call to the _assert_near test helper with line number for errors.
 */
string assert_near(const string& actual, const string& expected, const string& epsilon, int line) {
    return fmt::format("_assert_near({}, {}, {}, {})", actual, expected, epsilon, line);
}

} // namespace codegen
