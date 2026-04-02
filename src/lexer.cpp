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
            case '{':
                tokens.push_back(Token{TokenType::LCURLY, "{"});
                break;
            case '}':
                tokens.push_back(Token{TokenType::RCURLY, "}"});
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
                else if(isalpha(c)) tokens.push_back(readWord()); 
                else if(isdigit(c)) tokens.push_back(readNumber());
                else if(c == '"') tokens.push_back(readString());
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
    bool hasDot = false;
    while(currIndex_ < line_.size()) {
        if(isdigit(line_[currIndex_])) {
            number += line_[currIndex_];
        }
        else if(line_[currIndex_] == '.' && !hasDot) {
            number += line_[currIndex_];
            hasDot = true;
        }
        else break; 
        currIndex_++;
    }
    if(line_[currIndex_] == '.') throw std::runtime_error(std::string("Unexpected symbol in number definition: ")  + line_[currIndex_]);
    return Token{TokenType::NUMBER, number};
}

Token Lexer::readWord() {
    std::string variable;
    while(isalpha(line_[currIndex_]) && currIndex_ < line_.size()) {
        variable += line_[currIndex_];
        currIndex_++;
    }
    if(variable == "let") return Token{TokenType::LET, variable};
    if(variable == "if") return Token{TokenType::IF, variable};
    if(variable == "else") return Token{TokenType::ELSE, variable};
    if(variable == "print") return Token{TokenType::PRINT, variable};
    if(variable == "println") return Token{TokenType::PRINTLN, variable};
    if(variable == "true" || variable == "false") return Token{TokenType::BOOL, variable};
    return Token{TokenType::VARIABLE, variable};
}

Token Lexer::readString() {
    std::string strVar;
    currIndex_++;
    while(line_[currIndex_] != '"' && currIndex_ < line_.size()) {
        strVar += line_[currIndex_];
        currIndex_++;
    }
    if(line_[currIndex_] != '"') throw std::runtime_error("Missing closing \" for string");
    currIndex_++;
    return Token{TokenType::STRING, strVar};
}