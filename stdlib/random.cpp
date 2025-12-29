/**
 * @file random.cpp
 * @brief Built-in random module implementation.
 *
 * Creates the AST definitions for the random module.
 * The actual runtime is in runtime/random/random.hpp and included as a header.
 */

/**
 * @nog_fn int
 * @module random
 * @description Generates a random integer in the inclusive range [min, max].
 * @param min int - Minimum value (inclusive)
 * @param max int - Maximum value (inclusive)
 * @returns int - Random integer in the specified range
 * @example
 * dice := random.int(1, 6);
 * print(dice);  // 1-6
 */

/**
 * @nog_fn float
 * @module random
 * @description Generates a random floating-point number in [0.0, 1.0).
 * @returns f64 - Random float in [0.0, 1.0)
 * @example
 * chance := random.float();
 * print(chance);  // 0.0 to 1.0
 */

/**
 * @nog_fn float_range
 * @module random
 * @description Generates a random floating-point number in [min, max).
 * @param min f64 - Minimum value (inclusive)
 * @param max f64 - Maximum value (exclusive)
 * @returns f64 - Random float in the specified range
 * @example
 * temp := random.float_range(-10.0, 40.0);
 * print(temp);  // temperature in range
 */

/**
 * @nog_fn bool
 * @module random
 * @description Generates a random boolean with 50/50 probability.
 * @returns bool - Random boolean value
 * @example
 * coin := random.bool();
 * if coin { print("heads"); }
 */

/**
 * @nog_fn bool_prob
 * @module random
 * @description Generates a random boolean with specified probability.
 * @param probability f64 - Probability of returning true (0.0 to 1.0)
 * @returns bool - True with the specified probability
 * @example
 * rare := random.bool_prob(0.1);  // 10% chance of true
 */

/**
 * @nog_fn choice
 * @module random
 * @description Selects a random element from a string list.
 * @param list List<str> - The list to choose from
 * @returns str or err - A random element from the list, or error if list is empty
 * @example
 * items := ["a", "b", "c"];
 * pick := random.choice(items) or return;
 */

/**
 * @nog_fn choice_int
 * @module random
 * @description Selects a random element from an integer list.
 * @param list List<int> - The list to choose from
 * @returns int or err - A random element from the list, or error if list is empty
 * @example
 * nums := [10, 20, 30];
 * pick := random.choice_int(nums) or return;
 */

/**
 * @nog_fn shuffle
 * @module random
 * @description Shuffles a string list in place using Fisher-Yates algorithm.
 * @param list List<str> - The list to shuffle (modified in place)
 * @example
 * items := ["a", "b", "c"];
 * random.shuffle(items);
 * print(items);  // shuffled order
 */

/**
 * @nog_fn shuffle_int
 * @module random
 * @description Shuffles an integer list in place using Fisher-Yates algorithm.
 * @param list List<int> - The list to shuffle (modified in place)
 * @example
 * nums := [1, 2, 3, 4, 5];
 * random.shuffle_int(nums);
 */

/**
 * @nog_fn sample
 * @module random
 * @description Samples n random unique elements from a string list.
 * @param list List<str> - The list to sample from
 * @param count int - Number of elements to sample
 * @returns List<str> - A new list with the sampled elements
 * @example
 * items := ["a", "b", "c", "d", "e"];
 * sampled := random.sample(items, 2);
 */

/**
 * @nog_fn sample_int
 * @module random
 * @description Samples n random unique elements from an integer list.
 * @param list List<int> - The list to sample from
 * @param count int - Number of elements to sample
 * @returns List<int> - A new list with the sampled elements
 * @example
 * nums := [1, 2, 3, 4, 5];
 * sampled := random.sample_int(nums, 2);
 */

/**
 * @nog_fn seed
 * @module random
 * @description Seeds the random number generator for deterministic sequences.
 * @param n int - The seed value
 * @example
 * random.seed(42);
 * print(random.int(1, 100));  // always same sequence with seed 42
 */

#include "random.hpp"

using namespace std;

