/**
 * @file markdown.hpp
 * @brief Bishop Markdown runtime library using maddy.
 *
 * Provides Markdown parsing and HTML generation for Bishop programs.
 * This header is included when programs import the markdown module.
 *
 * Uses the maddy library (https://github.com/progsource/maddy) for parsing.
 */

#pragma once

#include <maddy/parser.h>
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <optional>
#include <algorithm>
#include <cctype>

namespace bishop {

/**
 * Simple Result type for fallible operations in markdown module.
 */
struct MarkdownError {
    std::string message;
    MarkdownError(const std::string& msg) : message(msg) {}
};

template<typename T>
class MarkdownResult {
private:
    std::optional<T> val_;
    std::shared_ptr<MarkdownError> err_;

public:
    MarkdownResult() : val_(std::nullopt), err_(nullptr) {}
    MarkdownResult(T v) : val_(std::move(v)), err_(nullptr) {}
    MarkdownResult(std::shared_ptr<MarkdownError> e) : val_(std::nullopt), err_(std::move(e)) {}

    static MarkdownResult<T> ok(T v) {
        return MarkdownResult<T>(std::move(v));
    }

    static MarkdownResult<T> err(const std::string& msg) {
        return MarkdownResult<T>(std::make_shared<MarkdownError>(msg));
    }

    static MarkdownResult<T> err(std::shared_ptr<MarkdownError> e) {
        return MarkdownResult<T>(std::move(e));
    }

    bool is_ok() const { return val_.has_value(); }
    bool is_error() const { return !is_ok(); }

    T& value() { return *val_; }
    const T& value() const { return *val_; }

    std::shared_ptr<MarkdownError> error() const { return err_; }

    explicit operator bool() const { return is_ok(); }
};

}  // namespace bishop

namespace markdown {

// ============================================================================
// Markdown Document Class
// ============================================================================

/**
 * Represents a parsed Markdown document.
 * Wraps the maddy parser and stores both source and generated HTML.
 */
class Markdown {
public:
    std::string text;        // Original markdown text
    std::string node_type;   // "document", "heading", "paragraph", etc.
    std::vector<std::shared_ptr<Markdown>> children;

private:
    std::string html_cache_;
    bool html_cached_ = false;

public:
    Markdown() : node_type("document") {}
    Markdown(const std::string& src) : text(src), node_type("document") {}
    Markdown(const std::string& src, const std::string& type) : text(src), node_type(type) {}

    // Convert to HTML using maddy
    std::string to_html() {
        if (html_cached_) {
            return html_cache_;
        }

        std::stringstream input(text);
        auto parser = std::make_shared<maddy::Parser>();
        html_cache_ = parser->Parse(input);
        html_cached_ = true;
        return html_cache_;
    }

    // Get plain text (strip HTML tags from the HTML output)
    std::string get_text() const {
        // Parse to HTML first, then strip tags
        std::stringstream input(text);
        auto parser = std::make_shared<maddy::Parser>();
        std::string html = parser->Parse(input);

        // Simple HTML tag stripping
        std::string result;
        bool in_tag = false;
        for (char c : html) {
            if (c == '<') {
                in_tag = true;
            } else if (c == '>') {
                in_tag = false;
            } else if (!in_tag) {
                result += c;
            }
        }

        // Decode common HTML entities
        std::string decoded = result;
        size_t pos;
        while ((pos = decoded.find("&lt;")) != std::string::npos) {
            decoded.replace(pos, 4, "<");
        }
        while ((pos = decoded.find("&gt;")) != std::string::npos) {
            decoded.replace(pos, 4, ">");
        }
        while ((pos = decoded.find("&amp;")) != std::string::npos) {
            decoded.replace(pos, 5, "&");
        }
        while ((pos = decoded.find("&quot;")) != std::string::npos) {
            decoded.replace(pos, 6, "\"");
        }
        while ((pos = decoded.find("&#39;")) != std::string::npos) {
            decoded.replace(pos, 5, "'");
        }

        // Trim whitespace
        size_t start = decoded.find_first_not_of(" \t\n\r");
        if (start == std::string::npos) return "";
        size_t end = decoded.find_last_not_of(" \t\n\r");
        return decoded.substr(start, end - start + 1);
    }

    // Type checking methods - analyze the source text
    bool is_heading() const {
        return !text.empty() && text[0] == '#';
    }

    bool is_paragraph() const {
        return !text.empty() && text[0] != '#' && text[0] != '-' &&
               text[0] != '*' && text[0] != '>' && text[0] != '`' &&
               text[0] != '!' && text[0] != '[';
    }

    bool is_list() const {
        if (text.empty()) return false;
        // Check for unordered list
        if (text[0] == '-' || text[0] == '*' || text[0] == '+') {
            return text.size() > 1 && text[1] == ' ';
        }
        // Check for ordered list (starts with digit)
        if (std::isdigit(text[0])) {
            size_t pos = text.find('.');
            if (pos != std::string::npos && pos < 10) {
                return pos + 1 < text.size() && text[pos + 1] == ' ';
            }
        }
        return false;
    }

    bool is_code_block() const {
        return text.size() >= 3 && text.substr(0, 3) == "```";
    }

    bool is_blockquote() const {
        return !text.empty() && text[0] == '>';
    }

    bool is_link() const {
        // Check for [text](url) pattern
        size_t bracket = text.find('[');
        if (bracket == std::string::npos) return false;
        size_t close = text.find("](", bracket);
        if (close == std::string::npos) return false;
        size_t paren = text.find(')', close);
        return paren != std::string::npos;
    }

    bool is_image() const {
        // Check for ![alt](url) pattern
        return text.size() >= 2 && text[0] == '!' && text[1] == '[';
    }

    // Get heading level (1-6)
    int heading_level() const {
        if (!is_heading()) return 0;
        int level = 0;
        for (char c : text) {
            if (c == '#') level++;
            else break;
        }
        return std::min(level, 6);
    }
};

// ============================================================================
// Module Functions
// ============================================================================

/**
 * Parse markdown text into a Markdown document.
 */
inline bishop::MarkdownResult<Markdown> parse(const std::string& text) {
    if (text.empty()) {
        return bishop::MarkdownResult<Markdown>::err("Empty markdown text");
    }
    return bishop::MarkdownResult<Markdown>::ok(Markdown(text));
}

/**
 * Convert markdown text directly to HTML.
 */
inline std::string to_html(const std::string& text) {
    std::stringstream input(text);
    auto parser = std::make_shared<maddy::Parser>();
    return parser->Parse(input);
}

/**
 * Extract plain text from markdown (strips formatting).
 */
inline std::string to_text(const std::string& text) {
    Markdown doc(text);
    return doc.get_text();
}

/**
 * Convert a Markdown document back to markdown text.
 * Since we store the original, just return it.
 */
inline std::string stringify(const Markdown& doc) {
    return doc.text;
}

}  // namespace markdown
