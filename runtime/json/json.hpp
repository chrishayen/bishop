/**
 * @file json.hpp
 * @brief Bishop JSON runtime library.
 *
 * Provides JSON parsing and serialization for Bishop programs.
 * This header is included when programs import the json module.
 *
 * Uses a simple recursive descent parser for JSON.
 * JSON values are represented using std::variant.
 */

#pragma once

#include <bishop/std.hpp>
#include <string>
#include <vector>
#include <map>
#include <variant>
#include <sstream>
#include <stdexcept>
#include <cctype>
#include <cstdlib>
#include <iomanip>
#include <memory>

namespace bishop {

/**
 * Simple Result type for fallible operations in json module.
 * Holds either a value of type T or an error.
 * Uses optional<T> for the value and shared_ptr<Error> for errors
 * to be compatible with the code generator.
 */
struct JsonError {
    std::string message;
    JsonError(const std::string& msg) : message(msg) {}
};

template<typename T>
class Result {
private:
    std::optional<T> val_;
    std::shared_ptr<JsonError> err_;

public:
    Result() : val_(std::nullopt), err_(nullptr) {}
    Result(T v) : val_(std::move(v)), err_(nullptr) {}
    Result(std::shared_ptr<JsonError> e) : val_(std::nullopt), err_(std::move(e)) {}

    static Result<T> ok(T v) {
        return Result<T>(std::move(v));
    }

    static Result<T> err(const std::string& msg) {
        return Result<T>(std::make_shared<JsonError>(msg));
    }

    static Result<T> err(std::shared_ptr<JsonError> e) {
        return Result<T>(std::move(e));
    }

    bool is_ok() const { return val_.has_value(); }
    bool is_error() const { return !is_ok(); }

    T& value() { return *val_; }
    const T& value() const { return *val_; }

    std::shared_ptr<JsonError> error() const { return err_; }

    explicit operator bool() const { return is_ok(); }
};

}  // namespace bishop

namespace json {

// Forward declaration
class Json;

// JSON value types
using JsonNull = std::monostate;
using JsonBool = bool;
using JsonInt = int64_t;
using JsonFloat = double;
using JsonString = std::string;
using JsonArray = std::vector<std::shared_ptr<Json>>;
using JsonObject = std::map<std::string, std::shared_ptr<Json>>;

using JsonValue = std::variant<JsonNull, JsonBool, JsonInt, JsonFloat, JsonString, JsonArray, JsonObject>;

/**
 * JSON value class.
 * Wraps a variant holding all possible JSON types.
 */
class Json {
public:
    JsonValue value;

    Json() : value(JsonNull{}) {}
    explicit Json(JsonNull) : value(JsonNull{}) {}
    explicit Json(bool b) : value(b) {}
    explicit Json(int64_t i) : value(i) {}
    explicit Json(int i) : value(static_cast<int64_t>(i)) {}
    explicit Json(long long i) : value(static_cast<int64_t>(i)) {}
    explicit Json(double d) : value(d) {}
    explicit Json(const std::string& s) : value(s) {}
    explicit Json(std::string&& s) : value(std::move(s)) {}
    explicit Json(const char* s) : value(std::string(s)) {}
    explicit Json(JsonArray arr) : value(std::move(arr)) {}
    explicit Json(JsonObject obj) : value(std::move(obj)) {}

    // Type checking methods
    bool is_null() const { return std::holds_alternative<JsonNull>(value); }
    bool is_bool() const { return std::holds_alternative<JsonBool>(value); }
    bool is_int() const { return std::holds_alternative<JsonInt>(value); }
    bool is_float() const { return std::holds_alternative<JsonFloat>(value); }
    bool is_str() const { return std::holds_alternative<JsonString>(value); }
    bool is_list() const { return std::holds_alternative<JsonArray>(value); }
    bool is_object() const { return std::holds_alternative<JsonObject>(value); }

    // Value extraction with error handling
    bishop::Result<std::string> as_str() const {
        if (!is_str()) {
            return bishop::Result<std::string>::err("TypeError: expected string");
        }
        return bishop::Result<std::string>::ok(std::get<JsonString>(value));
    }

