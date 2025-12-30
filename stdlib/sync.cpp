/**
 * @file sync.cpp
 * @brief Built-in sync (synchronization) module implementation.
 *
 * Creates the AST definitions for the sync module.
 * The actual runtime is in src/runtime/sync/sync.hpp and included as a header.
 */

/**
 * @nog_fn mutex_create
 * @module sync
 * @description Creates a new mutex for thread synchronization.
 * @returns Mutex - A new mutex object
 * @example
 * import sync;
 * mtx := sync.mutex_create();
 */

/**
 * @nog_fn mutex_lock
 * @module sync
 * @description Acquires the mutex lock. Blocks until lock is available.
 * @param mtx Mutex - The mutex to lock
 * @example
 * import sync;
 * mtx := sync.mutex_create();
 * sync.mutex_lock(mtx);
 * // critical section
 * sync.mutex_unlock(mtx);
 */

/**
 * @nog_fn mutex_unlock
 * @module sync
 * @description Releases the mutex lock.
 * @param mtx Mutex - The mutex to unlock
 * @example
 * import sync;
 * mtx := sync.mutex_create();
 * sync.mutex_lock(mtx);
 * // critical section
 * sync.mutex_unlock(mtx);
 */

/**
 * @nog_fn mutex_try_lock
 * @module sync
 * @description Attempts to acquire the mutex lock without blocking.
 * @param mtx Mutex - The mutex to try to lock
 * @returns bool - True if lock was acquired, false otherwise
 * @example
 * import sync;
 * mtx := sync.mutex_create();
 * if sync.mutex_try_lock(mtx) {
 *     // critical section
 *     sync.mutex_unlock(mtx);
 * }
 */

/**
 * @nog_fn waitgroup_create
 * @module sync
 * @description Creates a new WaitGroup for coordinating goroutines.
 * @returns WaitGroup - A new wait group object
 * @example
 * import sync;
 * wg := sync.waitgroup_create();
 */

/**
 * @nog_fn waitgroup_add
 * @module sync
 * @description Adds delta to the WaitGroup counter.
 * @param wg WaitGroup - The wait group
 * @param n int - The delta to add (usually 1)
 * @example
 * import sync;
 * wg := sync.waitgroup_create();
 * sync.waitgroup_add(wg, 1);
 */

/**
 * @nog_fn waitgroup_done
 * @module sync
 * @description Decrements the WaitGroup counter by 1.
 * @param wg WaitGroup - The wait group
 * @example
 * import sync;
 * wg := sync.waitgroup_create();
 * sync.waitgroup_add(wg, 1);
 * // ... do work ...
 * sync.waitgroup_done(wg);
 */

/**
 * @nog_fn waitgroup_wait
 * @module sync
 * @description Blocks until the WaitGroup counter reaches zero.
 * @param wg WaitGroup - The wait group to wait on
 * @example
 * import sync;
 * wg := sync.waitgroup_create();
 * sync.waitgroup_add(wg, 2);
 * go worker(wg);
 * go worker(wg);
 * sync.waitgroup_wait(wg);
 */

/**
 * @nog_fn once_create
 * @module sync
 * @description Creates a Once object for one-time initialization.
 * @returns Once - A new once object
 * @example
 * import sync;
 * once := sync.once_create();
 */

/**
 * @nog_fn once_do
 * @module sync
 * @description Executes the function exactly once, even if called concurrently.
 * @param once Once - The once object
 * @param fn fn() - The function to execute once
 * @example
 * import sync;
 * once := sync.once_create();
 * sync.once_do(once, init_fn);
 */

/**
 * @nog_fn atomic_int_create
 * @module sync
 * @description Creates a new atomic integer with initial value.
 * @param initial int - The initial value
 * @returns AtomicInt - A new atomic integer
 * @example
 * import sync;
 * counter := sync.atomic_int_create(0);
 */

/**
 * @nog_fn atomic_int_load
 * @module sync
 * @description Atomically loads and returns the current value.
 * @param a AtomicInt - The atomic integer
 * @returns int - The current value
 * @example
 * import sync;
 * counter := sync.atomic_int_create(0);
 * val := sync.atomic_int_load(counter);
 */

