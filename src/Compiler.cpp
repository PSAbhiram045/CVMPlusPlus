#include "../include/Compiler.h"

void Compiler::emitByte(uint8_t byte) { bytecode.push_back(byte); }

uint8_t Compiler::addString(const std::string& str) {
    // Prevent duplicate strings in memory
    for (size_t i = 0; i < stringPool.size(); ++i) {
        if (stringPool[i] == str) return static_cast<uint8_t>(i);
    }
    stringPool.push_back(str);
    return static_cast<uint8_t>(stringPool.size() - 1);
}

int Compiler::emitJump(uint8_t instruction) {
    emitByte(instruction);
    emitByte(0xff); // Dummy placeholder
    return bytecode.size() - 1; 
}

void Compiler::patchJump(int offsetIndex) {
    int jumpDistance = bytecode.size() - 1 - offsetIndex;
    bytecode[offsetIndex] = static_cast<uint8_t>(jumpDistance);
}

std::vector<uint8_t> Compiler::compile(const std::vector<std::unique_ptr<ASTNode>>& nodes) {
    bytecode.clear();
    stringPool.clear();
    for (const auto& node : nodes) visit(node.get());
    emitByte(static_cast<uint8_t>(OpCode::OP_HALT));
    return bytecode;
}

void Compiler::visit(ASTNode* node) {
    if (!node) return;

    if (auto boolNode = dynamic_cast<BoolNode*>(node)) {
        if (boolNode->value) emitByte(static_cast<uint8_t>(OpCode::OP_PUSH_TRUE));
        else emitByte(static_cast<uint8_t>(OpCode::OP_PUSH_FALSE));
    }
    else if (dynamic_cast<InputNode*>(node)) {
        emitByte(static_cast<uint8_t>(OpCode::OP_INPUT));
    }
    else if (auto numNode = dynamic_cast<NumberNode*>(node)) {
        emitByte(static_cast<uint8_t>(OpCode::OP_PUSH));
        emitByte(static_cast<uint8_t>(numNode->value)); 
    }
    else if (auto varNode = dynamic_cast<VariableNode*>(node)) {
        uint8_t index = addString(varNode->name);
        emitByte(static_cast<uint8_t>(OpCode::OP_LOAD_GLOBAL));
        emitByte(index);
    }
    else if (auto binNode = dynamic_cast<BinaryOpNode*>(node)) {
        visit(binNode->left.get());
        visit(binNode->right.get());
        switch (binNode->op.type) {
            case TokenType::PLUS:        emitByte(static_cast<uint8_t>(OpCode::OP_ADD)); break;
            case TokenType::MINUS:       emitByte(static_cast<uint8_t>(OpCode::OP_SUB)); break;
            case TokenType::STAR:        emitByte(static_cast<uint8_t>(OpCode::OP_MUL)); break;
            case TokenType::SLASH:       emitByte(static_cast<uint8_t>(OpCode::OP_DIV)); break;
            case TokenType::EQUAL_EQUAL: emitByte(static_cast<uint8_t>(OpCode::OP_EQUAL)); break;
            case TokenType::LESS:        emitByte(static_cast<uint8_t>(OpCode::OP_LESS)); break;
            default: break;
        }
    }
    else if (auto assignNode = dynamic_cast<AssignmentNode*>(node)) {
        visit(assignNode->value.get()); 
        uint8_t index = addString(assignNode->varName);
        emitByte(static_cast<uint8_t>(OpCode::OP_STORE_GLOBAL));
        emitByte(index);
    }
    else if (auto printNode = dynamic_cast<PrintNode*>(node)) {
        visit(printNode->expression.get());
        emitByte(static_cast<uint8_t>(OpCode::OP_PRINT));
    }
    else if (auto blockNode = dynamic_cast<BlockNode*>(node)) {
        for (const auto& statement : blockNode->statements) visit(statement.get());
    }
    else if (auto ifNode = dynamic_cast<IfNode*>(node)) {
        visit(ifNode->condition.get()); 
        int thenJump = emitJump(static_cast<uint8_t>(OpCode::OP_JUMP_IF_FALSE));
        visit(ifNode->thenBranch.get()); 
        
        if (ifNode->elseBranch) {
            int elseJump = emitJump(static_cast<uint8_t>(OpCode::OP_JUMP));
            patchJump(thenJump); 
            visit(ifNode->elseBranch.get()); 
            patchJump(elseJump); 
        } else {
            patchJump(thenJump); 
        }
    }
    else if (auto whileNode = dynamic_cast<WhileNode*>(node)) {
        int loopStart = bytecode.size(); 
        visit(whileNode->condition.get()); 
        int exitJump = emitJump(static_cast<uint8_t>(OpCode::OP_JUMP_IF_FALSE));
        
        visit(whileNode->body.get()); 
        emitByte(static_cast<uint8_t>(OpCode::OP_LOOP));
        int loopDistance = bytecode.size() + 1 - loopStart;
        emitByte(static_cast<uint8_t>(loopDistance));
        
        patchJump(exitJump); 
    }
}