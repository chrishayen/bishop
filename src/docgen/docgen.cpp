/**
 * @file docgen.cpp
 * @brief Documentation generator implementation.
 */

#include "docgen.hpp"
#include "lexer/lexer.hpp"
#include "parser/parser.hpp"

#include <fstream>
#include <sstream>
#include <regex>
#include <iostream>

using namespace std;

/**
 * Escapes HTML special characters in text.
 */
string DocGen::escape_html(const string& text) {
    string result;
    result.reserve(text.size());

    for (char c : text) {
        switch (c) {
            case '&': result += "&amp;"; break;
            case '<': result += "&lt;"; break;
            case '>': result += "&gt;"; break;
            case '"': result += "&quot;"; break;
            default: result += c;
        }
    }

    return result;
}

/**
 * Converts basic markdown to HTML.
 * Supports: **bold**, *italic*, `code`, ```code blocks```, # headings, - lists
 */
string DocGen::markdown_to_html(const string& md) {
    if (md.empty()) return "";

    string result;
    istringstream stream(md);
    string line;
    bool in_code_block = false;
    bool in_list = false;

    while (getline(stream, line)) {
        // Code blocks
        if (line.find("```") == 0) {
            if (in_code_block) {
                result += "</code></pre>\n";
                in_code_block = false;
            } else {
                result += "<pre><code>";
                in_code_block = true;
            }
            continue;
        }

        if (in_code_block) {
            result += escape_html(line) + "\n";
            continue;
        }

        // Close list if line doesn't start with -
        if (in_list && (line.empty() || line[0] != '-')) {
            result += "</ul>\n";
            in_list = false;
        }

        // Empty line = paragraph break
        if (line.empty()) {
            result += "<br>\n";
            continue;
        }

        // Headings
        if (line[0] == '#') {
            int level = 0;
            while (level < line.size() && line[level] == '#') level++;
            if (level <= 6 && level < line.size() && line[level] == ' ') {
                string heading = line.substr(level + 1);
                result += "<h" + to_string(level + 2) + ">" + escape_html(heading) + "</h" + to_string(level + 2) + ">\n";
                continue;
            }
        }

        // List items
        if (line[0] == '-' && line.size() > 1 && line[1] == ' ') {
            if (!in_list) {
                result += "<ul>\n";
                in_list = true;
            }
            string item = line.substr(2);
            result += "<li>" + escape_html(item) + "</li>\n";
            continue;
        }

        // Regular paragraph with inline formatting
        string processed = escape_html(line);

        // Bold: **text**
        regex bold_re("\\*\\*([^*]+)\\*\\*");
        processed = regex_replace(processed, bold_re, "<strong>$1</strong>");

        // Italic: *text*
        regex italic_re("\\*([^*]+)\\*");
        processed = regex_replace(processed, italic_re, "<em>$1</em>");

        // Inline code: `code`
        regex code_re("`([^`]+)`");
        processed = regex_replace(processed, code_re, "<code>$1</code>");

        result += "<p>" + processed + "</p>\n";
    }

    if (in_code_block) {
        result += "</code></pre>\n";
    }

    if (in_list) {
        result += "</ul>\n";
    }

    return result;
}

/**
 * Renders a function definition to HTML.
 */
string DocGen::render_function(const FunctionDef& fn) {
    stringstream ss;

    ss << "<section class=\"function\" id=\"fn-" << fn.name << "\">\n";

    // Signature
    ss << "<h3><code>fn " << fn.name << "(";

    for (size_t i = 0; i < fn.params.size(); i++) {
        if (i > 0) ss << ", ";
        ss << fn.params[i].type << " " << fn.params[i].name;
    }

    ss << ")";

    if (!fn.return_type.empty()) {
        ss << " -&gt; " << fn.return_type;
    }

    ss << "</code></h3>\n";

    // Documentation
    if (!fn.doc_comment.empty()) {
        ss << "<div class=\"doc\">" << markdown_to_html(fn.doc_comment) << "</div>\n";
    }

    ss << "</section>\n";

    return ss.str();
}

