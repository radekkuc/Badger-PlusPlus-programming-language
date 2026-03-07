#pragma once
#include "parser.h"

// Operation code
enum class OpCode : uint8_t {
    STORE, LOAD,
    ADD, SUB, DIV, MUL
};

struct Instruction {
    OpCode opcode;
    int operand;
};



class Compiler {

};