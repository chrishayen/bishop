/**
 * @file markdown.hpp
 * @brief Bishop Markdown runtime library.
 *
 * Provides Markdown parsing and HTML generation for Bishop programs.
 * This header is included when programs import the markdown module.
 *
 * Uses a recursive descent parser for Markdown.
 * Markdown nodes are represented using std::variant.
 */

#pragma once

#include <bishop/std.hpp>
#include <string>
#include <vector>
#include <variant>
#include <sstream>
#include <stdexcept>
#include <cctype>
#include <memory>
#include <algorithm>

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

// Forward declarations
class Markdown;

// ============================================================================
// Markdown Node Types
// ============================================================================

struct MarkdownText {
    std::string content;
    bool bold = false;
    bool italic = false;
    bool code = false;
};

struct MarkdownHeading {
    int level;  // 1-6
    std::vector<std::shared_ptr<Markdown>> children;
};

struct MarkdownParagraph {
    std::vector<std::shared_ptr<Markdown>> children;
};

struct MarkdownListItem {
    std::vector<std::shared_ptr<Markdown>> children;
};

struct MarkdownList {
    bool ordered;
    std::vector<MarkdownListItem> items;
};

struct MarkdownCodeBlock {
    std::string language;
    std::string code;
};

struct MarkdownBlockquote {
    std::vector<std::shared_ptr<Markdown>> children;
};

struct MarkdownLink {
    std::string text;
    std::string url;
    std::string title;
};

struct MarkdownImage {
    std::string alt;
    std::string url;
    std::string title;
};

struct MarkdownHorizontalRule {};

struct MarkdownLineBreak {};

// Document node - holds children
struct MarkdownDocument {
    std::vector<std::shared_ptr<Markdown>> children;
};

// Variant holding all node types
using MarkdownNode = std::variant<
    MarkdownText,
    MarkdownHeading,
    MarkdownParagraph,
    MarkdownList,
    MarkdownCodeBlock,
    MarkdownBlockquote,
    MarkdownLink,
    MarkdownImage,
    MarkdownHorizontalRule,
    MarkdownLineBreak,
    MarkdownDocument
>;

/**
 * Markdown node class.
 * Wraps a variant holding all possible Markdown node types.
 */
class Markdown {
public:
    MarkdownNode node;
    std::vector<std::shared_ptr<Markdown>> children;

    Markdown() : node(MarkdownDocument{}) {}
    explicit Markdown(MarkdownNode n) : node(std::move(n)) {}

    // Type checking methods
    bool is_text() const { return std::holds_alternative<MarkdownText>(node); }
    bool is_heading() const { return std::holds_alternative<MarkdownHeading>(node); }
    bool is_paragraph() const { return std::holds_alternative<MarkdownParagraph>(node); }
    bool is_list() const { return std::holds_alternative<MarkdownList>(node); }
    bool is_code_block() const { return std::holds_alternative<MarkdownCodeBlock>(node); }
    bool is_blockquote() const { return std::holds_alternative<MarkdownBlockquote>(node); }
    bool is_link() const { return std::holds_alternative<MarkdownLink>(node); }
    bool is_image() const { return std::holds_alternative<MarkdownImage>(node); }
    bool is_hr() const { return std::holds_alternative<MarkdownHorizontalRule>(node); }
    bool is_line_break() const { return std::holds_alternative<MarkdownLineBreak>(node); }
    bool is_document() const { return std::holds_alternative<MarkdownDocument>(node); }

    // Accessor methods
    int heading_level() const {
        if (is_heading()) {
            return std::get<MarkdownHeading>(node).level;
        }
        return 0;
    }

    std::string get_text() const {
        return std::visit([this](const auto& n) -> std::string {
            using T = std::decay_t<decltype(n)>;
            if constexpr (std::is_same_v<T, MarkdownText>) {
                return n.content;
            } else if constexpr (std::is_same_v<T, MarkdownHeading>) {
                std::string result;
                for (const auto& child : n.children) {
                    result += child->get_text();
                }
                return result;
            } else if constexpr (std::is_same_v<T, MarkdownParagraph>) {
                std::string result;
                for (const auto& child : n.children) {
                    result += child->get_text();
                }
                return result;
            } else if constexpr (std::is_same_v<T, MarkdownList>) {
                std::string result;
                for (const auto& item : n.items) {
                    for (const auto& child : item.children) {
                        result += child->get_text();
                    }
                    result += "\n";
                }
                return result;
            } else if constexpr (std::is_same_v<T, MarkdownCodeBlock>) {
                return n.code;
            } else if constexpr (std::is_same_v<T, MarkdownBlockquote>) {
                std::string result;
                for (const auto& child : n.children) {
                    result += child->get_text();
                }
                return result;
            } else if constexpr (std::is_same_v<T, MarkdownLink>) {
                return n.text;
            } else if constexpr (std::is_same_v<T, MarkdownImage>) {
                return n.alt;
            } else if constexpr (std::is_same_v<T, MarkdownDocument>) {
                std::string result;
                for (const auto& child : children) {
                    result += child->get_text();
                }
                return result;
            } else {
                return "";
            }
        }, node);
    }

