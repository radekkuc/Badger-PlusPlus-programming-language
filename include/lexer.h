#pragma once
#include <vector>
#include <string>
#include <cstdint>


enum class TokenType : uint8_t {
    ASTERISK, SLASH, PLUS, MINUS, EQUALS, SEMICOLON, 
    LPAREN, RPAREN, LCURLY, RCURLY,
    NUMBER, VARIABLE, ENDOFFILE,
    STRING, BOOL,
    LET, PRINT, PRINTLN,
    IF, ELSE, BODY
};


struct Token {
    TokenType type;
    std::string value;
};


class Lexer {
private:
    std::string line_;
    size_t currIndex_;
public:
    explicit Lexer(const std::string& line);

    std::vector<Token> identify();
    Token readString();
    Token readNumber();
    Token readWord();

    void skipWhiteSpace();
};