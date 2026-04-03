#include "parser.h"
#include <stdexcept>
#include <iostream>

ASTNode::ASTNode(NodeType nodeType, const std::string& value, std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right, std::vector<std::unique_ptr<ASTNode>> statements) : 
nodeType(nodeType), value(value), left(std::move(left)), right(std::move(right)), statements(std::move(statements)) {};


Parser::Parser(const std::vector<Token>& tokens) : currIndex_{}, tokens_(tokens) {}; 

std::vector<std::unique_ptr<ASTNode>> Parser::parseProgram() {
    std::unique_ptr<ASTNode> statementNode = nullptr;
    std::vector<std::unique_ptr<ASTNode>> nodes{};
    while(tokens_[currIndex_].type != TokenType::ENDOFFILE) {
        statementNode = parseStatement();
        if(needSemicolon(statementNode.get()->nodeType)) {
            if(!match(TokenType::SEMICOLON)) throw std::runtime_error("Missing ; at the end");
        }
        advance();
        nodes.emplace_back(std::move(statementNode));
    }
    return nodes;
} 

std::unique_ptr<ASTNode> Parser::parseStatement() {
    if(match(TokenType::LET)) {
        advance();
        if(match(TokenType::VARIABLE)) {
            Token variableToken = tokens_[currIndex_];
            advance();
            if(match(TokenType::EQUALS)) {
                advance();
                std::unique_ptr<ASTNode> expressionNode = parseExpression();
                return std::make_unique<ASTNode>(NodeType::VarDecl, variableToken.value, std::move(expressionNode));            
            }
            else {
                return std::make_unique<ASTNode>(NodeType::VarDecl, variableToken.value);
            }      
        }
        else throw std::runtime_error("Expected variable declaration but got: " + tokens_[currIndex_].value);
    }
    else if(match(TokenType::PRINT) || match(TokenType::PRINTLN)) {
        TokenType printToken = tokens_[currIndex_].type;
        advance();
        if(match(TokenType::LPAREN)) {
            advance();
            std::unique_ptr<ASTNode> node = parseExpression();
            if(!match(TokenType::RPAREN)) {
                throw std::runtime_error("Missing closing paren");
            }
            advance();
            if(printToken == TokenType::PRINT) return std::make_unique<ASTNode>(NodeType::Print, "Print", std::move(node));
            else if(printToken == TokenType::PRINTLN) return std::make_unique<ASTNode>(NodeType::Println, "Println", std::move(node));
        }
        throw std::runtime_error("Missing open paren");
    }
    else if(match(TokenType::VARIABLE)) {
        Token token = tokens_[currIndex_];
        advance();
        if(match(TokenType::EQUALS)) {
            advance();
            std::unique_ptr<ASTNode> node = parseExpression();
            return std::make_unique<ASTNode>(NodeType::Assignment, token.value, std::move(node));
        }
        throw std::runtime_error("Expected variable assigment but got: " + tokens_[currIndex_].value);
    }
    else if(match(TokenType::IF)) {
        std::cout << "Token here " << tokens_[currIndex_].value << std::endl;
        advance();
        if(match(TokenType::LPAREN)) {
            std::cout << "Token here " << tokens_[currIndex_].value << std::endl;
            advance();
            std::unique_ptr<ASTNode> conditionNode = parseExpression();
            if(match(TokenType::RPAREN)) {
                advance();
                if(match(TokenType::LCURLY)) {
                    std::vector<std::unique_ptr<ASTNode>> statementNodes;
                    advance();
                    while(!match(TokenType::RCURLY)) {
                        std::unique_ptr<ASTNode> bodyNode = parseStatement();
                        std::cout << "value here? " << bodyNode.get()->value << std::endl;
                        if(needSemicolon(bodyNode.get()->nodeType)) {
                            if(!match(TokenType::SEMICOLON)) throw std::runtime_error("Missing semicolon in if statement");
                        }
                        advance();
                        statementNodes.emplace_back(std::move(bodyNode));
                    }
                    if(!match(TokenType::RCURLY)) throw std::runtime_error("Missing closing curly brace in if statement");
                    advance();
                    std::unique_ptr<ASTNode> bodyNode = std::make_unique<ASTNode>(NodeType::Body, "Body", nullptr, nullptr, std::move(statementNodes));
                    return std::make_unique<ASTNode>(NodeType::If, "If", std::move(conditionNode), std::move(bodyNode));
                }
                throw std::runtime_error("Missing opening curly brace in if statement");
            } 
            throw std::runtime_error("Missing closing paren in if statement");
        }
        throw std::runtime_error("Missing opening paren in if statement");
    }
    else if(match(TokenType::ELSE)) {

    }
    throw std::runtime_error("Unexpected statement: " + tokens_[currIndex_].value);
}

