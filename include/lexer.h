#pragma once
#include "Token.h"
#include <vector>
#include <string>

class Lexer {
public:
    Lexer(const std::string& sourceCode);
    std::vector<Token> scanTokens();

private:
    std::string source;
    std::vector<Token> tokens;
    
    int start = 0;
    int current = 0;

    bool isAtEnd();
    char advance();
    char peek();           
    bool match(char expected); 
    bool isDigit(char c);  
    bool isAlpha(char c);  
    
    void scanToken();
    void addToken(TokenType type, std::string lexeme);
    
    void number();         
    void identifier();     
};