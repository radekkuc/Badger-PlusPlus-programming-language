#pragma once
#include "parser.h"
#include <variant>
#include <unordered_map>
#include <memory>

// Operation code
enum class OpCode : uint8_t {
    STORE, LOAD, PRINT, PRINTLN,
    CONSTANT, VARIABLE, STRING, BOOL,
    GREATER, SMALLER, OR, AND, EQUAL, N_EQUAL,
    NOT, UMINUS,
    ADD, SUB, DIV, MUL,
    JUMP_IF_FALSE, JUMP
};

struct Instruction {
    OpCode opcode;
    int operand = 0;
};

using Value = std::variant<int, std::string, float, bool>;

class Compiler {
private:
    const std::vector<std::unique_ptr<ASTNode>>& nodes_;
    std::vector<Instruction> bytecode;
    std::unordered_map<std::string, int> variableTable;
    std::vector<Value> constants;
    std::vector<Value> variables;
    std::unordered_map<std::string, bool> initialised;
    int variableCount;

    void compileNode(const ASTNode* node);
public:
    explicit Compiler(const std::vector<std::unique_ptr<ASTNode>>& nodes);
    void compileProgram(Compiler& compiler);
    void dumpBytecode() const;
    std::vector<Instruction> getByteCode() const;
    std::vector<Value> getConstants() const;
    std::vector<Value> getVariables() const;
    std::unordered_map<std::string, int> getMap() const;
    static std::string opcodeToString(OpCode op);
    int getVariableCount() const;
    void emit(Instruction instruction);
};