    std::string get_code() const {
        if (is_code_block()) {
            return std::get<MarkdownCodeBlock>(node).code;
        }
        return "";
    }

    std::string get_language() const {
        if (is_code_block()) {
            return std::get<MarkdownCodeBlock>(node).language;
        }
        return "";
    }

    std::string get_url() const {
        if (is_link()) {
            return std::get<MarkdownLink>(node).url;
        }
        if (is_image()) {
            return std::get<MarkdownImage>(node).url;
        }
        return "";
    }

    std::string get_alt() const {
        if (is_image()) {
            return std::get<MarkdownImage>(node).alt;
        }
        return "";
    }

    bool is_ordered_list() const {
        if (is_list()) {
            return std::get<MarkdownList>(node).ordered;
        }
        return false;
    }

    // Generate HTML from this node
    std::string to_html() const;
};

// ============================================================================
// HTML Escaping
// ============================================================================

inline std::string escape_html(const std::string& text) {
    std::string result;
    result.reserve(text.size());
    for (char c : text) {
        switch (c) {
            case '&': result += "&amp;"; break;
            case '<': result += "&lt;"; break;
            case '>': result += "&gt;"; break;
            case '"': result += "&quot;"; break;
            case '\'': result += "&#39;"; break;
            default: result += c;
        }
    }
    return result;
}

// ============================================================================
// Markdown Parser
// ============================================================================

class MarkdownParser {
public:
    explicit MarkdownParser(const std::string& input) : input_(input), pos_(0) {}

    static bishop::MarkdownResult<Markdown> parse(const std::string& text) {
        MarkdownParser parser(text);
        return parser.parse_document();
    }

private:
    const std::string& input_;
    size_t pos_;

    char peek(size_t offset = 0) const {
        size_t idx = pos_ + offset;
        return idx < input_.size() ? input_[idx] : '\0';
    }

    char consume() {
        return pos_ < input_.size() ? input_[pos_++] : '\0';
    }

    bool at_end() const {
        return pos_ >= input_.size();
    }

    void skip_spaces() {
        while (pos_ < input_.size() && (input_[pos_] == ' ' || input_[pos_] == '\t')) {
            ++pos_;
        }
    }

    std::string read_line() {
        std::string line;
        while (pos_ < input_.size() && input_[pos_] != '\n') {
            line += input_[pos_++];
        }
        if (pos_ < input_.size() && input_[pos_] == '\n') {
            ++pos_;
        }
        return line;
    }

    std::string peek_line() const {
        std::string line;
        size_t p = pos_;
        while (p < input_.size() && input_[p] != '\n') {
            line += input_[p++];
        }
        return line;
    }

    bool is_blank_line() const {
        std::string line = peek_line();
        return line.empty() || std::all_of(line.begin(), line.end(), [](char c) {
            return c == ' ' || c == '\t';
        });
    }

    void skip_blank_lines() {
        while (!at_end() && is_blank_line()) {
            read_line();
        }
    }

    // Parse the entire document
    bishop::MarkdownResult<Markdown> parse_document() {
        Markdown doc(MarkdownDocument{});

        while (!at_end()) {
            skip_blank_lines();
            if (at_end()) break;

            auto block = parse_block();
            if (block.is_ok()) {
                doc.children.push_back(std::make_shared<Markdown>(block.value()));
            }
        }

        return bishop::MarkdownResult<Markdown>::ok(doc);
    }

