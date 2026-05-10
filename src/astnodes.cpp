#include "astnodes.h"
#include "parser.h"
#include "compiler.h"

ASTNode::ASTNode(NodeType nodeType, std::string value) : nodeType(nodeType), value(value) {};

void BinaryNode::compile(Compiler& compiler) const {
    switch(nodeType) {
        case NodeType::Plus:
            left->compile(compiler);
            right->compile(compiler);
            compiler.emit({OpCode::ADD});
            break;

        case NodeType::Minus:
            compileNode(node->left.get());
            compileNode(node->right.get());
            bytecode.push_back({OpCode::SUB});
            break;

        case NodeType::Asterisk:
            compileNode(node->left.get());
            compileNode(node->right.get());
            bytecode.push_back({OpCode::MUL});
            break;

        case NodeType::Slash:
            compileNode(node->left.get());
            compileNode(node->right.get());
            bytecode.push_back({OpCode::DIV});
            break;
        
        case NodeType::Number:
        {
            Value val;
            if(node->value.find('.') != std::string::npos) {
                val = std::stof(node->value);
            } 
            else {
                val = std::stoi(node->value);

            }
            constants.push_back(val);
            bytecode.push_back({OpCode::CONSTANT, static_cast<int>(constants.size()) - 1});
            break;
        }
        
        default:
            break;
    }
}

BinaryNode::BinaryNode(NodeType nodeType, const std::string& value, std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right) : ASTNode(nodeType, value), left(std::move(left)), right(std::move(right))  {};