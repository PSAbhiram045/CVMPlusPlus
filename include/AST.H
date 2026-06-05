#pragma once
#include "Token.h"
#include <memory>
#include <vector>
#include <string>

struct ASTNode {
    virtual ~ASTNode() = default;
};

// --- LEAF NODES (Values) ---
struct NumberNode : public ASTNode {
    int value;
    NumberNode(int val) : value(val) {}
};



struct VariableNode : public ASTNode {
    std::string name;
    VariableNode(const std::string& n) : name(n) {}
};

struct BoolNode : public ASTNode {
    bool value;
    BoolNode(bool val) : value(val) {}
};

// NEW: Represents a request for user input
struct InputNode : public ASTNode {
    // No data needed, it just tells the VM to pause and ask for input
};

// --- BRANCH NODES (Operations) ---
struct BinaryOpNode : public ASTNode {
    std::unique_ptr<ASTNode> left;
    Token op; 
    std::unique_ptr<ASTNode> right;

    BinaryOpNode(std::unique_ptr<ASTNode> l, Token opToken, std::unique_ptr<ASTNode> r)
        : left(std::move(l)), op(opToken), right(std::move(r)) {}
};

struct AssignmentNode : public ASTNode {
    std::string varName;
    std::unique_ptr<ASTNode> value; 

    AssignmentNode(const std::string& name, std::unique_ptr<ASTNode> val)
        : varName(name), value(std::move(val)) {}
};

struct PrintNode : public ASTNode {
    std::unique_ptr<ASTNode> expression;
    PrintNode(std::unique_ptr<ASTNode> expr) : expression(std::move(expr)) {}
};

// --- CONTROL FLOW NODES ---
struct BlockNode : public ASTNode {
    std::vector<std::unique_ptr<ASTNode>> statements;
};

struct IfNode : public ASTNode {
    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<ASTNode> thenBranch;
    std::unique_ptr<ASTNode> elseBranch; 

    IfNode(std::unique_ptr<ASTNode> cond, std::unique_ptr<ASTNode> thenB, std::unique_ptr<ASTNode> elseB)
        : condition(std::move(cond)), thenBranch(std::move(thenB)), elseBranch(std::move(elseB)) {}
};

struct WhileNode : public ASTNode {
    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<ASTNode> body;

    WhileNode(std::unique_ptr<ASTNode> cond, std::unique_ptr<ASTNode> b)
        : condition(std::move(cond)), body(std::move(b)) {}
};