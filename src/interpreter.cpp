#include "interpreter.h"
#include "compiler.h"




class Interpreter {
private:
    std::vector<Instruction> byteCode_;

public:
    void run();
    Interpreter(const std::vector<Instruction>& byteCode);

};