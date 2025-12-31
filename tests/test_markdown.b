// Tests for the markdown module

import markdown;

// Test to_html for heading
fn test_to_html_heading() {
    html := markdown.to_html("# Title");
    assert_eq(html.contains("<h1>"), true);
    assert_eq(html.contains("Title"), true);
    assert_eq(html.contains("</h1>"), true);
}

// Test to_html for paragraph
fn test_to_html_paragraph() {
    html := markdown.to_html("Hello world");
    assert_eq(html.contains("<p>"), true);
    assert_eq(html.contains("Hello world"), true);
    assert_eq(html.contains("</p>"), true);
}

// Test to_html for bold
fn test_to_html_bold() {
    html := markdown.to_html("This is **bold** text");
    assert_eq(html.contains("<strong>bold</strong>"), true);
}

// Test to_html for italic (maddy uses <i> tag)
fn test_to_html_italic() {
    html := markdown.to_html("This is *italic* text");
    assert_eq(html.contains("<i>italic</i>"), true);
}

// Test to_html for inline code
fn test_to_html_inline_code() {
    html := markdown.to_html("Use `printf` function");
    assert_eq(html.contains("<code>printf</code>"), true);
}

// Test to_html for link
fn test_to_html_link() {
    html := markdown.to_html("[Click](https://example.com)");
    assert_eq(html.contains("<a href="), true);
    assert_eq(html.contains("https://example.com"), true);
}

// Test to_html for image
fn test_to_html_image() {
    html := markdown.to_html("![Alt](image.png)");
    assert_eq(html.contains("<img"), true);
    assert_eq(html.contains("image.png"), true);
}

// Test to_html for list
fn test_to_html_list() {
    html := markdown.to_html("- One\n- Two");
    assert_eq(html.contains("<ul>"), true);
    assert_eq(html.contains("<li>"), true);
    assert_eq(html.contains("</ul>"), true);
}

// Test to_html for code block
fn test_to_html_code_block() {
    html := markdown.to_html("```\nprint()\n```");
    assert_eq(html.contains("<pre><code>"), true);
    assert_eq(html.contains("</code></pre>"), true);
}

// Note: Maddy doesn't support blockquote syntax

// Test code block contains content (maddy doesn't escape HTML in code)
fn test_to_html_code_content() {
    html := markdown.to_html("```\nprint()\n```");
    assert_eq(html.contains("print()"), true);
}

// Test to_text
fn test_get_text() {
    text := markdown.to_text("# Hello World");
    assert_eq(text.contains("Hello"), true);
}

// Test parse returns a document
fn test_parse() {
    doc := markdown.parse("# Title") or return;
    html := doc.to_html();
    assert_eq(html.contains("<h1>"), true);
}

// Test stringify
fn test_stringify() {
    doc := markdown.parse("# Hello") or return;
    text := markdown.stringify(doc);
    assert_eq(text.contains("Hello"), true);
}

// Test complex document
fn test_complex_document() {
    md := "# Title\n\nParagraph with **bold**.\n\n- Item 1\n- Item 2";
    doc := markdown.parse(md) or return;
    html := doc.to_html();
    assert_eq(html.contains("<h1>"), true);
    assert_eq(html.contains("bold"), true);
    assert_eq(html.contains("<ul>"), true);
}
