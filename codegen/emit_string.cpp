/**
 * @file emit_string.cpp
 * @brief String method emission for the Bishop code generator.
 *
 * Handles emitting C++ code for extended string methods that don't
 * have direct std::string equivalents.
 */

#include "codegen.hpp"
#include <fmt/format.h>

using namespace std;

namespace codegen {

/**
 * Emits C++ code for the upper() string method.
 * Transforms all characters to uppercase.
 */
string emit_str_upper(const string& obj_str) {
    return fmt::format(
        "[](std::string s) {{ "
        "std::transform(s.begin(), s.end(), s.begin(), ::toupper); "
        "return s; "
        "}}({})",
        obj_str
    );
}

/**
 * Emits C++ code for the lower() string method.
 * Transforms all characters to lowercase.
 */
string emit_str_lower(const string& obj_str) {
    return fmt::format(
        "[](std::string s) {{ "
        "std::transform(s.begin(), s.end(), s.begin(), ::tolower); "
        "return s; "
        "}}({})",
        obj_str
    );
}

/**
 * Emits C++ code for the capitalize() string method.
 * Capitalizes the first character and lowercases the rest.
 */
string emit_str_capitalize(const string& obj_str) {
    return fmt::format(
        "[](std::string s) {{ "
        "if (s.empty()) return s; "
        "std::transform(s.begin(), s.end(), s.begin(), ::tolower); "
        "s[0] = std::toupper(s[0]); "
        "return s; "
        "}}({})",
        obj_str
    );
}

/**
 * Emits C++ code for the title() string method.
 * Capitalizes the first character of each word.
 */
string emit_str_title(const string& obj_str) {
    return fmt::format(
        "[](std::string s) {{ "
        "bool new_word = true; "
        "for (size_t i = 0; i < s.size(); ++i) {{ "
        "if (std::isspace(s[i])) {{ new_word = true; }} "
        "else if (new_word) {{ s[i] = std::toupper(s[i]); new_word = false; }} "
        "else {{ s[i] = std::tolower(s[i]); }} "
        "}} "
        "return s; "
        "}}({})",
        obj_str
    );
}

/**
 * Emits C++ code for the trim() string method.
 * Removes whitespace from both ends.
 */
string emit_str_trim(const string& obj_str) {
    return fmt::format(
        "[](std::string s) {{ "
        "size_t start = s.find_first_not_of(\" \\t\\n\\r\\f\\v\"); "
        "if (start == std::string::npos) return std::string(); "
        "size_t end = s.find_last_not_of(\" \\t\\n\\r\\f\\v\"); "
        "return s.substr(start, end - start + 1); "
        "}}({})",
        obj_str
    );
}

/**
 * Emits C++ code for the trim_left() string method.
 * Removes whitespace from the start.
 */
string emit_str_trim_left(const string& obj_str) {
    return fmt::format(
        "[](std::string s) {{ "
        "size_t start = s.find_first_not_of(\" \\t\\n\\r\\f\\v\"); "
        "if (start == std::string::npos) return std::string(); "
        "return s.substr(start); "
        "}}({})",
        obj_str
    );
}

/**
 * Emits C++ code for the trim_right() string method.
 * Removes whitespace from the end.
 */
string emit_str_trim_right(const string& obj_str) {
    return fmt::format(
        "[](std::string s) {{ "
        "size_t end = s.find_last_not_of(\" \\t\\n\\r\\f\\v\"); "
        "if (end == std::string::npos) return std::string(); "
        "return s.substr(0, end + 1); "
        "}}({})",
        obj_str
    );
}

/**
 * Emits C++ code for the replace() string method.
 * Replaces the first occurrence of a substring.
 */
string emit_str_replace(const string& obj_str, const string& old_str, const string& new_str) {
    return fmt::format(
        "[](std::string s, const std::string& from, const std::string& to) {{ "
        "if (from.empty()) return s; "
        "size_t pos = s.find(from); "
        "if (pos != std::string::npos) {{ "
        "s.replace(pos, from.length(), to); "
        "}} "
        "return s; "
        "}}({}, {}, {})",
        obj_str, old_str, new_str
    );
}

/**
 * Emits C++ code for the replace_all() string method.
 * Replaces all occurrences of a substring.
 */
string emit_str_replace_all(const string& obj_str, const string& old_str, const string& new_str) {
    return fmt::format(
        "[](std::string s, const std::string& from, const std::string& to) {{ "
        "if (from.empty()) return s; "
        "size_t pos = 0; "
        "while ((pos = s.find(from, pos)) != std::string::npos) {{ "
        "s.replace(pos, from.length(), to); "
        "pos += to.length(); "
        "}} "
        "return s; "
        "}}({}, {}, {})",
        obj_str, old_str, new_str
    );
}

/**
 * Emits C++ code for the reverse() string method.
 * Reverses the characters in the string.
 */
string emit_str_reverse(const string& obj_str) {
    return fmt::format(
        "[](std::string s) {{ "
        "std::reverse(s.begin(), s.end()); "
        "return s; "
        "}}({})",
        obj_str
    );
}

/**
 * Emits C++ code for the repeat() string method.
 * Repeats the string a specified number of times.
 */
string emit_str_repeat(const string& obj_str, const string& count) {
    return fmt::format(
        "[](const std::string& s, int n) {{ "
        "std::string result; "
        "result.reserve(s.size() * n); "
        "for (int i = 0; i < n; ++i) result += s; "
        "return result; "
        "}}({}, {})",
        obj_str, count
    );
}

/**
 * Emits C++ code for the split() string method.
 * Splits the string by a delimiter.
 */
string emit_str_split(const string& obj_str, const string& delimiter) {
    return fmt::format(
        "[](const std::string& s, const std::string& delim) {{ "
        "std::vector<std::string> result; "
        "if (delim.empty()) {{ result.push_back(s); return result; }} "
        "size_t start = 0, end = 0; "
        "while ((end = s.find(delim, start)) != std::string::npos) {{ "
        "result.push_back(s.substr(start, end - start)); "
        "start = end + delim.length(); "
        "}} "
        "result.push_back(s.substr(start)); "
        "return result; "
        "}}({}, {})",
        obj_str, delimiter
    );
}

/**
 * Emits C++ code for the split_lines() string method.
 * Splits the string by newlines.
 */
string emit_str_split_lines(const string& obj_str) {
    return fmt::format(
        "[](const std::string& s) {{ "
        "std::vector<std::string> result; "
        "size_t start = 0, end = 0; "
        "while ((end = s.find('\\n', start)) != std::string::npos) {{ "
        "result.push_back(s.substr(start, end - start)); "
        "start = end + 1; "
        "}} "
        "result.push_back(s.substr(start)); "
        "return result; "
        "}}({})",
        obj_str
    );
}

/**
 * Emits C++ code for the pad_left() string method.
 * Pads the string on the left to reach a specified width.
 */
string emit_str_pad_left(const string& obj_str, const string& width, const string& fill_char) {
    return fmt::format(
        "[](const std::string& s, int w, char c) {{ "
        "if (static_cast<int>(s.size()) >= w) return s; "
        "return std::string(w - s.size(), c) + s; "
        "}}({}, {}, {})",
        obj_str, width, fill_char
    );
}

/**
 * Emits C++ code for the pad_right() string method.
 * Pads the string on the right to reach a specified width.
 */
string emit_str_pad_right(const string& obj_str, const string& width, const string& fill_char) {
    return fmt::format(
        "[](const std::string& s, int w, char c) {{ "
        "if (static_cast<int>(s.size()) >= w) return s; "
        "return s + std::string(w - s.size(), c); "
        "}}({}, {}, {})",
        obj_str, width, fill_char
    );
}

/**
 * Emits C++ code for the center() string method.
 * Centers the string within a specified width.
 */
string emit_str_center(const string& obj_str, const string& width, const string& fill_char) {
    return fmt::format(
        "[](const std::string& s, int w, char c) {{ "
        "if (static_cast<int>(s.size()) >= w) return s; "
        "int total_pad = w - s.size(); "
        "int left_pad = total_pad / 2; "
        "int right_pad = total_pad - left_pad; "
        "return std::string(left_pad, c) + s + std::string(right_pad, c); "
        "}}({}, {}, {})",
        obj_str, width, fill_char
    );
}

/**
 * Emits C++ code for the to_int() string method.
 * Parses the string as an integer.
 */
string emit_str_to_int(const string& obj_str) {
    return fmt::format("std::stoi({})", obj_str);
}

/**
 * Emits C++ code for the to_float() string method.
 * Parses the string as a floating-point number.
 */
string emit_str_to_float(const string& obj_str) {
    return fmt::format("std::stod({})", obj_str);
}

/**
 * Emits a string method call, handling extended methods that don't
 * have direct std::string equivalents.
 */
string emit_str_method_call(CodeGenState& state, const MethodCall& call, const string& obj_str, const vector<string>& args) {
    const string& method = call.method_name;

    // Case transformation methods
    if (method == "upper") {
        return emit_str_upper(obj_str);
    }

    if (method == "lower") {
        return emit_str_lower(obj_str);
    }

    if (method == "capitalize") {
        return emit_str_capitalize(obj_str);
    }

    if (method == "title") {
        return emit_str_title(obj_str);
    }

    // Trimming methods
    if (method == "trim") {
        return emit_str_trim(obj_str);
    }

    if (method == "trim_left") {
        return emit_str_trim_left(obj_str);
    }

    if (method == "trim_right") {
        return emit_str_trim_right(obj_str);
    }

    // Replace methods
    if (method == "replace") {
        return emit_str_replace(obj_str, args[0], args[1]);
    }

    if (method == "replace_all") {
        return emit_str_replace_all(obj_str, args[0], args[1]);
    }

    // Reverse and repeat
    if (method == "reverse") {
        return emit_str_reverse(obj_str);
    }

    if (method == "repeat") {
        return emit_str_repeat(obj_str, args[0]);
    }

    // Split methods
    if (method == "split") {
        return emit_str_split(obj_str, args[0]);
    }

    if (method == "split_lines") {
        return emit_str_split_lines(obj_str);
    }

    // Padding methods (with optional char parameter)
    if (method == "pad_left") {
        string fill_char = args.size() > 1 ? args[1] : "' '";
        return emit_str_pad_left(obj_str, args[0], fill_char);
    }

    if (method == "pad_right") {
        string fill_char = args.size() > 1 ? args[1] : "' '";
        return emit_str_pad_right(obj_str, args[0], fill_char);
    }

    if (method == "center") {
        string fill_char = args.size() > 1 ? args[1] : "' '";
        return emit_str_center(obj_str, args[0], fill_char);
    }

    // Conversion methods
    if (method == "to_int") {
        return emit_str_to_int(obj_str);
    }

    if (method == "to_float") {
        return emit_str_to_float(obj_str);
    }

    // Fall back to direct method call for existing methods
    // (length, empty, contains, starts_with, ends_with, find, substr, at)
    return "";
}

} // namespace codegen
