/**
 * @file template_render.cpp
 * @brief Implementation of template-based code emission.
 *
 * This module provides template loading and rendering functionality
 * for generating C++ code. Templates use {{PLACEHOLDER}} syntax.
 */

#include "template_render.hpp"
#include <fstream>
#include <sstream>
#include <regex>

namespace codegen {
namespace templates {

// Cache for loaded templates to avoid repeated file I/O
static std::unordered_map<std::string, std::string> template_cache;

/**
 * @brief Renders a template string by substituting placeholders.
 */
std::string render(const std::string& template_str,
                   const std::unordered_map<std::string, std::string>& subs) {
    std::string result = template_str;

    for (const auto& [key, value] : subs) {
        std::string placeholder = "{{" + key + "}}";
        size_t pos = 0;

        while ((pos = result.find(placeholder, pos)) != std::string::npos) {
            result.replace(pos, placeholder.length(), value);
            pos += value.length();
        }
    }

    return result;
}

/**
 * @brief Loads a template file from the templates directory.
 *
 * Uses a cache to avoid repeated file reads. Templates are expected
 * to be in the codegen/templates/ directory.
 */
std::string load_template(const std::string& name) {
    // Check cache first
    auto it = template_cache.find(name);

    if (it != template_cache.end()) {
        return it->second;
    }

    // Try to load from file
    // Note: In a production build, templates would be embedded as string literals
    // or the path would be configurable. For this prototype, we use a relative path.
    std::string path = "codegen/templates/" + name + ".cpp.tmpl";
    std::ifstream file(path);

    if (!file.is_open()) {
        // Try alternative path (when running from build directory)
        path = "../codegen/templates/" + name + ".cpp.tmpl";
        file.open(path);
    }

    if (!file.is_open()) {
        return "";  // Template not found
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    // Cache the template
    template_cache[name] = content;

    return content;
}

/**
 * @brief Indents each line of text by the specified number of tabs.
 */
std::string indent(const std::string& text, int tabs) {
    if (text.empty() || tabs <= 0) {
        return text;
    }

    std::string prefix(tabs, '\t');
    std::string result;
    std::istringstream stream(text);
    std::string line;
    bool first = true;

    while (std::getline(stream, line)) {
        if (!first) {
            result += "\n";
        }

        if (!line.empty()) {
            result += prefix + line;
        }

        first = false;
    }

    return result;
}

/**
 * @brief Joins statement strings into a body with proper indentation.
 */
std::string join_body(const std::vector<std::string>& statements, int tabs) {
    std::string result;
    std::string prefix(tabs, '\t');

    for (size_t i = 0; i < statements.size(); ++i) {
        result += prefix + statements[i];

        if (i + 1 < statements.size()) {
            result += "\n";
        }
    }

    return result;
}

// ============================================================================
// Convenience functions using templates
// ============================================================================

/**
 * @brief Renders a for-range loop using the for_range template.
 */
std::string for_range(const std::string& var,
                      const std::string& start,
                      const std::string& end,
                      const std::vector<std::string>& body) {
    std::string tmpl = load_template("for_range");

    if (tmpl.empty()) {
        // Fallback to inline template if file not found
        tmpl = "for (int {{VAR}} = {{START}}; {{VAR}} < {{END}}; {{VAR}}++) {\n{{BODY}}\n}";
    }

    return render(tmpl, {
        {"VAR", var},
        {"START", start},
        {"END", end},
        {"BODY", join_body(body, 1)}
    });
}

/**
 * @brief Renders a for-each loop using the for_each template.
 */
std::string for_each(const std::string& var,
                     const std::string& collection,
                     const std::vector<std::string>& body) {
    std::string tmpl = load_template("for_each");

    if (tmpl.empty()) {
        tmpl = "for (auto& {{VAR}} : {{COLLECTION}}) {\n{{BODY}}\n}";
    }

    return render(tmpl, {
        {"VAR", var},
        {"COLLECTION", collection},
        {"BODY", join_body(body, 1)}
    });
}

/**
 * @brief Renders a function definition using the function_def template.
 */
std::string function_def(const std::string& name,
                         const std::string& params,
                         const std::string& return_type,
                         const std::vector<std::string>& body) {
    std::string tmpl = load_template("function_def");

    if (tmpl.empty()) {
        tmpl = "{{RETURN_TYPE}} {{NAME}}({{PARAMS}}) {\n{{BODY}}\n}\n";
    }

    std::string rt = return_type.empty() ? "void" : return_type;

    return render(tmpl, {
        {"NAME", name},
        {"PARAMS", params},
        {"RETURN_TYPE", rt},
        {"BODY", join_body(body, 1)}
    });
}

/**
 * @brief Renders a struct definition using the struct_def template.
 */
std::string struct_def(const std::string& name,
                       const std::vector<std::string>& fields) {
    std::string tmpl = load_template("struct_def");

    if (tmpl.empty()) {
        tmpl = "struct {{NAME}} {\n{{FIELDS}}};\n";
    }

    return render(tmpl, {
        {"NAME", name},
        {"FIELDS", join_body(fields, 1)}
    });
}

/**
 * @brief Renders an if statement using the if_stmt or if_else_stmt template.
 */
std::string if_stmt(const std::string& condition,
                    const std::vector<std::string>& then_body,
                    const std::vector<std::string>& else_body) {
    if (else_body.empty()) {
        std::string tmpl = load_template("if_stmt");

        if (tmpl.empty()) {
            tmpl = "if ({{CONDITION}}) {\n{{THEN_BODY}}\n}";
        }

        return render(tmpl, {
            {"CONDITION", condition},
            {"THEN_BODY", join_body(then_body, 1)}
        });
    }

    std::string tmpl = load_template("if_else_stmt");

    if (tmpl.empty()) {
        tmpl = "if ({{CONDITION}}) {\n{{THEN_BODY}}\n} else {\n{{ELSE_BODY}}\n}";
    }

    return render(tmpl, {
        {"CONDITION", condition},
        {"THEN_BODY", join_body(then_body, 1)},
        {"ELSE_BODY", join_body(else_body, 1)}
    });
}

} // namespace templates
} // namespace codegen
