#pragma once

#include "compiler.h"
#include <variant>
#include <stack>

using Value = std::variant<int, std::string, float, bool>;

class Interpreter {
private:
    std::vector<Value> variables;
    std::vector<Value> constants_;
    std::vector<Value> stack;
    std::vector<Instruction> byteCode_;
public:
    void run();
    void add(const Value& a, const Value& b);
    void sub(const Value& a, const Value& b);
    void mul(const Value& a, const Value& b);
    void div(const Value& a, const Value& b);

    void grt(const Value& a, const Value& b);
    void less(const Value& a, const Value& b);
    void eq(const Value& a, const Value& b);
    void neq(const Value& a, const Value& b);

    explicit Interpreter(const std::vector<Instruction>& byteCode, const std::vector<Value>& constants, int varCount);
};