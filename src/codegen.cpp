#include "codegen.hpp"

std::string CodeGen::generate(const std::unique_ptr<Program>& program) {
    std::string output = "#include <iostream>\n\n";

    for (const auto& func : program->functions) {
        output += generate_function(*func);
    }

    return output;
}

std::string CodeGen::generate_function(const FunctionDef& func) {
    std::string output;

    if (func.name == "main") {
        output += "int main() {\n";
    } else {
        output += "void " + func.name + "() {\n";
    }

    for (const auto& stmt : func.body) {
        if (stmt) {
            output += "    " + generate_statement(*stmt) + "\n";
        }
    }

    if (func.name == "main") {
        output += "    return 0;\n";
    }

    output += "}\n";
    return output;
}

std::string CodeGen::generate_statement(const ASTNode& node) {
    if (auto* call = dynamic_cast<const FunctionCall*>(&node)) {
        if (call->name == "print") {
            std::string output = "std::cout << ";
            for (size_t i = 0; i < call->args.size(); i++) {
                if (auto* str = dynamic_cast<const StringLiteral*>(call->args[i].get())) {
                    output += "\"" + str->value + "\"";
                }
            }
            output += " << std::endl;";
            return output;
        }
    }
    return "";
}
