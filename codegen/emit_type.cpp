/**
 * @file emit_type.cpp
 * @brief Type mapping for Bishop to C++ conversion.
 *
 * Maps Nog primitive types to their C++ equivalents.
 */

#include "codegen.hpp"

using namespace std;

namespace codegen {

/**
 * Extracts the element type from a generic type string using proper bracket matching.
 *
 * For example:
 *   - extract_element_type("List<int>", "List<") returns "int"
 *   - extract_element_type("Pair<List<int>>", "Pair<") returns "List<int>"
 *   - extract_element_type("Channel<Pair<str>>", "Channel<") returns "Pair<str>"
 *
 * @param generic_type The full generic type string (e.g., "Pair<List<int>>")
 * @param prefix The expected prefix including the opening bracket (e.g., "Pair<")
 * @return The element type, or empty string if the type doesn't match the prefix
 */
string extract_element_type(const string& generic_type, const string& prefix) {
    if (generic_type.rfind(prefix, 0) != 0 || generic_type.back() != '>') {
        return "";
    }

    size_t start = prefix.length();
    int depth = 1;

    for (size_t i = start; i < generic_type.size(); i++) {
        if (generic_type[i] == '<') {
            depth++;
        } else if (generic_type[i] == '>') {
            depth--;

            if (depth == 0) {
                return generic_type.substr(start, i - start);
            }
        }
    }

    return "";
}

/**
 * Maps a Bishop type name to its C++ equivalent.
 * Returns the input unchanged if it's a user-defined type (struct).
 * Handles qualified types (module.Type -> module::Type).
 * Handles function types (fn(int, int) -> int -> std::function<int(int, int)>).
 */
string map_type(const string& t) {
    if (t == "int") return "int";
    if (t == "str") return "std::string";
    if (t == "bool") return "bool";
    if (t == "f32") return "float";
    if (t == "f64") return "double";
    if (t == "u32") return "uint32_t";
    if (t == "u64") return "uint64_t";
    if (t == "cint") return "int";
    if (t == "cstr") return "const char*";
    if (t == "void") return "void";
    if (t.empty()) return "void";

    // Handle Channel<T> types: Channel<int> -> bishop::rt::Channel<int>&
    if (t.rfind("Channel<", 0) == 0 && t.back() == '>') {
        string element_type = extract_element_type(t, "Channel<");
        return "bishop::rt::Channel<" + map_type(element_type) + ">&";
    }

    // Handle List<T> types: List<int> -> std::vector<int>
    if (t.rfind("List<", 0) == 0 && t.back() == '>') {
        string element_type = extract_element_type(t, "List<");
        return "std::vector<" + map_type(element_type) + ">";
    }

    // Handle Pair<T> types: Pair<int> -> std::pair<int, int>
    if (t.rfind("Pair<", 0) == 0 && t.back() == '>') {
        string element_type = extract_element_type(t, "Pair<");
        string cpp_type = map_type(element_type);
        return "std::pair<" + cpp_type + ", " + cpp_type + ">";
    }

    // Handle Tuple<T> types: Tuple<int> -> std::vector<int>
    // We use vector for homogeneous tuples since all elements are the same type
    if (t.rfind("Tuple<", 0) == 0 && t.back() == '>') {
        string element_type = extract_element_type(t, "Tuple<");
        return "std::vector<" + map_type(element_type) + ">";
    }

    // Handle function types: fn(int, str) -> bool -> std::function<bool(int, std::string)>
    if (t.rfind("fn(", 0) == 0) {
        // Find the closing paren and extract param types
        size_t paren_start = 3;  // After "fn("
        int paren_depth = 1;
        size_t paren_end = paren_start;

        while (paren_end < t.size() && paren_depth > 0) {
            if (t[paren_end] == '(') paren_depth++;
            else if (t[paren_end] == ')') paren_depth--;
            paren_end++;
        }

        paren_end--;  // Point to the closing paren
        string params_str = t.substr(paren_start, paren_end - paren_start);

        // Parse the return type (after " -> ")
        string ret_type = "void";
        size_t arrow_pos = t.find(" -> ", paren_end);

        if (arrow_pos != string::npos) {
            ret_type = map_type(t.substr(arrow_pos + 4));
        }

        // Parse parameter types (comma-separated, but handle nested fn types)
        string cpp_params;
        size_t pos = 0;
        int depth = 0;
        size_t param_start = 0;

        while (pos <= params_str.size()) {
            char c = (pos < params_str.size()) ? params_str[pos] : ',';

            if (c == '(') depth++;
            else if (c == ')') depth--;
            else if ((c == ',' || pos == params_str.size()) && depth == 0) {
                string param = params_str.substr(param_start, pos - param_start);

                // Trim whitespace
                size_t start = param.find_first_not_of(" ");
                size_t end = param.find_last_not_of(" ");

                if (start != string::npos) {
                    param = param.substr(start, end - start + 1);

                    if (!cpp_params.empty()) {
                        cpp_params += ", ";
                    }

                    cpp_params += map_type(param);
                }

                param_start = pos + 1;
            }

            pos++;
        }

        return "std::function<" + ret_type + "(" + cpp_params + ")>";
    }

    // Handle qualified types: module.Type -> module::Type
    size_t dot_pos = t.find('.');

    if (dot_pos != string::npos) {
        return t.substr(0, dot_pos) + "::" + t.substr(dot_pos + 1);
    }

    return t;
}

} // namespace codegen