/**
 * @nog_fn atomic_int_store
 * @module sync
 * @description Atomically stores a new value.
 * @param a AtomicInt - The atomic integer
 * @param val int - The value to store
 * @example
 * import sync;
 * counter := sync.atomic_int_create(0);
 * sync.atomic_int_store(counter, 42);
 */

/**
 * @nog_fn atomic_int_add
 * @module sync
 * @description Atomically adds delta and returns the previous value.
 * @param a AtomicInt - The atomic integer
 * @param delta int - The value to add
 * @returns int - The previous value before addition
 * @example
 * import sync;
 * counter := sync.atomic_int_create(0);
 * prev := sync.atomic_int_add(counter, 1);
 */

/**
 * @nog_fn atomic_int_swap
 * @module sync
 * @description Atomically swaps the value and returns the old value.
 * @param a AtomicInt - The atomic integer
 * @param val int - The new value
 * @returns int - The old value
 * @example
 * import sync;
 * counter := sync.atomic_int_create(0);
 * old := sync.atomic_int_swap(counter, 100);
 */

/**
 * @nog_fn atomic_int_compare_swap
 * @module sync
 * @description Atomically compares and swaps if equal.
 * @param a AtomicInt - The atomic integer
 * @param expected int - The expected current value
 * @param desired int - The value to set if current equals expected
 * @returns bool - True if swap occurred, false otherwise
 * @example
 * import sync;
 * counter := sync.atomic_int_create(0);
 * success := sync.atomic_int_compare_swap(counter, 0, 1);
 */

#include "sync.hpp"

using namespace std;

