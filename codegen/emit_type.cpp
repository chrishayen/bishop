/**
 * @file emit_type.cpp
 * @brief Type mapping for Bishop to C++ conversion.
 *
 * Maps Nog primitive types to their C++ equivalents.
 */

#include "codegen.hpp"
#include "common/type_utils.hpp"
#include <cassert>

using namespace std;

namespace codegen {

/**
 * Extracts the element type from a generic type string using proper bracket matching.
 * Delegates to the shared implementation in common/type_utils.hpp.
 */
string extract_element_type(const string& generic_type, const string& prefix) {
    return bishop::extract_element_type(generic_type, prefix);
}

/**
 * Maps a Bishop type name to its C++ equivalent for variable declarations.
 * Returns the type without reference suffix (no &).
 */
string map_type_for_decl(const string& t);

/**
 * Maps a Bishop type name to its C++ equivalent.
 * Returns the input unchanged if it's a user-defined type (struct).
 * Handles qualified types (module.Type -> module::Type).
 * Handles function types (fn(int, int) -> int -> std::function<int(int, int)>).
 * For Channel types, returns a reference type (Channel<T>&) suitable for function parameters.
 * Use map_type_for_decl for variable declarations.
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
        assert(!element_type.empty() && "malformed Channel type passed typechecker");
        return "bishop::rt::Channel<" + map_type(element_type) + ">&";
    }

    // Handle List<T> types: List<int> -> std::vector<int>
    if (t.rfind("List<", 0) == 0 && t.back() == '>') {
        string element_type = extract_element_type(t, "List<");
        assert(!element_type.empty() && "malformed List type passed typechecker");
        return "std::vector<" + map_type(element_type) + ">";
    }

    // Handle Map<K, V> types: Map<str, int> -> std::unordered_map<std::string, int>
    if (t.rfind("Map<", 0) == 0 && t.back() == '>') {
        // Extract key and value types by finding the comma at depth 1
        size_t start = 4;  // After "Map<"
        int depth = 1;
        size_t comma_pos = string::npos;

        for (size_t i = start; i < t.size(); i++) {
            if (t[i] == '<') depth++;
            else if (t[i] == '>') depth--;
            else if (t[i] == ',' && depth == 1 && comma_pos == string::npos) {
                comma_pos = i;
                break;
            }
        }

        assert(comma_pos != string::npos && "malformed Map type passed typechecker");
        string key_type = t.substr(start, comma_pos - start);
        string value_type = t.substr(comma_pos + 2, t.size() - comma_pos - 3);  // Skip ", " and ">"
        return "std::unordered_map<" + map_type(key_type) + ", " + map_type(value_type) + ">";
    }

    // Handle MapItem<K, V> types for iteration
    if (t.rfind("MapItem<", 0) == 0 && t.back() == '>') {
        // MapItem is a struct with key and value fields - we use an anonymous struct
        size_t start = 8;  // After "MapItem<"
        int depth = 1;
        size_t comma_pos = string::npos;

        for (size_t i = start; i < t.size(); i++) {
            if (t[i] == '<') depth++;
            else if (t[i] == '>') depth--;
            else if (t[i] == ',' && depth == 1 && comma_pos == string::npos) {
                comma_pos = i;
                break;
            }
        }

        assert(comma_pos != string::npos && "malformed MapItem type passed typechecker");
        string key_type = t.substr(start, comma_pos - start);
        string value_type = t.substr(comma_pos + 2, t.size() - comma_pos - 3);
        // Use auto for the struct since it's generated inline
        return "auto";
    }

    // Handle Pair<T> types: Pair<int> -> std::pair<int, int>
    if (t.rfind("Pair<", 0) == 0 && t.back() == '>') {
        string element_type = extract_element_type(t, "Pair<");
        assert(!element_type.empty() && "malformed Pair type passed typechecker");
        string cpp_type = map_type(element_type);
        return "std::pair<" + cpp_type + ", " + cpp_type + ">";
    }

    // Handle Tuple<T> types: Tuple<int> -> std::vector<int>
    // We use vector for homogeneous tuples since all elements are the same type
    if (t.rfind("Tuple<", 0) == 0 && t.back() == '>') {
        string element_type = extract_element_type(t, "Tuple<");
        assert(!element_type.empty() && "malformed Tuple type passed typechecker");
        return "std::vector<" + map_type(element_type) + ">";
    }

    // Handle Deque<T> types: Deque<int> -> std::deque<int>
    if (t.rfind("Deque<", 0) == 0 && t.back() == '>') {
        string element_type = extract_element_type(t, "Deque<");
        assert(!element_type.empty() && "malformed Deque type passed typechecker");
        return "std::deque<" + map_type(element_type) + ">";
    }

    // Handle Stack<T> types: Stack<int> -> std::stack<int>
    if (t.rfind("Stack<", 0) == 0 && t.back() == '>') {
        string element_type = extract_element_type(t, "Stack<");
        assert(!element_type.empty() && "malformed Stack type passed typechecker");
        return "std::stack<" + map_type(element_type) + ">";
    }

    // Handle Queue<T> types: Queue<int> -> std::queue<int>
    if (t.rfind("Queue<", 0) == 0 && t.back() == '>') {
        string element_type = extract_element_type(t, "Queue<");
        assert(!element_type.empty() && "malformed Queue type passed typechecker");
        return "std::queue<" + map_type(element_type) + ">";
    }

    // Handle PriorityQueue<T> types: PriorityQueue<int> -> bishop::MaxPriorityQueue<int>
    // Note: This maps to MaxPriorityQueue by default; min heap is handled at creation time
    if (t.rfind("PriorityQueue<", 0) == 0 && t.back() == '>') {
        string element_type = extract_element_type(t, "PriorityQueue<");
        assert(!element_type.empty() && "malformed PriorityQueue type passed typechecker");
        return "bishop::PriorityQueueBase<" + map_type(element_type) + ">";
    }

    // Handle Set<T> types: Set<int> -> std::unordered_set<int>
    if (t.rfind("Set<", 0) == 0 && t.back() == '>') {
        string element_type = extract_element_type(t, "Set<");
        assert(!element_type.empty() && "malformed Set type passed typechecker");
        return "std::unordered_set<" + map_type(element_type) + ">";
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
        string module_name = t.substr(0, dot_pos);
        string type_name = t.substr(dot_pos + 1);

        // Map module names that conflict with C/C++ identifiers
        if (module_name == "time") {
            module_name = "bishop_time";
        } else if (module_name == "log") {
            module_name = "bishop_log";
        }

        return module_name + "::" + type_name;
    }

    return t;
}

/**
 * Maps a Bishop type name to its C++ equivalent for variable declarations.
 * Unlike map_type(), this strips the reference suffix from Channel types
 * since variable declarations cannot use reference types.
 */
string map_type_for_decl(const string& t) {
    string mapped = map_type(t);

    // Strip trailing '&' from Channel types for variable declarations
    if (!mapped.empty() && mapped.back() == '&') {
        return mapped.substr(0, mapped.size() - 1);
    }

    return mapped;
}

} // namespace codegen
