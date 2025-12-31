/**
 * @file yaml.hpp
 * @brief Bishop YAML runtime library using yaml-cpp.
 */

#ifndef BISHOP_YAML_HPP
#define BISHOP_YAML_HPP

#pragma once

#include <bishop/std.hpp>
#include <yaml-cpp/yaml.h>
#include <string>
#include <vector>
#include <map>
#include <optional>
#include <memory>
#include <sstream>

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

    /**
     * Returns the error as a shared_ptr<bishop::rt::Error>.
     * Used by or_error() helper for error chaining.
     */
    std::shared_ptr<bishop::rt::Error> error() const {
        if (err_) {
            return std::make_shared<bishop::rt::Error>(err_->message);
        }
        return std::make_shared<bishop::rt::Error>("unknown yaml error");
    }
};

/**
 * @brief Wrapper around yaml-cpp's YAML::Node with Bishop-friendly API.
 */
class Yaml {
private:
    YAML::Node node_;

public:
    Yaml() : node_(YAML::NodeType::Null) {}
    explicit Yaml(const YAML::Node& node) : node_(node) {}
    explicit Yaml(std::nullptr_t) : node_(YAML::NodeType::Null) {}
    explicit Yaml(bool b) : node_(b) {}
    explicit Yaml(int64_t i) : node_(i) {}
    explicit Yaml(int i) : node_(static_cast<int64_t>(i)) {}
    explicit Yaml(double d) : node_(d) {}
    explicit Yaml(const std::string& s) : node_(s) {}
    explicit Yaml(const char* s) : node_(std::string(s)) {}

    static Yaml make_object() {
        YAML::Node node(YAML::NodeType::Map);
        return Yaml(node);
    }

    static Yaml make_array() {
        YAML::Node node(YAML::NodeType::Sequence);
        return Yaml(node);
    }

    bool is_null() const { return node_.IsNull(); }
    bool is_bool() const {
        if (!node_.IsScalar()) return false;
        std::string val = node_.Scalar();
        return val == "true" || val == "false" ||
               val == "yes" || val == "no" ||
               val == "on" || val == "off";
    }
    bool is_int() const {
        if (!node_.IsScalar()) return false;
        try {
            std::string s = node_.Scalar();
            size_t pos;
            std::stoll(s, &pos);
            return pos == s.size() && s.find('.') == std::string::npos;
        } catch (...) { return false; }
    }
    bool is_float() const {
        if (!node_.IsScalar()) return false;
        try {
            std::string s = node_.Scalar();
            size_t pos;
            std::stod(s, &pos);
            return pos == s.size() && s.find('.') != std::string::npos;
        } catch (...) { return false; }
    }
    bool is_str() const {
        return node_.IsScalar() && !is_bool() && !is_int() && !is_float() && !is_null();
    }
    bool is_list() const { return node_.IsSequence(); }
    bool is_object() const { return node_.IsMap(); }

    YamlResult<bool> as_bool() const {
        if (!node_.IsScalar()) return YamlResult<bool>::error("not a bool");
        std::string val = node_.Scalar();
        if (val == "true" || val == "yes" || val == "on") return YamlResult<bool>::ok(true);
        if (val == "false" || val == "no" || val == "off") return YamlResult<bool>::ok(false);
        return YamlResult<bool>::error("not a bool");
    }

    YamlResult<int64_t> as_int() const {
        if (!is_int()) return YamlResult<int64_t>::error("not an int");
        try {
            return YamlResult<int64_t>::ok(std::stoll(node_.Scalar()));
        } catch (...) {
            return YamlResult<int64_t>::error("not an int");
        }
    }

    YamlResult<double> as_float() const {
        if (!node_.IsScalar()) return YamlResult<double>::error("not a float");
        try {
            return YamlResult<double>::ok(std::stod(node_.Scalar()));
        } catch (...) {
            return YamlResult<double>::error("not a float");
        }
    }

    YamlResult<std::string> as_str() const {
        if (!node_.IsScalar()) return YamlResult<std::string>::error("not a string");
        return YamlResult<std::string>::ok(node_.Scalar());
    }

    int length() const {
        if (is_list()) return static_cast<int>(node_.size());
        if (is_object()) return static_cast<int>(node_.size());
        if (node_.IsScalar()) return static_cast<int>(node_.Scalar().size());
        return 0;
    }

    bool has(const std::string& key) const {
        if (!is_object()) return false;
        return node_[key].IsDefined();
    }

    YamlResult<Yaml> get(const std::string& key) const {
        if (is_object()) {
            if (!node_[key].IsDefined()) {
                return YamlResult<Yaml>::error("key not found: " + key);
            }
            return YamlResult<Yaml>::ok(Yaml(node_[key]));
        }
        if (is_list()) {
            try {
                size_t idx = std::stoull(key);
                if (idx >= node_.size()) {
                    return YamlResult<Yaml>::error("index out of bounds");
                }
                return YamlResult<Yaml>::ok(Yaml(node_[idx]));
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

    void set_str(const std::string& key, const std::string& val) {
        node_[key] = val;
    }

    void set_int(const std::string& key, int64_t val) {
        node_[key] = val;
    }

    void set_bool(const std::string& key, bool val) {
        node_[key] = val;
    }

    void set_float(const std::string& key, double val) {
        node_[key] = val;
    }

    void push_str(const std::string& val) {
        node_.push_back(val);
    }

    void push_int(int64_t val) {
        node_.push_back(val);
    }

    void push_bool(bool val) {
        node_.push_back(val);
    }

    void remove(const std::string& key) {
        if (is_object()) {
            node_.remove(key);
        }
    }

    std::vector<std::string> keys() const {
        std::vector<std::string> result;
        if (!is_object()) return result;
        for (auto it = node_.begin(); it != node_.end(); ++it) {
            result.push_back(it->first.Scalar());
        }
        return result;
    }

    const YAML::Node& raw() const { return node_; }
    YAML::Node& raw() { return node_; }
};

class YamlParser {
public:
    static YamlResult<Yaml> parse(const std::string& input) {
        try {
            YAML::Node node = YAML::Load(input);
            return YamlResult<Yaml>::ok(Yaml(node));
        } catch (const YAML::Exception& e) {
            return YamlResult<Yaml>::error(std::string("YAML parse error: ") + e.what());
        }
    }
};

class YamlSerializer {
public:
    static std::string stringify(const Yaml& yaml) {
        YAML::Emitter out;
        out << yaml.raw();
        return out.c_str();
    }
};

}  // namespace bishop

#endif  // BISHOP_YAML_HPP
