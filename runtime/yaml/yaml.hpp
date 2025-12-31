/**
 * @file yaml.hpp
 * @brief Bishop YAML runtime library.
 */

#ifndef BISHOP_YAML_HPP
#define BISHOP_YAML_HPP

#pragma once

#include <bishop/std.hpp>
#include <string>
#include <vector>
#include <map>
#include <variant>
#include <sstream>
#include <cctype>
#include <cstdlib>
#include <memory>
#include <algorithm>

namespace bishop {

struct YamlError {
    std::string message;
    YamlError(const std::string& msg) : message(msg) {}
};

template<typename T>
class YamlResult {
private:
    std::optional<T> val_;
    std::shared_ptr<YamlError> err_;

public:
    YamlResult() : val_(std::nullopt), err_(nullptr) {}
    YamlResult(T v) : val_(std::move(v)), err_(nullptr) {}
    YamlResult(std::shared_ptr<YamlError> e) : val_(std::nullopt), err_(std::move(e)) {}

    static YamlResult<T> ok(T v) { return YamlResult<T>(std::move(v)); }
    static YamlResult<T> error(const std::string& msg) {
        return YamlResult<T>(std::make_shared<YamlError>(msg));
    }

    bool is_error() const { return err_ != nullptr; }
    bool is_ok() const { return !is_error(); }
    T value() const { return *val_; }
    T& value_ref() { return *val_; }
    std::string error_message() const { return err_ ? err_->message : ""; }
};

class Yaml;
using YamlValue = std::variant<
    std::nullptr_t, bool, int64_t, double, std::string,
    std::vector<Yaml>, std::map<std::string, Yaml>
>;

class Yaml {
private:
    YamlValue value_;

public:
    Yaml() : value_(nullptr) {}
    explicit Yaml(std::nullptr_t) : value_(nullptr) {}
    explicit Yaml(bool b) : value_(b) {}
    explicit Yaml(int64_t i) : value_(i) {}
    explicit Yaml(int i) : value_(static_cast<int64_t>(i)) {}
    explicit Yaml(double d) : value_(d) {}
    explicit Yaml(const std::string& s) : value_(s) {}
    explicit Yaml(std::string&& s) : value_(std::move(s)) {}
    explicit Yaml(const char* s) : value_(std::string(s)) {}
    explicit Yaml(std::vector<Yaml> arr) : value_(std::move(arr)) {}
    explicit Yaml(std::map<std::string, Yaml> obj) : value_(std::move(obj)) {}

    static Yaml make_object() { return Yaml(std::map<std::string, Yaml>{}); }
    static Yaml make_array() { return Yaml(std::vector<Yaml>{}); }

    bool is_null() const { return std::holds_alternative<std::nullptr_t>(value_); }
    bool is_bool() const { return std::holds_alternative<bool>(value_); }
    bool is_int() const { return std::holds_alternative<int64_t>(value_); }
    bool is_float() const { return std::holds_alternative<double>(value_); }
    bool is_str() const { return std::holds_alternative<std::string>(value_); }
    bool is_list() const { return std::holds_alternative<std::vector<Yaml>>(value_); }
    bool is_object() const { return std::holds_alternative<std::map<std::string, Yaml>>(value_); }

    YamlResult<bool> as_bool() const {
        if (!is_bool()) return YamlResult<bool>::error("not a bool");
        return YamlResult<bool>::ok(std::get<bool>(value_));
    }

    YamlResult<int64_t> as_int() const {
        if (!is_int()) return YamlResult<int64_t>::error("not an int");
        return YamlResult<int64_t>::ok(std::get<int64_t>(value_));
    }

    YamlResult<double> as_float() const {
        if (is_int()) return YamlResult<double>::ok(static_cast<double>(std::get<int64_t>(value_)));
        if (!is_float()) return YamlResult<double>::error("not a float");
        return YamlResult<double>::ok(std::get<double>(value_));
    }

    YamlResult<std::string> as_str() const {
        if (!is_str()) return YamlResult<std::string>::error("not a string");
        return YamlResult<std::string>::ok(std::get<std::string>(value_));
    }

