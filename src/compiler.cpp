#include "compiler.h"
#include <string>

Compiler::Compiler(const std::vector<ASTNode*>& nodes) : variableCount{}, nodes_(nodes) {};

void Compiler::compileNode(ASTNode* node) {
    switch(node->nodeType) {
        case NodeType::VarDecl:
        compileNode(node->left);
        bytecode.push_back({OpCode::STORE, variableCount});
        variableTable[node->value] = variableCount;
        variableCount++;
        break;

        case NodeType::Print:
        compileNode(node->left);
        bytecode.push_back({OpCode::PRINT});
        break;

        case NodeType::Assignment:
        compileNode(node->left);
        bytecode.push_back({OpCode::STORE, variableTable[node->value]});
        break;

        case NodeType::Number:
        bytecode.push_back({OpCode::CONSTANT, stoi(node->value)});
        break;

        case NodeType::Variable:
        bytecode.push_back({OpCode::LOAD, variableTable[node->value]});
        break;

        case NodeType::Plus:
        compileNode(node->left);
        compileNode(node->right);
        bytecode.push_back({OpCode::ADD});
        break;

        case NodeType::Minus:
        compileNode(node->left);
        compileNode(node->right);
        bytecode.push_back({OpCode::SUB});
        break;

        case NodeType::Asterisk:
        compileNode(node->left);
        compileNode(node->right);
        bytecode.push_back({OpCode::MUL});
        break;

        case NodeType::Slash:
        compileNode(node->left);
        compileNode(node->right);
        bytecode.push_back({OpCode::DIV});
        break;

        default:
        break;
    }
}

void Compiler::compileProgram() {
    for(const auto& node : nodes_) {
        compileNode(node);
    }
}