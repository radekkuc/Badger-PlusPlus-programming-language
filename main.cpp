#include <iostream>
#include "lexer.h"
#include "parser.h"


int main(int, char**){
    std::string line = R"(let x = 10;
    print(x);
    x = x + 1;
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
    std::cout << "Using printing method\n"; 
    parser.printAST(nodes);
}