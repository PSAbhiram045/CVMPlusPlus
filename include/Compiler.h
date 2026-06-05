#pragma once
#include "AST.h"
#include "OpCode.h"
#include <vector>
#include <string>
#include <memory>

class Compiler {
public:
    std::vector<uint8_t> compile(const std::vector<std::unique_ptr<ASTNode>>& nodes);
    const std::vector<std::string>& getStringPool() const { return stringPool; }

private:
    std::vector<uint8_t> bytecode;
    std::vector<std::string> stringPool; 

    void emitByte(uint8_t byte);
    uint8_t addString(const std::string& str); 
    
    // Backpatching Helpers
    int emitJump(uint8_t instruction); 
    void patchJump(int offsetIndex);   
    
    void visit(ASTNode* node);
};