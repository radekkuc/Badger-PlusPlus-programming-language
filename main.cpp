#include <iostream>
#include "compiler.h"


int main(int, char**){
    std::string line = R"(let x;
    print(x);
    let y = 15;
    let z = 20;
    x = y + z;
    print(x);)";
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
    compiler.dumpBytecode();
}