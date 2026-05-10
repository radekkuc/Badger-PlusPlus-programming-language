#include "parser.h"
#include <stdexcept>
#include <iostream>
#include "utils.h"

Parser::Parser(const std::vector<Token>& tokens) : currIndex_{}, tokens_(tokens) {}; 

std::vector<std::unique_ptr<ASTNode>> Parser::parseProgram() {
    std::unique_ptr<ASTNode> statementNode = nullptr;
    std::vector<std::unique_ptr<ASTNode>> nodes{};
    while(peek().type != TokenType::ENDOFFILE) {
        statementNode = parseStatement();
        if(needSemicolon(statementNode.get()->nodeType)) expect(TokenType::SEMICOLON, "Missing ; at the end");
        advance();
        nodes.emplace_back(std::move(statementNode));
    }
    return nodes;
} 

std::unique_ptr<ASTNode> Parser::parseStatement() {
    if(match(TokenType::LET)) return parseLetStatement();
    if(match(TokenType::PRINT) || match(TokenType::PRINTLN)) return parsePrintStatement();  
    if(match(TokenType::VARIABLE)) return parseAssignment();
    if(match(TokenType::IF)) return parseIfStatement();
    if(match(TokenType::ELSE)) return parseElseStatement();
    if(match(TokenType::WHILE)) return parseWhileStatement();
    throw std::runtime_error("Unexpected statement: " + peek().value);
}

std::unique_ptr<ASTNode> Parser::parseExpression() {
    return parseOr();
}

std::unique_ptr<ASTNode> Parser::parseOr() {
    std::unique_ptr<ASTNode> left = parseAnd();
    while(match(TokenType::OR)) {
        advance();
        std::unique_ptr<ASTNode> right = parseAnd();
        left = std::make_unique<ASTNode>(NodeType::Or, "Or", std::move(left), std::move(right));
    }
    return left;
}

std::unique_ptr<ASTNode> Parser::parseAnd() {
    std::unique_ptr<ASTNode> left = parseEquality(); 
    while(match(TokenType::AND)) {
        advance();
        std::unique_ptr<ASTNode> right = parseEquality();
        left = std::make_unique<ASTNode>(NodeType::And, "And", std::move(left), std::move(right));
    }
    return left;
}

std::unique_ptr<ASTNode> Parser::parseEquality() {
    std::unique_ptr<ASTNode> left = parseComparison();
        while(match(TokenType::COMPARISON) || match(TokenType::NCOMPARISON)) {
        TokenType token = peek().type;
        advance();
        std::unique_ptr<ASTNode> right = parseComparison();
        if(token == TokenType::COMPARISON) left = std::make_unique<ASTNode>(NodeType::Comparison, "Comparison", std::move(left), std::move(right));
        if(token == TokenType::NCOMPARISON) left = std::make_unique<ASTNode>(NodeType::NComparison, "NComparison", std::move(left), std::move(right));
    }
    return left;
}

std::unique_ptr<ASTNode> Parser::parseComparison() {
    std::unique_ptr<ASTNode> left = parseAddSub();
    while(match(TokenType::GREATER) || match(TokenType::SMALLER)) {
        TokenType token = peek().type;
        advance();
        std::unique_ptr<ASTNode> right = parseAddSub();
        if(token == TokenType::GREATER) left = std::make_unique<ASTNode>(NodeType::Greater, "Greater", std::move(left), std::move(right));
        if(token == TokenType::SMALLER) left = std::make_unique<ASTNode>(NodeType::Smaller, "Smaller", std::move(left), std::move(right));
    }
    return left;
}

std::unique_ptr<ASTNode> Parser::parseAddSub() {
    std::unique_ptr<ASTNode> left = parseTerm(); 
    while(match(TokenType::PLUS) || match(TokenType::MINUS)) {
        TokenType signToken = peek().type;
        advance();
        std::unique_ptr<ASTNode> right = parseTerm();
        if(signToken == TokenType::PLUS) left = std::make_unique<ASTNode>(NodeType::Plus, "Plus", std::move(left), std::move(right));  
        if(signToken == TokenType::MINUS) left = std::make_unique<ASTNode>(NodeType::Minus, "Minus", std::move(left), std::move(right)); 
    }
    return left;
}

std::unique_ptr<ASTNode> Parser::parseTerm() {
    std::unique_ptr<ASTNode> left = parseUnary();
    while(match(TokenType::ASTERISK) || match(TokenType::SLASH)) {
        TokenType signToken = peek().type;
        advance();
        std::unique_ptr<ASTNode> right = parseUnary();
        if(signToken == TokenType::ASTERISK) left = std::make_unique<ASTNode>(NodeType::Asterisk, "Asterisk", std::move(left), std::move(right));
        if(signToken == TokenType::SLASH) left = std::make_unique<ASTNode>(NodeType::Slash, "Slash", std::move(left), std::move(right));
    } 
    return left;
}

std::unique_ptr<ASTNode> Parser::parseUnary() {
    if(match(TokenType::NOT) || match(TokenType::MINUS)) {
        TokenType token = peek().type;
        advance();
        std::unique_ptr<ASTNode> left = parseUnary();
        if(token == TokenType::NOT) return std::make_unique<ASTNode>(NodeType::Not, "Not", std::move(left), nullptr);
        if(token == TokenType::MINUS) return std::make_unique<ASTNode>(NodeType::UnaryMinus, "UnaryMinus", std::move(left), nullptr);
    }
    return parseFactor();
}

