#include "compiler.h"
#include <string>
#include <iostream>

Compiler::Compiler(const std::vector<std::unique_ptr<ASTNode>>& nodes) : variableCount{}, nodes_(nodes) {};

void Compiler::compileNode(const ASTNode* node) {
    switch(node->nodeType) {
        case NodeType::VarDecl:
            if(variableTable.find(node->value) != variableTable.end()) throw std::runtime_error("Variable " + node->value + " already exists"); 
            if(node->left == nullptr) {
                variableTable[node->value] = variableCount;
                initialised[node->value] = false;
                variableCount++;    
                break;
            }
            variableTable[node->value] = variableCount;
            compileNode(node->left.get());
            bytecode.push_back({OpCode::STORE, variableCount});
            initialised[node->value] = true;

            if(variableTable.find(node->value) == variableTable.end()) throw std::runtime_error("Undefined variable: " + node->value);
            variableCount++;
            break;

        case NodeType::Print:
            compileNode(node->left.get());
            bytecode.push_back({OpCode::PRINT});
            break;

        case NodeType::Println:
            compileNode(node->left.get());
            bytecode.push_back({OpCode::PRINTLN});
            break;
        
        case NodeType::If:
        {   
            compileNode(node->left.get());
            
            size_t jumpIndex = bytecode.size();
            bytecode.push_back({OpCode::JUMP_IF_FALSE, 0});
            compileNode(node->right.get());
            bytecode[jumpIndex].operand = bytecode.size();
            break;
        }

        case NodeType::Block:
        {
            for(const auto& stm : node->statements) {
                compileNode(stm.get());
            }
            break;
        }

        case NodeType::Assignment:
            compileNode(node->left.get());
            if(variableTable.find(node->value) == variableTable.end()) throw std::runtime_error("Undefined variable: " + node->value);
            bytecode.push_back({OpCode::STORE, variableTable[node->value]});
            initialised[node->value] = true;
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

        case NodeType::Variable:
            if(variableTable.find(node->value) == variableTable.end()) throw std::runtime_error("Undefined variable: " + node->value);
            if(!initialised[node->value]) throw std::runtime_error("Using uninitialised variable: " + node->value);
            bytecode.push_back({OpCode::LOAD, variableTable[node->value]});
            break;
        
        case NodeType::String:
            constants.push_back(node->value);
            bytecode.push_back({OpCode::CONSTANT, static_cast<int>(constants.size()) - 1});
            break;
            
        case NodeType::Bool:
        {
            bool val = (node->value == "true");  
            constants.push_back(val);
            bytecode.push_back({OpCode::CONSTANT, static_cast<int>(constants.size()) - 1});
            break;
        }
        case NodeType::Plus:
            compileNode(node->left.get());
            compileNode(node->right.get());
            bytecode.push_back({OpCode::ADD});
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
        default:
            break;
    }
}

void Compiler::compileProgram() {
    for(const auto& node : nodes_) {
        compileNode(node.get());
    }
}

std::string Compiler::opcodeToString(OpCode op) {
    switch(op) {
        case OpCode::STORE: return "STORE";
        case OpCode::LOAD: return "LOAD";
        case OpCode::PRINT: return "PRINT";
        case OpCode::CONSTANT: return "CONSTANT";
        case OpCode::VARIABLE: return "VARIABLE";
        case OpCode::ADD: return "ADD";
        case OpCode::SUB: return "SUB";
        case OpCode::DIV: return "DIV";
        case OpCode::MUL: return "MUL";
        case OpCode::JUMP_IF_FALSE: return "JUMP_IF_FALSE";
        default: return "UNKNOWN";
    }
}

void Compiler::dumpBytecode() const {
    std::cout << "\n==== BYTECODE ====\n";

    for(size_t i = 0; i < bytecode.size(); ++i) {
        const Instruction& instr = bytecode[i];

        std::cout << i << " : "
                  << opcodeToString(instr.opcode);

        // only print operand when meaningful
        if(instr.opcode == OpCode::STORE ||
           instr.opcode == OpCode::LOAD ||
           instr.opcode == OpCode::CONSTANT ||
           instr.opcode == OpCode::VARIABLE ||
           instr.opcode == OpCode::JUMP_IF_FALSE) 
           
        {
            std::cout << " " << instr.operand;
        }

        std::cout << "\n";
    }

    std::cout << "\nVariables:\n";

    for(const auto& [name, slot] : variableTable) {
        std::cout << name << " -> slot " << slot << "\n";
    }

    std::cout << "\nVariable count: " << variableCount << "\n";
}



std::unordered_map<std::string, int> Compiler::getMap() const {
    return variableTable;
}

std::vector<Instruction> Compiler::getByteCode() const {
    return bytecode;
}


std::vector<Value> Compiler::getConstants() const {
    return constants;
}

std::vector<Value> Compiler::getVariables() const {
    return variables;
}

int Compiler::getVariableCount() const {
    return variableCount;
}