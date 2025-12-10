#include <gtest/gtest.h>
#include "lexer.hpp"
#include "parser.hpp"
#include "codegen.hpp"

TEST(Types, VariableDeclaration) {
    std::string source = R"(
fn main() {
    int x = 42
    str name = "hello"
    bool flag = true
    f64 pi = 3.14
}
)";

    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto ast = parser.parse();

    CodeGen codegen;
    std::string output = codegen.generate(ast);

    std::string expected = R"(#include <iostream>
#include <string>

int main() {
    int x = 42;
    std::string name = "hello";
    bool flag = true;
    double pi = 3.14;
    return 0;
}
)";

    EXPECT_EQ(output, expected);
}

TEST(Types, InferredDeclaration) {
    std::string source = R"(
fn main() {
    x := 42
    name := "hello"
}
)";

    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto ast = parser.parse();

    CodeGen codegen;
    std::string output = codegen.generate(ast);

    std::string expected = R"(#include <iostream>
#include <string>

int main() {
    auto x = 42;
    auto name = "hello";
    return 0;
}
)";

    EXPECT_EQ(output, expected);
}

TEST(Types, FunctionWithParams) {
    std::string source = R"(
fn add(int a, int b) -> int {
    return a + b
}
)";

    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto ast = parser.parse();

    CodeGen codegen;
    std::string output = codegen.generate(ast);

    std::string expected = R"(#include <iostream>
#include <string>

int add(int a, int b) {
    return a + b;
}
)";

    EXPECT_EQ(output, expected);
}
