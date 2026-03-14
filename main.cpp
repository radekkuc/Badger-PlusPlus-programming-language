#include <iostream>
#include "interpreter.h"

int main(int, char**){
    try {
        std::string line = R"(let x = 10 + 5;
        let y;
        y = x;
        print(y);
        print(x);
        )";

        Lexer lexer(line);
        
        std::vector<Token> tokens = lexer.identify();

        // for(Token token : tokens) {
        //     std::cout << token.value << std::endl;
        // }

        Parser parser(tokens);
        std::vector<ASTNode*> nodes = parser.parseProgram();

        // for(const auto& node : nodes) {
        //     std::cout << node->value << std::endl;
        // }
        // std::cout << "Using printing method\n"; 
        // parser.printAST(nodes);

        Compiler compiler(nodes);
        compiler.compileProgram();
        std::unordered_map<std::string, int> variableMap = compiler.getMap();
        compiler.dumpBytecode();

        // for(const auto& pair : variableMap) {
        //     std::cout << pair.first << "," << pair.second << std::endl; 
        // }

        std::vector<Instruction> byteCode = compiler.getByteCode();
        int variableCount = compiler.getVariableCount();
        Interpreter interpreter(byteCode, variableCount);
        interpreter.run();

    }
    catch(const std::exception& e) {
        std::cerr << "Exception has occurred: " << e.what() << std::endl; 
    }
}