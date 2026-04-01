#pragma once
#include "lexer.h"
#include <memory>

enum class NodeType : uint8_t {
    VarDecl, Print, Println, Assignment,
    Plus, Minus, Asterisk, Slash, Equal, Lparen, Rparen,
    Number, Variable, Bool, String
};

// Sub Branch
struct ASTNode {
    NodeType nodeType;
    std::string value;    
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;

    ASTNode(NodeType nodeType, const std::string& value, std::unique_ptr<ASTNode> left = nullptr, std::unique_ptr<ASTNode> right = nullptr);
};

class Parser {
private:
    size_t currIndex_;
    std::vector<Token> tokens_;

public:
    explicit Parser(const std::vector<Token>& tokens);
    std::vector<ASTNode*> parseProgram(); // parsing whole program into AST
    ASTNode* parseStatement(); // parsing statements like let, print
    ASTNode* parseExpression(); // adds/subtracts factors
    ASTNode* parseTerm(); // divides/multiplies factors
    ASTNode* parseFactor(); // the smallest value we can get: x, 10, 10 + 5
    bool match(TokenType type);
    void advance();
    void printAST(ASTNode* node, int depth = 0);
    void printAST(const std::vector<ASTNode*>& nodes);
    const char* tokenType(TokenType type);
    const char* nodeType(NodeType type);
};