/**
 * @file markdown.cpp
 * @brief Built-in markdown module implementation.
 *
 * Creates the AST definitions for the markdown module.
 * The actual runtime is in runtime/markdown/markdown.hpp and included as a header.
 */

/**
 * @nog_fn parse
 * @module markdown
 * @description Parses a Markdown string into a Markdown document.
 * @param text str - The Markdown text to parse
 * @returns Markdown - Parsed Markdown document
 * @errors ParseError - If the Markdown is malformed
 * @example
 * import markdown;
 * doc := markdown.parse("# Hello World") or return;
 */

/**
 * @nog_fn to_html
 * @module markdown
 * @description Converts Markdown text directly to HTML.
 * @param text str - The Markdown text to convert
 * @returns str - HTML representation
 * @example
 * import markdown;
 * html := markdown.to_html("# Title");
 */

/**
 * @nog_fn to_text
 * @module markdown
 * @description Extracts plain text from Markdown, stripping formatting.
 * @param text str - The Markdown text
 * @returns str - Plain text content
 * @example
 * import markdown;
 * text := markdown.to_text("# Hello **World**");
 */

/**
 * @nog_fn stringify
 * @module markdown
 * @description Converts a Markdown document back to markdown text.
 * @param doc Markdown - The Markdown document
 * @returns str - Markdown text representation
 * @example
 * import markdown;
 * text := markdown.stringify(doc);
 */

/**
 * @nog_struct Markdown
 * @module markdown
 * @description A parsed Markdown document or node. Nodes can be documents,
 *              headings, paragraphs, lists, code blocks, blockquotes, links,
 *              images, or text. Document nodes have children accessible via
 *              the children field.
 * @example
 * doc := markdown.parse("# Title\n\nParagraph") or return;
 * if !doc.children.is_empty() {
 *     first := doc.children[0];
 *     if first.is_heading() {
 *         level := first.heading_level();
 *     }
 * }
 */

/**
 * @nog_method to_html
 * @type Markdown
 * @description Converts the Markdown node/document to HTML.
 * @returns str - HTML representation
 * @example
 * doc := markdown.parse("# Title") or return;
 * html := doc.to_html();
 */

/**
 * @nog_method get_text
 * @type Markdown
 * @description Extracts plain text content from the node.
 * @returns str - Plain text content
 * @example
 * text := doc.get_text();
 */

/**
 * @nog_method is_heading
 * @type Markdown
 * @description Checks if this node is a heading.
 * @returns bool - True if heading
 * @example
 * if node.is_heading() { level := node.heading_level(); }
 */

/**
 * @nog_method is_paragraph
 * @type Markdown
 * @description Checks if this node is a paragraph.
 * @returns bool - True if paragraph
 */

/**
 * @nog_method is_list
 * @type Markdown
 * @description Checks if this node is a list (ordered or unordered).
 * @returns bool - True if list
 */

/**
 * @nog_method is_code_block
 * @type Markdown
 * @description Checks if this node is a fenced code block.
 * @returns bool - True if code block
 * @example
 * if node.is_code_block() { code := node.get_code(); }
 */

/**
 * @nog_method is_blockquote
 * @type Markdown
 * @description Checks if this node is a blockquote.
 * @returns bool - True if blockquote
 */

/**
 * @nog_method is_link
 * @type Markdown
 * @description Checks if this node is a link.
 * @returns bool - True if link
 * @example
 * if node.is_link() { url := node.get_url(); }
 */

/**
 * @nog_method is_image
 * @type Markdown
 * @description Checks if this node is an image.
 * @returns bool - True if image
 * @example
 * if node.is_image() { alt := node.get_alt(); }
 */

/**
 * @nog_method heading_level
 * @type Markdown
 * @description Returns the heading level (1-6) for heading nodes.
 * @returns int - Heading level, or 0 if not a heading
 */

/**
 * @nog_method get_code
 * @type Markdown
 * @description Gets the code content from a code block.
 * @returns str - Code content
 */

/**
 * @nog_method get_language
 * @type Markdown
 * @description Gets the language identifier from a code block.
 * @returns str - Language string (e.g., "python", "javascript")
 */

/**
 * @nog_method get_url
 * @type Markdown
 * @description Gets the URL from a link or image node.
 * @returns str - URL string
 */

