#include <iostream>
#include "lexer.h"
#include "parser.h"


int main(int, char**){
    //std::string line = "let x = 10;";
    //std::string line = "let x = 10 + 5 + 2;";
    //std::string line = "let x = 10 * 2 + 4;";
    //std::string line = "let x = 2 + 6 * (4 / 2) - 3;";
    std::string line = "let x;";
    Lexer lexer(line);
    std::vector<Token> tokens = lexer.identify();

    // for(Token token : tokens) {
    //     std::cout << token.value << std::endl;
    // }
    Parser parser(tokens);
    ASTNode* node = parser.parseProgram();
    std::cout << "Using printing method\n"; 
    parser.printAST(node);
}