    // Parse a single block element
    bishop::MarkdownResult<Markdown> parse_block() {
        skip_blank_lines();
        if (at_end()) {
            return bishop::MarkdownResult<Markdown>::err("Unexpected end of input");
        }

        // Check for fenced code block
        if (peek() == '`' && peek(1) == '`' && peek(2) == '`') {
            return parse_code_block();
        }

        // Check for heading
        if (peek() == '#') {
            return parse_heading();
        }

        // Check for horizontal rule (---, ***, ___)
        std::string line = peek_line();
        if (is_horizontal_rule(line)) {
            read_line();
            return bishop::MarkdownResult<Markdown>::ok(Markdown(MarkdownHorizontalRule{}));
        }

        // Check for blockquote
        if (peek() == '>') {
            return parse_blockquote();
        }

        // Check for unordered list
        if ((peek() == '-' || peek() == '*' || peek() == '+') &&
            (peek(1) == ' ' || peek(1) == '\t')) {
            return parse_list(false);
        }

        // Check for ordered list
        if (std::isdigit(static_cast<unsigned char>(peek()))) {
            size_t p = pos_;
            while (p < input_.size() && std::isdigit(static_cast<unsigned char>(input_[p]))) {
                ++p;
            }
            if (p < input_.size() && (input_[p] == '.' || input_[p] == ')') &&
                p + 1 < input_.size() && (input_[p + 1] == ' ' || input_[p + 1] == '\t')) {
                return parse_list(true);
            }
        }

        // Default: paragraph
        return parse_paragraph();
    }

    bool is_horizontal_rule(const std::string& line) const {
        std::string stripped;
        for (char c : line) {
            if (c != ' ' && c != '\t') {
                stripped += c;
            }
        }
        if (stripped.size() < 3) return false;
        char marker = stripped[0];
        if (marker != '-' && marker != '*' && marker != '_') return false;
        return std::all_of(stripped.begin(), stripped.end(), [marker](char c) {
            return c == marker;
        });
    }

    // Parse heading (# ## ### etc)
    bishop::MarkdownResult<Markdown> parse_heading() {
        int level = 0;
        while (pos_ < input_.size() && input_[pos_] == '#' && level < 6) {
            ++level;
            ++pos_;
        }

        if (level == 0 || (pos_ < input_.size() && input_[pos_] != ' ' && input_[pos_] != '\t' && input_[pos_] != '\n')) {
            // Not a valid heading, treat as paragraph
            pos_ -= level;
            return parse_paragraph();
        }

        skip_spaces();
        std::string content = read_line();

        // Remove trailing # characters
        while (!content.empty() && content.back() == '#') {
            content.pop_back();
        }
        while (!content.empty() && (content.back() == ' ' || content.back() == '\t')) {
            content.pop_back();
        }

        MarkdownHeading heading;
        heading.level = level;

        // Parse inline content
        auto inlines = parse_inline(content);
        heading.children = std::move(inlines);

        return bishop::MarkdownResult<Markdown>::ok(Markdown(heading));
    }

    // Parse paragraph
    bishop::MarkdownResult<Markdown> parse_paragraph() {
        std::string content;

        while (!at_end() && !is_blank_line()) {
            std::string line = peek_line();

            // Check if next line starts a new block
            if (line[0] == '#' || line[0] == '>' || line[0] == '`' ||
                ((line[0] == '-' || line[0] == '*' || line[0] == '+') &&
                 line.size() > 1 && (line[1] == ' ' || line[1] == '\t'))) {
                break;
            }

            if (!content.empty()) {
                content += " ";
            }
            content += read_line();
        }

        MarkdownParagraph para;
        para.children = parse_inline(content);

        return bishop::MarkdownResult<Markdown>::ok(Markdown(para));
    }

    // Parse code block (``` ... ```)
    bishop::MarkdownResult<Markdown> parse_code_block() {
        // Skip opening ```
        pos_ += 3;

        // Read language
        std::string language;
        while (pos_ < input_.size() && input_[pos_] != '\n') {
            language += input_[pos_++];
        }
        if (pos_ < input_.size()) ++pos_;  // Skip newline

        // Trim language
        while (!language.empty() && (language.front() == ' ' || language.front() == '\t')) {
            language.erase(language.begin());
        }
        while (!language.empty() && (language.back() == ' ' || language.back() == '\t')) {
            language.pop_back();
        }

        // Read code until closing ```
        std::string code;
        while (!at_end()) {
            if (peek() == '`' && peek(1) == '`' && peek(2) == '`') {
                pos_ += 3;
                // Skip rest of line
                while (pos_ < input_.size() && input_[pos_] != '\n') {
                    ++pos_;
                }
                if (pos_ < input_.size()) ++pos_;
                break;
            }
            code += read_line();
            code += '\n';
        }

        // Remove trailing newline
        if (!code.empty() && code.back() == '\n') {
            code.pop_back();
        }

        MarkdownCodeBlock block;
        block.language = language;
        block.code = code;

        return bishop::MarkdownResult<Markdown>::ok(Markdown(block));
    }

