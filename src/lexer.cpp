#include <iostream>
#include "lexer.h"
#include <stdexcept>
#include <sstream>
#include <fstream>

Lexer::Lexer(const std::string& line) : line_{line}, currIndex_{0} {};

std::vector<Token> Lexer::identify() {
    std::vector<Token> tokens;
    while(currIndex_ < line_.size()) {
        char c = line_[currIndex_];
        switch(c) {
            case '*':
                tokens.push_back(Token{TokenType::ASTERISK, "*"});
                currIndex_++;
                break;

            case '/':
                tokens.push_back(Token{TokenType::SLASH, "/"});
                currIndex_++;
                break;

            case '+':
                tokens.push_back(Token{TokenType::PLUS, "+"}); 
                currIndex_++;
                break;

            case '-':
                tokens.push_back(Token{TokenType::MINUS, "-"});
                currIndex_++;
                break;

            case '(':
                tokens.push_back(Token{TokenType::LPAREN, "("});
                currIndex_++;
                break;

            case ')':
                tokens.push_back(Token{TokenType::RPAREN, ")"});
                currIndex_++;
                break;

            case '=':
                tokens.push_back(Token{TokenType::EQUALS, "="});
                currIndex_++;
                break;

            case ';':
                tokens.push_back(Token{TokenType::SEMICOLON, ";"});
                currIndex_++;
                break;

            default:
                if(isspace(c)) skipWhiteSpace();
                else if(line_.substr(currIndex_, 3) == "let") {
                    tokens.push_back(Token{TokenType::LET, "let"});
                    currIndex_ += 3; 
                }
                else if(line_.substr(currIndex_, 5) == "print") {
                    tokens.push_back(Token{TokenType::PRINT, "print"});
                    currIndex_ += 5;
                }
        
                else if(isalpha(c)) tokens.push_back(readWord()); 
                else if(isdigit(c)) tokens.push_back(readNumber());
                else throw std::runtime_error("Unknown character provided: " + c);
                break;
        }   
    }
    tokens.push_back(Token{TokenType::ENDOFFILE, "EOF"});   
    return tokens;
}

void Lexer::skipWhiteSpace() {
    while(isspace(line_[currIndex_]) && currIndex_ < line_.size()) {
        currIndex_++;
    }
}

Token Lexer::readNumber() {
    std::string number;
    while(isdigit(line_[currIndex_]) && currIndex_ < line_.size()) {
        number += line_[currIndex_];
        currIndex_++;
    }
    return Token{TokenType::NUMBER, number};
}

Token Lexer::readWord() {
    std::string variable;
    while(isalpha(line_[currIndex_]) && currIndex_ < line_.size()) {
        variable += line_[currIndex_];
        currIndex_++;
    }
    return Token{TokenType::VARIABLE, variable};
}