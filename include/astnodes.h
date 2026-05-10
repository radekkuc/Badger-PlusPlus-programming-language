#pragma once
#include <memory>
#include <cstdint>
#include <string>
#include <vector>

enum class NodeType : uint8_t {
    VarDecl, Print, Println, Assignment,
    If, Else, While, Block,
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
    virtual ~ASTNode() = default;
};

class BinaryNode : public ASTNode {
private:
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;
public:
    void compile(Compiler& compiler) const override;
    BinaryNode(NodeType nodeType, const std::string& value, std::unique_ptr<ASTNode> left = nullptr, std::unique_ptr<ASTNode> right = nullptr);
};

class UnaryNode : public ASTNode {
private:
    std::unique_ptr<ASTNode> operand;
public:
    void compile(Compiler& compiler) const override;
    UnaryNode(NodeType nodeType, const std::string& value, std::unique_ptr<ASTNode> left);
};

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
public:
    void compile(Compiler& compiler) const override;
    IfNode(NodeType nodeType, const std::string& value, std::unique_ptr<ASTNode> conditionNode, std::unique_ptr<ASTNode> blockNode);
};

class WhileNode : public ASTNode {
private:
    std::unique_ptr<ASTNode> conditionNode;
    std::unique_ptr<ASTNode> blockNode;
public:
    void compile(Compiler& compiler) const override;
    WhileNode(NodeType nodeType, const std::string& value, std::unique_ptr<ASTNode> conditionNode, std::unique_ptr<ASTNode> blockNode);
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
}