#pragma once
#include <string>
#include <fmt/format.h>

using namespace std;

namespace nog::runtime {

inline string string_literal(const string& value) {
    return fmt::format("\"{}\"", value);
}

inline string number_literal(const string& value) {
    return value;
}

inline string float_literal(const string& value) {
    return value;
}

inline string bool_literal(bool value) {
    return value ? "true" : "false";
}

inline string none_literal() {
    return "std::nullopt";
}

}
