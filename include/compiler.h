#pragma once
#include "parser.h"
#include <unordered_map>

// Operation code
enum class OpCode : uint8_t {
    STORE, LOAD, PRINT,
    CONSTANT, VARIABLE, 
    ADD, SUB, DIV, MUL
};

struct Instruction {
    OpCode opcode;
    int operand = 0;
};


class Compiler {
private:
    int variableCount;
    std::vector<ASTNode*> nodes_;
    std::vector<Instruction> bytecode;
    std::unordered_map<std::string, int> variableTable;

    void compileNode(ASTNode* node);
public:
    Compiler(const std::vector<ASTNode*>& nodes);
    void compileProgram();
};