    bishop::Result<int64_t> as_int() const {
        if (is_int()) {
            return bishop::Result<int64_t>::ok(std::get<JsonInt>(value));
        }
        if (is_float()) {
            return bishop::Result<int64_t>::ok(static_cast<int64_t>(std::get<JsonFloat>(value)));
        }
        return bishop::Result<int64_t>::err("TypeError: expected integer");
    }

    bishop::Result<double> as_float() const {
        if (is_float()) {
            return bishop::Result<double>::ok(std::get<JsonFloat>(value));
        }
        if (is_int()) {
            return bishop::Result<double>::ok(static_cast<double>(std::get<JsonInt>(value)));
        }
        return bishop::Result<double>::err("TypeError: expected float");
    }

    bishop::Result<bool> as_bool() const {
        if (!is_bool()) {
            return bishop::Result<bool>::err("TypeError: expected boolean");
        }
        return bishop::Result<bool>::ok(std::get<JsonBool>(value));
    }

    // Get by key (for objects) or index (for arrays)
    bishop::Result<Json> get(const std::string& key) const {
        if (is_object()) {
            const auto& obj = std::get<JsonObject>(value);
            auto it = obj.find(key);
            if (it == obj.end()) {
                return bishop::Result<Json>::err("KeyError: key not found: " + key);
            }
            return bishop::Result<Json>::ok(*it->second);
        }
        if (is_list()) {
            // Try to parse key as index
            try {
                size_t idx = std::stoull(key);
                const auto& arr = std::get<JsonArray>(value);
                if (idx >= arr.size()) {
                    return bishop::Result<Json>::err("IndexError: index out of bounds: " + key);
                }
                return bishop::Result<Json>::ok(*arr[idx]);
            } catch (...) {
                return bishop::Result<Json>::err("TypeError: array index must be an integer");
            }
        }
        return bishop::Result<Json>::err("TypeError: cannot index into non-object/non-array");
    }

    // Typed getters
    bishop::Result<std::string> get_str(const std::string& key) const {
        auto result = get(key);
        if (!result.is_ok()) return bishop::Result<std::string>::err(result.error());
        return result.value().as_str();
    }

    bishop::Result<int64_t> get_int(const std::string& key) const {
        auto result = get(key);
        if (!result.is_ok()) return bishop::Result<int64_t>::err(result.error());
        return result.value().as_int();
    }

    bishop::Result<double> get_float(const std::string& key) const {
        auto result = get(key);
        if (!result.is_ok()) return bishop::Result<double>::err(result.error());
        return result.value().as_float();
    }

    bishop::Result<bool> get_bool(const std::string& key) const {
        auto result = get(key);
        if (!result.is_ok()) return bishop::Result<bool>::err(result.error());
        return result.value().as_bool();
    }

    bishop::Result<Json> get_list(const std::string& key) const {
        auto result = get(key);
        if (!result.is_ok()) return result;
        if (!result.value().is_list()) {
            return bishop::Result<Json>::err("TypeError: expected array");
        }
        return result;
    }

    bishop::Result<Json> get_object(const std::string& key) const {
        auto result = get(key);
        if (!result.is_ok()) return result;
        if (!result.value().is_object()) {
            return bishop::Result<Json>::err("TypeError: expected object");
        }
        return result;
    }

    // Path access with dot notation
    bishop::Result<Json> path(const std::string& path_str) const {
        Json current = *this;
        std::string segment;
        std::istringstream stream(path_str);

        while (std::getline(stream, segment, '.')) {
            if (segment.empty()) continue;
            auto result = current.get(segment);
            if (!result.is_ok()) return result;
            current = result.value();
        }

        return bishop::Result<Json>::ok(current);
    }

    // Setters for objects
    void set(const std::string& key, const Json& val) {
        if (!is_object()) return;
        auto& obj = std::get<JsonObject>(value);
        obj[key] = std::make_shared<Json>(val);
    }

    void set_str(const std::string& key, const std::string& val) {
        set(key, Json(val));
    }

    void set_int(const std::string& key, int64_t val) {
        set(key, Json(val));
    }

    void set_float(const std::string& key, double val) {
        set(key, Json(val));
    }

    void set_bool(const std::string& key, bool val) {
        set(key, Json(val));
    }

    void set_null(const std::string& key) {
        set(key, Json(JsonNull{}));
    }

    // Push for arrays
    void push(const Json& val) {
        if (!is_list()) return;
        auto& arr = std::get<JsonArray>(value);
        arr.push_back(std::make_shared<Json>(val));
    }

