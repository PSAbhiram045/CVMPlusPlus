#pragma once
#include "OpCode.h"
#include <vector>
#include <variant>
#include <unordered_map>
#include <string>

// The engine memory can hold ints or native bools safely
using Value = std::variant<int, bool>;

class VM {
public:
    void interpret(const std::vector<uint8_t>& bytecode, const std::vector<std::string>& strings);

private:
    std::vector<uint8_t> code;  
    std::vector<std::string> stringPool; 
    std::unordered_map<std::string, Value> globals; // Memory map

    int ip = 0;                 
    std::vector<Value> stack;   

    void push(Value value);
    Value pop();
    uint8_t readByte();
};