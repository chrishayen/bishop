/**
 * @nog_method length
 * @type str
 * @description Returns the number of characters in the string.
 * @returns int - The string length
 * @example
 * s := "hello";
 * len := s.length();  // 5
 */

/**
 * @nog_method empty
 * @type str
 * @description Returns true if the string has no characters.
 * @returns bool - True if empty, false otherwise
 * @example
 * s := "";
 * if s.empty() {
 *     print("String is empty");
 * }
 */

/**
 * @nog_method contains
 * @type str
 * @description Checks if the string contains the given substring.
 * @param substr str - The substring to search for
 * @returns bool - True if found, false otherwise
 * @example
 * s := "hello world";
 * if s.contains("world") {
 *     print("Found it!");
 * }
 */

/**
 * @nog_method starts_with
 * @type str
 * @description Checks if the string starts with the given prefix.
 * @param prefix str - The prefix to check
 * @returns bool - True if string starts with prefix
 * @example
 * path := "/api/users";
 * if path.starts_with("/api") {
 *     print("API route");
 * }
 */

/**
 * @nog_method ends_with
 * @type str
 * @description Checks if the string ends with the given suffix.
 * @param suffix str - The suffix to check
 * @returns bool - True if string ends with suffix
 * @example
 * file := "image.png";
 * if file.ends_with(".png") {
 *     print("PNG image");
 * }
 */

/**
 * @nog_method find
 * @type str
 * @description Returns the index of the first occurrence of a substring, or -1 if not found.
 * @param substr str - The substring to find
 * @returns int - Index of first occurrence, or -1
 * @example
 * s := "hello world";
 * idx := s.find("world");  // 6
 */

/**
 * @nog_method substr
 * @type str
 * @description Extracts a portion of the string.
 * @param start int - Starting index (0-based)
 * @param length int - Number of characters to extract
 * @returns str - The extracted substring
 * @example
 * s := "hello world";
 * sub := s.substr(0, 5);  // "hello"
 */

/**
 * @nog_method at
 * @type str
 * @description Returns the character at the specified index.
 * @param index int - The index (0-based)
 * @returns char - The character at that position
 * @example
 * s := "hello";
 * c := s.at(0);  // 'h'
 */

/**
 * @nog_method upper
 * @type str
 * @description Converts all characters to uppercase.
 * @returns str - The uppercase string
 * @example
 * s := "hello";
 * result := s.upper();  // "HELLO"
 */

/**
 * @nog_method lower
 * @type str
 * @description Converts all characters to lowercase.
 * @returns str - The lowercase string
 * @example
 * s := "HELLO";
 * result := s.lower();  // "hello"
 */

/**
 * @nog_method capitalize
 * @type str
 * @description Capitalizes the first character and lowercases the rest.
 * @returns str - The capitalized string
 * @example
 * s := "hello world";
 * result := s.capitalize();  // "Hello world"
 */

/**
 * @nog_method title
 * @type str
 * @description Capitalizes the first character of each word.
 * @returns str - The title-cased string
 * @example
 * s := "hello world";
 * result := s.title();  // "Hello World"
 */

/**
 * @nog_method trim
 * @type str
 * @description Removes whitespace from both ends of the string.
 * @returns str - The trimmed string
 * @example
 * s := "  hello  ";
 * result := s.trim();  // "hello"
 */

/**
 * @nog_method trim_left
 * @type str
 * @description Removes whitespace from the start of the string.
 * @returns str - The left-trimmed string
 * @example
 * s := "  hello  ";
 * result := s.trim_left();  // "hello  "
 */

/**
 * @nog_method trim_right
 * @type str
 * @description Removes whitespace from the end of the string.
 * @returns str - The right-trimmed string
 * @example
 * s := "  hello  ";
 * result := s.trim_right();  // "  hello"
 */

/**
 * @nog_method replace
 * @type str
 * @description Replaces the first occurrence of a substring.
 * @param old_str str - The substring to find
 * @param new_str str - The replacement string
 * @returns str - The string with the replacement
 * @example
 * s := "hello world";
 * result := s.replace("world", "there");  // "hello there"
 */

/**
 * @nog_method replace_all
 * @type str
 * @description Replaces all occurrences of a substring.
 * @param old_str str - The substring to find
 * @param new_str str - The replacement string
 * @returns str - The string with all replacements
 * @example
 * s := "hello world world";
 * result := s.replace_all("world", "there");  // "hello there there"
 */

