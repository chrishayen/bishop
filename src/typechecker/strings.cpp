#include "strings.hpp"

#include <map>

namespace nog {

/**
 * Returns type information for built-in str methods.
 * Maps method names to their parameter types and return types.
 */
std::optional<StrMethodInfo> get_str_method_info(const std::string& method_name) {
    static const std::map<std::string, StrMethodInfo> str_methods = {
        {"length", {{}, "int"}},
        {"empty", {{}, "bool"}},
        {"contains", {{"str"}, "bool"}},
        {"starts_with", {{"str"}, "bool"}},
        {"ends_with", {{"str"}, "bool"}},
        {"find", {{"str"}, "int"}},
        {"substr", {{"int", "int"}, "str"}},
        {"at", {{"int"}, "char"}},
    };

    auto it = str_methods.find(method_name);

    if (it != str_methods.end()) {
        return it->second;
    }

    return std::nullopt;
}

}  // namespace nog
