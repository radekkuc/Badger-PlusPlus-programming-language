#include "interpreter.h"
#include "utils.h"
#include <iostream>

Interpreter::Interpreter(const std::vector<Instruction>& byteCode, const std::vector<Value>& constants, int varCount) : byteCode_(byteCode), constants_(constants) {variables.resize(varCount);};

void Interpreter::run() {
    size_t index = 0;
    while(index < byteCode_.size()) {
        switch(byteCode_[index].opcode) {
            case OpCode::CONSTANT:
            {
                stack.push_back(constants_[byteCode_[index].operand]);
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
            case OpCode::PRINTLN:
            { 
                Value value = stack.back();
                std::visit([](const auto& val) {
                    std::cout << val << std::endl;
                }, value);
                stack.pop_back();
                break;
            }
            case OpCode::JUMP_IF_FALSE:
            {
                Value value = stack.back(); stack.pop_back();

                if(!std::holds_alternative<bool>(value)) throw std::runtime_error("Must provide bool in condition of if statement");
                bool condition = std::get<bool>(value);
                if(!condition) {
                    int target = byteCode_[index].operand - 1; 
                    if(target < 0 || target >= static_cast<int>(byteCode_.size())) throw std::runtime_error("Wrong jump location");
                    index = static_cast<size_t>(target);
                }
                break;
            }
            case OpCode::ADD:
            {
                Value b = stack.back(); stack.pop_back();
                Value a = stack.back(); stack.pop_back();
                add(a,b);
                break;
            }
            case OpCode::SUB:
            {   
                Value b = stack.back(); stack.pop_back();
                Value a = stack.back(); stack.pop_back();
                sub(a,b);
                break;
            }
            case OpCode::MUL:
            {   
                Value b = stack.back(); stack.pop_back();
                Value a = stack.back(); stack.pop_back();
                mul(a,b);
                break;
            }
            case OpCode::DIV:
            {
                Value b = stack.back(); stack.pop_back();
                Value a = stack.back(); stack.pop_back();
                div(a,b);
                break;
            }
            case OpCode::GREATER:
            {
                Value b = stack.back(); stack.pop_back();
                Value a = stack.back(); stack.pop_back();
                grt(a,b);
                break;
            }
            case OpCode::SMALLER:
            {
                Value b = stack.back(); stack.pop_back();
                Value a = stack.back(); stack.pop_back();
                less(a,b);
                break;
            }
            case OpCode::EQUAL:
            {
                Value b = stack.back(); stack.pop_back();
                Value a = stack.back(); stack.pop_back();
                eq(a,b);
                break;
            }
            case OpCode::N_EQUAL:
            {
                Value b = stack.back(); stack.pop_back();
                Value a = stack.back(); stack.pop_back();
                neq(a,b);
                break;
            }
            case OpCode::NOT:
            {
                Value a = stack.back(); stack.pop_back();
                if(!std::holds_alternative<bool>(a)) throw std::runtime_error("Parameter must be of boolean type in NOT operation");
                bool result = !std::get<bool>(a);
                stack.push_back(result);
                break;
            }
            case OpCode::OR:
            {
                Value b = stack.back(); stack.pop_back();
                Value a = stack.back(); stack.pop_back();
                if(!std::holds_alternative<bool>(b) || !std::holds_alternative<bool>(a)) throw std::runtime_error("Both parameters must be boolean value in OR operation");
                bool result = std::get<bool>(b) || std::get<bool>(a);
                stack.push_back(result);
                break;
            }
            case OpCode::AND:
            {
                Value b = stack.back(); stack.pop_back();
                Value a = stack.back(); stack.pop_back();
                if(!std::holds_alternative<bool>(b) || !std::holds_alternative<bool>(a)) throw std::runtime_error("Both parameters must be boolean value in AND operation");
                bool result = std::get<bool>(b) && std::get<bool>(a);
                stack.push_back(result);
                break;
            }
            default:
                break;
        }
        index++;
    }
}


void Interpreter::add(const Value& a, const Value& b) {
    std::visit([this](const auto& v1, const auto& v2) {
        using V1 = std::decay_t<decltype(v1)>;
        using V2 = std::decay_t<decltype(v2)>;
        
        if constexpr(is_numeric<V1> && is_numeric<V2>) {
            using Type = std::common_type_t<V1,V2>;
            stack.push_back(static_cast<Type>(v1) + static_cast<Type>(v2));
        }
        else if constexpr(std::is_same_v<std::string, V1> && std::is_same_v<std::string, V2>) {
            stack.push_back(v1 + v2);
        }
        else throw std::runtime_error("Wrong addition of paramters");

    }, a, b);
}

void Interpreter::sub(const Value& a, const Value& b) {
    std::visit([this](const auto& v1, const auto& v2) {
        using V1 = std::decay_t<decltype(v1)>;
        using V2 = std::decay_t<decltype(v2)>;

        if constexpr(is_numeric<V1> && is_numeric<V2>) {
            using Type = std::common_type_t<V1,V2>;
            stack.push_back(static_cast<Type>(v1)  - static_cast<Type>(v2));
        }
        else throw std::runtime_error("Wrong subtraction of parameters");
    }, a, b);
}

void Interpreter::mul(const Value& a, const Value& b) {
    std::visit([this](const auto& v1, const auto& v2) {
        using V1 = std::decay_t<decltype(v1)>;
        using V2 = std::decay_t<decltype(v2)>;

        if constexpr(is_numeric<V1> && is_numeric<V2>) {
            using Type = std::common_type_t<V1,V2>;
            stack.push_back(static_cast<Type>(v1) * static_cast<Type>(v2));
        }
        else throw std::runtime_error("Wrong multiplication of parameters"); 
    }, a, b); 
}

void Interpreter::div(const Value& a, const Value& b) {
    std::visit([this](const auto& v1, const auto& v2) {
        using V1 = std::decay_t<decltype(v1)>;
        using V2 = std::decay_t<decltype(v2)>;

        if constexpr(is_numeric<V1> && is_numeric<V2>) {
            using Type = std::common_type_t<V1,V2>;
            Type divisor = static_cast<Type>(v2);
            if(divisor == 0) throw std::runtime_error("Cannot divide by 0");

            stack.push_back(static_cast<Type>(v1) / divisor);
        }
        else throw std::runtime_error("Wrong division parameters");
    }, a, b);
}

void Interpreter::grt(const Value& a, const Value& b) {
    std::visit([this](const auto& v1, const auto& v2) {
        using V1 = std::decay_t<decltype(v1)>;
        using V2 = std::decay_t<decltype(v2)>;

        if constexpr(is_numeric<V1> && is_numeric<V2>) {
            using Type = std::common_type_t<V1,V2>;
            stack.push_back(static_cast<Type>(v1) > static_cast<Type>(v2));
        }
        else throw std::runtime_error("Wrong parameters for Greater operation");
    }, a, b);
}

void Interpreter::less(const Value& a, const Value& b) {
    std::visit([this](const auto& v1, const auto& v2) {
        using V1 = std::decay_t<decltype(v1)>;
        using V2 = std::decay_t<decltype(v2)>;

        if constexpr(is_numeric<V1> && is_numeric<V2>) {
            using Type = std::common_type_t<V1,V2>;
            stack.push_back(static_cast<Type>(v1) < static_cast<Type>(v2));
        }
        else throw std::runtime_error("Wrong parameter for Smaller operation");
    }, a, b); 
}

void Interpreter::eq(const Value& a, const Value& b) {
        std::visit([this](const auto& v1, const auto& v2) {
        using V1 = std::decay_t<decltype(v1)>;
        using V2 = std::decay_t<decltype(v2)>;

        if constexpr(is_numeric<V1> && is_numeric<V2>) {
            using Type = std::common_type_t<V1,V2>;
            stack.push_back(static_cast<Type>(v1) == static_cast<Type>(v2));
        }
        else if constexpr(std::is_same_v<V1, bool> && std::is_same_v<V2, bool>) {
            stack.push_back(v1 == v2);
        }
        else if constexpr(std::is_same_v<V1, std::string> && std::is_same_v<V2, std::string> ) {
            stack.push_back(v1 == v2);
        }
        else throw std::runtime_error("Wrong parameter for Equal operation");
    }, a, b); 
}

void Interpreter::neq(const Value& a, const Value& b) {
        std::visit([this](const auto& v1, const auto& v2) {
        using V1 = std::decay_t<decltype(v1)>;
        using V2 = std::decay_t<decltype(v2)>;

        if constexpr(is_numeric<V1> && is_numeric<V2>) {
            using Type = std::common_type_t<V1,V2>;
            stack.push_back(static_cast<Type>(v1) != static_cast<Type>(v2));
        }
        else if constexpr(std::is_same_v<V1, bool> && std::is_same_v<V2, bool>) {
            stack.push_back(v1 != v2);
        }
        else if constexpr(std::is_same_v<V1, std::string> && std::is_same_v<V2, std::string> ) {
            stack.push_back(v1 != v2);
        }
        else throw std::runtime_error("Wrong parameter for Equal operation");
    }, a, b); 
}