/**
 * Renders a method definition to HTML.
 */
string DocGen::render_method(const MethodDef& m) {
    stringstream ss;

    ss << "<div class=\"method\" id=\"method-" << m.struct_name << "-" << m.name << "\">\n";

    // Signature (skip self parameter in display)
    ss << "<h4><code>" << m.name << "(";

    bool first = true;
    for (size_t i = 0; i < m.params.size(); i++) {
        if (m.params[i].name == "self") continue;

        if (!first) ss << ", ";
        first = false;
        ss << m.params[i].type << " " << m.params[i].name;
    }

    ss << ")";

    if (!m.return_type.empty()) {
        ss << " -&gt; " << m.return_type;
    }

    ss << "</code></h4>\n";

    // Documentation
    if (!m.doc_comment.empty()) {
        ss << "<div class=\"doc\">" << markdown_to_html(m.doc_comment) << "</div>\n";
    }

    ss << "</div>\n";

    return ss.str();
}

/**
 * Renders a struct definition with its methods to HTML.
 */
string DocGen::render_struct(const StructDef& s, const vector<const MethodDef*>& methods) {
    stringstream ss;

    ss << "<section class=\"struct\" id=\"struct-" << s.name << "\">\n";
    ss << "<h2>" << s.name << "</h2>\n";

    // Struct documentation
    if (!s.doc_comment.empty()) {
        ss << "<div class=\"doc\">" << markdown_to_html(s.doc_comment) << "</div>\n";
    }

    // Fields
    if (!s.fields.empty()) {
        ss << "<h3>Fields</h3>\n";
        ss << "<ul class=\"fields\">\n";

        for (const auto& field : s.fields) {
            ss << "<li><code>" << field.name << ": " << field.type << "</code>";

            if (!field.doc_comment.empty()) {
                ss << " - " << escape_html(field.doc_comment);
            }

            ss << "</li>\n";
        }

        ss << "</ul>\n";
    }

    // Methods
    if (!methods.empty()) {
        ss << "<h3>Methods</h3>\n";

        for (const auto* m : methods) {
            ss << render_method(*m);
        }
    }

    ss << "</section>\n";

    return ss.str();
}

/**
 * Generates the CSS stylesheet.
 */
string DocGen::generate_css() {
    return R"(
:root {
    --bg: #1a1a2e;
    --bg-light: #16213e;
    --text: #eee;
    --text-muted: #888;
    --accent: #e94560;
    --code-bg: #0f0f1a;
    --border: #333;
}

* { box-sizing: border-box; margin: 0; padding: 0; }

body {
    font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
    background: var(--bg);
    color: var(--text);
    line-height: 1.6;
    padding: 2rem;
    max-width: 900px;
    margin: 0 auto;
}

h1 { color: var(--accent); margin-bottom: 1.5rem; }
h2 { color: var(--accent); margin: 2rem 0 1rem; border-bottom: 1px solid var(--border); padding-bottom: 0.5rem; }
h3 { margin: 1.5rem 0 0.5rem; color: var(--text); }
h4 { margin: 1rem 0 0.3rem; color: var(--text-muted); }

code {
    font-family: 'JetBrains Mono', 'Fira Code', monospace;
    background: var(--code-bg);
    padding: 0.2em 0.4em;
    border-radius: 3px;
    font-size: 0.9em;
}

pre code {
    display: block;
    padding: 1rem;
    overflow-x: auto;
    margin: 1rem 0;
}

.doc { margin: 0.5rem 0 1rem; color: var(--text-muted); }
.doc p { margin: 0.5rem 0; }
.doc ul { margin-left: 1.5rem; }
.doc strong { color: var(--text); }

.struct, .function {
    background: var(--bg-light);
    padding: 1.5rem;
    border-radius: 8px;
    margin: 1rem 0;
}

.method {
    border-left: 3px solid var(--accent);
    padding-left: 1rem;
    margin: 1rem 0;
}

.fields { list-style: none; margin: 0.5rem 0; }
.fields li { padding: 0.3rem 0; }

nav { margin-bottom: 2rem; }
nav a { color: var(--accent); text-decoration: none; margin-right: 1rem; }
nav a:hover { text-decoration: underline; }
)";
}

