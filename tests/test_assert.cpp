#include <gtest/gtest.h>
#include "lexer.hpp"
#include "parser.hpp"
#include "codegen.hpp"

TEST(Assert, TranspilesAssertEq) {
    std::string source = R"(
fn test_greeting() {
    assert_eq("hello", "hello")
}
)";

    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto ast = parser.parse();

    CodeGen codegen;
    std::string output = codegen.generate(ast, true);

    std::string expected = R"(#include <iostream>
#include <string>
#include <cstdint>

int _failures = 0;

template<typename T>
void _assert_eq(T a, T b, int line) {
    if (a != b) {
        std::cerr << "line " << line << ": FAIL: " << a << " != " << b << std::endl;
        _failures++;
    }
}

void test_greeting() {
    _assert_eq("hello", "hello", 3);
}

int main() {
    test_greeting();
    // exit code signals pass/fail
    return _failures;
}
)";

    EXPECT_EQ(output, expected);
}
