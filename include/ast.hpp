#pragma once
#include <string>
#include <vector>
#include <memory>

using namespace std;

struct ASTNode {
    virtual ~ASTNode() = default;
};

// Literals
struct StringLiteral : ASTNode {
    string value;
    explicit StringLiteral(const string& v) : value(v) {}
};

struct NumberLiteral : ASTNode {
    string value;
    explicit NumberLiteral(const string& v) : value(v) {}
};

struct FloatLiteral : ASTNode {
    string value;
    explicit FloatLiteral(const string& v) : value(v) {}
};

struct BoolLiteral : ASTNode {
    bool value;
    explicit BoolLiteral(bool v) : value(v) {}
};

struct VariableRef : ASTNode {
    string name;
    explicit VariableRef(const string& n) : name(n) {}
};

// Expressions
struct BinaryExpr : ASTNode {
    string op;
    unique_ptr<ASTNode> left;
    unique_ptr<ASTNode> right;
};

struct FunctionCall : ASTNode {
    string name;
    vector<unique_ptr<ASTNode>> args;
    int line = 0;
};

// Statements
struct VariableDecl : ASTNode {
    string type;
    string name;
    unique_ptr<ASTNode> value;
};

struct ReturnStmt : ASTNode {
    unique_ptr<ASTNode> value;
};

// Function
struct FunctionParam {
    string type;
    string name;
};

struct FunctionDef : ASTNode {
    string name;
    vector<FunctionParam> params;
    string return_type;
    vector<unique_ptr<ASTNode>> body;
};

struct Program : ASTNode {
    vector<unique_ptr<FunctionDef>> functions;
};
