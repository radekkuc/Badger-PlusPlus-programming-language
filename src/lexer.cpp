#include <iostream>
#include "lexer.h"
#include <stdexcept>
#include <sstream>
#include <fstream>
#include "utils.h"

Lexer::Lexer(const std::string& line) : line_{line}, currIndex_{0} {};

std::vector<Token> Lexer::identify() {
    std::vector<Token> tokens;
    while(currIndex_ < line_.size()) {
        char c = peek();
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

            case '>':
                tokens.push_back(Token{TokenType::GREATER, ">"});
                currIndex_++;
                break;
            
            case '<':
                tokens.push_back(Token{TokenType::SMALLER, "<"});
                currIndex_++;
                break;

            case '|':
                if(peekNext() == '|') tokens.push_back(Token{TokenType::OR, "||"});
                else throw std::runtime_error("Missing second | in logic operation");
                currIndex_++;
                break;

            case '&':
                if(peekNext() == '&') tokens.push_back(Token{TokenType::AND, "&&"});
                else throw std::runtime_error("Missing second & in logic operation");
                currIndex_++;
                break;

		    case '!':
                if(peekNext() == '=') tokens.push_back(Token{TokenType::NCOMPARISON, "!="});
                else tokens.push_back(Token{TokenType::NOT, "!"});
                currIndex_++;
                break;

            case '=':
                if(peekNext() == '=') tokens.push_back(Token{TokenType::COMPARISON, "=="});
                else tokens.push_back(Token{TokenType::EQUALS, "="});
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
                currIndex_++;
                break;

            case '}':
                tokens.push_back(Token{TokenType::RCURLY, "}"});
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
    while(isspace(peek()) && currIndex_ < line_.size()) {
        currIndex_++;
    }
}

char Lexer::peek() {
    return line_[currIndex_];
}

char Lexer::peekNext() {
    int indexHolder = static_cast<int>(currIndex_);
    indexHolder++;
    if(indexHolder >= line_.size()) throw std::runtime_error("Cannot peek next token as it is out of bounds");
    currIndex_++;
    return peek();
}

Token Lexer::readNumber() {
    std::string number;
    bool hasDot = false;
    while(currIndex_ < line_.size()) {
        if(isdigit(peek())) {
            number += peek();
        }
        else if(peek() == '.' && !hasDot) {
            number += peek();
            hasDot = true;
        }
        else break; 
        currIndex_++;
    }
    if(peek() == '.') throw std::runtime_error(std::string("Unexpected symbol in number definition: ")  + peek());
    return Token{TokenType::NUMBER, number};
}

Token Lexer::readWord() {
    std::string variable;
    while(isalpha(peek()) && currIndex_ < line_.size()) {
        variable += peek();
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
    while(peek() != '"' && currIndex_ < line_.size()) {
        strVar += peek();
        currIndex_++;
    }
    if(peek() != '"') throw std::runtime_error("Missing closing \" for string");
    currIndex_++;
    return Token{TokenType::STRING, strVar};
}