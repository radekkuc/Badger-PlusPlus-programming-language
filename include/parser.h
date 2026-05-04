#pragma once
#include "lexer.h"
#include <memory>

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

class Parser {
private:
    std::vector<Token> tokens_;
    size_t currIndex_;

public:
    explicit Parser(const std::vector<Token>& tokens);
    std::vector<std::unique_ptr<ASTNode>> parseProgram(); // parsing whole program into AST
    std::unique_ptr<ASTNode> parseStatement(); // parsing statements like let, print
    std::unique_ptr<ASTNode> parseExpression(); 
    std::unique_ptr<ASTNode> parseAddSub();
    std::unique_ptr<ASTNode> parseTerm(); // divides/multiplies factors
    std::unique_ptr<ASTNode> parseFactor(); // the smallest value we can get: x, 10, 10 + 5

    std::unique_ptr<ASTNode> parseLetStatement();
    std::unique_ptr<ASTNode> parsePrintStatement();
    std::unique_ptr<ASTNode> parseAssignment();
    std::unique_ptr<ASTNode> parseIfStatement();
    std::unique_ptr<ASTNode> parseElseStatement();
    std::unique_ptr<ASTNode> parseWhileStatement();
    std::unique_ptr<ASTNode> parseBlock();

    std::unique_ptr<ASTNode> parseOr();
    std::unique_ptr<ASTNode> parseAnd();
    std::unique_ptr<ASTNode> parseEquality();
    std::unique_ptr<ASTNode> parseComparison();
    std::unique_ptr<ASTNode> parseUnary();

    Token peek();
    bool match(TokenType type);
    bool needSemicolon(NodeType type);
    const char* tokenType(TokenType type);
    const char* nodeType(NodeType type);
    void advance();
    void expect(TokenType token, const std::string& errorMessage);

    void printAST(const ASTNode* node, int depth = 0);
    void printAST(const std::vector<std::unique_ptr<ASTNode>>& nodes);  
};