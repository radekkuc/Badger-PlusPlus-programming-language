#include "parser.h"
#include <stdexcept>
#include <iostream>

ASTNode::ASTNode(NodeType nodeType, const std::string& value, ASTNode* left, ASTNode* right) : 
nodeType(nodeType), value(value), left(left), right(right) {};

Parser::Parser(const std::vector<Token>& tokens) : tokens_(tokens), currIndex_(0) {}; 

// Implement some vector to store those nodes
ASTNode* Parser::parseProgram() {
    ASTNode* statementNode = nullptr;
    while(tokens_[currIndex_].type != TokenType::ENDOFFILE) {
        statementNode = parseStatement();
        if(!match(TokenType::SEMICOLON)) {
            throw std::runtime_error("Missing ; at the end");
        } 
        else advance();
    }
    return statementNode;
} 

ASTNode* Parser::parseStatement() {
    if(match(TokenType::LET)) {
        advance();
        if(match(TokenType::VARIABLE)) {
            Token variableToken = tokens_[currIndex_];
            advance();
            if(match(TokenType::EQUALS)) {
                advance();
                ASTNode* expressionNode = parseExpression();
                return new ASTNode(NodeType::VarDecl, variableToken.value, expressionNode);
            }
            else {
                return new ASTNode(NodeType::VarDecl, variableToken.value);
            }      
        }
        else throw std::runtime_error("Expected variable declaration but got: " + tokens_[currIndex_].value);
    }
    else if(match(TokenType::PRINT)) {
        advance();
        if(match(TokenType::LPAREN)) {
            advance();
            ASTNode* node = parseExpression();
            if(!match(TokenType::RPAREN)) {
                throw std::runtime_error("Missing closing paren");
            }
            advance();
            return new ASTNode(NodeType::Print, "", node);
        }
        throw std::runtime_error("Missing open paren");
    }
    else if(match(TokenType::VARIABLE)) {
        Token token = tokens_[currIndex_];
        advance();
        if(match(TokenType::EQUALS)) {
            advance();
            ASTNode* node = parseExpression();
            return new ASTNode(NodeType::Assignment, token.value, node);
        }
        throw std::runtime_error("Expected variable assigment but got: " + tokens_[currIndex_].value);
    }
    throw std::runtime_error("Unexpected statement: " + tokens_[currIndex_].value);
}

ASTNode* Parser::parseExpression() {
    ASTNode* left = parseTerm(); 
    while(match(TokenType::PLUS) || match(TokenType::MINUS)) {
        TokenType signToken = tokens_[currIndex_].type;
        advance();
        ASTNode* right = parseTerm();
        if(signToken == TokenType::PLUS) left = new ASTNode(NodeType::Plus, "Plus", left, right); 
        if(signToken == TokenType::MINUS) left = new ASTNode(NodeType::Minus, "Minus", left, right); 
    }
    return left;
}

ASTNode* Parser::parseTerm() {
    ASTNode* left = parseFactor();
    while(match(TokenType::ASTERISK) || match(TokenType::SLASH)) {
        TokenType signToken = tokens_[currIndex_].type;
        advance();
        
        ASTNode* right = parseFactor();

        if(signToken == TokenType::ASTERISK) left = new ASTNode(NodeType::Asterisk, "Asterisk", left, right);
        if(signToken == TokenType::SLASH) left = new ASTNode(NodeType::Slash, "Slash", left, right);
    } 
    return left;
}

ASTNode* Parser::parseFactor() {
    if(match(TokenType::NUMBER)) {
        ASTNode* node = new ASTNode(NodeType::Number, tokens_[currIndex_].value); 
        advance();
        return node;
    }

    if(match(TokenType::LPAREN)) {
        advance();
        ASTNode* node = parseExpression();
        if(!match(TokenType::RPAREN)) throw std::runtime_error("Missing closing paren");
        advance();
        return node;
    }

    if(match(TokenType::VARIABLE)) {
        ASTNode* node = new ASTNode(NodeType::Variable, tokens_[currIndex_].value);
        advance(); 
        return node; 
    }
    throw std::runtime_error("Unexpected token: " + tokens_[currIndex_].value);
}

bool Parser::match(TokenType type) {
    if(currIndex_ >= tokens_.size()) {
        std::cerr << "Cannot check token as it is out of bounds\n";
        return false;
    } 
    if(tokens_[currIndex_].type == type) return true;
    return false;
}

void Parser::advance() {
    currIndex_++;
    if(currIndex_ >= tokens_.size()) throw std::runtime_error("Cannot advance as currIndex is out of bounds");
}

void Parser::printAST(ASTNode* node, int depth) {
    if (!node) return;
    for (int i = 0; i < depth; i++) std::cout << "  ";

    std::cout << " (" << node->value << ")\n";
    if (node->left) {
        std::cout << std::string(depth * 2, ' ') << "Left: \n";
        printAST(node->left, depth + 1);
    }
    if (node->right) {
        std::cout << std::string(depth * 2, ' ') << "Right: \n";
        printAST(node->right, depth + 1);
    }
}