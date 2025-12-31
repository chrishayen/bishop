/**
 * @file regex.cpp
 * @brief Built-in regex module implementation.
 *
 * Creates the AST definitions for the regex module.
 * The actual runtime is in runtime/regex/regex.hpp and uses std::regex.
 */

/**
 * @bishop_fn compile
 * @module regex
 * @description Compiles a regular expression pattern.
 * @param pattern str - The regex pattern to compile
 * @returns regex.Regex or err - Compiled regex or error if pattern is invalid
 * @example
 * import regex;
 * re := regex.compile(r"(\d+)-(\d+)") or return;
 */

/**
 * @bishop_fn split
 * @module regex
 * @description Splits a string by a regex pattern.
 * @param pattern str - The regex pattern to split on
 * @param text str - The text to split
 * @returns List<str> or err - List of parts, or error if pattern is invalid
 * @example
 * import regex;
 * parts := regex.split(r"\s+", "a  b   c") or return;
 * // parts == ["a", "b", "c"]
 */

/**
 * @bishop_struct regex.Match
 * @module regex
 * @description A match result containing matched text and capture groups.
 * @field text str - The matched text (empty if no match)
 * @field start int - Start index of match (-1 if no match)
 * @field end int - End index of match (-1 if no match)
 * @field groups List<str> - Capture groups (index 0 is full match)
 */

/**
 * @bishop_method found
 * @type regex.Match
 * @description Check if this match is valid (found a match).
 * @returns bool - True if a match was found
 * @example
 * m := re.find("text");
 * if m.found() { print(m.text); }
 */

/**
 * @bishop_method group
 * @type regex.Match
 * @description Get a capture group by index.
 * @param n int - Group index (0 is full match, 1+ are capture groups)
 * @returns str - The captured text, or empty string if index out of bounds
 * @example
 * m := re.find("100-200");
 * m.group(0);  // "100-200"
 * m.group(1);  // "100"
 */

/**
 * @bishop_struct regex.Regex
 * @module regex
 * @description A compiled regular expression.
 */

/**
 * @bishop_method matches
 * @type regex.Regex
 * @description Check if the entire string matches the pattern.
 * @param text str - The text to match against
 * @returns bool - True if the entire string matches
 * @example
 * re := regex.compile(r"\d+") or return;
 * re.matches("123");     // true
 * re.matches("abc123");  // false
 */

/**
 * @bishop_method contains
 * @type regex.Regex
 * @description Check if the string contains the pattern anywhere.
 * @param text str - The text to search
 * @returns bool - True if the pattern is found anywhere
 * @example
 * re := regex.compile(r"\d+") or return;
 * re.contains("abc123def");  // true
 */

/**
 * @bishop_method find
 * @type regex.Regex
 * @description Find the first match in the string.
 * @param text str - The text to search
 * @returns regex.Match - Match result (with start=-1 if no match)
 * @example
 * re := regex.compile(r"(\d+)") or return;
 * m := re.find("abc123def");
 * m.text;   // "123"
 * m.start;  // 3
 */

/**
 * @bishop_method find_all
 * @type regex.Regex
 * @description Find all matches in the string.
 * @param text str - The text to search
 * @returns List<regex.Match> - All matches (empty list if none)
 * @example
 * re := regex.compile(r"\d+") or return;
 * matches := re.find_all("a1b2c3");
 * // matches contains Match for "1", "2", "3"
 */

/**
 * @bishop_method replace
 * @type regex.Regex
 * @description Replace the first match with the replacement string.
 * @param text str - The text to search
 * @param replacement str - Replacement string ($0 = full match, $1-$99 = capture groups, $$ = literal $)
 * @returns str - The modified string (original if no match)
 * @example
 * re := regex.compile(r"(\d+)-(\d+)") or return;
 * re.replace("123-456", "$2-$1");  // "456-123"
 */

/**
 * @bishop_method replace_all
 * @type regex.Regex
 * @description Replace all matches with the replacement string.
 * @param text str - The text to search
 * @param replacement str - Replacement string ($0 = full match, $1-$99 = capture groups, $$ = literal $)
 * @returns str - The modified string (original if no matches)
 * @example
 * re := regex.compile(r"\d+") or return;
 * re.replace_all("a1b2c3", "X");  // "aXbXcX"
 */

#include "regex.hpp"

using namespace std;

