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
    std::unordered_map<std::string, bool> initialised;
    int variableCount;
public:
    explicit Compiler(const std::vector<std::unique_ptr<ASTNode>>& nodes);
    void compileProgram();
    void dumpBytecode() const;

    std::vector<Instruction> getByteCode() const;
    std::vector<Value> getConstants() const;
    std::unordered_map<std::string, int> getMap() const;

    static std::string opcodeToString(OpCode op);
    
    size_t getByteCodeSize() const;
    size_t getConstantsSize() const;
    int getVariableCount() const;
    int getInstrOperand(const std::string& value);

    bool resolveVariable(const std::string& variable) const;
    bool isInitialized(const std::string& value);

    void defineVariable(const std::string& value);
    void addConstant(const Value& constant);
    void markInitialized(const std::string& value, bool init = true);
    void emit(Instruction instruction);
    void setInstrOperand(int op, int val);
};