/**
 * @nog_method get_alt
 * @type Markdown
 * @description Gets the alt text from an image node.
 * @returns str - Alt text
 */

/**
 * @nog_method is_ordered_list
 * @type Markdown
 * @description Checks if a list node is ordered (numbered).
 * @returns bool - True if ordered list
 */

#include "markdown.hpp"

using namespace std;

namespace nog::stdlib {

/**
 * Creates the AST for the built-in markdown module.
 */
unique_ptr<Program> create_markdown_module() {
    auto program = make_unique<Program>();

    // Markdown :: struct { children: list<Markdown> }
    // A unified node type - documents have children, leaf nodes don't
    auto markdown_struct = make_unique<StructDef>();
    markdown_struct->name = "Markdown";
    markdown_struct->visibility = Visibility::Public;
    markdown_struct->fields.push_back({"children", "list<markdown.Markdown>", ""});
    program->structs.push_back(move(markdown_struct));

    // fn parse(str text) -> markdown.Markdown or err
    auto parse_fn = make_unique<FunctionDef>();
    parse_fn->name = "parse";
    parse_fn->visibility = Visibility::Public;
    parse_fn->params.push_back({"str", "text"});
    parse_fn->return_type = "markdown.Markdown";
    parse_fn->error_type = "err";
    program->functions.push_back(move(parse_fn));

    // fn to_html(str text) -> str
    auto to_html_fn = make_unique<FunctionDef>();
    to_html_fn->name = "to_html";
    to_html_fn->visibility = Visibility::Public;
    to_html_fn->params.push_back({"str", "text"});
    to_html_fn->return_type = "str";
    program->functions.push_back(move(to_html_fn));

    // fn to_text(str text) -> str
    auto to_text_fn = make_unique<FunctionDef>();
    to_text_fn->name = "to_text";
    to_text_fn->visibility = Visibility::Public;
    to_text_fn->params.push_back({"str", "text"});
    to_text_fn->return_type = "str";
    program->functions.push_back(move(to_text_fn));

    // fn stringify(markdown.Markdown doc) -> str
    auto stringify_fn = make_unique<FunctionDef>();
    stringify_fn->name = "stringify";
    stringify_fn->visibility = Visibility::Public;
    stringify_fn->params.push_back({"markdown.Markdown", "doc"});
    stringify_fn->return_type = "str";
    program->functions.push_back(move(stringify_fn));

    // ===== Markdown Methods =====

    // Markdown :: to_html(self) -> str
    auto md_to_html = make_unique<MethodDef>();
    md_to_html->struct_name = "Markdown";
    md_to_html->name = "to_html";
    md_to_html->visibility = Visibility::Public;
    md_to_html->params.push_back({"markdown.Markdown", "self"});
    md_to_html->return_type = "str";
    program->methods.push_back(move(md_to_html));

    // Markdown :: get_text(self) -> str
    auto get_text = make_unique<MethodDef>();
    get_text->struct_name = "Markdown";
    get_text->name = "get_text";
    get_text->visibility = Visibility::Public;
    get_text->params.push_back({"markdown.Markdown", "self"});
    get_text->return_type = "str";
    program->methods.push_back(move(get_text));

    // Markdown :: is_heading(self) -> bool
    auto is_heading = make_unique<MethodDef>();
    is_heading->struct_name = "Markdown";
    is_heading->name = "is_heading";
    is_heading->visibility = Visibility::Public;
    is_heading->params.push_back({"markdown.Markdown", "self"});
    is_heading->return_type = "bool";
    program->methods.push_back(move(is_heading));

    // Markdown :: is_paragraph(self) -> bool
    auto is_paragraph = make_unique<MethodDef>();
    is_paragraph->struct_name = "Markdown";
    is_paragraph->name = "is_paragraph";
    is_paragraph->visibility = Visibility::Public;
    is_paragraph->params.push_back({"markdown.Markdown", "self"});
    is_paragraph->return_type = "bool";
    program->methods.push_back(move(is_paragraph));

    // Markdown :: is_list(self) -> bool
    auto is_list = make_unique<MethodDef>();
    is_list->struct_name = "Markdown";
    is_list->name = "is_list";
    is_list->visibility = Visibility::Public;
    is_list->params.push_back({"markdown.Markdown", "self"});
    is_list->return_type = "bool";
    program->methods.push_back(move(is_list));

    // Markdown :: is_code_block(self) -> bool
    auto is_code_block = make_unique<MethodDef>();
    is_code_block->struct_name = "Markdown";
    is_code_block->name = "is_code_block";
    is_code_block->visibility = Visibility::Public;
    is_code_block->params.push_back({"markdown.Markdown", "self"});
    is_code_block->return_type = "bool";
    program->methods.push_back(move(is_code_block));

    // Markdown :: is_blockquote(self) -> bool
    auto is_blockquote = make_unique<MethodDef>();
    is_blockquote->struct_name = "Markdown";
    is_blockquote->name = "is_blockquote";
    is_blockquote->visibility = Visibility::Public;
    is_blockquote->params.push_back({"markdown.Markdown", "self"});
    is_blockquote->return_type = "bool";
    program->methods.push_back(move(is_blockquote));

    // Markdown :: is_link(self) -> bool
    auto is_link = make_unique<MethodDef>();
    is_link->struct_name = "Markdown";
    is_link->name = "is_link";
    is_link->visibility = Visibility::Public;
    is_link->params.push_back({"markdown.Markdown", "self"});
    is_link->return_type = "bool";
    program->methods.push_back(move(is_link));

    // Markdown :: is_image(self) -> bool
    auto is_image = make_unique<MethodDef>();
    is_image->struct_name = "Markdown";
    is_image->name = "is_image";
    is_image->visibility = Visibility::Public;
    is_image->params.push_back({"markdown.Markdown", "self"});
    is_image->return_type = "bool";
    program->methods.push_back(move(is_image));

    // Markdown :: heading_level(self) -> int
    auto heading_level = make_unique<MethodDef>();
    heading_level->struct_name = "Markdown";
    heading_level->name = "heading_level";
    heading_level->visibility = Visibility::Public;
    heading_level->params.push_back({"markdown.Markdown", "self"});
    heading_level->return_type = "int";
    program->methods.push_back(move(heading_level));

    // Markdown :: get_code(self) -> str
    auto get_code = make_unique<MethodDef>();
    get_code->struct_name = "Markdown";
    get_code->name = "get_code";
    get_code->visibility = Visibility::Public;
    get_code->params.push_back({"markdown.Markdown", "self"});
    get_code->return_type = "str";
    program->methods.push_back(move(get_code));

    // Markdown :: get_language(self) -> str
    auto get_language = make_unique<MethodDef>();
    get_language->struct_name = "Markdown";
    get_language->name = "get_language";
    get_language->visibility = Visibility::Public;
    get_language->params.push_back({"markdown.Markdown", "self"});
    get_language->return_type = "str";
    program->methods.push_back(move(get_language));

    // Markdown :: get_url(self) -> str
    auto get_url = make_unique<MethodDef>();
    get_url->struct_name = "Markdown";
    get_url->name = "get_url";
    get_url->visibility = Visibility::Public;
    get_url->params.push_back({"markdown.Markdown", "self"});
    get_url->return_type = "str";
    program->methods.push_back(move(get_url));

    // Markdown :: get_alt(self) -> str
    auto get_alt = make_unique<MethodDef>();
    get_alt->struct_name = "Markdown";
    get_alt->name = "get_alt";
    get_alt->visibility = Visibility::Public;
    get_alt->params.push_back({"markdown.Markdown", "self"});
    get_alt->return_type = "str";
    program->methods.push_back(move(get_alt));

    // Markdown :: is_ordered_list(self) -> bool
    auto is_ordered_list = make_unique<MethodDef>();
    is_ordered_list->struct_name = "Markdown";
    is_ordered_list->name = "is_ordered_list";
    is_ordered_list->visibility = Visibility::Public;
    is_ordered_list->params.push_back({"markdown.Markdown", "self"});
    is_ordered_list->return_type = "bool";
    program->methods.push_back(move(is_ordered_list));

    return program;
}

/**
 * Returns empty - markdown.hpp is included at the top of generated code
 * for precompiled header support.
 */
string generate_markdown_runtime() {
    return "";
}

}  // namespace nog::stdlib
