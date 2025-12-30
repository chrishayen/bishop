/**
 * @file random.hpp
 * @brief Bishop random runtime library.
 *
 * Provides random number generation functionality for Bishop programs.
 * This header is included when programs import the random module.
 * Uses std::mt19937 for high-quality pseudorandom number generation.
 */

#pragma once

#include <bishop/std.hpp>
#include <bishop/error.hpp>
#include <random>
#include <algorithm>
#include <vector>
#include <string>

namespace bishop_random {

/**
 * Global random engine instance.
 * Uses Mersenne Twister for high-quality randomness.
 * Uses thread_local storage to be safe when called from multiple threads/goroutines.
 */
inline std::mt19937& engine() {
    static thread_local std::mt19937 gen(std::random_device{}());
    return gen;
}

/**
 * Generates a random integer in the inclusive range [min, max].
 * If min > max, the values are swapped to ensure valid distribution.
 * @param min Minimum value (inclusive)
 * @param max Maximum value (inclusive)
 * @return Random integer in the specified range
 */
inline int int_(int min, int max) {
    if (min > max) {
        std::swap(min, max);
    }

    std::uniform_int_distribution<int> dist(min, max);
    return dist(engine());
}

/**
 * Generates a random floating-point number in [0.0, 1.0).
 * @return Random float in [0.0, 1.0)
 */
inline double float_() {
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(engine());
}

/**
 * Generates a random floating-point number in [min, max).
 * If min > max, the values are swapped to ensure valid distribution.
 * @param min Minimum value (inclusive)
 * @param max Maximum value (exclusive)
 * @return Random float in the specified range
 */
inline double float_range(double min, double max) {
    if (min > max) {
        std::swap(min, max);
    }

    std::uniform_real_distribution<double> dist(min, max);
    return dist(engine());
}

/**
 * Generates a random boolean with 50/50 probability.
 * @return Random boolean value
 */
inline bool bool_() {
    std::uniform_int_distribution<int> dist(0, 1);
    return dist(engine()) == 1;
}

/**
 * Generates a random boolean with specified probability.
 * @param probability Probability of returning true (0.0 to 1.0)
 * @return True with the specified probability
 */
inline bool bool_prob(double probability) {
    if (probability <= 0.0) {
        return false;
    }

    if (probability >= 1.0) {
        return true;
    }

    std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(engine()) < probability;
}

/**
 * Selects a random element from a string list.
 * @param list The list to choose from
 * @return A random element from the list, or error if list is empty
 */
inline bishop::rt::Result<std::string> choice(const std::vector<std::string>& list) {
    if (list.empty()) {
        return bishop::rt::make_error<std::string>("cannot choose from empty list");
    }

    std::uniform_int_distribution<unsigned long> dist(0, list.size() - 1);
    return list[dist(engine())];
}

/**
 * Selects a random element from an integer list.
 * @param list The list to choose from
 * @return A random element from the list, or error if list is empty
 */
inline bishop::rt::Result<int> choice_int(const std::vector<int>& list) {
    if (list.empty()) {
        return bishop::rt::make_error<int>("cannot choose from empty list");
    }

    std::uniform_int_distribution<unsigned long> dist(0, list.size() - 1);
    return list[dist(engine())];
}

/**
 * Shuffles a string list in place using Fisher-Yates algorithm.
 * @param list The list to shuffle (modified in place)
 */
inline void shuffle(std::vector<std::string>& list) {
    std::shuffle(list.begin(), list.end(), engine());
}

/**
 * Shuffles an integer list in place using Fisher-Yates algorithm.
 * @param list The list to shuffle (modified in place)
 */
inline void shuffle_int(std::vector<int>& list) {
    std::shuffle(list.begin(), list.end(), engine());
}

/**
 * Samples n random unique elements from a string list.
 * Uses reservoir sampling for efficiency when count is much smaller than list size.
 * @param list The list to sample from
 * @param count Number of elements to sample
 * @return A new list with the sampled elements
 */
inline std::vector<std::string> sample(
    const std::vector<std::string>& list,
    int count
) {
    if (count <= 0 || list.empty()) {
        return {};
    }

    std::size_t n = std::min(static_cast<std::size_t>(count), list.size());

    // Use reservoir sampling to avoid copying and shuffling the entire list
    std::vector<std::string> result;
    result.reserve(n);

    // Fill reservoir with first n elements
    for (std::size_t i = 0; i < n; ++i) {
        result.push_back(list[i]);
    }

    // Replace elements in reservoir with decreasing probability
    for (std::size_t i = n; i < list.size(); ++i) {
        std::uniform_int_distribution<unsigned long> dist(0, i);
        std::size_t j = dist(engine());

        if (j < n) {
            result[j] = list[i];
        }
    }

    return result;
}

/**
 * Samples n random unique elements from an integer list.
 * Uses reservoir sampling for efficiency when count is much smaller than list size.
 * @param list The list to sample from
 * @param count Number of elements to sample
 * @return A new list with the sampled elements
 */
inline std::vector<int> sample_int(const std::vector<int>& list, int count) {
    if (count <= 0 || list.empty()) {
        return {};
    }

    std::size_t n = std::min(static_cast<std::size_t>(count), list.size());

    // Use reservoir sampling to avoid copying and shuffling the entire list
    std::vector<int> result;
    result.reserve(n);

    // Fill reservoir with first n elements
    for (std::size_t i = 0; i < n; ++i) {
        result.push_back(list[i]);
    }

    // Replace elements in reservoir with decreasing probability
    for (std::size_t i = n; i < list.size(); ++i) {
        std::uniform_int_distribution<unsigned long> dist(0, i);
        std::size_t j = dist(engine());

        if (j < n) {
            result[j] = list[i];
        }
    }

    return result;
}

/**
 * Seeds the random number generator for deterministic sequences.
 * @param n The seed value (unsigned). Negative values are converted to unsigned
 *          using two's complement representation, which produces consistent but
 *          potentially unintuitive results.
 */
inline void seed(unsigned int n) {
    engine().seed(n);
}

}  // namespace bishop_random
