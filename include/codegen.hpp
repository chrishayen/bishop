#pragma once
#include <string>
#include <memory>
#include "ast.hpp"

class CodeGen {
public:
    std::string generate(const std::unique_ptr<Program>& program);

private:
    std::string generate_function(const FunctionDef& func);
    std::string generate_statement(const ASTNode& node);
};
