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
        {
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
        }
        default:
            break;
    }
}

VarDeclNode::VarDeclNode(NodeType nodeType, const std::string& value, std::unique_ptr<ASTNode> expression) : ASTNode(nodeType, value), expression(std::move(expression)) {};

void AssignmentNode::compile(Compiler& compiler) const {
    switch(nodeType) {
        case NodeType::Assignment:
            expression->compile(compiler);
            if(!compiler.resolveVariable(value)) throw std::runtime_error("Undefined variable: " + value);
            compiler.emit({OpCode::STORE, compiler.getInstrOperand(value)});
            compiler.markInitialized(value);
            break;
        
        default:
            break;
    }
}

AssignmentNode::AssignmentNode(NodeType nodeType, const std::string& value, std::unique_ptr<ASTNode> expression) : ASTNode(nodeType, value), expression(std::move(expression)) {};

void PrintNode::compile(Compiler& compiler) const {
    switch(nodeType) {
        case NodeType::Print:
            expression->compile(compiler);
            compiler.emit({OpCode::PRINT});
            break;

        case NodeType::Println:
            expression->compile(compiler);
            compiler.emit({OpCode::PRINTLN});
            break;
        
        default:
            break;
    }
}

PrintNode::PrintNode(NodeType nodeType, const std::string& value, std::unique_ptr<ASTNode> expression) : ASTNode(nodeType, value), expression(std::move(expression)) {};

void BlockNode::compile(Compiler& compiler) const {
    switch(nodeType) {
        case NodeType::Block:
        {
            for(const auto& stm : statementNodes) {
                stm->compile(compiler);
            }
            break;
        }

        default:
            break;
    }
}

BlockNode::BlockNode(NodeType nodeType, const std::string& value, std::vector<std::unique_ptr<ASTNode>> statementNodes) : ASTNode(nodeType, value), statementNodes(std::move(statementNodes)) {};

void IfNode::compile(Compiler& compiler) const {
    switch(nodeType) {
        case NodeType::If:
        {   
            conditionNode->compile(compiler);

            size_t jumpIndex = compiler.getByteCodeSize();
            compiler.emit({OpCode::JUMP_IF_FALSE, 0});
            blockNode->compile(compiler);
            compiler.setInstrOperand(jumpIndex, compiler.getByteCodeSize());
            break;
        }
        default:
            break;
    }
}

IfNode::IfNode(NodeType nodeType, const std::string& value, std::unique_ptr<ASTNode> conditionNode, std::unique_ptr<ASTNode> blockNode) : ASTNode(nodeType, value), conditionNode(std::move(conditionNode)), blockNode(std::move(blockNode)) {};

void WhileNode::compile(Compiler& compiler) const {
    switch(nodeType) {
        case NodeType::While:
        {
            size_t loopStartIndex = compiler.getByteCodeSize();
            conditionNode->compile(compiler);
            size_t jumpIndexIfFalse = compiler.getByteCodeSize();
            compiler.emit({OpCode::JUMP_IF_FALSE, 0});
            blockNode->compile(compiler);
            compiler.emit({OpCode::JUMP, static_cast<int>(loopStartIndex)});
            compiler.setInstrOperand(jumpIndexIfFalse, compiler.getByteCodeSize());
            break;
        }
        default:
            break;
    }
}

WhileNode::WhileNode(NodeType nodeType, const std::string& value, std::unique_ptr<ASTNode> conditionNode, std::unique_ptr<ASTNode> blockNode) : ASTNode(nodeType, value), conditionNode(std::move(conditionNode)), blockNode(std::move(blockNode)) {};

void ValueNode::compile(Compiler& compiler) const {
    switch(nodeType) {
        case NodeType::Number:
        {
            Value val;
            if(value.find('.') != std::string::npos) {
                val = std::stof(value);
            } 
            else {
                val = std::stoi(value);

            }
            compiler.addConstant(val);
            compiler.emit({OpCode::CONSTANT, static_cast<int>(compiler.getConstantsSize()) - 1});
            break;
        }
        
        case NodeType::String:
            compiler.addConstant(value);
            compiler.emit({OpCode::CONSTANT, static_cast<int>(compiler.getConstantsSize()) - 1});
            break;
            
        case NodeType::Bool:
        {
            bool val = (value == "true");  
            compiler.addConstant(val);
            compiler.emit({OpCode::CONSTANT, static_cast<int>(compiler.getConstantsSize()) - 1});
            break;
        }
        default:
            break;
    }
}

ValueNode::ValueNode(NodeType nodeType, const std::string& value) : ASTNode(nodeType, value) {};

void VariableNode::compile(Compiler& compiler) const {
    switch(nodeType) {
        case NodeType::Variable:
            if(!compiler.resolveVariable(value)) throw std::runtime_error("Undefined variable: " + value);
            if(!compiler.isInitialized(value)) throw std::runtime_error("Using uninitialised variable: " + value);
            compiler.emit({OpCode::LOAD, compiler.getInstrOperand(value)});
            break;
    }
}

VariableNode::VariableNode(NodeType nodeType, const std::string& value) : ASTNode(nodeType, value) {};
