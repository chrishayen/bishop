#pragma once
#include <string>
#include <vector>
#include <fmt/format.h>
#include <fmt/ranges.h>

using namespace std;

namespace nog::runtime {

inline string variable_ref(const string& name) {
    return name;
}

inline string binary_expr(const string& left, const string& op, const string& right) {
    return fmt::format("{} {} {}", left, op, right);
}

inline string function_call(const string& name, const vector<string>& args) {
    return fmt::format("{}({})", name, fmt::join(args, ", "));
}

inline string is_none(const string& value) {
    return fmt::format("!{}.has_value()", value);
}

}
