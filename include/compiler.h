#pragma once
#include "parser.h"
#include <unordered_map>

// Operation code
enum class OpCode : uint8_t {
    STORE, LOAD,
    PRINT,
    ADD, SUB, DIV, MUL
};

struct Instruction {
    OpCode opcode;
    int operand;
};


class Compiler {
private:
    std::vector<Instruction> bytecode;
    std::unordered_map<int, std::string> variables;
public:
    void compile(const std::vector<ASTNode*> nodes);
};