    int length() const {
        if (is_list()) return static_cast<int>(std::get<std::vector<Yaml>>(value_).size());
        if (is_object()) return static_cast<int>(std::get<std::map<std::string, Yaml>>(value_).size());
        if (is_str()) return static_cast<int>(std::get<std::string>(value_).size());
        return 0;
    }

    bool has(const std::string& key) const {
        if (!is_object()) return false;
        const auto& obj = std::get<std::map<std::string, Yaml>>(value_);
        return obj.find(key) != obj.end();
    }

    YamlResult<Yaml> get(const std::string& key) const {
        if (is_object()) {
            const auto& obj = std::get<std::map<std::string, Yaml>>(value_);
            auto it = obj.find(key);
            if (it == obj.end()) return YamlResult<Yaml>::error("key not found: " + key);
            return YamlResult<Yaml>::ok(it->second);
        }
        if (is_list()) {
            try {
                size_t idx = std::stoull(key);
                const auto& arr = std::get<std::vector<Yaml>>(value_);
                if (idx >= arr.size()) return YamlResult<Yaml>::error("index out of bounds");
                return YamlResult<Yaml>::ok(arr[idx]);
            } catch (...) {
                return YamlResult<Yaml>::error("invalid index");
            }
        }
        return YamlResult<Yaml>::error("not an object or list");
    }

    YamlResult<std::string> get_str(const std::string& key) const {
        auto result = get(key);
        if (result.is_error()) return YamlResult<std::string>::error(result.error_message());
        return result.value().as_str();
    }

    YamlResult<int64_t> get_int(const std::string& key) const {
        auto result = get(key);
        if (result.is_error()) return YamlResult<int64_t>::error(result.error_message());
        return result.value().as_int();
    }

    YamlResult<bool> get_bool(const std::string& key) const {
        auto result = get(key);
        if (result.is_error()) return YamlResult<bool>::error(result.error_message());
        return result.value().as_bool();
    }

    YamlResult<Yaml> get_list(const std::string& key) const {
        auto result = get(key);
        if (result.is_error()) return result;
        if (!result.value().is_list()) return YamlResult<Yaml>::error("not a list");
        return result;
    }

    YamlResult<Yaml> get_object(const std::string& key) const {
        auto result = get(key);
        if (result.is_error()) return result;
        if (!result.value().is_object()) return YamlResult<Yaml>::error("not an object");
        return result;
    }

    YamlResult<Yaml> path(const std::string& dotpath) const {
        Yaml current = *this;
        std::istringstream ss(dotpath);
        std::string segment;
        while (std::getline(ss, segment, '.')) {
            auto result = current.get(segment);
            if (result.is_error()) return result;
            current = result.value();
        }
        return YamlResult<Yaml>::ok(current);
    }

    void set(const std::string& key, Yaml val) {
        if (!is_object()) value_ = std::map<std::string, Yaml>{};
        std::get<std::map<std::string, Yaml>>(value_)[key] = std::move(val);
    }

    void set_str(const std::string& key, const std::string& val) { set(key, Yaml(val)); }
    void set_int(const std::string& key, int64_t val) { set(key, Yaml(val)); }
    void set_bool(const std::string& key, bool val) { set(key, Yaml(val)); }
    void set_float(const std::string& key, double val) { set(key, Yaml(val)); }

    void push(Yaml val) {
        if (!is_list()) value_ = std::vector<Yaml>{};
        std::get<std::vector<Yaml>>(value_).push_back(std::move(val));
    }

    void push_str(const std::string& val) { push(Yaml(val)); }
    void push_int(int64_t val) { push(Yaml(val)); }
    void push_bool(bool val) { push(Yaml(val)); }

    void remove(const std::string& key) {
        if (!is_object()) return;
        std::get<std::map<std::string, Yaml>>(value_).erase(key);
    }

    std::string keys() const {
        if (!is_object()) return "";
        std::string result;
        const auto& obj = std::get<std::map<std::string, Yaml>>(value_);
        for (const auto& [key, _] : obj) {
            if (!result.empty()) result += "\n";
            result += key;
        }
        return result;
    }