    void push_str(const std::string& val) {
        push(Json(val));
    }

    void push_int(int64_t val) {
        push(Json(val));
    }

    void push_float(double val) {
        push(Json(val));
    }

    void push_bool(bool val) {
        push(Json(val));
    }

    void push_null() {
        push(Json(JsonNull{}));
    }

    // Utility methods
    int64_t length() const {
        if (is_list()) {
            return static_cast<int64_t>(std::get<JsonArray>(value).size());
        }
        if (is_object()) {
            return static_cast<int64_t>(std::get<JsonObject>(value).size());
        }
        return 0;
    }

    /**
     * Returns the keys of a JSON object as a vector.
     * Returns an empty vector if not an object.
     */
    std::vector<std::string> keys() const {
        std::vector<std::string> result;
        if (!is_object()) return result;
        const auto& obj = std::get<JsonObject>(value);
        for (const auto& [key, _] : obj) {
            result.push_back(key);
        }
        return result;
    }

    bool has(const std::string& key) const {
        if (!is_object()) return false;
        const auto& obj = std::get<JsonObject>(value);
        return obj.find(key) != obj.end();
    }

    void remove(const std::string& key) {
        if (!is_object()) return;
        auto& obj = std::get<JsonObject>(value);
        obj.erase(key);
    }
};

// JSON Parser
class JsonParser {
public:
    explicit JsonParser(const std::string& input) : input_(input), pos_(0) {}

    bishop::Result<Json> parse() {
        skip_whitespace();
        if (pos_ >= input_.size()) {
            return bishop::Result<Json>::err("ParseError: empty input");
        }
        auto result = parse_value();
        if (!result.is_ok()) return result;
        skip_whitespace();
        if (pos_ < input_.size()) {
            return bishop::Result<Json>::err("ParseError: unexpected characters after JSON");
        }
        return result;
    }

private:
    const std::string& input_;
    size_t pos_;

    char peek() const {
        return pos_ < input_.size() ? input_[pos_] : '\0';
    }

    char consume() {
        return pos_ < input_.size() ? input_[pos_++] : '\0';
    }

    void skip_whitespace() {
        while (pos_ < input_.size() && std::isspace(static_cast<unsigned char>(input_[pos_]))) {
            ++pos_;
        }
    }

    bishop::Result<Json> parse_value() {
        skip_whitespace();
        char c = peek();

        if (c == '"') return parse_string();
        if (c == '{') return parse_object();
        if (c == '[') return parse_array();
        if (c == 't' || c == 'f') return parse_bool();
        if (c == 'n') return parse_null();
        if (c == '-' || std::isdigit(static_cast<unsigned char>(c))) return parse_number();

        return bishop::Result<Json>::err("ParseError: unexpected character: " + std::string(1, c));
    }

    bishop::Result<Json> parse_string() {
        if (consume() != '"') {
            return bishop::Result<Json>::err("ParseError: expected '\"'");
        }

        std::string result;
        while (pos_ < input_.size() && peek() != '"') {
            char c = consume();
            if (c == '\\') {
                if (pos_ >= input_.size()) {
                    return bishop::Result<Json>::err("ParseError: unexpected end of string");
                }
                char escaped = consume();
                switch (escaped) {
                    case '"': result += '"'; break;
                    case '\\': result += '\\'; break;
                    case '/': result += '/'; break;
                    case 'b': result += '\b'; break;
                    case 'f': result += '\f'; break;
                    case 'n': result += '\n'; break;
                    case 'r': result += '\r'; break;
                    case 't': result += '\t'; break;
                    case 'u': {
                        // Unicode escape: \uXXXX
                        if (pos_ + 4 > input_.size()) {
                            return bishop::Result<Json>::err("ParseError: invalid unicode escape");
                        }
                        std::string hex = input_.substr(pos_, 4);
                        pos_ += 4;
                        try {
                            unsigned int codepoint = std::stoul(hex, nullptr, 16);
                            if (codepoint < 0x80) {
                                result += static_cast<char>(codepoint);
                            } else if (codepoint < 0x800) {
                                result += static_cast<char>(0xC0 | (codepoint >> 6));
                                result += static_cast<char>(0x80 | (codepoint & 0x3F));
                            } else {
                                result += static_cast<char>(0xE0 | (codepoint >> 12));
                                result += static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
                                result += static_cast<char>(0x80 | (codepoint & 0x3F));
                            }
                        } catch (...) {
                            return bishop::Result<Json>::err("ParseError: invalid unicode escape");
                        }
                        break;
                    }
                    default:
                        return bishop::Result<Json>::err("ParseError: invalid escape sequence");
                }
            } else {
                result += c;
            }
        }

        if (consume() != '"') {
            return bishop::Result<Json>::err("ParseError: unterminated string");
        }

        return bishop::Result<Json>::ok(Json(result));
    }

