/**
 * @file priority_queue.hpp
 * @brief Priority queue implementation for Bishop.
 *
 * Provides MaxPriorityQueue and MinPriorityQueue templates that wrap
 * std::priority_queue with a unified interface for both primitives
 * and custom struct types with less_than methods.
 */

#ifndef BISHOP_COLLECTIONS_PRIORITY_QUEUE_HPP
#define BISHOP_COLLECTIONS_PRIORITY_QUEUE_HPP

#include <queue>
#include <vector>
#include <type_traits>
#include <functional>

namespace bishop {

namespace detail {

/**
 * SFINAE helper to detect if a type has a less_than method.
 */
template<typename T, typename = void>
struct has_less_than : std::false_type {};

template<typename T>
struct has_less_than<T, std::void_t<decltype(std::declval<T>().less_than(std::declval<T>()))>> : std::true_type {};

template<typename T>
inline constexpr bool has_less_than_v = has_less_than<T>::value;

}  // namespace detail

/**
 * Base class for priority queues providing the common interface.
 * The Comparator template parameter determines max heap vs min heap behavior.
 */
template<typename T, typename Comparator>
class PriorityQueueImpl {
public:
    PriorityQueueImpl() = default;

    /**
     * Adds an element to the priority queue.
     */
    void push(const T& value) {
        pq_.push(value);
    }

    /**
     * Removes and returns the highest priority element.
     */
    T pop() {
        T top = pq_.top();
        pq_.pop();
        return top;
    }

    /**
     * Returns the highest priority element without removing it.
     */
    const T& top() const {
        return pq_.top();
    }

    /**
     * Returns the number of elements in the queue.
     */
    int size() const {
        return static_cast<int>(pq_.size());
    }

    /**
     * Returns true if the queue is empty.
     */
    bool empty() const {
        return pq_.empty();
    }

private:
    std::priority_queue<T, std::vector<T>, Comparator> pq_;
};

/**
 * Max heap comparator for primitive types (uses <).
 */
template<typename T>
struct MaxHeapComparator {
    bool operator()(const T& a, const T& b) const {
        // For max heap in std::priority_queue, return true if a < b
        // (this causes larger elements to be at the top)
        return a < b;
    }
};

/**
 * Max heap comparator for custom types with less_than method.
 * Uses const_cast because Bishop methods don't have const qualifiers yet,
 * but less_than is logically const (pure comparison, no mutation).
 */
template<typename T>
struct MaxHeapComparatorLessThan {
    bool operator()(const T& a, const T& b) const {
        // For max heap, return true if a < b
        return const_cast<T&>(a).less_than(const_cast<T&>(b));
    }
};

/**
 * Min heap comparator for primitive types (uses >).
 */
template<typename T>
struct MinHeapComparator {
    bool operator()(const T& a, const T& b) const {
        // For min heap in std::priority_queue, return true if a > b
        // (this causes smaller elements to be at the top)
        return a > b;
    }
};

/**
 * Min heap comparator for custom types with less_than method.
 * Uses const_cast because Bishop methods don't have const qualifiers yet,
 * but less_than is logically const (pure comparison, no mutation).
 */
template<typename T>
struct MinHeapComparatorLessThan {
    bool operator()(const T& a, const T& b) const {
        // For min heap, return true if a > b (using less_than: b < a)
        return const_cast<T&>(b).less_than(const_cast<T&>(a));
    }
};

/**
 * Max priority queue (default).
 * Elements with higher priority (greater value) come first.
 * For custom types, uses less_than method; for primitives uses <.
 */
template<typename T>
using MaxPriorityQueue = PriorityQueueImpl<T,
    std::conditional_t<detail::has_less_than_v<T>,
        MaxHeapComparatorLessThan<T>,
        MaxHeapComparator<T>>>;

/**
 * Min priority queue.
 * Elements with lower priority (smaller value) come first.
 * For custom types, uses less_than method; for primitives uses >.
 */
template<typename T>
using MinPriorityQueue = PriorityQueueImpl<T,
    std::conditional_t<detail::has_less_than_v<T>,
        MinHeapComparatorLessThan<T>,
        MinHeapComparator<T>>>;

/**
 * Base type alias for PriorityQueue<T>.
 * Used by codegen for type declarations. Actual instantiation
 * uses MaxPriorityQueue or MinPriorityQueue directly.
 */
template<typename T>
using PriorityQueueBase = MaxPriorityQueue<T>;

}  // namespace bishop

#endif  // BISHOP_COLLECTIONS_PRIORITY_QUEUE_HPP