    // Parse blockquote (> ...)
    bishop::MarkdownResult<Markdown> parse_blockquote() {
        std::string content;

        while (!at_end() && peek() == '>') {
            ++pos_;  // Skip >
            if (pos_ < input_.size() && input_[pos_] == ' ') {
                ++pos_;  // Skip optional space
            }
            content += read_line();
            content += '\n';
        }

        // Parse the blockquote content as markdown
        MarkdownParser inner_parser(content);
        auto inner_doc = inner_parser.parse_document();

        MarkdownBlockquote quote;
        if (inner_doc.is_ok()) {
            quote.children = inner_doc.value().children;
        }

        return bishop::MarkdownResult<Markdown>::ok(Markdown(quote));
    }

    // Parse list
    bishop::MarkdownResult<Markdown> parse_list(bool ordered) {
        MarkdownList list;
        list.ordered = ordered;

        while (!at_end()) {
            skip_spaces();

            bool is_list_item = false;
            if (ordered) {
                size_t p = pos_;
                while (p < input_.size() && std::isdigit(static_cast<unsigned char>(input_[p]))) {
                    ++p;
                }
                if (p < input_.size() && (input_[p] == '.' || input_[p] == ')') &&
                    p + 1 < input_.size() && (input_[p + 1] == ' ' || input_[p + 1] == '\t')) {
                    is_list_item = true;
                    pos_ = p + 2;
                }
            } else {
                if ((peek() == '-' || peek() == '*' || peek() == '+') &&
                    (peek(1) == ' ' || peek(1) == '\t')) {
                    is_list_item = true;
                    pos_ += 2;
                }
            }

            if (!is_list_item) break;

            std::string content = read_line();
            MarkdownListItem item;
            item.children = parse_inline(content);
            list.items.push_back(std::move(item));

            skip_blank_lines();
        }

        return bishop::MarkdownResult<Markdown>::ok(Markdown(list));
    }

    // Parse inline content (bold, italic, code, links, images)
    std::vector<std::shared_ptr<Markdown>> parse_inline(const std::string& text) {
        std::vector<std::shared_ptr<Markdown>> result;
        size_t i = 0;
        std::string current_text;

        auto flush_text = [&]() {
            if (!current_text.empty()) {
                MarkdownText t;
                t.content = current_text;
                result.push_back(std::make_shared<Markdown>(Markdown(t)));
                current_text.clear();
            }
        };

        while (i < text.size()) {
            // Check for image ![alt](url)
            if (text[i] == '!' && i + 1 < text.size() && text[i + 1] == '[') {
                flush_text();
                size_t alt_start = i + 2;
                size_t alt_end = text.find(']', alt_start);
                if (alt_end != std::string::npos && alt_end + 1 < text.size() && text[alt_end + 1] == '(') {
                    size_t url_start = alt_end + 2;
                    size_t url_end = text.find(')', url_start);
                    if (url_end != std::string::npos) {
                        MarkdownImage img;
                        img.alt = text.substr(alt_start, alt_end - alt_start);
                        img.url = text.substr(url_start, url_end - url_start);
                        result.push_back(std::make_shared<Markdown>(Markdown(img)));
                        i = url_end + 1;
                        continue;
                    }
                }
            }

            // Check for link [text](url)
            if (text[i] == '[') {
                size_t text_end = text.find(']', i + 1);
                if (text_end != std::string::npos && text_end + 1 < text.size() && text[text_end + 1] == '(') {
                    size_t url_start = text_end + 2;
                    size_t url_end = text.find(')', url_start);
                    if (url_end != std::string::npos) {
                        flush_text();
                        MarkdownLink link;
                        link.text = text.substr(i + 1, text_end - i - 1);
                        link.url = text.substr(url_start, url_end - url_start);
                        result.push_back(std::make_shared<Markdown>(Markdown(link)));
                        i = url_end + 1;
                        continue;
                    }
                }
            }

            // Check for inline code `...`
            if (text[i] == '`') {
                size_t end = text.find('`', i + 1);
                if (end != std::string::npos) {
                    flush_text();
                    MarkdownText t;
                    t.content = text.substr(i + 1, end - i - 1);
                    t.code = true;
                    result.push_back(std::make_shared<Markdown>(Markdown(t)));
                    i = end + 1;
                    continue;
                }
            }

            // Check for bold **...**
            if (text[i] == '*' && i + 1 < text.size() && text[i + 1] == '*') {
                size_t end = text.find("**", i + 2);
                if (end != std::string::npos) {
                    flush_text();
                    MarkdownText t;
                    t.content = text.substr(i + 2, end - i - 2);
                    t.bold = true;
                    result.push_back(std::make_shared<Markdown>(Markdown(t)));
                    i = end + 2;
                    continue;
                }
            }

            // Check for italic *...*
            if (text[i] == '*') {
                size_t end = text.find('*', i + 1);
                if (end != std::string::npos && (end + 1 >= text.size() || text[end + 1] != '*')) {
                    flush_text();
                    MarkdownText t;
                    t.content = text.substr(i + 1, end - i - 1);
                    t.italic = true;
                    result.push_back(std::make_shared<Markdown>(Markdown(t)));
                    i = end + 1;
                    continue;
                }
            }

            current_text += text[i];
            ++i;
        }

        flush_text();
        return result;
    }
};