namespace nog::stdlib {

/**
 * Creates the AST for the built-in random module.
 */
unique_ptr<Program> create_random_module() {
    auto program = make_unique<Program>();

    // fn int(int min, int max) -> int
    auto int_fn = make_unique<FunctionDef>();
    int_fn->name = "int";
    int_fn->visibility = Visibility::Public;
    int_fn->params.push_back({"int", "min"});
    int_fn->params.push_back({"int", "max"});
    int_fn->return_type = "int";
    program->functions.push_back(move(int_fn));

    // fn float() -> f64
    auto float_fn = make_unique<FunctionDef>();
    float_fn->name = "float";
    float_fn->visibility = Visibility::Public;
    float_fn->return_type = "f64";
    program->functions.push_back(move(float_fn));

    // fn float_range(f64 min, f64 max) -> f64
    auto float_range_fn = make_unique<FunctionDef>();
    float_range_fn->name = "float_range";
    float_range_fn->visibility = Visibility::Public;
    float_range_fn->params.push_back({"f64", "min"});
    float_range_fn->params.push_back({"f64", "max"});
    float_range_fn->return_type = "f64";
    program->functions.push_back(move(float_range_fn));

    // fn bool() -> bool
    auto bool_fn = make_unique<FunctionDef>();
    bool_fn->name = "bool";
    bool_fn->visibility = Visibility::Public;
    bool_fn->return_type = "bool";
    program->functions.push_back(move(bool_fn));

    // fn bool_prob(f64 probability) -> bool
    auto bool_prob_fn = make_unique<FunctionDef>();
    bool_prob_fn->name = "bool_prob";
    bool_prob_fn->visibility = Visibility::Public;
    bool_prob_fn->params.push_back({"f64", "probability"});
    bool_prob_fn->return_type = "bool";
    program->functions.push_back(move(bool_prob_fn));

    // fn choice(List<str> list) -> str or err
    auto choice_fn = make_unique<FunctionDef>();
    choice_fn->name = "choice";
    choice_fn->visibility = Visibility::Public;
    choice_fn->params.push_back({"List<str>", "list"});
    choice_fn->return_type = "str";
    choice_fn->error_type = "err";
    program->functions.push_back(move(choice_fn));

    // fn choice_int(List<int> list) -> int or err
    auto choice_int_fn = make_unique<FunctionDef>();
    choice_int_fn->name = "choice_int";
    choice_int_fn->visibility = Visibility::Public;
    choice_int_fn->params.push_back({"List<int>", "list"});
    choice_int_fn->return_type = "int";
    choice_int_fn->error_type = "err";
    program->functions.push_back(move(choice_int_fn));

    // fn shuffle(List<str> list) - void, mutates in place
    auto shuffle_fn = make_unique<FunctionDef>();
    shuffle_fn->name = "shuffle";
    shuffle_fn->visibility = Visibility::Public;
    shuffle_fn->params.push_back({"List<str>", "list"});
    shuffle_fn->return_type = "";
    program->functions.push_back(move(shuffle_fn));

    // fn shuffle_int(List<int> list) - void, mutates in place
    auto shuffle_int_fn = make_unique<FunctionDef>();
    shuffle_int_fn->name = "shuffle_int";
    shuffle_int_fn->visibility = Visibility::Public;
    shuffle_int_fn->params.push_back({"List<int>", "list"});
    shuffle_int_fn->return_type = "";
    program->functions.push_back(move(shuffle_int_fn));

    // fn sample(List<str> list, int count) -> List<str>
    auto sample_fn = make_unique<FunctionDef>();
    sample_fn->name = "sample";
    sample_fn->visibility = Visibility::Public;
    sample_fn->params.push_back({"List<str>", "list"});
    sample_fn->params.push_back({"int", "count"});
    sample_fn->return_type = "List<str>";
    program->functions.push_back(move(sample_fn));

    // fn sample_int(List<int> list, int count) -> List<int>
    auto sample_int_fn = make_unique<FunctionDef>();
    sample_int_fn->name = "sample_int";
    sample_int_fn->visibility = Visibility::Public;
    sample_int_fn->params.push_back({"List<int>", "list"});
    sample_int_fn->params.push_back({"int", "count"});
    sample_int_fn->return_type = "List<int>";
    program->functions.push_back(move(sample_int_fn));

    // fn seed(int n) - void
    auto seed_fn = make_unique<FunctionDef>();
    seed_fn->name = "seed";
    seed_fn->visibility = Visibility::Public;
    seed_fn->params.push_back({"int", "n"});
    seed_fn->return_type = "";
    program->functions.push_back(move(seed_fn));

    return program;
}

/**
 * Returns empty - random.hpp is included at the top of generated code
 * for precompiled header support.
 */
string generate_random_runtime() {
    return "";
}

}  // namespace nog::stdlib
