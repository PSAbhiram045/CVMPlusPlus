#pragma once
#include "Token.h"
#include "AST.h"
#include <vector>
#include <memory>
#include <stdexcept>

class Parser {
public:
    Parser(const std::vector<Token>& tokenList);
    std::vector<std::unique_ptr<ASTNode>> parse();

private:
    std::vector<Token> tokens;
    int current = 0;

    // Statements
    std::unique_ptr<ASTNode> declaration();        
    std::unique_ptr<ASTNode> varDeclaration();     
    std::unique_ptr<ASTNode> statement();          
    std::unique_ptr<ASTNode> ifStatement();        
    std::unique_ptr<ASTNode> whileStatement();     
    std::unique_ptr<ASTNode> block();              
    std::unique_ptr<ASTNode> printStatement();     
    std::unique_ptr<ASTNode> expressionStatement();

    // Math Precedence
    std::unique_ptr<ASTNode> assignment(); // x = 10;
    std::unique_ptr<ASTNode> equality();   // ==
    std::unique_ptr<ASTNode> comparison(); // <
    std::unique_ptr<ASTNode> expression(); // +, -
    std::unique_ptr<ASTNode> term();       // *, /
    std::unique_ptr<ASTNode> primary();    // Numbers, Variables, ()

    Token peek();
    Token previous();
    bool isAtEnd();
    bool check(TokenType type);
    bool match(std::initializer_list<TokenType> types);
};