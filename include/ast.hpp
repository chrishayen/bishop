#pragma once
#include <string>
#include <vector>
#include <memory>

struct ASTNode {
    virtual ~ASTNode() = default;
};

// Literals
struct StringLiteral : ASTNode {
    std::string value;
    explicit StringLiteral(const std::string& v) : value(v) {}
};

struct NumberLiteral : ASTNode {
    std::string value;
    explicit NumberLiteral(const std::string& v) : value(v) {}
};

struct FloatLiteral : ASTNode {
    std::string value;
    explicit FloatLiteral(const std::string& v) : value(v) {}
};

struct BoolLiteral : ASTNode {
    bool value;
    explicit BoolLiteral(bool v) : value(v) {}
};

struct VariableRef : ASTNode {
    std::string name;
    explicit VariableRef(const std::string& n) : name(n) {}
};

// Expressions
struct BinaryExpr : ASTNode {
    std::string op;
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;
};

struct FunctionCall : ASTNode {
    std::string name;
    std::vector<std::unique_ptr<ASTNode>> args;
    int line = 0;
};

// Statements
struct VariableDecl : ASTNode {
    std::string type;  // empty for inferred (:=)
    std::string name;
    std::unique_ptr<ASTNode> value;
};

struct ReturnStmt : ASTNode {
    std::unique_ptr<ASTNode> value;
};

// Function
struct FunctionParam {
    std::string type;
    std::string name;
};

struct FunctionDef : ASTNode {
    std::string name;
    std::vector<FunctionParam> params;
    std::string return_type;  // empty for void
    std::vector<std::unique_ptr<ASTNode>> body;
};

struct Program : ASTNode {
    std::vector<std::unique_ptr<FunctionDef>> functions;
};
