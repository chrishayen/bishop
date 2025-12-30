/**
 * @file sync.hpp
 * @brief Bishop synchronization runtime library.
 *
 * Provides synchronization primitives for Bishop programs.
 * This header is included when programs import the sync module.
 */

#pragma once

#include <mutex>
#include <atomic>
#include <condition_variable>
#include <memory>
#include <functional>

namespace bsync {

// ============================================================================
// Mutex - Mutual exclusion lock
// ============================================================================

/**
 * Mutex wrapper for Bishop.
 * Uses shared_ptr to allow copying in Bishop's value semantics.
 */
struct Mutex {
    std::shared_ptr<std::mutex> impl;

    Mutex() : impl(std::make_shared<std::mutex>()) {}
};

/**
 * Creates a new mutex.
 */
inline Mutex mutex_create() {
    return Mutex();
}

/**
 * Acquires the mutex lock. Blocks until available.
 */
inline void mutex_lock(Mutex& mtx) {
    mtx.impl->lock();
}

/**
 * Releases the mutex lock.
 */
inline void mutex_unlock(Mutex& mtx) {
    mtx.impl->unlock();
}

/**
 * Attempts to acquire the lock without blocking.
 * Returns true if lock was acquired, false otherwise.
 */
inline bool mutex_try_lock(Mutex& mtx) {
    return mtx.impl->try_lock();
}

// ============================================================================
// WaitGroup - Coordinate completion of goroutines
// ============================================================================

/**
 * WaitGroup implementation for Bishop.
 * Uses shared_ptr to allow copying in Bishop's value semantics.
 */
struct WaitGroup {
    struct Impl {
        std::mutex mtx;
        std::condition_variable cv;
        int counter = 0;
    };

    std::shared_ptr<Impl> impl;

    WaitGroup() : impl(std::make_shared<Impl>()) {}
};

/**
 * Creates a new WaitGroup.
 */
inline WaitGroup waitgroup_create() {
    return WaitGroup();
}

/**
 * Adds delta to the WaitGroup counter.
 */
inline void waitgroup_add(WaitGroup& wg, int64_t n) {
    std::lock_guard<std::mutex> lock(wg.impl->mtx);
    wg.impl->counter += static_cast<int>(n);
}

/**
 * Decrements the WaitGroup counter by 1.
 * Wakes waiting goroutines when counter reaches zero.
 */
inline void waitgroup_done(WaitGroup& wg) {
    std::unique_lock<std::mutex> lock(wg.impl->mtx);
    wg.impl->counter--;

    if (wg.impl->counter <= 0) {
        lock.unlock();
        wg.impl->cv.notify_all();
    }
}

/**
 * Blocks until the WaitGroup counter reaches zero.
 */
inline void waitgroup_wait(WaitGroup& wg) {
    std::unique_lock<std::mutex> lock(wg.impl->mtx);
    wg.impl->cv.wait(lock, [&wg]() { return wg.impl->counter <= 0; });
}

// ============================================================================
// Once - Execute exactly once
// ============================================================================

/**
 * Once implementation for Bishop.
 * Uses shared_ptr to allow copying in Bishop's value semantics.
 */
struct Once {
    struct Impl {
        std::once_flag flag;
    };

    std::shared_ptr<Impl> impl;

    Once() : impl(std::make_shared<Impl>()) {}
};

/**
 * Creates a new Once object.
 */
inline Once once_create() {
    return Once();
}

/**
 * Executes the function exactly once, even if called concurrently.
 */
inline void once_do(Once& once, std::function<void()> f) {
    std::call_once(once.impl->flag, f);
}

// ============================================================================
// AtomicInt - Lock-free atomic integer
// ============================================================================

/**
 * AtomicInt implementation for Bishop.
 * Uses shared_ptr to allow copying in Bishop's value semantics.
 */
struct AtomicInt {
    std::shared_ptr<std::atomic<int64_t>> impl;

    AtomicInt() : impl(std::make_shared<std::atomic<int64_t>>(0)) {}
    explicit AtomicInt(int64_t initial) : impl(std::make_shared<std::atomic<int64_t>>(initial)) {}
};

/**
 * Creates a new atomic integer with the given initial value.
 */
inline AtomicInt atomic_int_create(int64_t initial) {
    return AtomicInt(initial);
}

/**
 * Atomically loads and returns the current value.
 */
inline int64_t atomic_int_load(AtomicInt& a) {
    return a.impl->load(std::memory_order_seq_cst);
}

/**
 * Atomically stores a new value.
 */
inline void atomic_int_store(AtomicInt& a, int64_t val) {
    a.impl->store(val, std::memory_order_seq_cst);
}

/**
 * Atomically adds delta and returns the previous value.
 */
inline int64_t atomic_int_add(AtomicInt& a, int64_t delta) {
    return a.impl->fetch_add(delta, std::memory_order_seq_cst);
}

/**
 * Atomically swaps the value and returns the old value.
 */
inline int64_t atomic_int_swap(AtomicInt& a, int64_t val) {
    return a.impl->exchange(val, std::memory_order_seq_cst);
}

/**
 * Atomically compares and swaps if equal.
 * Returns true if swap occurred, false otherwise.
 */
inline bool atomic_int_compare_swap(AtomicInt& a, int64_t expected, int64_t desired) {
    return a.impl->compare_exchange_strong(expected, desired, std::memory_order_seq_cst);
}

}  // namespace bsync
