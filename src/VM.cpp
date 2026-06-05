#include "../include/VM.h"
#include <iostream>
#include <stdexcept>

void VM::push(Value value) { stack.push_back(value); }
Value VM::pop() {
    Value value = stack.back();
    stack.pop_back();
    return value;
}
uint8_t VM::readByte() { return code[ip++]; }

void VM::interpret(const std::vector<uint8_t>& bytecode, const std::vector<std::string>& strings) {
    code = bytecode;
    stringPool = strings; 
    ip = 0;

    while (true) {
        uint8_t instruction = readByte();

        switch (static_cast<OpCode>(instruction)) {
            case OpCode::OP_PUSH: {
                uint8_t value = readByte(); 
                push(static_cast<int>(value));
                break;
            }
            case OpCode::OP_PUSH_TRUE: push(true); break;
            case OpCode::OP_PUSH_FALSE: push(false); break;
            case OpCode::OP_INPUT: {
                int inputValue;
                std::cout << "> Enter a number: ";
                
                // Read a string to prevent buffer issues with the REPL
                std::string inputStr;
                std::getline(std::cin, inputStr);
                
                try {
                    inputValue = std::stoi(inputStr);
                } catch (...) {
                    throw std::runtime_error("Invalid input. Expected an integer.");
                }
                
                push(inputValue);
                break;
            }
            
            case OpCode::OP_ADD: {
                int b = std::get<int>(pop()); 
                int a = std::get<int>(pop()); 
                push(a + b);
                break;
            }
            case OpCode::OP_SUB: {
                int b = std::get<int>(pop());
                int a = std::get<int>(pop());
                push(a - b);
                break;
            }
            case OpCode::OP_MUL: {
                int b = std::get<int>(pop()); 
                int a = std::get<int>(pop()); 
                push(a * b);
                break;
            }
            case OpCode::OP_DIV: {
                int b = std::get<int>(pop());
                int a = std::get<int>(pop());
                if (b == 0) throw std::runtime_error("Error: Division by zero.");
                push(a / b);
                break;
            }
            case OpCode::OP_EQUAL: {
                Value b = pop();
                Value a = pop();
                push(a == b); 
                break;
            }
            case OpCode::OP_LESS: {
                int b = std::get<int>(pop());
                int a = std::get<int>(pop());
                push(a < b);
                break;
            }
            case OpCode::OP_STORE_GLOBAL: {
                uint8_t nameIndex = readByte(); 
                std::string varName = stringPool[nameIndex]; 
                globals[varName] = pop(); 
                break;
            }
            case OpCode::OP_LOAD_GLOBAL: {
                uint8_t nameIndex = readByte();
                std::string varName = stringPool[nameIndex];
                if (globals.find(varName) == globals.end()) {
                    throw std::runtime_error("Undefined variable: " + varName);
                }
                push(globals[varName]); 
                break;
            }
            case OpCode::OP_JUMP_IF_FALSE: {
                uint8_t jumpOffset = readByte();
                Value condition = pop();
                bool isTrue = false;
                
                if (std::holds_alternative<bool>(condition)) isTrue = std::get<bool>(condition);
                else isTrue = std::get<int>(condition) != 0; 

                if (!isTrue) ip += jumpOffset; 
                break;
            }
            case OpCode::OP_JUMP: {
                uint8_t jumpOffset = readByte();
                ip += jumpOffset;
                break;
            }
            case OpCode::OP_LOOP: {
                uint8_t jumpOffset = readByte();
                ip -= jumpOffset;
                break;
            }
            case OpCode::OP_PRINT: {
                Value value = pop();
                if (std::holds_alternative<int>(value)) std::cout << "VM Output: " << std::get<int>(value) << "\n";
                else std::cout << "VM Output: " << (std::get<bool>(value) ? "true" : "false") << "\n";
                break;
            }
            case OpCode::OP_HALT: return; 
        }
    }
}