namespace bishop::stdlib {

/**
 * Creates the AST for the built-in regex module.
 */
unique_ptr<Program> create_regex_module() {
    auto program = make_unique<Program>();

    // ==========================================
    // Match struct
    // ==========================================
    auto match_struct = make_unique<StructDef>();
    match_struct->name = "Match";
    match_struct->visibility = Visibility::Public;
    match_struct->fields.push_back({"text", "str", ""});
    match_struct->fields.push_back({"start", "int", ""});
    match_struct->fields.push_back({"end", "int", ""});
    match_struct->fields.push_back({"groups", "List<str>", ""});
    program->structs.push_back(move(match_struct));

    // Match::found(self) -> bool
    auto match_found = make_unique<MethodDef>();
    match_found->struct_name = "Match";
    match_found->name = "found";
    match_found->visibility = Visibility::Public;
    match_found->params.push_back({"Match", "self"});
    match_found->return_type = "bool";
    program->methods.push_back(move(match_found));

    // Match::group(self, int n) -> str
    auto match_group = make_unique<MethodDef>();
    match_group->struct_name = "Match";
    match_group->name = "group";
    match_group->visibility = Visibility::Public;
    match_group->params.push_back({"Match", "self"});
    match_group->params.push_back({"int", "n"});
    match_group->return_type = "str";
    program->methods.push_back(move(match_group));

    // ==========================================
    // Regex struct
    // ==========================================
    auto regex_struct = make_unique<StructDef>();
    regex_struct->name = "Regex";
    regex_struct->visibility = Visibility::Public;
    program->structs.push_back(move(regex_struct));

    // Regex::matches(self, str text) -> bool
    auto regex_matches = make_unique<MethodDef>();
    regex_matches->struct_name = "Regex";
    regex_matches->name = "matches";
    regex_matches->visibility = Visibility::Public;
    regex_matches->params.push_back({"Regex", "self"});
    regex_matches->params.push_back({"str", "text"});
    regex_matches->return_type = "bool";
    program->methods.push_back(move(regex_matches));

    // Regex::contains(self, str text) -> bool
    auto regex_contains = make_unique<MethodDef>();
    regex_contains->struct_name = "Regex";
    regex_contains->name = "contains";
    regex_contains->visibility = Visibility::Public;
    regex_contains->params.push_back({"Regex", "self"});
    regex_contains->params.push_back({"str", "text"});
    regex_contains->return_type = "bool";
    program->methods.push_back(move(regex_contains));

    // Regex::find(self, str text) -> regex.Match
    auto regex_find = make_unique<MethodDef>();
    regex_find->struct_name = "Regex";
    regex_find->name = "find";
    regex_find->visibility = Visibility::Public;
    regex_find->params.push_back({"Regex", "self"});
    regex_find->params.push_back({"str", "text"});
    regex_find->return_type = "regex.Match";
    program->methods.push_back(move(regex_find));

    // Regex::find_all(self, str text) -> List<regex.Match>
    auto regex_find_all = make_unique<MethodDef>();
    regex_find_all->struct_name = "Regex";
    regex_find_all->name = "find_all";
    regex_find_all->visibility = Visibility::Public;
    regex_find_all->params.push_back({"Regex", "self"});
    regex_find_all->params.push_back({"str", "text"});
    regex_find_all->return_type = "List<regex.Match>";
    program->methods.push_back(move(regex_find_all));

    // Regex::replace(self, str text, str replacement) -> str
    auto regex_replace = make_unique<MethodDef>();
    regex_replace->struct_name = "Regex";
    regex_replace->name = "replace";
    regex_replace->visibility = Visibility::Public;
    regex_replace->params.push_back({"Regex", "self"});
    regex_replace->params.push_back({"str", "text"});
    regex_replace->params.push_back({"str", "replacement"});
    regex_replace->return_type = "str";
    program->methods.push_back(move(regex_replace));

    // Regex::replace_all(self, str text, str replacement) -> str
    auto regex_replace_all = make_unique<MethodDef>();
    regex_replace_all->struct_name = "Regex";
    regex_replace_all->name = "replace_all";
    regex_replace_all->visibility = Visibility::Public;
    regex_replace_all->params.push_back({"Regex", "self"});
    regex_replace_all->params.push_back({"str", "text"});
    regex_replace_all->params.push_back({"str", "replacement"});
    regex_replace_all->return_type = "str";
    program->methods.push_back(move(regex_replace_all));

    // ==========================================
    // Module functions
    // ==========================================

    // fn compile(str pattern) -> regex.Regex or err
    auto compile_fn = make_unique<FunctionDef>();
    compile_fn->name = "compile";
    compile_fn->visibility = Visibility::Public;
    compile_fn->params.push_back({"str", "pattern"});
    compile_fn->return_type = "regex.Regex";
    compile_fn->error_type = "err";
    program->functions.push_back(move(compile_fn));

    // fn split(str pattern, str text) -> List<str> or err
    auto split_fn = make_unique<FunctionDef>();
    split_fn->name = "split";
    split_fn->visibility = Visibility::Public;
    split_fn->params.push_back({"str", "pattern"});
    split_fn->params.push_back({"str", "text"});
    split_fn->return_type = "List<str>";
    split_fn->error_type = "err";
    program->functions.push_back(move(split_fn));

    return program;
}

/**
 * Returns empty - regex.hpp is included at the top of generated code.
 */
string generate_regex_runtime() {
    return "";
}

}  // namespace bishop::stdlib
