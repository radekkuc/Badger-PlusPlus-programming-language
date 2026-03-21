#include <iostream>
#include "interpreter.h"
#include <sstream>
#include <fstream>

int main(int argc, char** argv){
    try {
        std::ifstream file(argv[1]);
        if(argc < 2) throw std::runtime_error("No file provided");

        std::stringstream ss;
        ss << file.rdbuf();
        const std::string& code = ss.str();

        Lexer lexer(code);
        
        std::vector<Token> tokens = lexer.identify();

        // for(Token token : tokens) {
        //     std::cout << token.value << std::endl;
        // }

        Parser parser(tokens);
        std::vector<ASTNode*> nodes = parser.parseProgram();

        // for(const auto& node : nodes) {
        //     std::cout << node->value  << "," << node->left->value << std::endl;
        // }
        // std::cout << "Using printing method\n"; 
        // parser.printAST(nodes);

        Compiler compiler(nodes);
        compiler.compileProgram();
        // std::unordered_map<std::string, int> variableMap = compiler.getMap();
        // compiler.dumpBytecode();

        // for(const auto& pair : variableMap) {
        //     std::cout << pair.first << "," << pair.second << std::endl; 
        // }

        std::vector<Instruction> byteCode = compiler.getByteCode();
        std::vector<Value> constants = compiler.getConstants();
        // for (const auto& constant : constants) {
        //     std::visit([](const auto& v) {
        //     std::cout << v << std::endl;
        //     }, constant);
        // }
        Interpreter interpreter(byteCode, constants);
        interpreter.run();
    }
    catch(const std::exception& e) {
        std::cerr << "Exception has occurred: " << e.what() << std::endl; 
    }
}