std::unique_ptr<ASTNode> Parser::parseExpression() {
    std::unique_ptr<ASTNode> left = parseTerm(); 
    while(match(TokenType::PLUS) || match(TokenType::MINUS)) {
        TokenType signToken = tokens_[currIndex_].type;
        advance();
        std::unique_ptr<ASTNode> right = parseTerm();
        if(signToken == TokenType::PLUS) left = std::make_unique<ASTNode>(NodeType::Plus, "Plus", std::move(left), std::move(right));  
        if(signToken == TokenType::MINUS) left = std::make_unique<ASTNode>(NodeType::Minus, "Minus", std::move(left), std::move(right)); 
    }
    return left;
}

std::unique_ptr<ASTNode> Parser::parseTerm() {
    std::unique_ptr<ASTNode> left = parseFactor();
    while(match(TokenType::ASTERISK) || match(TokenType::SLASH)) {
        TokenType signToken = tokens_[currIndex_].type;
        advance();
        
        std::unique_ptr<ASTNode> right = parseFactor();

        if(signToken == TokenType::ASTERISK) left = std::make_unique<ASTNode>(NodeType::Asterisk, "Asterisk", std::move(left), std::move(right));
        if(signToken == TokenType::SLASH) left = std::make_unique<ASTNode>(NodeType::Slash, "Slash", std::move(left), std::move(right));
    } 
    return left;
}

std::unique_ptr<ASTNode> Parser::parseFactor() {
    if(match(TokenType::NUMBER)) {
        std::unique_ptr<ASTNode> node = std::make_unique<ASTNode>(NodeType::Number, tokens_[currIndex_].value); 
        advance();
        return node;
    }

    if(match(TokenType::LPAREN)) {
        advance();
        std::unique_ptr<ASTNode> node = parseExpression();
        if(!match(TokenType::RPAREN)) throw std::runtime_error("Missing closing paren");
        advance();
        return node;
    }

    if(match(TokenType::VARIABLE)) {
        std::unique_ptr<ASTNode> node = std::make_unique<ASTNode>(NodeType::Variable, tokens_[currIndex_].value); 
        advance(); 
        return node; 
    }

    if(match(TokenType::BOOL)) {
        std::unique_ptr<ASTNode> node = std::make_unique<ASTNode>(NodeType::Bool, tokens_[currIndex_].value);
        advance();
        return node;
    }

    if(match(TokenType::STRING)) {
        std::unique_ptr<ASTNode> node = std::make_unique<ASTNode>(NodeType::String, tokens_[currIndex_].value);
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

bool Parser::needSemicolon(NodeType type) {
    if(type == NodeType::If) return false;
    return true;
}

void Parser::advance() {
    currIndex_++;
    if(currIndex_ >= tokens_.size()) throw std::runtime_error("Cannot advance as currIndex is out of bounds");
}

void Parser::printAST(const ASTNode* node, int depth) {
    if (!node) return;
    for (int i = 0; i < depth; i++) std::cout << "  ";

    std::cout << " (" << node->value << ")\n";
    if (node->left) {
        std::cout << std::string(depth * 2, ' ') << "Left: \n";
        printAST(node->left.get(), depth + 1);
    }
    if (node->right) {
        std::cout << std::string(depth * 2, ' ') << "Right: \n";
        printAST(node->right.get(), depth + 1);
    }
}

void Parser::printAST(const std::vector<std::unique_ptr<ASTNode>>& nodes) {
    std::cout << "Program AST:\n";

    for (size_t i = 0; i < nodes.size(); ++i) {
        std::cout << "\nStatement " << i + 1 << ":\n";
        printAST(nodes[i].get(), 1);
    }
}


const char* Parser::tokenType(TokenType type) {
    switch(type) {
        case TokenType::ASTERISK:   return "ASTERISK";
        case TokenType::SLASH:      return "SLASH";
        case TokenType::PLUS:       return "PLUS";
        case TokenType::MINUS:      return "MINUS";
        case TokenType::LPAREN:     return "LPAREN";
        case TokenType::RPAREN:     return "RPAREN";
        case TokenType::EQUALS:     return "EQUALS";
        case TokenType::SEMICOLON:  return "SEMICOLON";

        case TokenType::NUMBER:     return "NUMBER";
        case TokenType::VARIABLE:   return "VARIABLE";
        case TokenType::ENDOFFILE:  return "ENDOFFILE";

        case TokenType::STRING:     return "STRING";
        case TokenType::BOOL:       return "BOOL";

        case TokenType::LET:        return "LET";
        case TokenType::PRINT:      return "PRINT";
    }
    return "UNKNOWN";
}

const char* Parser::nodeType(NodeType type) {
    switch (type) {
        case NodeType::VarDecl:    return "VarDecl";
        case NodeType::Print:      return "Print";
        case NodeType::Assignment: return "Assignment";

        case NodeType::Plus:       return "Plus";
        case NodeType::Minus:      return "Minus";
        case NodeType::Asterisk:   return "Asterisk";
        case NodeType::Slash:      return "Slash";
        case NodeType::Equal:      return "Equal";
        case NodeType::Lparen:     return "Lparen";
        case NodeType::Rparen:     return "Rparen";

        case NodeType::Number:     return "Number";
        case NodeType::Variable:   return "Variable";
        case NodeType::Bool:       return "Bool";
        case NodeType::String:     return "String";
    }

    return "Unknown"; 
}
