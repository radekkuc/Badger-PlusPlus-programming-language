#pragma once

#include "compiler.h"
#include <variant>
#include <stack>




class Interpreter {
private:
    using Value = std::variant<int, std::string, float, char, double>;
    std::vector<Value> variables;
    std::vector<Value> stack;
    std::vector<Instruction> byteCode_;
public:
    void run();
    Interpreter(const std::vector<Instruction>& byteCode, int variableCount);

};