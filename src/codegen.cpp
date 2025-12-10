#include "codegen.hpp"

std::string CodeGen::map_type(const std::string& nog_type) {
    if (nog_type == "int") return "int";
    if (nog_type == "str") return "std::string";
    if (nog_type == "bool") return "bool";
    if (nog_type == "char") return "char";
    if (nog_type == "f32") return "float";
    if (nog_type == "f64") return "double";
    if (nog_type == "u32") return "uint32_t";
    if (nog_type == "u64") return "uint64_t";
    return "void";
}

std::string CodeGen::generate(const std::unique_ptr<Program>& program, bool test_mode) {
    this->test_mode = test_mode;

    if (test_mode) {
        return generate_test_harness(program);
    }

    std::string output = "#include <iostream>\n#include <string>\n\n";
    for (const auto& func : program->functions) {
        output += generate_function(*func);
    }
    return output;
}

std::string CodeGen::generate_test_harness(const std::unique_ptr<Program>& program) {
    std::string output = "#include <iostream>\n#include <string>\n#include <cstdint>\n\n";

    output += "int _failures = 0;\n\n";
    output += "template<typename T>\n";
    output += "void _assert_eq(T a, T b, int line) {\n";
    output += "    if (a != b) {\n";
    output += "        std::cerr << \"line \" << line << \": FAIL: \" << a << \" != \" << b << std::endl;\n";
    output += "        _failures++;\n";
    output += "    }\n";
    output += "}\n\n";

    std::vector<std::string> test_names;
    for (const auto& func : program->functions) {
        if (func->name.rfind("test_", 0) == 0) {
            test_names.push_back(func->name);
        }
        output += generate_function(*func);
    }

    output += "\nint main() {\n";
    for (const auto& name : test_names) {
        output += "    " + name + "();\n";
    }
    output += "    // exit code signals pass/fail\n";
    output += "    return _failures;\n";
    output += "}\n";

    return output;
}

std::string CodeGen::generate_function(const FunctionDef& func) {
    std::string output;

    // Return type
    if (func.name == "main" && !test_mode) {
        output += "int";
    } else if (!func.return_type.empty()) {
        output += map_type(func.return_type);
    } else {
        output += "void";
    }

    output += " " + func.name + "(";

    // Parameters
    for (size_t i = 0; i < func.params.size(); i++) {
        if (i > 0) output += ", ";
        output += map_type(func.params[i].type) + " " + func.params[i].name;
    }

    output += ") {\n";

    for (const auto& stmt : func.body) {
        if (stmt) {
            output += "    " + generate_statement(*stmt) + "\n";
        }
    }

    if (func.name == "main" && !test_mode && func.return_type.empty()) {
        output += "    return 0;\n";
    }

    output += "}\n";
    return output;
}

std::string CodeGen::generate_statement(const ASTNode& node) {
    if (auto* decl = dynamic_cast<const VariableDecl*>(&node)) {
        std::string type_str = decl->type.empty() ? "auto" : map_type(decl->type);
        return type_str + " " + decl->name + " = " + generate_expression(*decl->value) + ";";
    }

    if (auto* ret = dynamic_cast<const ReturnStmt*>(&node)) {
        return "return " + generate_expression(*ret->value) + ";";
    }

    if (auto* call = dynamic_cast<const FunctionCall*>(&node)) {
        if (call->name == "print") {
            std::string output = "std::cout << ";
            for (size_t i = 0; i < call->args.size(); i++) {
                if (i > 0) output += " << ";
                output += generate_expression(*call->args[i]);
            }
            output += " << std::endl;";
            return output;
        }
        if (call->name == "assert_eq" && test_mode) {
            if (call->args.size() >= 2) {
                return "_assert_eq(" + generate_expression(*call->args[0]) + ", " +
                       generate_expression(*call->args[1]) + ", " +
                       std::to_string(call->line) + ");";
            }
        }
        // Generic function call
        std::string output = call->name + "(";
        for (size_t i = 0; i < call->args.size(); i++) {
            if (i > 0) output += ", ";
            output += generate_expression(*call->args[i]);
        }
        output += ");";
        return output;
    }

    return "";
}

std::string CodeGen::generate_expression(const ASTNode& node) {
    if (auto* str = dynamic_cast<const StringLiteral*>(&node)) {
        return "\"" + str->value + "\"";
    }

    if (auto* num = dynamic_cast<const NumberLiteral*>(&node)) {
        return num->value;
    }

    if (auto* flt = dynamic_cast<const FloatLiteral*>(&node)) {
        return flt->value;
    }

    if (auto* b = dynamic_cast<const BoolLiteral*>(&node)) {
        return b->value ? "true" : "false";
    }

    if (auto* var = dynamic_cast<const VariableRef*>(&node)) {
        return var->name;
    }

    if (auto* bin = dynamic_cast<const BinaryExpr*>(&node)) {
        return generate_expression(*bin->left) + " " + bin->op + " " + generate_expression(*bin->right);
    }

    if (auto* call = dynamic_cast<const FunctionCall*>(&node)) {
        std::string output = call->name + "(";
        for (size_t i = 0; i < call->args.size(); i++) {
            if (i > 0) output += ", ";
            output += generate_expression(*call->args[i]);
        }
        output += ")";
        return output;
    }

    return "";
}