namespace nog::stdlib {

/**
 * Creates the AST for the built-in sync module.
 */
unique_ptr<Program> create_sync_module() {
    auto program = make_unique<Program>();

    // Mutex functions

    // fn mutex_create() -> Mutex
    auto mutex_create_fn = make_unique<FunctionDef>();
    mutex_create_fn->name = "mutex_create";
    mutex_create_fn->visibility = Visibility::Public;
    mutex_create_fn->return_type = "Mutex";
    program->functions.push_back(move(mutex_create_fn));

    // fn mutex_lock(Mutex mtx)
    auto mutex_lock_fn = make_unique<FunctionDef>();
    mutex_lock_fn->name = "mutex_lock";
    mutex_lock_fn->visibility = Visibility::Public;
    mutex_lock_fn->params.push_back({"Mutex", "mtx"});
    program->functions.push_back(move(mutex_lock_fn));

    // fn mutex_unlock(Mutex mtx)
    auto mutex_unlock_fn = make_unique<FunctionDef>();
    mutex_unlock_fn->name = "mutex_unlock";
    mutex_unlock_fn->visibility = Visibility::Public;
    mutex_unlock_fn->params.push_back({"Mutex", "mtx"});
    program->functions.push_back(move(mutex_unlock_fn));

    // fn mutex_try_lock(Mutex mtx) -> bool
    auto mutex_try_lock_fn = make_unique<FunctionDef>();
    mutex_try_lock_fn->name = "mutex_try_lock";
    mutex_try_lock_fn->visibility = Visibility::Public;
    mutex_try_lock_fn->params.push_back({"Mutex", "mtx"});
    mutex_try_lock_fn->return_type = "bool";
    program->functions.push_back(move(mutex_try_lock_fn));

    // WaitGroup functions

    // fn waitgroup_create() -> WaitGroup
    auto wg_create_fn = make_unique<FunctionDef>();
    wg_create_fn->name = "waitgroup_create";
    wg_create_fn->visibility = Visibility::Public;
    wg_create_fn->return_type = "WaitGroup";
    program->functions.push_back(move(wg_create_fn));

    // fn waitgroup_add(WaitGroup wg, int n)
    auto wg_add_fn = make_unique<FunctionDef>();
    wg_add_fn->name = "waitgroup_add";
    wg_add_fn->visibility = Visibility::Public;
    wg_add_fn->params.push_back({"WaitGroup", "wg"});
    wg_add_fn->params.push_back({"int", "n"});
    program->functions.push_back(move(wg_add_fn));

    // fn waitgroup_done(WaitGroup wg)
    auto wg_done_fn = make_unique<FunctionDef>();
    wg_done_fn->name = "waitgroup_done";
    wg_done_fn->visibility = Visibility::Public;
    wg_done_fn->params.push_back({"WaitGroup", "wg"});
    program->functions.push_back(move(wg_done_fn));

    // fn waitgroup_wait(WaitGroup wg)
    auto wg_wait_fn = make_unique<FunctionDef>();
    wg_wait_fn->name = "waitgroup_wait";
    wg_wait_fn->visibility = Visibility::Public;
    wg_wait_fn->params.push_back({"WaitGroup", "wg"});
    program->functions.push_back(move(wg_wait_fn));

    // Once functions

    // fn once_create() -> Once
    auto once_create_fn = make_unique<FunctionDef>();
    once_create_fn->name = "once_create";
    once_create_fn->visibility = Visibility::Public;
    once_create_fn->return_type = "Once";
    program->functions.push_back(move(once_create_fn));

    // fn once_do(Once once, fn() f)
    auto once_do_fn = make_unique<FunctionDef>();
    once_do_fn->name = "once_do";
    once_do_fn->visibility = Visibility::Public;
    once_do_fn->params.push_back({"Once", "once"});
    once_do_fn->params.push_back({"fn()", "f"});
    program->functions.push_back(move(once_do_fn));

    // Atomic integer functions

    // fn atomic_int_create(int initial) -> AtomicInt
    auto atomic_create_fn = make_unique<FunctionDef>();
    atomic_create_fn->name = "atomic_int_create";
    atomic_create_fn->visibility = Visibility::Public;
    atomic_create_fn->params.push_back({"int", "initial"});
    atomic_create_fn->return_type = "AtomicInt";
    program->functions.push_back(move(atomic_create_fn));

    // fn atomic_int_load(AtomicInt a) -> int
    auto atomic_load_fn = make_unique<FunctionDef>();
    atomic_load_fn->name = "atomic_int_load";
    atomic_load_fn->visibility = Visibility::Public;
    atomic_load_fn->params.push_back({"AtomicInt", "a"});
    atomic_load_fn->return_type = "int";
    program->functions.push_back(move(atomic_load_fn));

    // fn atomic_int_store(AtomicInt a, int val)
    auto atomic_store_fn = make_unique<FunctionDef>();
    atomic_store_fn->name = "atomic_int_store";
    atomic_store_fn->visibility = Visibility::Public;
    atomic_store_fn->params.push_back({"AtomicInt", "a"});
    atomic_store_fn->params.push_back({"int", "val"});
    program->functions.push_back(move(atomic_store_fn));

    // fn atomic_int_add(AtomicInt a, int delta) -> int
    auto atomic_add_fn = make_unique<FunctionDef>();
    atomic_add_fn->name = "atomic_int_add";
    atomic_add_fn->visibility = Visibility::Public;
    atomic_add_fn->params.push_back({"AtomicInt", "a"});
    atomic_add_fn->params.push_back({"int", "delta"});
    atomic_add_fn->return_type = "int";
    program->functions.push_back(move(atomic_add_fn));

    // fn atomic_int_swap(AtomicInt a, int val) -> int
    auto atomic_swap_fn = make_unique<FunctionDef>();
    atomic_swap_fn->name = "atomic_int_swap";
    atomic_swap_fn->visibility = Visibility::Public;
    atomic_swap_fn->params.push_back({"AtomicInt", "a"});
    atomic_swap_fn->params.push_back({"int", "val"});
    atomic_swap_fn->return_type = "int";
    program->functions.push_back(move(atomic_swap_fn));

    // fn atomic_int_compare_swap(AtomicInt a, int expected, int desired) -> bool
    auto atomic_cas_fn = make_unique<FunctionDef>();
    atomic_cas_fn->name = "atomic_int_compare_swap";
    atomic_cas_fn->visibility = Visibility::Public;
    atomic_cas_fn->params.push_back({"AtomicInt", "a"});
    atomic_cas_fn->params.push_back({"int", "expected"});
    atomic_cas_fn->params.push_back({"int", "desired"});
    atomic_cas_fn->return_type = "bool";
    program->functions.push_back(move(atomic_cas_fn));

    return program;
}

/**
 * Returns empty - sync.hpp is included at the top of generated code.
 * The code generator maps "sync" module calls to "bsync::" namespace
 * to avoid conflict with POSIX sync() function.
 */
string generate_sync_runtime() {
    return "";
}

}  // namespace nog::stdlib