/**
 * Wraps content in HTML document structure.
 */
string DocGen::wrap_html(const string& title, const string& content) {
    stringstream ss;

    ss << "<!DOCTYPE html>\n";
    ss << "<html lang=\"en\">\n";
    ss << "<head>\n";
    ss << "  <meta charset=\"UTF-8\">\n";
    ss << "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
    ss << "  <title>" << escape_html(title) << " - Nog Documentation</title>\n";
    ss << "  <style>" << generate_css() << "</style>\n";
    ss << "</head>\n";
    ss << "<body>\n";
    ss << "  <h1>" << escape_html(title) << "</h1>\n";
    ss << content;
    ss << "</body>\n";
    ss << "</html>\n";

    return ss.str();
}

/**
 * Generates documentation for a single parsed program.
 */
string DocGen::generate_file(const Program& program, const string& filename) {
    stringstream content;

    // Collect methods by struct name
    map<string, vector<const MethodDef*>> methods_by_struct;

    for (const auto& m : program.methods) {
        methods_by_struct[m->struct_name].push_back(m.get());
    }

    // Render structs with their methods
    for (const auto& s : program.structs) {
        auto it = methods_by_struct.find(s->name);
        vector<const MethodDef*> struct_methods;

        if (it != methods_by_struct.end()) {
            struct_methods = it->second;
        }

        content << render_struct(*s, struct_methods);
    }

    // Render standalone functions
    for (const auto& fn : program.functions) {
        // Skip test functions in docs
        if (fn->name.find("test_") == 0) continue;

        content << render_function(*fn);
    }

    return wrap_html(filename, content.str());
}

/**
 * Generates documentation for all .n files in a directory.
 */
void DocGen::generate_directory(const string& input_dir, const string& output_dir) {
    // Create output directory
    fs::create_directories(output_dir);

    vector<string> file_names;

    // Process each .n file
    for (const auto& entry : fs::recursive_directory_iterator(input_dir)) {
        if (!entry.is_regular_file()) continue;
        if (entry.path().extension() != ".n") continue;

        // Skip test files
        string filename = entry.path().filename().string();
        if (filename.find("test_") == 0) continue;

        // Read source
        ifstream file(entry.path());
        stringstream buffer;
        buffer << file.rdbuf();
        string source = buffer.str();

        // Parse
        Lexer lexer(source);
        auto tokens = lexer.tokenize();
        Parser parser(tokens);
        auto program = parser.parse();

        // Generate HTML
        string html = generate_file(*program, filename);

        // Write output
        string output_name = entry.path().stem().string() + ".html";
        fs::path output_path = fs::path(output_dir) / output_name;

        ofstream out(output_path);
        out << html;
        out.close();

        file_names.push_back(output_name);
        cout << "Generated: " << output_path.string() << endl;
    }

    // Generate index
    stringstream index_content;
    index_content << "<nav>\n";

    for (const auto& name : file_names) {
        string display = name.substr(0, name.size() - 5);  // Remove .html
        index_content << "  <a href=\"" << name << "\">" << display << "</a>\n";
    }

    index_content << "</nav>\n";
    index_content << "<p>Select a module from the navigation above.</p>\n";

    string index_html = wrap_html("Documentation", index_content.str());
    fs::path index_path = fs::path(output_dir) / "index.html";

    ofstream index_out(index_path);
    index_out << index_html;
    index_out.close();

    cout << "Generated: " << index_path.string() << endl;
}
