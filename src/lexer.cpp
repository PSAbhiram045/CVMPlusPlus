#include "../include/Lexer.h"

Lexer::Lexer(const std::string& sourceCode) {
    source = sourceCode;
}

bool Lexer::isAtEnd() { return current >= source.length(); }
char Lexer::advance() { return source[current++]; }
char Lexer::peek() {
    if (isAtEnd()) return '\0';
    return source[current];
}
bool Lexer::match(char expected) {
    if (isAtEnd() || source[current] != expected) return false;
    current++;
    return true;
}
bool Lexer::isDigit(char c) { return c >= '0' && c <= '9'; }
bool Lexer::isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

void Lexer::addToken(TokenType type, std::string lexeme) {
    Token token;
    token.type = type;
    token.lexeme = lexeme;
    tokens.push_back(token);
}

void Lexer::scanToken() {
    char c = advance();
    switch (c) {
        case '+': addToken(TokenType::PLUS, "+"); break;
        case '-': addToken(TokenType::MINUS, "-"); break;
        case '*': addToken(TokenType::STAR, "*"); break;
        case '/': addToken(TokenType::SLASH, "/"); break;
        case ';': addToken(TokenType::SEMICOLON, ";"); break;
        case '(': addToken(TokenType::LEFT_PAREN, "("); break;
        case ')': addToken(TokenType::RIGHT_PAREN, ")"); break;
        case '{': addToken(TokenType::LEFT_BRACE, "{"); break;
        case '}': addToken(TokenType::RIGHT_BRACE, "}"); break;
        case '<': addToken(TokenType::LESS, "<"); break;
        
        case '=':
            if (match('=')) addToken(TokenType::EQUAL_EQUAL, "==");
            else addToken(TokenType::EQUALS, "=");
            break;
        
        case ' ': case '\r': case '\t': case '\n': break; 

        default:
            if (isDigit(c)) number();
            else if (isAlpha(c)) identifier();
            break;
    }
}

void Lexer::number() {
    while (isDigit(peek())) advance();
    addToken(TokenType::NUMBER, source.substr(start, current - start));
}

void Lexer::identifier() {
    while (isAlpha(peek()) || isDigit(peek())) advance();
    std::string text = source.substr(start, current - start);
    
    TokenType type = TokenType::IDENTIFIER;
    if (text == "let") type = TokenType::LET;
    else if (text == "print") type = TokenType::PRINT;
    else if (text == "if") type = TokenType::IF;
    else if (text == "else") type = TokenType::ELSE;
    else if (text == "while") type = TokenType::WHILE;
    else if (text == "true") type = TokenType::TRUE_TOKEN;   // Native Boolean
    else if (text == "false") type = TokenType::FALSE_TOKEN; // Native Boolean
    else if (text == "input") type = TokenType::INPUT;
    addToken(type, text);
}

std::vector<Token> Lexer::scanTokens() {
    while (!isAtEnd()) {
        start = current;
        scanToken();
    }
    addToken(TokenType::EOF_TOKEN, "");
    return tokens;
}