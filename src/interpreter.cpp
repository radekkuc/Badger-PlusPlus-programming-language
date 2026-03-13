#include "interpreter.h"

Interpreter::Interpreter(const std::vector<Instruction>& byteCode, int variableCount) : byteCode_(byteCode) { variables.resize(variableCount); } 

void Interpreter::run() {
    size_t index = 0;
    while(index < byteCode_.size()) {
        switch(byteCode_[index].opcode) {
            case OpCode::CONSTANT:
                stack.push_back(byteCode_[index].operand);
                break;

            case OpCode::STORE:
                variables[byteCode_[index].operand] = stack.back();
                stack.pop_back();
                break;

            case OpCode::LOAD:

                break;
            
            case OpCode::PRINT:
                break;
            
            case OpCode::ADD:
                break;

            default:
                break;
        }
        index++;
    }
}