    bishop::Result<Json> parse_number() {
        size_t start = pos_;
        bool is_float = false;

        if (peek() == '-') consume();

        if (peek() == '0') {
            consume();
        } else if (std::isdigit(static_cast<unsigned char>(peek()))) {
            while (std::isdigit(static_cast<unsigned char>(peek()))) consume();
        } else {
            return bishop::Result<Json>::err("ParseError: invalid number");
        }

        if (peek() == '.') {
            is_float = true;
            consume();
            if (!std::isdigit(static_cast<unsigned char>(peek()))) {
                return bishop::Result<Json>::err("ParseError: invalid number");
            }
            while (std::isdigit(static_cast<unsigned char>(peek()))) consume();
        }

        if (peek() == 'e' || peek() == 'E') {
            is_float = true;
            consume();
            if (peek() == '+' || peek() == '-') consume();
            if (!std::isdigit(static_cast<unsigned char>(peek()))) {
                return bishop::Result<Json>::err("ParseError: invalid number");
            }
            while (std::isdigit(static_cast<unsigned char>(peek()))) consume();
        }

        std::string num_str = input_.substr(start, pos_ - start);
        try {
            if (is_float) {
                return bishop::Result<Json>::ok(Json(std::stod(num_str)));
            } else {
                return bishop::Result<Json>::ok(Json(std::stoll(num_str)));
            }
        } catch (...) {
            return bishop::Result<Json>::err("ParseError: invalid number: " + num_str);
        }
    }

    bishop::Result<Json> parse_bool() {
        if (input_.substr(pos_, 4) == "true") {
            pos_ += 4;
            return bishop::Result<Json>::ok(Json(true));
        }
        if (input_.substr(pos_, 5) == "false") {
            pos_ += 5;
            return bishop::Result<Json>::ok(Json(false));
        }
        return bishop::Result<Json>::err("ParseError: expected 'true' or 'false'");
    }

    bishop::Result<Json> parse_null() {
        if (input_.substr(pos_, 4) == "null") {
            pos_ += 4;
            return bishop::Result<Json>::ok(Json(JsonNull{}));
        }
        return bishop::Result<Json>::err("ParseError: expected 'null'");
    }

    bishop::Result<Json> parse_array() {
        if (consume() != '[') {
            return bishop::Result<Json>::err("ParseError: expected '['");
        }

        JsonArray arr;
        skip_whitespace();

        if (peek() == ']') {
            consume();
            return bishop::Result<Json>::ok(Json(std::move(arr)));
        }

        while (true) {
            auto elem = parse_value();
            if (!elem.is_ok()) return elem;
            arr.push_back(std::make_shared<Json>(elem.value()));

            skip_whitespace();
            if (peek() == ']') {
                consume();
                break;
            }
            if (peek() != ',') {
                return bishop::Result<Json>::err("ParseError: expected ',' or ']'");
            }
            consume();
        }

        return bishop::Result<Json>::ok(Json(std::move(arr)));
    }

