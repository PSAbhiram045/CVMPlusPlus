#pragma once
#include <cstdint>

// These are the raw byte instructions your Virtual Machine will execute
enum class OpCode : uint8_t {
    // Math & Stack
    OP_PUSH, OP_ADD, OP_SUB,
     OP_MUL, OP_DIV, OP_EQUAL,
      OP_LESS, 
    
    // Memory
    OP_STORE_GLOBAL,
     OP_LOAD_GLOBAL,
    
    // Control Flow
    OP_JUMP_IF_FALSE, 
    OP_JUMP, 
    OP_LOOP,OP_INPUT,
    
    // Native Booleans
    OP_PUSH_TRUE, 
    OP_PUSH_FALSE,
    
    // System
    OP_PRINT, 
    OP_HALT
};