#include "astnodes.h"
#include "parser.h"
#include "compiler.h"
#include <stdexcept>

ASTNode::ASTNode(NodeType nodeType, std::string value) : nodeType(nodeType), value(value) {};

void BinaryNode::compile(Compiler& compiler) const {
    switch(nodeType) {
        case NodeType::Plus:
            left->compile(compiler);
            right->compile(compiler);
            compiler.emit({OpCode::ADD});
            break;

        case NodeType::Minus:
            left->compile(compiler);
            right->compile(compiler);
            compiler.emit({OpCode::SUB});
            break;

        case NodeType::Asterisk:
            left->compile(compiler);
            right->compile(compiler);
            compiler.emit({OpCode::MUL});
            break;

        case NodeType::Slash:
            left->compile(compiler);
            right->compile(compiler);
            compiler.emit({OpCode::DIV});
            break;

        case NodeType::Greater:
            left->compile(compiler);
            right->compile(compiler);
            compiler.emit({OpCode::GREATER});
            break;

        case NodeType::Smaller:
            left->compile(compiler);
            right->compile(compiler);
            compiler.emit({OpCode::SMALLER});
            break;

        case NodeType::Comparison:
            left->compile(compiler);
            right->compile(compiler);
            compiler.emit({OpCode::EQUAL});
            break;
        
        case NodeType::NComparison:
            left->compile(compiler);
            right->compile(compiler);
            compiler.emit({OpCode::N_EQUAL});
            break;

        case NodeType::Or:
            left->compile(compiler);
            right->compile(compiler);
            compiler.emit({OpCode::OR});
            break;
        
        case NodeType::And:
            left->compile(compiler);
            right->compile(compiler);
            compiler.emit({OpCode::AND});
            break;
            
        default:
            break;
    }
}

BinaryNode::BinaryNode(NodeType nodeType, const std::string& value, std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right) : ASTNode(nodeType, value), left(std::move(left)), right(std::move(right))  {};

void UnaryNode::compile(Compiler& compiler) const {
    switch(nodeType) {
        case NodeType::Not:
            operand->compile(compiler);
            compiler.emit({OpCode::NOT});
            break;
        
        case NodeType::UnaryMinus:
            operand->compile(compiler);
            compiler.emit({OpCode::UMINUS});
            break;
        
        default:
            break;
    }
}

UnaryNode::UnaryNode(NodeType nodeType, const std::string& value, std::unique_ptr<ASTNode> operand) : ASTNode(nodeType, value), operand(std::move(operand)) {};


void VarDeclNode::compile(Compiler& compiler) const {
    switch(nodeType) {
        case NodeType::VarDecl:
            if(compiler.resolveVariable(value)) throw std::runtime_error("Variable " + value + " already exists"); 
            if(expression == nullptr) {
                compiler.defineVariable(value);
                compiler.markInitialized(value, false);
                break;
            }
            int operand = compiler.getVariableCount();
            compiler.defineVariable(value);
            expression->compile(compiler);
            compiler.emit({OpCode::STORE, operand});
            compiler.markInitialized(value);

            if(!compiler.resolveVariable(value)) throw std::runtime_error("Undefined variable: " + value);
            break;
        
        default:
            break;
    }
}

VarDeclNode::VarDeclNode(NodeType nodeType, const std::string& value, std::unique_ptr<ASTNode> expression) : ASTNode(nodeType, value), expression(std::move(expression)) {};
