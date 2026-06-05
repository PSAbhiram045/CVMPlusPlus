======================================================
CVM++ : A Custom Compiler and Virtual Machine
======================================================

BUILDING (Windows / MinGW)
--------------------------
From your terminal, clone the repository and navigate into the project root:

  > git clone https://github.com/PSAbhiram045/CVM.git
  > cd CVM

Then, run the following commands to build via CMake:

  > mkdir build
  > cd build
  > cmake -G "MinGW Makefiles" ..
  > cmake --build .

OVERVIEW
--------
CVM++ is a custom programming language built entirely from scratch in C++.
Rather than relying on existing interpreter tools or wrappers, we wrote the
entire pipeline by hand. Source code is processed through a custom lexer, a
recursive-descent parser, a bytecode compiler, and finally a stack-based
virtual machine.

The language supports variables, arithmetic, comparisons, conditionals
(if/else), and loops (while) — enough to implement any general-purpose
algorithm from Fibonacci to sorting. We built this project to peel back the
layers of abstraction and understand exactly how raw text on a screen is
transformed into executable instructions.

HOW IT WORKS (THE PIPELINE)
---------------------------
Code execution moves through four distinct stages:

  1. Lexer:      Scans raw text character-by-character and converts it into a
                 stream of recognizable tokens (e.g., NUMBER, PLUS, LET).

  2. Parser:     A recursive-descent parser consumes the tokens and builds an
                 Abstract Syntax Tree (AST). It enforces operator precedence
                 (so 1 + 2 * 3 correctly parses as 1 + (2 * 3)).

  3. Compiler:   Walks the AST and flattens it into a linear array of bytecode
                 instructions. Control flow (if/while) is managed using jump
                 instructions with backpatched offsets.

  4. VM:         A stack-based execution loop reads the bytecode one opcode
                 at a time. Values are pushed and popped from a value stack,
                 while variables are stored in a global map keyed by name.

Each stage has a single clear input and output, which kept the architecture
modular and made it testable layer by layer.

PREREQUISITES
-------------
- A C++17 compiler (g++ via MinGW on Windows)
- CMake 3.10 or newer

BUILDING (Windows / MinGW)
--------------------------
From the project root directory, run the following commands to build via CMake:

  > mkdir build
  > cd build
  > cmake -G "MinGW Makefiles" ..
  > cmake --build .

The compiled executable, cvm.exe, will be generated inside the build folder.

RUNNING CVM++
-------------
The engine supports two execution modes:

1. Interactive REPL (Read-Eval-Print Loop)
   Useful for quick experiments. It supports multi-line block detection for
   loops:

   > .\cvm.exe

2. File Execution
   Run a complete script by passing the file path as an argument:

   > .\cvm.exe ..\demo.cvm

A sample script (demo.cvm) is included in the root directory.

PROJECT LAYOUT
--------------
  include/
    Token.h        - Token definitions and types
    AST.h          - Node structures for the Abstract Syntax Tree
    OpCode.h       - Bytecode opcode definitions
    Lexer.h, Parser.h, Compiler.h, VM.h
  src/
    Lexer.cpp      - Source text -> Tokens
    Parser.cpp     - Tokens -> AST
    Compiler.cpp   - AST -> Bytecode
    VM.cpp         - Executes bytecode on the stack
    main.cpp       - CLI entry point (REPL and File reader)

DESIGN PHILOSOPHY
-----------------
The goal of this project was to build a complete, working compiler pipeline
end to end rather than a massive language feature set. By focusing on core
data types (integers and booleans), foundational arithmetic, and basic
control flow, we were able to keep every stage testable in isolation. AST node
ownership is handled with smart pointers rather than manual new/delete, which
kept memory management predictable across the parser and compiler.