    const YamlValue& raw() const { return value_; }
};

class YamlParser {
private:
    std::string input_;
    size_t pos_ = 0;

    char peek() const { return pos_ < input_.size() ? input_[pos_] : '\0'; }
    char get() { return pos_ < input_.size() ? input_[pos_++] : '\0'; }
    void skip_ws_inline() { while (peek() == ' ' || peek() == '\t') get(); }

    void skip_comment() {
        if (peek() == '#') while (peek() && peek() != '\n') get();
    }

    void skip_ws_comments() {
        while (true) {
            while (std::isspace(static_cast<unsigned char>(peek()))) get();
            if (peek() == '#') skip_comment();
            else break;
        }
    }

    // Skip only newlines (not spaces), used when indentation is significant
    void skip_newlines() {
        while (peek() == '\n' || peek() == '\r') get();
    }

    int count_indent() {
        int indent = 0;
        size_t saved = pos_;
        while (peek() == ' ') { get(); indent++; }
        if (peek() == '\n' || peek() == '#' || peek() == '\0') return -1;
        pos_ = saved;
        return indent;
    }

    std::string parse_key() {
        std::string key;
        while (peek() && peek() != ':' && peek() != '\n' && peek() != '#') key += get();
        while (!key.empty() && (key.back() == ' ' || key.back() == '\t')) key.pop_back();
        return key;
    }

    Yaml parse_scalar(const std::string& val) {
        std::string s = val;
        while (!s.empty() && std::isspace(static_cast<unsigned char>(s.front()))) s.erase(0, 1);
        while (!s.empty() && std::isspace(static_cast<unsigned char>(s.back()))) s.pop_back();

        size_t comment_pos = s.find(" #");
        if (comment_pos != std::string::npos) {
            s = s.substr(0, comment_pos);
            while (!s.empty() && std::isspace(static_cast<unsigned char>(s.back()))) s.pop_back();
        }

        if (s.empty() || s == "null" || s == "~") return Yaml(nullptr);
        if (s == "true" || s == "yes" || s == "on") return Yaml(true);
        if (s == "false" || s == "no" || s == "off") return Yaml(false);

        if ((s.front() == '"' && s.back() == '"') || (s.front() == '\'' && s.back() == '\'')) {
            return Yaml(s.substr(1, s.size() - 2));
        }

        bool has_dot = s.find('.') != std::string::npos;
        bool is_neg = !s.empty() && s[0] == '-';
        size_t start = is_neg ? 1 : 0;
        bool is_num = start < s.size();
        for (size_t i = start; i < s.size() && is_num; i++) {
            if (!std::isdigit(static_cast<unsigned char>(s[i])) && s[i] != '.') is_num = false;
        }
        if (is_num && !s.empty()) {
            if (has_dot) return Yaml(std::stod(s));
            return Yaml(static_cast<int64_t>(std::stoll(s)));
        }
        return Yaml(s);
    }

    std::string read_line_val() {
        std::string val;
        while (peek() && peek() != '\n') val += get();
        return val;
    }

    Yaml parse_flow_array() {
        get();
        std::vector<Yaml> arr;
        skip_ws_comments();
        while (peek() && peek() != ']') {
            std::string val;
            while (peek() && peek() != ',' && peek() != ']') val += get();
            arr.push_back(parse_scalar(val));
            skip_ws_comments();
            if (peek() == ',') { get(); skip_ws_comments(); }
        }
        if (peek() == ']') get();
        return Yaml(std::move(arr));
    }

    Yaml parse_flow_object() {
        get();
        std::map<std::string, Yaml> obj;
        skip_ws_comments();
        while (peek() && peek() != '}') {
            std::string key;
            while (peek() && peek() != ':' && peek() != '}') key += get();
            while (!key.empty() && std::isspace(static_cast<unsigned char>(key.back()))) key.pop_back();
            if (peek() == ':') {
                get();
                skip_ws_inline();
                std::string val;
                while (peek() && peek() != ',' && peek() != '}') val += get();
                obj[key] = parse_scalar(val);
            }
            skip_ws_comments();
            if (peek() == ',') { get(); skip_ws_comments(); }
        }
        if (peek() == '}') get();
        return Yaml(std::move(obj));
    }