    bishop::Result<Json> parse_object() {
        if (consume() != '{') {
            return bishop::Result<Json>::err("ParseError: expected '{'");
        }

        JsonObject obj;
        skip_whitespace();

        if (peek() == '}') {
            consume();
            return bishop::Result<Json>::ok(Json(std::move(obj)));
        }

        while (true) {
            skip_whitespace();
            if (peek() != '"') {
                return bishop::Result<Json>::err("ParseError: expected string key");
            }

            auto key_result = parse_string();
            if (!key_result.is_ok()) return key_result;
            std::string key = std::get<JsonString>(key_result.value().value);

            skip_whitespace();
            if (consume() != ':') {
                return bishop::Result<Json>::err("ParseError: expected ':'");
            }

            auto val = parse_value();
            if (!val.is_ok()) return val;
            obj[key] = std::make_shared<Json>(val.value());

            skip_whitespace();
            if (peek() == '}') {
                consume();
                break;
            }
            if (peek() != ',') {
                return bishop::Result<Json>::err("ParseError: expected ',' or '}'");
            }
            consume();
        }

        return bishop::Result<Json>::ok(Json(std::move(obj)));
    }
};

// Stringify functions
inline std::string escape_string(const std::string& s) {
    std::string result;
    result.reserve(s.size() + 2);
    result += '"';
    for (char c : s) {
        switch (c) {
            case '"': result += "\\\""; break;
            case '\\': result += "\\\\"; break;
            case '\b': result += "\\b"; break;
            case '\f': result += "\\f"; break;
            case '\n': result += "\\n"; break;
            case '\r': result += "\\r"; break;
            case '\t': result += "\\t"; break;
            default:
                if (static_cast<unsigned char>(c) < 0x20) {
                    std::ostringstream oss;
                    oss << "\\u" << std::hex << std::setfill('0') << std::setw(4)
                        << static_cast<int>(static_cast<unsigned char>(c));
                    result += oss.str();
                } else {
                    result += c;
                }
        }
    }
    result += '"';
    return result;
}

inline std::string stringify_impl(const Json& j, bool pretty, int indent_level);

inline std::string stringify_value(const JsonValue& v, bool pretty, int indent_level) {
    return std::visit([pretty, indent_level](const auto& val) -> std::string {
        using T = std::decay_t<decltype(val)>;
        if constexpr (std::is_same_v<T, JsonNull>) {
            return "null";
        } else if constexpr (std::is_same_v<T, JsonBool>) {
            return val ? "true" : "false";
        } else if constexpr (std::is_same_v<T, JsonInt>) {
            return std::to_string(val);
        } else if constexpr (std::is_same_v<T, JsonFloat>) {
            std::ostringstream oss;
            oss << std::setprecision(17) << val;
            std::string s = oss.str();
            // Ensure it looks like a float
            if (s.find('.') == std::string::npos && s.find('e') == std::string::npos) {
                s += ".0";
            }
            return s;
        } else if constexpr (std::is_same_v<T, JsonString>) {
            return escape_string(val);
        } else if constexpr (std::is_same_v<T, JsonArray>) {
            if (val.empty()) return "[]";
            std::string result = "[";
            std::string indent(indent_level * 2, ' ');
            std::string inner_indent((indent_level + 1) * 2, ' ');
            for (size_t i = 0; i < val.size(); ++i) {
                if (pretty) {
                    result += "\n" + inner_indent;
                }
                result += stringify_impl(*val[i], pretty, indent_level + 1);
                if (i + 1 < val.size()) result += ",";
            }
            if (pretty) {
                result += "\n" + indent;
            }
            result += "]";
            return result;
        } else if constexpr (std::is_same_v<T, JsonObject>) {
            if (val.empty()) return "{}";
            std::string result = "{";
            std::string indent(indent_level * 2, ' ');
            std::string inner_indent((indent_level + 1) * 2, ' ');
            size_t i = 0;
            for (const auto& [key, value] : val) {
                if (pretty) {
                    result += "\n" + inner_indent;
                }
                result += escape_string(key);
                result += pretty ? ": " : ":";
                result += stringify_impl(*value, pretty, indent_level + 1);
                if (i + 1 < val.size()) result += ",";
                ++i;
            }
            if (pretty) {
                result += "\n" + indent;
            }
            result += "}";
            return result;
        }
        return "null";
    }, v);
}

inline std::string stringify_impl(const Json& j, bool pretty, int indent_level) {
    return stringify_value(j.value, pretty, indent_level);
}

// Public API functions
inline bishop::Result<Json> parse(const std::string& text) {
    JsonParser parser(text);
    return parser.parse();
}

inline Json object() {
    return Json(JsonObject{});
}

inline Json array() {
    return Json(JsonArray{});
}

inline std::string stringify(const Json& value) {
    return stringify_impl(value, false, 0);
}

inline std::string stringify_pretty(const Json& value) {
    return stringify_impl(value, true, 0);
}

}  // namespace json
