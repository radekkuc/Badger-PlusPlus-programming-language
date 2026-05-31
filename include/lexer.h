#pragma once
#include <vector>
#include <string>
#include <cstdint>


enum class TokenType : uint8_t {
    ASTERISK, SLASH, PLUS, MINUS, EQUALS, SEMICOLON, COMMA,
    GREATER, SMALLER, OR, AND, NOT, COMPARISON, NCOMPARISON,
    LPAREN, RPAREN, LCURLY, RCURLY,
    NUMBER, VARIABLE, ENDOFFILE,
    STRING, BOOL,
    LET, PRINT, PRINTLN, RETURN,
    IF, ELSE, WHILE, FUN
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
    char peek();
    char peekNext();

    void skipWhiteSpace();
};