    Yaml parse_block_seq(int base_indent) {
        std::vector<Yaml> arr;
        bool first_item = true;
        while (true) {
            if (!first_item) {
                // Only skip newlines, not spaces (indent is significant)
                skip_newlines();
                if (peek() == '\0') break;
            }

            size_t line_start = pos_;
            int indent = 0;

            // On the first item, we might already be positioned at '-' with indent consumed
            if (first_item && peek() == '-') {
                indent = base_indent;  // Assume we're at the right indentation
                first_item = false;
            } else {
                while (peek() == ' ') { get(); indent++; }
                if (indent < base_indent || peek() != '-') {
                    pos_ = line_start;
                    break;
                }
            }

            get();  // consume '-'
            skip_ws_inline();

            if (peek() == '\n') {
                get();
                int next_ind = count_indent();
                if (next_ind > indent) {
                    for (int i = 0; i < next_ind; i++) get();
                    arr.push_back(parse_value(next_ind));
                } else {
                    arr.push_back(Yaml(nullptr));
                }
            } else if (peek() == '-') {
                arr.push_back(parse_block_seq(indent + 2));
            } else {
                size_t saved = pos_;
                std::string first_key;
                while (peek() && peek() != ':' && peek() != '\n') first_key += get();
                if (peek() == ':') {
                    pos_ = saved;
                    arr.push_back(parse_inline_obj_or_val(indent + 2));
                } else {
                    pos_ = saved;
                    arr.push_back(parse_scalar(read_line_val()));
                }
            }
        }
        return Yaml(std::move(arr));
    }

    Yaml parse_inline_obj_or_val(int base_indent) {
        std::map<std::string, Yaml> obj;
        std::string key = parse_key();
        if (peek() != ':') return parse_scalar(key);
        get();
        skip_ws_inline();

        if (peek() == '\n' || peek() == '\0' || peek() == '#') {
            skip_comment();
            if (peek() == '\n') get();
            int next_ind = count_indent();
            if (next_ind > base_indent) {
                for (int i = 0; i < next_ind; i++) get();
                obj[key] = parse_value(next_ind);
            } else {
                obj[key] = Yaml(nullptr);
            }
        } else if (peek() == '[') {
            obj[key] = parse_flow_array();
        } else if (peek() == '{') {
            obj[key] = parse_flow_object();
        } else if (peek() == '|' || peek() == '>') {
            char style = get();
            while (peek() == ' ' || peek() == '\t') get();
            if (peek() == '\n') get();
            obj[key] = parse_multiline_str(base_indent, style);
        } else {
            obj[key] = parse_scalar(read_line_val());
        }

        while (true) {
            skip_ws_comments();
            if (peek() == '\0') break;

            size_t line_start = pos_;
            int indent = 0;
            while (peek() == ' ') { get(); indent++; }

            if (indent != base_indent || peek() == '-') {
                pos_ = line_start;
                break;
            }

            key = parse_key();
            if (key.empty() || peek() != ':') {
                pos_ = line_start;
                break;
            }
            get();
            skip_ws_inline();

            if (peek() == '\n' || peek() == '\0' || peek() == '#') {
                skip_comment();
                if (peek() == '\n') get();
                int next_ind = count_indent();
                if (next_ind > base_indent) {
                    for (int i = 0; i < next_ind; i++) get();
                    obj[key] = parse_value(next_ind);
                } else {
                    obj[key] = Yaml(nullptr);
                }
            } else if (peek() == '[') {
                obj[key] = parse_flow_array();
            } else if (peek() == '{') {
                obj[key] = parse_flow_object();
            } else if (peek() == '|' || peek() == '>') {
                char style = get();
                while (peek() == ' ' || peek() == '\t') get();
                if (peek() == '\n') get();
                obj[key] = parse_multiline_str(base_indent, style);
            } else {
                obj[key] = parse_scalar(read_line_val());
            }
        }
        return Yaml(std::move(obj));
    }

