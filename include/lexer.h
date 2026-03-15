#pragma once
#include <vector>
#include <string>
#include <cstdint>


enum class TokenType : uint8_t {
    ASTERISK, SLASH, PLUS, MINUS, LPAREN, RPAREN, EQUALS, SEMICOLON, 
    NUMBER, VARIABLE, ENDOFFILE,
    INTEGER, STRING, BOOL, FLOAT, 
    LET, PRINT };


struct Token {
    TokenType type;
    std::string value;
};


class Lexer {
private:
    size_t currIndex_;
    std::string line_;
public:
    explicit Lexer(const std::string& line);

    std::vector<Token> identify();
    Token readNumber();
    Token readWord();

    void skipWhiteSpace();
};