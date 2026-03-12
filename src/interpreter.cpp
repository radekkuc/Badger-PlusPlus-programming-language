#include "interpreter.h"

Interpreter::Interpreter(const std::vector<Instruction>& byteCode, int variableCount) : byteCode_(byteCode) { variables.resize(variableCount); } 

void Interpreter::run() {
    size_t index = 0;
    while(index < byteCode_.size()) {
        switch(byteCode_[index].opcode) {
            case OpCode::CONSTANT:
            
            break;
        }
        
    }
}