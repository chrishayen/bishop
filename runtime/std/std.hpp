/**
 * @file std.hpp
 * @brief Base standard library header for all Bishop programs.
 *
 * This header is lightweight - no boost includes.
 * Boost-dependent features (Channel) are in separate headers.
 */

#ifndef BISHOP_STD_HPP
#define BISHOP_STD_HPP

#pragma once

#include <iostream>
#include <string>
#include <cstdint>
#include <optional>
#include <functional>
#include <memory>
#include <vector>
#include <algorithm>
#include <map>
#include <utility>
#include <thread>
#include <mutex>
#include <chrono>
#include <deque>
#include <stack>
#include <queue>

// Error handling primitives
#include <bishop/error.hpp>

namespace bishop::rt {

// ============================================================================
// Runtime Functions (implemented in runtime.cpp)
// ============================================================================

/**
 * Initialize the fiber-asio scheduler.
 * Called automatically by run(), but can be called manually for tests.
 */
void init_runtime();

/**
 * Initialize and run the main function in a fiber context.
 * Sets up the fiber-asio scheduler.
 */
void run(std::function<void()> main_fn);

/**
 * Run a function in a fiber and wait for completion.
 * Assumes runtime is already initialized.
 */
void run_in_fiber(std::function<void()> fn);

/**
 * Spawn a new fiber (goroutine).
 */
void spawn(std::function<void()> fn);

/**
 * Sleep for the specified milliseconds, yielding to other fibers.
 */
void sleep_ms(int ms);

/**
 * Yield to other fibers.
 */
void yield();

// ============================================================================
// Arena Allocator (header-only, no boost dependency)
// ============================================================================

/**
 * Simple arena (bump) allocator for per-function memory management.
 */
class Arena {
public:
    static constexpr size_t DEFAULT_BLOCK_SIZE = 64 * 1024;

    Arena() = default;
    ~Arena() { reset(); }

    Arena(const Arena&) = delete;
    Arena& operator=(const Arena&) = delete;

    void* alloc(size_t size, size_t alignment = alignof(max_align_t)) {
        size_t aligned_offset = (offset_ + alignment - 1) & ~(alignment - 1);
        size_t new_offset = aligned_offset + size;

        if (blocks_.empty() || new_offset > current_block_size_) {
            allocate_block(std::max(size, DEFAULT_BLOCK_SIZE));
            aligned_offset = 0;
            new_offset = size;
        }

        void* ptr = static_cast<char*>(blocks_.back()) + aligned_offset;
        offset_ = new_offset;
        return ptr;
    }

    void reset() {
        for (void* block : blocks_) {
            std::free(block);
        }
        blocks_.clear();
        offset_ = 0;
        current_block_size_ = 0;
    }

private:
    void allocate_block(size_t size) {
        void* block = std::malloc(size);
        if (!block) throw std::bad_alloc();
        blocks_.push_back(block);
        current_block_size_ = size;
        offset_ = 0;
    }

    std::vector<void*> blocks_;
    size_t offset_ = 0;
    size_t current_block_size_ = 0;
};

inline thread_local std::vector<Arena*> arena_stack;

inline Arena* current_arena() {
    if (arena_stack.empty()) return nullptr;
    return arena_stack.back();
}

class FunctionArena {
public:
    FunctionArena() { arena_stack.push_back(&arena_); }
    ~FunctionArena() { arena_stack.pop_back(); }
    Arena& arena() { return arena_; }
private:
    Arena arena_;
};

template<typename T, typename... Args>
T* arena_new(Args&&... args) {
    Arena* arena = current_arena();
    if (!arena) {
        return new T(std::forward<Args>(args)...);
    }
    void* mem = arena->alloc(sizeof(T), alignof(T));
    return new(mem) T(std::forward<Args>(args)...);
}

}  // namespace bishop::rt

// Legacy compatibility - inline wrapper for sleep
namespace bishop::rt {
    inline void sleep(int ms) { sleep_ms(ms); }
}

// ============================================================================
// Truthy/Falsy Value Checking (for default expressions)
// ============================================================================

namespace bishop {

namespace detail {
    // SFINAE helper to detect if type has .empty() method
    template<typename T, typename = void>
    struct has_empty : std::false_type {};

    template<typename T>
    struct has_empty<T, std::void_t<decltype(std::declval<T>().empty())>> : std::true_type {};

    template<typename T>
    inline constexpr bool has_empty_v = has_empty<T>::value;
}

/**
 * Check if a value is "truthy" (non-falsy).
 * Used by the default keyword to determine if fallback should be used.
 *
 * Falsy values:
 * - Empty strings and containers (via .empty())
 * - Zero for arithmetic types
 * - False for bool
 * - None/nullopt for optional types
 */
template<typename T>
inline bool truthy(const T& value) {
    if constexpr (std::is_same_v<std::decay_t<T>, bool>) {
        // bool: false is falsy
        return value;
    } else if constexpr (std::is_arithmetic_v<std::decay_t<T>>) {
        // Numeric types: zero is falsy
        return value != 0;
    } else if constexpr (detail::has_empty_v<std::decay_t<T>>) {
        // Containers/strings with .empty(): empty is falsy
        return !value.empty();
    } else {
        // For other types (optional, etc), use implicit bool conversion
        return static_cast<bool>(value);
    }
}

namespace detail {
    // Type trait to detect Result types (have .is_error() method)
    template<typename T, typename = void>
    struct is_result : std::false_type {};

    template<typename T>
    struct is_result<T, std::void_t<decltype(std::declval<T>().is_error())>> : std::true_type {};

    template<typename T>
    inline constexpr bool is_result_v = is_result<T>::value;
}

/**
 * Check if a value should trigger the 'or' handler.
 * Used by 'or fail', 'or return', 'or continue', 'or break', etc.
 *
 * For Result<T> types: returns is_error()
 * For falsy types (bool, int, float, str, list, optional): returns !truthy(value)
 */
template<typename T>
inline bool is_or_falsy(const T& value) {
    if constexpr (detail::is_result_v<std::decay_t<T>>) {
        // Result types: check is_error()
        return value.is_error();
    } else {
        // Falsy types: check !truthy(value)
        return !truthy(value);
    }
}

/**
 * Get the actual value from either a Result or falsy type.
 * Used by 'or' handlers to extract the value when condition passes.
 *
 * For Result<T> types: returns .value()
 * For falsy types: returns the value as-is
 */
template<typename T>
inline decltype(auto) or_value(const T& value) {
    if constexpr (detail::is_result_v<std::decay_t<T>>) {
        return value.value();
    } else {
        return value;
    }
}

template<typename T>
inline decltype(auto) or_value(T& value) {
    if constexpr (detail::is_result_v<std::decay_t<T>>) {
        return value.value();
    } else {
        return value;
    }
}

/**
 * Get the error from a Result type, or a synthetic error for falsy types.
 * Used by 'or' blocks that may access the 'err' variable.
 *
 * For Result<T> types: returns .error()
 * For falsy types: returns a shared_ptr to a generic error (falsy values don't have errors)
 */
template<typename T>
inline auto or_error(const T& value) {
    if constexpr (detail::is_result_v<std::decay_t<T>>) {
        return value.error();
    } else {
        // Falsy types don't have errors, return a synthetic one
        // This should never be called if typechecker is working correctly
        return std::make_shared<rt::Error>("falsy value");
    }
}

}  // namespace bishop

#endif  // BISHOP_STD_HPP
