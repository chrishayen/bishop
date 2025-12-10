#pragma once
#include <string>
#include <fmt/format.h>
#include "types.hpp"

using namespace std;

namespace nog::runtime {

inline string variable_decl(const string& type, const string& name, const string& value) {
    string t = type.empty() ? "auto" : map_type(type);
    return fmt::format("{} {} = {};", t, name, value);
}

inline string return_stmt(const string& value) {
    return fmt::format("return {};", value);
}

}
