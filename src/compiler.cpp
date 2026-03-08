#include "compiler.h"


void Compiler::compileNode(ASTNode* node) {
    int index{};
    switch(node->nodeType) {
        case NodeType::VarDecl:
       //bytecode.push_back({OpCode::});

        case NodeType::Print:

        case NodeType::Assignment:

        case NodeType::Number:
        bytecode.push_back({OpCode::CONSTANT, index});
        break;

        case NodeType::Variable:


    }
}

void Compiler::compileProgram(const std::vector<ASTNode*>& nodes) {
    for(const auto& node : nodes) {
        compileNode(node);
    }
}