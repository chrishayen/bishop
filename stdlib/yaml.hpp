#pragma once
#include "parser/ast.hpp"
#include <memory>
#include <string>

namespace nog::stdlib {
std::unique_ptr<Program> create_yaml_module();
std::string generate_yaml_runtime();
}  // namespace nog::stdlib
