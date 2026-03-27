#include "interpreter.h"
#include <iostream>

Interpreter::Interpreter(const std::vector<Instruction>& byteCode, const std::vector<Value>& constants) : byteCode_(byteCode), variables(constants) {};

void Interpreter::run() {
    size_t index = 0;
    while(index < byteCode_.size()) {
        switch(byteCode_[index].opcode) {
            case OpCode::CONSTANT:
            {
                stack.push_back(variables[byteCode_[index].operand]);
                break;
            }
            case OpCode::STORE:
            {
                variables[byteCode_[index].operand] = stack.back();
                stack.pop_back();
                break;
            }
            case OpCode::LOAD:
            { 
                stack.push_back(variables[byteCode_[index].operand]);
                break;
            }
            case OpCode::PRINT:
            { 
                Value value = stack.back();
                std::visit([](const auto& val) {
                    std::cout << val;
                }, value);
                stack.pop_back();
                break;
            }
            case OpCode::ADD:
            {
                int b = std::get<int>(stack.back()); stack.pop_back();
                int a = std::get<int>(stack.back()); stack.pop_back();
                stack.push_back(a + b);
                break;
            }
            case OpCode::SUB:
            {
                int b = std::get<int>(stack.back()); stack.pop_back();
                int a = std::get<int>(stack.back()); stack.pop_back();
                stack.push_back(a - b);
                break;
            }
            case OpCode::MUL:
            {
                int b = std::get<int>(stack.back()); stack.pop_back();
                int a = std::get<int>(stack.back()); stack.pop_back();
                stack.push_back(a * b);
                break;
            }
            case OpCode::DIV:
            {
                int b = std::get<int>(stack.back()); stack.pop_back();
                int a = std::get<int>(stack.back()); stack.pop_back();
                if(b == 0) throw std::runtime_error("Divider cannot equal to 0");
                stack.push_back(a / b);
                break;
            }
            default:
                break;
        }
        index++;
    }
}


void Interpreter::add(Value a, Value b) {
    std::visit([](const auto& v1, const auto& v2) {
        using V1 = std::decay_t<decltype(v1)>;
        using V2 = std::decay_t<decltype(v2)>;

        //if constexpr(is_numeric_v<)
    }, a, b);
}
