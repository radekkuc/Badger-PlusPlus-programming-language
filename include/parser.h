#pragma once
#include "lexer.h"

enum class NodeType : uint8_t {
    VarDecl, Print, Assigment,
    Plus, Minus, Asterisk, Slash, Equal, Lparent, Rparen,
    Number, Variable
};

struct ASTNode {
    NodeType nodeType;
    std::string value;    
    ASTNode* left;
    ASTNode* right;

    ASTNode(NodeType nodeType, const std::string& value, ASTNode* left = nullptr, ASTNode* right = nullptr);
};

class Parser {
private:
    size_t currIndex_;
    std::vector<Token> tokens_;

public:
    explicit Parser(const std::vector<Token>& tokens);
    ASTNode* parseProgram(); // parsing whole program into AST
    ASTNode* parseStatement(); // parsing statements like let, print
    ASTNode* parseExpression(); // adds/subtracts factors
    ASTNode* parseTerm(); // divides/multiplies factors
    ASTNode* parseFactor(); // the smallest value we can get: x, 10, 10 + 5
    bool match(TokenType type);
    void advance();
    void printAST(ASTNode* node, int depth = 0);
};