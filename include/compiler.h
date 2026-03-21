#pragma once
#include "parser.h"
#include <variant>
#include <unordered_map>

// Operation code
enum class OpCode : uint8_t {
    STORE, LOAD, PRINT,
    CONSTANT, VARIABLE, STRING, BOOL,
    ADD, SUB, DIV, MUL
};

struct Instruction {
    OpCode opcode;
    int operand = 0;
};

using Value = std::variant<int, std::string, float, bool>;

class Compiler {
private:
    int variableCount;
    std::vector<ASTNode*> nodes_;
    std::vector<Instruction> bytecode;
    std::unordered_map<std::string, int> variableTable;
    std::vector<Value> constants;
    std::unordered_map<std::string, bool> initialised;

    void compileNode(ASTNode* node);
public:
    explicit Compiler(const std::vector<ASTNode*>& nodes);
    void compileProgram();
    void dumpBytecode() const;
    std::vector<Instruction> getByteCode() const;
    std::vector<Value> getConstants() const;
    std::unordered_map<std::string, int> getMap() const;
    static std::string opcodeToString(OpCode op);
};