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
#include <random>
#include <algorithm>
#include <vector>
#include <string>

namespace bishop_random {

/**
 * Global random engine instance.
 * Uses Mersenne Twister for high-quality randomness.
 */
inline std::mt19937& engine() {
    static std::mt19937 gen(std::random_device{}());
    return gen;
}

/**
 * Generates a random integer in the inclusive range [min, max].
 * @param min Minimum value (inclusive)
 * @param max Maximum value (inclusive)
 * @return Random integer in the specified range
 */
inline int int_(int min, int max) {
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
 * @param min Minimum value (inclusive)
 * @param max Maximum value (exclusive)
 * @return Random float in the specified range
 */
inline double float_range(double min, double max) {
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
 * @return A random element from the list
 */
inline std::string choice(const std::vector<std::string>& list) {
    if (list.empty()) {
        return "";
    }

    std::uniform_int_distribution<size_t> dist(0, list.size() - 1);
    return list[dist(engine())];
}

/**
 * Selects a random element from an integer list.
 * @param list The list to choose from
 * @return A random element from the list
 */
inline int choice_int(const std::vector<int>& list) {
    if (list.empty()) {
        return 0;
    }

    std::uniform_int_distribution<size_t> dist(0, list.size() - 1);
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

    size_t n = std::min(static_cast<size_t>(count), list.size());

    // Copy and shuffle to get random sample
    std::vector<std::string> result = list;
    std::shuffle(result.begin(), result.end(), engine());
    result.resize(n);
    return result;
}

/**
 * Samples n random unique elements from an integer list.
 * @param list The list to sample from
 * @param count Number of elements to sample
 * @return A new list with the sampled elements
 */
inline std::vector<int> sample_int(const std::vector<int>& list, int count) {
    if (count <= 0 || list.empty()) {
        return {};
    }

    size_t n = std::min(static_cast<size_t>(count), list.size());

    // Copy and shuffle to get random sample
    std::vector<int> result = list;
    std::shuffle(result.begin(), result.end(), engine());
    result.resize(n);
    return result;
}

/**
 * Seeds the random number generator for deterministic sequences.
 * @param n The seed value
 */
inline void seed(int n) {
    engine().seed(static_cast<unsigned int>(n));
}

}  // namespace bishop_random