// ============================================================================
// HTML Generation
// ============================================================================

inline std::string Markdown::to_html() const {
    return std::visit([this](const auto& n) -> std::string {
        using T = std::decay_t<decltype(n)>;

        if constexpr (std::is_same_v<T, MarkdownText>) {
            std::string content = escape_html(n.content);
            if (n.code) {
                return "<code>" + content + "</code>";
            }
            if (n.bold) {
                return "<strong>" + content + "</strong>";
            }
            if (n.italic) {
                return "<em>" + content + "</em>";
            }
            return content;
        } else if constexpr (std::is_same_v<T, MarkdownHeading>) {
            std::string tag = "h" + std::to_string(n.level);
            std::string content;
            for (const auto& child : n.children) {
                content += child->to_html();
            }
            return "<" + tag + ">" + content + "</" + tag + ">\n";
        } else if constexpr (std::is_same_v<T, MarkdownParagraph>) {
            std::string content;
            for (const auto& child : n.children) {
                content += child->to_html();
            }
            return "<p>" + content + "</p>\n";
        } else if constexpr (std::is_same_v<T, MarkdownList>) {
            std::string tag = n.ordered ? "ol" : "ul";
            std::string result = "<" + tag + ">\n";
            for (const auto& item : n.items) {
                result += "<li>";
                for (const auto& child : item.children) {
                    result += child->to_html();
                }
                result += "</li>\n";
            }
            result += "</" + tag + ">\n";
            return result;
        } else if constexpr (std::is_same_v<T, MarkdownCodeBlock>) {
            std::string escaped_code = escape_html(n.code);
            if (!n.language.empty()) {
                return "<pre><code class=\"language-" + escape_html(n.language) + "\">" +
                       escaped_code + "</code></pre>\n";
            }
            return "<pre><code>" + escaped_code + "</code></pre>\n";
        } else if constexpr (std::is_same_v<T, MarkdownBlockquote>) {
            std::string content;
            for (const auto& child : n.children) {
                content += child->to_html();
            }
            return "<blockquote>" + content + "</blockquote>\n";
        } else if constexpr (std::is_same_v<T, MarkdownLink>) {
            return "<a href=\"" + escape_html(n.url) + "\">" + escape_html(n.text) + "</a>";
        } else if constexpr (std::is_same_v<T, MarkdownImage>) {
            return "<img src=\"" + escape_html(n.url) + "\" alt=\"" + escape_html(n.alt) + "\">";
        } else if constexpr (std::is_same_v<T, MarkdownHorizontalRule>) {
            return "<hr>\n";
        } else if constexpr (std::is_same_v<T, MarkdownLineBreak>) {
            return "<br>\n";
        } else if constexpr (std::is_same_v<T, MarkdownDocument>) {
            std::string result;
            for (const auto& child : children) {
                result += child->to_html();
            }
            return result;
        }
        return "";
    }, node);
}

// ============================================================================
// Public API Functions
// ============================================================================

/**
 * Parse markdown text into a Markdown document.
 */
inline bishop::MarkdownResult<Markdown> parse(const std::string& text) {
    return MarkdownParser::parse(text);
}

/**
 * Convert markdown text directly to HTML.
 */
inline std::string to_html(const std::string& text) {
    auto result = parse(text);
    if (result.is_ok()) {
        return result.value().to_html();
    }
    return "";
}

/**
 * Extract plain text from markdown.
 */
inline std::string to_text(const std::string& text) {
    auto result = parse(text);
    if (result.is_ok()) {
        return result.value().get_text();
    }
    return "";
}

/**
 * Convert a Markdown document back to markdown string (best effort).
 */
inline std::string stringify(const Markdown& doc) {
    // Simple stringify - just return the text content
    return doc.get_text();
}

}  // namespace markdown
