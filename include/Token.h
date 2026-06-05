#pragma once
#include <string>

enum class TokenType {
    // Single-character tokens
    PLUS, MINUS, STAR, SLASH, SEMICOLON,
    LEFT_BRACE, RIGHT_BRACE, 
    LEFT_PAREN, RIGHT_PAREN, 

    // One or two character tokens
    EQUALS, EQUAL_EQUAL, // = and ==
    LESS,                // <

    // Literals (Values)
    NUMBER, IDENTIFIER,

    // Keywords
    LET, PRINT, INPUT, IF, ELSE, WHILE,
    TRUE_TOKEN, FALSE_TOKEN, // Native boolean values

    // End of File marker
    EOF_TOKEN   
};

struct Token {
    TokenType type;
    std::string lexeme;
};