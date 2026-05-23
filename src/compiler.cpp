#include "compiler.h"
#include <string>
#include <iostream>

Compiler::Compiler(const std::vector<std::unique_ptr<ASTNode>>& nodes) : variableCount{}, nodes_(nodes) {};

void Compiler::compileProgram() {
    std::unordered_map<std::string, VariableInfo> globalScope;
    variableTable.emplace_back(globalScope);
    for(const auto& node : nodes_) {
        node->compile(*this);
    }
}

std::string Compiler::opcodeToString(OpCode op) {
    switch(op) {
        case OpCode::STORE: return "STORE";
        case OpCode::LOAD: return "LOAD";
        case OpCode::PRINT: return "PRINT";
        case OpCode::PRINTLN: return "PRINTLN";
        case OpCode::CONSTANT: return "CONSTANT";
        case OpCode::VARIABLE: return "VARIABLE";
        case OpCode::ADD: return "ADD";
        case OpCode::SUB: return "SUB";
        case OpCode::DIV: return "DIV";
        case OpCode::MUL: return "MUL";
        case OpCode::JUMP_IF_FALSE: return "JUMP_IF_FALSE";
        case OpCode::JUMP: return "JUMP";
        case OpCode::NOT: return "NOT";
        case OpCode::UMINUS: return "UMINUS";
        case OpCode::GREATER: return "GREATER";
        case OpCode::SMALLER: return "SMALLER";
        case OpCode::EQUAL : return "EQUAL";
        case OpCode::N_EQUAL: return "N_EQUAL";
        case OpCode::OR: return "OR";
        case OpCode::AND: return "AND";
        default: return "UNKNOWN";
    }
}

std::ostream& operator<<(std::ostream& o, const VariableInfo& v) {
    o << v.operand;
    return o;
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
           instr.opcode == OpCode::JUMP ||
           instr.opcode == OpCode::JUMP_IF_FALSE) 
           
        {
            std::cout << " " << instr.operand;
        }

        std::cout << "\n";
    }

    std::cout << "\nVariables:\n";

    for(const auto& [name, slot] : variableTable.back()) {
        std::cout << name << " -> slot " << slot << "\n";
    }

    std::cout << "\nVariable count: " << variableCount << "\n";
}

// std::unordered_map<std::string, int> Compiler::getMap() const {
//     return variableTable;
// }

std::vector<Instruction> Compiler::getByteCode() const {
    return bytecode;
}

std::vector<Value> Compiler::getConstants() const {
    return constants;
}

size_t Compiler::getByteCodeSize() const {
    return bytecode.size();
}

size_t Compiler::getConstantsSize() const {
    return constants.size();
}

int Compiler::getVariableCount() const {
    return variableCount;
}

//
int Compiler::getInstrOperand(const std::string& value) {
    return variableTable.back()[value].operand;
}

std::optional<VariableScopeInfo> Compiler::resolveVariableAnyScope(const std::string& variable) {
    for(size_t i = variableTable.size(); i > 0; i--) {
        if(auto it = variableTable[i - 1].find(variable); it != variableTable[i - 1].end()) {
            VariableScopeInfo varScopeInfo{i - 1, &it->second};
            return varScopeInfo;
        }
    }
    return std::nullopt;
}

bool Compiler::resolveVariableCurrentScope(const std::string& variable) const {
    if(variableTable.back().find(variable) != variableTable.back().end()) return true;
    return false;
}

//
bool Compiler::isInitialized(const std::string& value) {
    return variableTable.back()[value].initialized;
}

void Compiler::defineVariable(const std::string& value) {
    variableTable.back()[value].operand = variableCount;
    variableCount++;
}

void Compiler::addConstant(const Value& constant) {
    constants.push_back(constant);
}

void Compiler::markInitialized(const std::string& value, bool init) {
    variableTable.back()[value].initialized = init;
}

void Compiler::markScopeBasedInitialization(const std::string& value, size_t scopeIndex) {
    variableTable[scopeIndex][value].initialized = true;
}

void Compiler::emit(Instruction instruction) {
    bytecode.push_back(instruction);
}

void Compiler::setInstrOperand(int op, int val) {
    bytecode[op].operand = val;
}

void Compiler::enterScope() {
    std::unordered_map<std::string, VariableInfo> map;
    variableTable.emplace_back(map);
}

void Compiler::leaveScope() {
    variableTable.pop_back();
}


