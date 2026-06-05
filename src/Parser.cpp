#include "../include/Parser.h"

Parser::Parser(const std::vector<Token>& tokenList) { tokens = tokenList; }
Token Parser::peek() { return tokens[current]; }
Token Parser::previous() { return tokens[current - 1]; }
bool Parser::isAtEnd() { return peek().type == TokenType::EOF_TOKEN; }
bool Parser::check(TokenType type) {
    if (isAtEnd()) return false;
    return peek().type == type;
}
bool Parser::match(std::initializer_list<TokenType> types) {
    for (TokenType type : types) {
        if (check(type)) { current++; return true; }
    }
    return false;
}

std::vector<std::unique_ptr<ASTNode>> Parser::parse() {
    std::vector<std::unique_ptr<ASTNode>> statements;
    while (!isAtEnd()) statements.push_back(declaration());
    return statements;
}

std::unique_ptr<ASTNode> Parser::declaration() {
    if (match({TokenType::LET})) return varDeclaration();
    return statement(); 
}

std::unique_ptr<ASTNode> Parser::varDeclaration() {
    if (!match({TokenType::IDENTIFIER})) throw std::runtime_error("Expect variable name.");
    std::string name = previous().lexeme;
    std::unique_ptr<ASTNode> initializer = nullptr;
    
    if (match({TokenType::EQUALS})) initializer = assignment(); 
    if (!match({TokenType::SEMICOLON})) throw std::runtime_error("Expect ';' after variable declaration.");
    
    return std::make_unique<AssignmentNode>(name, std::move(initializer));
}

std::unique_ptr<ASTNode> Parser::statement() {
    if (match({TokenType::IF})) return ifStatement();
    if (match({TokenType::WHILE})) return whileStatement();
    if (match({TokenType::LEFT_BRACE})) return block();
    if (match({TokenType::PRINT})) return printStatement();
    return expressionStatement();
}

std::unique_ptr<ASTNode> Parser::ifStatement() {
    std::unique_ptr<ASTNode> condition = assignment();
    std::unique_ptr<ASTNode> thenBranch = statement();
    std::unique_ptr<ASTNode> elseBranch = nullptr;
    if (match({TokenType::ELSE})) elseBranch = statement();
    return std::make_unique<IfNode>(std::move(condition), std::move(thenBranch), std::move(elseBranch));
}

std::unique_ptr<ASTNode> Parser::whileStatement() {
    std::unique_ptr<ASTNode> condition = assignment();
    std::unique_ptr<ASTNode> body = statement();
    return std::make_unique<WhileNode>(std::move(condition), std::move(body));
}

std::unique_ptr<ASTNode> Parser::block() {
    auto blockNode = std::make_unique<BlockNode>();
    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        blockNode->statements.push_back(declaration());
    }
    if (!match({TokenType::RIGHT_BRACE})) throw std::runtime_error("Expect '}' after block.");
    return blockNode;
}

std::unique_ptr<ASTNode> Parser::printStatement() {
    std::unique_ptr<ASTNode> value = assignment(); 
    if (!match({TokenType::SEMICOLON})) throw std::runtime_error("Expect ';' after value.");
    return std::make_unique<PrintNode>(std::move(value));
}

std::unique_ptr<ASTNode> Parser::expressionStatement() {
    std::unique_ptr<ASTNode> expr = assignment();
    if (!match({TokenType::SEMICOLON})) throw std::runtime_error("Expect ';' after expression.");
    return expr;
}

// Handles updating variables: x = x + 1;
std::unique_ptr<ASTNode> Parser::assignment() {
    std::unique_ptr<ASTNode> expr = equality(); 
    if (match({TokenType::EQUALS})) {
        std::unique_ptr<ASTNode> value = assignment(); 
        if (auto varNode = dynamic_cast<VariableNode*>(expr.get())) {
            return std::make_unique<AssignmentNode>(varNode->name, std::move(value));
        }
        throw std::runtime_error("Invalid assignment target.");
    }
    return expr;
}

std::unique_ptr<ASTNode> Parser::equality() {
    std::unique_ptr<ASTNode> expr = comparison();
    while (match({TokenType::EQUAL_EQUAL})) {
        Token op = previous();
        std::unique_ptr<ASTNode> right = comparison();
        expr = std::make_unique<BinaryOpNode>(std::move(expr), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<ASTNode> Parser::comparison() {
    std::unique_ptr<ASTNode> expr = expression();
    while (match({TokenType::LESS})) {
        Token op = previous();
        std::unique_ptr<ASTNode> right = expression();
        expr = std::make_unique<BinaryOpNode>(std::move(expr), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<ASTNode> Parser::expression() {
    std::unique_ptr<ASTNode> expr = term();
    while (match({TokenType::PLUS, TokenType::MINUS})) {
        Token op = previous();
        std::unique_ptr<ASTNode> right = term();
        expr = std::make_unique<BinaryOpNode>(std::move(expr), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<ASTNode> Parser::term() {
    std::unique_ptr<ASTNode> expr = primary();
    while (match({TokenType::STAR, TokenType::SLASH})) {
        Token op = previous();
        std::unique_ptr<ASTNode> right = primary();
        expr = std::make_unique<BinaryOpNode>(std::move(expr), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<ASTNode> Parser::primary() {
    if (match({TokenType::TRUE_TOKEN})) return std::make_unique<BoolNode>(true);
    if (match({TokenType::FALSE_TOKEN})) return std::make_unique<BoolNode>(false);
    if (match({TokenType::INPUT})) return std::make_unique<InputNode>();
    if (match({TokenType::NUMBER})) return std::make_unique<NumberNode>(std::stoi(previous().lexeme));
    if (match({TokenType::IDENTIFIER})) return std::make_unique<VariableNode>(previous().lexeme);
    if (match({TokenType::LEFT_PAREN})) {
        std::unique_ptr<ASTNode> expr = assignment(); 
        match({TokenType::RIGHT_PAREN}); 
        return expr;
    }
    throw std::runtime_error("Expect expression.");
}