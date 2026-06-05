#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "../include/Lexer.h"
#include "../include/Parser.h"
#include "../include/Compiler.h"
#include "../include/VM.h"
#include <iomanip>
// Prints the compiled bytecode to the terminal!
void debugPrintBytecode(const std::vector<uint8_t>& bytecode, const std::vector<std::string>& stringPool) {
    std::cout << "\n=== COMPILER OUTPUT (BYTECODE) ===\n";
    for (size_t i = 0; i < bytecode.size(); ) {
        // Print the byte index (e.g., 0000, 0002)
        std::cout << std::setfill('0') << std::setw(4) << i << "  ";
        
        uint8_t instruction = bytecode[i++];
        switch (static_cast<OpCode>(instruction)) {
            case OpCode::OP_PUSH:          std::cout << "OP_PUSH " << (int)bytecode[i++] << "\n"; break;
            case OpCode::OP_PUSH_TRUE:     std::cout << "OP_PUSH_TRUE\n"; break;
            case OpCode::OP_PUSH_FALSE:    std::cout << "OP_PUSH_FALSE\n"; break;
            case OpCode::OP_INPUT:         std::cout << "OP_INPUT\n"; break;
            case OpCode::OP_ADD:           std::cout << "OP_ADD\n"; break;
            case OpCode::OP_SUB:           std::cout << "OP_SUB\n"; break;
            case OpCode::OP_MUL:           std::cout << "OP_MUL\n"; break;
            case OpCode::OP_DIV:           std::cout << "OP_DIV\n"; break;
            case OpCode::OP_EQUAL:         std::cout << "OP_EQUAL\n"; break;
            case OpCode::OP_LESS:          std::cout << "OP_LESS\n"; break;
            case OpCode::OP_PRINT:         std::cout << "OP_PRINT\n"; break;
            case OpCode::OP_HALT:          std::cout << "OP_HALT\n"; break;
            
            case OpCode::OP_STORE_GLOBAL: {
                uint8_t index = bytecode[i++];
                std::cout << "OP_STORE_GLOBAL " << (int)index << " ('" << stringPool[index] << "')\n";
                break;
            }
            case OpCode::OP_LOAD_GLOBAL: {
                uint8_t index = bytecode[i++];
                std::cout << "OP_LOAD_GLOBAL " << (int)index << " ('" << stringPool[index] << "')\n";
                break;
            }
            case OpCode::OP_JUMP_IF_FALSE: std::cout << "OP_JUMP_IF_FALSE (offset: " << (int)bytecode[i++] << ")\n"; break;
            case OpCode::OP_JUMP:          std::cout << "OP_JUMP (offset: " << (int)bytecode[i++] << ")\n"; break;
            case OpCode::OP_LOOP:          std::cout << "OP_LOOP (offset: " << (int)bytecode[i++] << ")\n"; break;
            default:                       std::cout << "UNKNOWN_INSTRUCTION\n"; break;
        }
    }
    std::cout << "==================================\n";
}
// Safe execution pipeline wrapper
void runScript(const std::string& code, VM& virtualMachine, Compiler& compiler) {
    try {
        Lexer lexer(code);
        std::vector<Token> tokens = lexer.scanTokens();
        
        Parser parser(tokens);
        std::vector<std::unique_ptr<ASTNode>> ast = parser.parse();
        
        std::vector<uint8_t> bytecode = compiler.compile(ast);
        
        // Print the bytecode before running the VM!
        debugPrintBytecode(bytecode, compiler.getStringPool());
        
        std::cout << "=== VIRTUAL MACHINE EXECUTION ===\n";
        virtualMachine.interpret(bytecode, compiler.getStringPool());
        
    } catch (const std::exception& e) {
        std::cerr << "Compile Error: " << e.what() << "\n";
    }
}

// 1. FILE-BASED EXECUTION
void runFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << path << "\n";
        return;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string code = buffer.str();

    Compiler compiler;
    VM vm;
    runScript(code, vm, compiler);
}

// 2. INTERACTIVE REPL
// 2. ADVANCED INTERACTIVE REPL (Multi-line Support)
void runREPL() {
    std::cout << "CVM++ Interactive REPL (v1.0)\n";
    std::cout << "Type 'exit' to quit.\n";

    Compiler compiler;
    VM vm;

    std::string buffer = ""; // Hoards the text for multi-line blocks
    int openBraces = 0;      // Tracks { and }

    while (true) {
        // Change the prompt if we are inside a block
        if (openBraces == 0) {
            std::cout << "cvm> ";
        } else {
            std::cout << "...> "; 
        }

        std::string line;
        std::getline(std::cin, line);

        if (line == "exit" && openBraces == 0) break;
        if (line.empty() && openBraces == 0) continue;

        // Add the new line to our hoarding buffer
        buffer += line + "\n";

        // Count any braces in the line the user just typed
        for (char c : line) {
            if (c == '{') openBraces++;
            if (c == '}') openBraces--;
        }

        // If openBraces drops below 0, the user typed an extra '}', which is a syntax error
        if (openBraces < 0) {
            std::cerr << "Syntax Error: Unexpected '}'\n";
            buffer = "";
            openBraces = 0;
            continue;
        }

        // Only run the compiler if all braces are closed!
        if (openBraces == 0) {
            runScript(buffer, vm, compiler);
            buffer = ""; // Clear the buffer for the next command
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc == 2) runFile(argv[1]);
    else if (argc > 2) std::cout << "Usage: cvm [path_to_script]\n";
    else runREPL();

    return 0;
}