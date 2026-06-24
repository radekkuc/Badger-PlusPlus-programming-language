#pragma once
#include <memory>
#include <cstdint>
#include <string>
#include <vector>

enum class NodeType : uint8_t {
    VarDecl, Print, Println, Assignment,
    If, Else, While, Block, FunDecl, Return,
    Greater, Smaller, Or, And, Not, Comparison, NComparison,
    Plus, Minus, Asterisk, Slash, Equal, Lparen, Rparen,
    UnaryMinus, Number, Variable, Bool, String
};

class Compiler;

class ASTNode {
protected:
    NodeType nodeType;
    std::string value;
public:
    virtual void compile(Compiler& compiler) const = 0;
    ASTNode(NodeType nodeType, std::string value);
    NodeType getNodeType() const;
    virtual ~ASTNode() = default;
};

// + - * /
class BinaryNode : public ASTNode {
private:
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;
public:
    void compile(Compiler& compiler) const override;
    BinaryNode(NodeType nodeType, const std::string& value, std::unique_ptr<ASTNode> left = nullptr, std::unique_ptr<ASTNode> right = nullptr);
};

// ! u-
class UnaryNode : public ASTNode {
private:
    std::unique_ptr<ASTNode> operand;
public:
    void compile(Compiler& compiler) const override;
    UnaryNode(NodeType nodeType, const std::string& value, std::unique_ptr<ASTNode> operand);
};

// let x = 10, let x;
class VarDeclNode : public ASTNode {
private:    
    std::unique_ptr<ASTNode> expression;
public:
    void compile(Compiler& compiler) const override;
    VarDeclNode(NodeType nodeType, const std::string& value, std::unique_ptr<ASTNode> expression = nullptr);
};

class AssignmentNode : public ASTNode {
private:
    std::unique_ptr<ASTNode> expression;
public:
    void compile(Compiler& compiler) const override;
    AssignmentNode(NodeType nodeType, const std::string& value, std::unique_ptr<ASTNode> expression);
};

class PrintNode : public ASTNode {
private:
    std::unique_ptr<ASTNode> expression;
public:
    void compile(Compiler& compiler) const override;
    PrintNode(NodeType nodeType, const std::string& value, std::unique_ptr<ASTNode> expression);
};

class BlockNode : public ASTNode {
private:
    std::vector<std::unique_ptr<ASTNode>> statementNodes;
public:
    void compile(Compiler& compiler) const override;  
    BlockNode(NodeType nodeType, const std::string& value, std::vector<std::unique_ptr<ASTNode>> statementNodes = {});
};

class IfNode : public ASTNode {
private:
    std::unique_ptr<ASTNode> conditionNode;
    std::unique_ptr<ASTNode> blockNode;
    std::unique_ptr<ASTNode> elseBranch;
public:
    void compile(Compiler& compiler) const override;
    IfNode(NodeType nodeType, const std::string& value, std::unique_ptr<ASTNode> conditionNode, std::unique_ptr<ASTNode> blockNode, std::unique_ptr<ASTNode> elseBranch = nullptr);
};

class WhileNode : public ASTNode {
private:
    std::unique_ptr<ASTNode> conditionNode;
    std::unique_ptr<ASTNode> blockNode;
public:
    void compile(Compiler& compiler) const override;
    WhileNode(NodeType nodeType, const std::string& value, std::unique_ptr<ASTNode> conditionNode, std::unique_ptr<ASTNode> blockNode);
};

class FunDeclNode : public ASTNode {
private:
    std::string funName;
    std::vector<std::string> parameters;
    std::unique_ptr<ASTNode> blockNode;
public:
    void compile(Compiler& compiler) const override;
    FunDeclNode(NodeType nodeType, const std::string& value, const std::string& funName, std::vector<std::string> parameters, std::unique_ptr<ASTNode> blockNode);
};

class FunCallNode : public ASTNode {

};

class ReturnNode : public ASTNode {
private:
    std::unique_ptr<ASTNode> retVal;
public:
    void compile(Compiler& compiler) const override;
    ReturnNode(NodeType nodeType, const std::string& value, std::unique_ptr<ASTNode> retVal);
};

// CONSTANT
class ValueNode : public ASTNode {
private:

public:
    void compile(Compiler& compiler) const override;
    ValueNode(NodeType nodeType, const std::string& value);
};

// LOAD
class VariableNode : public ASTNode {
private:

public:
    void compile(Compiler& compiler) const override;
    VariableNode(NodeType nodeType, const std::string& value);
};