    Yaml parse_multiline_str(int base_indent, char style) {
        std::string result;
        bool first = true;
        while (true) {
            size_t line_start = pos_;
            int indent = 0;
            while (peek() == ' ') { get(); indent++; }

            if (peek() == '\0') break;
            if (peek() == '\n') { get(); result += '\n'; continue; }
            if (indent <= base_indent) { pos_ = line_start; break; }

            if (!first && style == '|') result += '\n';
            else if (!first && style == '>') result += ' ';
            first = false;

            while (peek() && peek() != '\n') result += get();
            if (peek() == '\n') get();
        }
        return Yaml(result);
    }

    Yaml parse_value(int base_indent) {
        skip_ws_inline();
        // Check for block sequence: "-" followed by space or newline
        if (peek() == '-') {
            size_t saved = pos_;
            get(); // consume '-'
            char next = peek();
            pos_ = saved; // restore position
            // Only treat as block sequence if dash is followed by space, newline, or end
            if (next == ' ' || next == '\n' || next == '\0') {
                return parse_block_seq(base_indent);
            }
            // Otherwise treat as scalar (could be negative number)
        }
        if (peek() == '[') return parse_flow_array();
        if (peek() == '{') return parse_flow_object();

        size_t saved = pos_;
        std::string test_key;
        while (peek() && peek() != ':' && peek() != '\n') test_key += get();
        if (peek() == ':') {
            pos_ = saved;
            return parse_inline_obj_or_val(base_indent);
        }
        pos_ = saved;
        return parse_scalar(read_line_val());
    }

public:
    static YamlResult<Yaml> parse(const std::string& input) {
        YamlParser parser;
        parser.input_ = input;
        parser.pos_ = 0;
        parser.skip_ws_comments();
        if (parser.peek() == '\0') return YamlResult<Yaml>::ok(Yaml(nullptr));
        int base_indent = 0;
        while (parser.peek() == ' ') { parser.get(); base_indent++; }
        return YamlResult<Yaml>::ok(parser.parse_value(base_indent));
    }
};

class YamlSerializer {
public:
    static std::string stringify(const Yaml& yaml, int indent = 0) {
        return stringify_val(yaml, indent, false);
    }

private:
    static std::string stringify_val(const Yaml& yaml, int indent, bool inline_ctx) {
        if (yaml.is_null()) return "null";
        if (yaml.is_bool()) return yaml.as_bool().value() ? "true" : "false";
        if (yaml.is_int()) return std::to_string(yaml.as_int().value());
        if (yaml.is_float()) {
            std::ostringstream ss;
            ss << yaml.as_float().value();
            return ss.str();
        }
        if (yaml.is_str()) {
            std::string s = yaml.as_str().value();
            bool needs_quotes = s.empty() || s.find(':') != std::string::npos ||
                               s.find('#') != std::string::npos || s.find('\n') != std::string::npos;
            if (needs_quotes) return "\"" + s + "\"";
            return s;
        }
        if (yaml.is_list()) {
            const auto& arr = std::get<std::vector<Yaml>>(yaml.raw());
            if (arr.empty()) return "[]";
            std::string result;
            std::string prefix(indent, ' ');
            for (const auto& item : arr) {
                result += prefix + "- " + stringify_val(item, indent + 2, false) + "\n";
            }
            if (!result.empty()) result.pop_back();
            return result;
        }
        if (yaml.is_object()) {
            const auto& obj = std::get<std::map<std::string, Yaml>>(yaml.raw());
            if (obj.empty()) return "{}";
            std::string result;
            std::string prefix(indent, ' ');
            for (const auto& [k, v] : obj) {
                if (v.is_object() || v.is_list()) {
                    result += prefix + k + ":\n" + stringify_val(v, indent + 2, false) + "\n";
                } else {
                    result += prefix + k + ": " + stringify_val(v, indent, true) + "\n";
                }
            }
            if (!result.empty()) result.pop_back();
            return result;
        }
        return "";
    }
};

}  // namespace bishop

#endif  // BISHOP_YAML_HPP
