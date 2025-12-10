#pragma once
#include <string>
#include <vector>
#include <fmt/format.h>
#include <fmt/ranges.h>

using namespace std;

namespace nog::runtime {

inline string print(const string& arg) {
    return fmt::format("std::cout << {} << std::endl", arg);
}

inline string print_multi(const vector<string>& args) {
    return fmt::format("std::cout << {} << std::endl", fmt::join(args, " << "));
}

inline string assert_eq(const string& a, const string& b, int line) {
    return fmt::format("_assert_eq({}, {}, {})", a, b, line);
}

}