std::unique_ptr<ASTNode> Parser::parseFactor() {
    if(match(TokenType::NUMBER)) {
        std::unique_ptr<ASTNode> node = std::make_unique<ASTNode>(NodeType::Number, peek().value); 
        advance();
        return node;
    }

    if(match(TokenType::LPAREN)) {
        advance();
        std::unique_ptr<ASTNode> node = parseExpression();
        expect(TokenType::RPAREN, "Missing closing paren");
        advance();
        return node;
    }

    if(match(TokenType::VARIABLE)) {
        std::unique_ptr<ASTNode> node = std::make_unique<ASTNode>(NodeType::Variable, peek().value); 
        advance(); 
        return node; 
    }

    if(match(TokenType::BOOL)) {
        std::unique_ptr<ASTNode> node = std::make_unique<ASTNode>(NodeType::Bool, peek().value);
        advance();
        return node;
    }

    if(match(TokenType::STRING)) {
        std::unique_ptr<ASTNode> node = std::make_unique<ASTNode>(NodeType::String, peek().value);
        advance();
        return node;
    }
    throw std::runtime_error("Unexpected token: " + peek().value);
}

std::unique_ptr<ASTNode> Parser::parseLetStatement() {
        advance();
        if(match(TokenType::VARIABLE)) {
            Token variableToken = peek();
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
        else throw std::runtime_error("Expected variable declaration but got: " + peek().value);
}

std::unique_ptr<ASTNode> Parser::parsePrintStatement() {
        TokenType printToken = peek().type;
        advance();
        if(match(TokenType::LPAREN)) {
            advance();
            std::unique_ptr<ASTNode> node = parseExpression();
            expect(TokenType::RPAREN, "Missing closing paren");
            advance();
            if(printToken == TokenType::PRINT) return std::make_unique<ASTNode>(NodeType::Print, "Print", std::move(node));
            else if(printToken == TokenType::PRINTLN) return std::make_unique<ASTNode>(NodeType::Println, "Println", std::move(node));
        }
        throw std::runtime_error("Missing open paren");
}

std::unique_ptr<ASTNode> Parser::parseAssignment() {
        Token token = peek();
        advance();
        if(match(TokenType::EQUALS)) {
            advance();
            std::unique_ptr<ASTNode> node = parseExpression();
            return std::make_unique<ASTNode>(NodeType::Assignment, token.value, std::move(node));
        }
        throw std::runtime_error("Expected variable assigment but got: " + peek().value);
}

std::unique_ptr<ASTNode> Parser::parseIfStatement() {
    advance();
    if(match(TokenType::LPAREN)) {
        advance();
        std::unique_ptr<ASTNode> conditionNode = parseExpression();
        std::unique_ptr<ASTNode> blockNode;
        if(match(TokenType::RPAREN)) {
            advance();
            if(match(TokenType::LCURLY)) blockNode = parseBlock();        
            else throw std::runtime_error("Missing opening curly brace in if statement");
        } 
        return std::make_unique<ASTNode>(NodeType::If, "If", std::move(conditionNode), std::move(blockNode));
    }
    throw std::runtime_error("Missing opening paren in if statement");
}

std::unique_ptr<ASTNode> Parser::parseWhileStatement() {
    advance();
    if(match(TokenType::LPAREN)) {
        advance();
        std::unique_ptr<ASTNode> conditionNode = parseExpression();
        std::unique_ptr<ASTNode> blockNode;
        if(match(TokenType::RPAREN)) {
            advance();
            if(match(TokenType::LCURLY)) blockNode = parseBlock();
            else throw std::runtime_error("Missing opening curly brance in while loop"); 
        }
        return std::make_unique<ASTNode>(NodeType::While, "While", std::move(conditionNode), std::move(blockNode));    
    }
    throw std::runtime_error("Missing opening paren in while loop");
}

std::unique_ptr<ASTNode> Parser::parseBlock() {
    std::vector<std::unique_ptr<ASTNode>> statementNodes;
    advance();
    while(!match(TokenType::RCURLY)) {
        std::unique_ptr<ASTNode> bodyNode = parseStatement();
        if(needSemicolon(bodyNode.get()->nodeType)) expect(TokenType::SEMICOLON, "Missing semicolon in if statement");
        advance();
        statementNodes.emplace_back(std::move(bodyNode));
        }
    expect(TokenType::RCURLY, "Missing closing curly brace in statement");
    return std::make_unique<ASTNode>(NodeType::Block, "Block", nullptr, nullptr, std::move(statementNodes));
}

std::unique_ptr<ASTNode> Parser::parseElseStatement() {
    return nullptr;
}

Token Parser::peek() {
    return tokens_[currIndex_];
}

bool Parser::match(TokenType type) {
    if(currIndex_ >= tokens_.size()) {
        std::cerr << "Cannot check token as it is out of bounds\n";
        return false;
    } 
    if(peek().type == type) return true;
    return false;
}

bool Parser::needSemicolon(NodeType type) {
    if(type == NodeType::If ||
        type == NodeType::Else ||
        type == NodeType::While) {
        return false;
       } 
    return true;
}

void Parser::advance() {
    currIndex_++;
    if(currIndex_ >= tokens_.size()) throw std::runtime_error("Cannot advance as currIndex is out of bounds");
}

void Parser::expect(TokenType token, const std::string &errorMessage) {
    if(!match(token)) throw std::runtime_error(errorMessage);
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
        case NodeType::If:         return "If";
    }

    return "Unknown"; 
}
