/**
 * @file regex.hpp
 * @brief Bishop regex runtime library.
 *
 * Provides regular expression utilities for Bishop programs.
 * Uses std::regex for all regex operations.
 * This header is included when programs import the regex module.
 */

#pragma once

#include <bishop/std.hpp>
#include <regex>
#include <string>
#include <vector>

namespace regex {

/**
 * Match result structure containing match details.
 * text is the matched text, start/end are positions, groups contains capture groups.
 */
struct Match {
    std::string text;
    int start;
    int end;
    std::vector<std::string> groups;

    /**
     * Check if this match is valid (found a match).
     */
    bool found() const {
        return start >= 0;
    }

    /**
     * Get a capture group by index.
     * Group 0 is the full match, 1+ are capture groups.
     * Returns empty string if index is out of bounds.
     */
    std::string group(int n) const {
        if (n < 0 || static_cast<size_t>(n) >= groups.size()) {
            return "";
        }

        return groups[static_cast<size_t>(n)];
    }
};

/**
 * Compiled regular expression.
 * Wraps std::regex with a simpler interface.
 */
struct Regex {
    std::regex re;
    std::string pattern;

    /**
     * Check if the entire string matches the pattern.
     */
    bool matches(const std::string& text) const {
        return std::regex_match(text, re);
    }

    /**
     * Check if the string contains the pattern anywhere.
     */
    bool contains(const std::string& text) const {
        return std::regex_search(text, re);
    }

    /**
     * Find the first match in the string.
     * Returns Match with start=-1 if no match found.
     */
    Match find(const std::string& text) const {
        Match result;
        result.text = "";
        result.start = -1;
        result.end = -1;

        std::smatch match;

        if (std::regex_search(text, match, re)) {
            result.text = match[0].str();
            result.start = static_cast<int>(match.position(0));
            result.end = result.start + static_cast<int>(match[0].length());

            for (size_t i = 0; i < match.size(); ++i) {
                result.groups.push_back(match[i].str());
            }
        }

        return result;
    }

    /**
     * Find all matches in the string.
     * Returns empty vector if no matches found.
     */
    std::vector<Match> find_all(const std::string& text) const {
        std::vector<Match> results;
        std::string::const_iterator search_start = text.cbegin();
        std::smatch match;
        int base_offset = 0;

        while (std::regex_search(search_start, text.cend(), match, re)) {
            Match m;
            m.text = match[0].str();
            m.start = base_offset + static_cast<int>(match.position(0));
            m.end = m.start + static_cast<int>(match[0].length());

            for (size_t i = 0; i < match.size(); ++i) {
                m.groups.push_back(match[i].str());
            }

            results.push_back(m);

            base_offset += static_cast<int>(match.position(0)) + static_cast<int>(match[0].length());
            search_start = match.suffix().first;
        }

        return results;
    }

    /**
     * Replace the first match with the replacement string.
     * Supports capture group references ($1, $2, etc.).
     * Returns original string if no match found.
     */
    std::string replace(const std::string& text, const std::string& replacement) const {
        std::smatch match;

        if (!std::regex_search(text, match, re)) {
            return text;
        }

        std::string result = match.prefix().str();
        result += expand_replacement(replacement, match);
        result += match.suffix().str();
        return result;
    }

    /**
     * Replace all matches with the replacement string.
     * Supports capture group references ($1, $2, etc.).
     * Returns original string if no matches found.
     */
    std::string replace_all(const std::string& text, const std::string& replacement) const {
        std::string result;
        std::string::const_iterator search_start = text.cbegin();
        std::smatch match;

        while (std::regex_search(search_start, text.cend(), match, re)) {
            result += match.prefix().str();
            result += expand_replacement(replacement, match);
            search_start = match.suffix().first;
        }

        result += std::string(search_start, text.cend());
        return result;
    }

private:
    /**
     * Expand replacement string with capture group references.
     * $0, $1, $2, etc. are replaced with the corresponding capture groups.
     * $0 is the full match, $1-$99 are capture groups.
     * $$ inserts a literal dollar sign.
     */
    std::string expand_replacement(const std::string& replacement, const std::smatch& match) const {
        std::string result;

        for (size_t i = 0; i < replacement.size(); ++i) {
            if (replacement[i] == '$' && i + 1 < replacement.size()) {
                char next = replacement[i + 1];

                // Handle $$ -> literal $
                if (next == '$') {
                    result += '$';
                    ++i;
                    continue;
                }

                // Handle $0-$99 capture group references
                if (next >= '0' && next <= '9') {
                    int group_num = next - '0';

                    // Check for second digit (for $10-$99)
                    if (i + 2 < replacement.size()) {
                        char next2 = replacement[i + 2];

                        if (next2 >= '0' && next2 <= '9') {
                            int two_digit = group_num * 10 + (next2 - '0');

                            // Only use two-digit if it's a valid group
                            if (static_cast<size_t>(two_digit) < match.size()) {
                                result += match[two_digit].str();
                                i += 2;
                                continue;
                            }
                        }
                    }

                    // Use single digit group
                    if (static_cast<size_t>(group_num) < match.size()) {
                        result += match[group_num].str();
                    }

                    ++i;
                    continue;
                }
            }

            result += replacement[i];
        }

        return result;
    }
};

/**
 * Compile a regular expression pattern.
 * Returns Result with Regex or error if pattern is invalid.
 */
inline bishop::rt::Result<Regex> compile(const std::string& pattern) {
    try {
        Regex result;
        result.pattern = pattern;
        result.re = std::regex(pattern);
        return result;
    } catch (const std::regex_error& e) {
        return bishop::rt::make_error<Regex>("invalid regex pattern: " + std::string(e.what()));
    }
}

/**
 * Split a string by a regex pattern.
 * Returns Result with vector of strings or error if pattern is invalid.
 * Handles trailing delimiters (e.g., "a,b," splits to ["a", "b", ""]).
 */
inline bishop::rt::Result<std::vector<std::string>> split(const std::string& pattern, const std::string& text) {
    try {
        std::regex re(pattern);
        std::vector<std::string> result;

        if (text.empty()) {
            result.push_back("");
            return result;
        }

        // Use regex_search to manually iterate and handle trailing delimiters
        std::string::const_iterator search_start = text.cbegin();
        std::smatch match;
        size_t last_end = 0;

        while (std::regex_search(search_start, text.cend(), match, re)) {
            // Add the text before this match
            size_t match_start = last_end + static_cast<size_t>(match.position(0));
            result.push_back(text.substr(last_end, match_start - last_end));

            last_end = match_start + match[0].length();
            search_start = match.suffix().first;
        }

        // Add any remaining text after the last match (including empty trailing string)
        result.push_back(text.substr(last_end));

        if (result.empty()) {
            result.push_back(text);
        }

        return result;
    } catch (const std::regex_error& e) {
        return bishop::rt::make_error<std::vector<std::string>>("invalid regex pattern: " + std::string(e.what()));
    }
}

}  // namespace regex