/**
 * @nog_method reverse
 * @type str
 * @description Reverses the characters in the string.
 * @returns str - The reversed string
 * @example
 * s := "hello";
 * result := s.reverse();  // "olleh"
 */

/**
 * @nog_method repeat
 * @type str
 * @description Repeats the string a specified number of times.
 * @param count int - Number of times to repeat
 * @returns str - The repeated string
 * @example
 * s := "ab";
 * result := s.repeat(3);  // "ababab"
 */

/**
 * @nog_method split
 * @type str
 * @description Splits the string by a delimiter.
 * @param delimiter str - The delimiter to split on
 * @returns List<str> - List of substrings
 * @example
 * s := "hello world";
 * parts := s.split(" ");  // ["hello", "world"]
 */

/**
 * @nog_method split_lines
 * @type str
 * @description Splits the string by newlines.
 * @returns List<str> - List of lines
 * @example
 * s := "line1\nline2";
 * lines := s.split_lines();  // ["line1", "line2"]
 */

/**
 * @nog_method pad_left
 * @type str
 * @description Pads the string on the left to reach a specified width.
 * @param width int - The target width
 * @param fill str - The fill character (optional, default " ", first char used)
 * @returns str - The padded string
 * @example
 * s := "hi";
 * result := s.pad_left(5);       // "   hi"
 * result := s.pad_left(5, "0");  // "000hi"
 */

/**
 * @nog_method pad_right
 * @type str
 * @description Pads the string on the right to reach a specified width.
 * @param width int - The target width
 * @param fill str - The fill character (optional, default " ", first char used)
 * @returns str - The padded string
 * @example
 * s := "hi";
 * result := s.pad_right(5);       // "hi   "
 * result := s.pad_right(5, ".");  // "hi..."
 */

/**
 * @nog_method center
 * @type str
 * @description Centers the string within a specified width.
 * @param width int - The target width
 * @param fill str - The fill character (optional, default " ", first char used)
 * @returns str - The centered string
 * @example
 * s := "hi";
 * result := s.center(6);       // "  hi  "
 * result := s.center(6, "-");  // "--hi--"
 */

/**
 * @nog_method to_int
 * @type str
 * @description Parses the string as an integer.
 * @returns int - The parsed integer
 * @example
 * s := "42";
 * num := s.to_int();  // 42
 */

/**
 * @nog_method to_float
 * @type str
 * @description Parses the string as a floating-point number.
 * @returns f64 - The parsed float
 * @example
 * s := "3.14";
 * num := s.to_float();  // 3.14
 */

#include "strings.hpp"

#include <map>

namespace nog {

/**
 * Returns type information for built-in str methods.
 * Maps method names to their parameter types and return types.
 */
std::optional<StrMethodInfo> get_str_method_info(const std::string& method_name) {
    static const std::map<std::string, StrMethodInfo> str_methods = {
        // Existing methods
        {"length", {{}, "int"}},
        {"empty", {{}, "bool"}},
        {"contains", {{"str"}, "bool"}},
        {"starts_with", {{"str"}, "bool"}},
        {"ends_with", {{"str"}, "bool"}},
        {"find", {{"str"}, "int"}},
        {"substr", {{"int", "int"}, "str"}},
        {"at", {{"int"}, "char"}},

        // Case transformation methods
        {"upper", {{}, "str"}},
        {"lower", {{}, "str"}},
        {"capitalize", {{}, "str"}},
        {"title", {{}, "str"}},

        // Trimming methods
        {"trim", {{}, "str"}},
        {"trim_left", {{}, "str"}},
        {"trim_right", {{}, "str"}},

        // Replace methods
        {"replace", {{"str", "str"}, "str"}},
        {"replace_all", {{"str", "str"}, "str"}},

        // Reverse and repeat
        {"reverse", {{}, "str"}},
        {"repeat", {{"int"}, "str"}},

        // Split methods
        {"split", {{"str"}, "List<str>"}},
        {"split_lines", {{}, "List<str>"}},

        // Padding methods (with optional char parameter)
        {"pad_left", {{"int"}, "str"}},
        {"pad_right", {{"int"}, "str"}},
        {"center", {{"int"}, "str"}},

        // Conversion methods
        {"to_int", {{}, "int"}},
        {"to_float", {{}, "f64"}},
    };

    auto it = str_methods.find(method_name);

    if (it != str_methods.end()) {
        return it->second;
    }

    return std::nullopt;
}

}  // namespace nog
