# REPL for Minimal Turing Complete CPU

## Overview

This REPL (Read-Eval-Print Loop) provides an interactive environment for executing code on a minimal Turing complete CPU architecture. It demonstrates how a high-level programming language can be interpreted and executed through a multi-stage compilation pipeline.

## Why the REPL Works

The REPL works through a **four-stage pipeline** that transforms high-level code into executable operations:

1. **Lexical Analysis** - Tokenizes input text into meaningful symbols
2. **Parsing** - Builds an Abstract Syntax Tree (AST) and generates Intermediate Representation (IR)
3. **Interpretation** - Executes the IR instructions using a virtual machine

### Key Design Principles

- **Separation of Concerns**: Each stage has a distinct responsibility
- **Intermediate Representation**: IR acts as a bridge between high-level syntax and low-level execution
- **Virtual Machine**: The interpreter provides a clean abstraction over the CPU
- **Error Handling**: Comprehensive error reporting at each stage

## Architecture

```
Input Code -> Lexer -> Parser -> IR -> Interpreter -> Output
              |         |      |        |
           Tokens   AST/IR   IR Code  Execution
```

### Components

#### 1. Lexer (`lexer.h/cpp`)
- **Purpose**: Converts raw text into tokens
- **Input**: Source code string
- **Output**: Stream of tokens (numbers, identifiers, operators, keywords)
- **Key Features**:
  - Handles whitespace and comments
  - Recognizes keywords, operators, and literals
  - Tracks line/column information for error reporting

#### 2. Parser (`parser.h/cpp`)
- **Purpose**: Builds syntax trees and generates IR
- **Input**: Token stream from lexer
- **Output**: Vector of IR instructions
- **Key Features**:
  - **Pratt Parser**: Implements operator precedence parsing for expressions
  - **IR Generation**: Converts AST nodes to intermediate instructions
  - **Error Recovery**: Provides detailed syntax error messages

#### 3. IR Interpreter (`interpreter.h/cpp`)
- **Purpose**: Executes IR instructions
- **Input**: Vector of IR instructions
- **Output**: Program execution results
- **Key Features**:
  - **Variable Management**: Maintains symbol table for variables
  - **Instruction Execution**: Handles arithmetic, control flow, and I/O
  - **Runtime Error Handling**: Detects undefined variables and type errors


### IR (Intermediate Representation)

The IR uses a simple three-address code format:

```cpp
enum class OpCode {
    LOAD_CONST,  // Load constant value
    LOAD_VAR,    // Load variable value
    ADD, SUB,    // Arithmetic operations
    STORE,       // Store to variable
    STORE_CONST, // Store constant
    IFLEQ,       // Conditional branch
    GOTO,        // Unconditional branch
    LABEL,       // Jump target
    OUT,         // Output value
    HALT         // Stop execution
};
```

Each IR instruction has:
- **OpCode**: The operation to perform
- **arg1, arg2**: Source operands
- **result**: Destination operand

## Implementation Details

### Expression Parsing

The parser uses a **Pratt parser** (operator precedence parser) to handle expressions:
```

### Variable Management

The interpreter maintains a symbol table using `std::unordered_map`:

```cpp
std::unordered_map<std::string, int> variables;
```

### Error Handling

Each stage provides specific error messages:
- **Lexer**: Invalid characters, unterminated strings
- **Parser**: Syntax errors with line/column information
- **Interpreter**: Runtime errors (undefined variables, type mismatches)

actually it's pretty simple:
```c++
void IRInterpreter::execute(const std::vector<IR>& ir) {
    for (const auto& inst : ir) {
        if (inst.op == OpCode::LOAD_CONST) {
            variables[inst.result] = std::stoi(inst.arg1);
        } else if (inst.op == OpCode::LOAD_VAR) {
            if (variables.find(inst.arg1) == variables.end()) {
                throw std::runtime_error("Undefined variable: " + inst.arg1);
            }
            variables[inst.result] = variables[inst.arg1];
        } else if (inst.op == OpCode::ADD) {
            if (variables.find(inst.arg1) == variables.end()) {
                throw std::runtime_error("Undefined variable: " + inst.arg1);
            }
            if (variables.find(inst.arg2) == variables.end()) {
                throw std::runtime_error("Undefined variable: " + inst.arg2);
            }
            variables[inst.result] = variables[inst.arg1] + variables[inst.arg2];
        } else if (inst.op == OpCode::SUB) {
            if (variables.find(inst.arg1) == variables.end()) {
                throw std::runtime_error("Undefined variable: " + inst.arg1);
            }
            if (variables.find(inst.arg2) == variables.end()) {
                throw std::runtime_error("Undefined variable: " + inst.arg2);
            }
            variables[inst.result] = variables[inst.arg1] - variables[inst.arg2];
        } else if (inst.op == OpCode::STORE) {
            if (variables.find(inst.arg1) == variables.end()) {
                throw std::runtime_error("Undefined variable: " + inst.arg1);
            }
            variables[inst.result] = variables[inst.arg1];
        } else if (inst.op == OpCode::STORE_CONST) {
            variables[inst.result] = std::stoi(inst.arg1);
        } else if (inst.op == OpCode::OUT) {
            if (variables.find(inst.arg1) == variables.end()) {
                throw std::runtime_error("Undefined variable: " + inst.arg1);
            }
            std::cout << variables[inst.arg1] << std::endl;
        } else if (inst.op == OpCode::HALT) {
            throw std::runtime_error("HALT instruction executed");
        }
    }
}
```

## Building and Running



### Build
```bash
make
```

### Run
```bash
make run
```

### Debug Build
```bash
make DEBUG=1
```

## Usage Examples

### Basic Arithmetic
```
>>> let x = 5
>>> let y = 3
>>> let result = x + y
>>> out result
8
```

### Variables and Expressions
```
>>> let a = 10
>>> let b = (a + 5) * 2
>>> out b
30
```

### Program Termination
```
>>> halt
Program halted.
```

## Supported Language Features

### Variable Declaration
```
let variable_name = expression
```

### Assignment
```
variable_name = expression
```

### Arithmetic Expressions
- Addition: `+`
- Subtraction: `-`
- Parentheses for grouping: `(`, `)`

### Control Flow
- Conditional: `ifleq variable value label`
- Unconditional jump: `goto label`
- Labels: `label name`

### I/O
- Output: `out variable`

### Program Control
- Halt: `halt`

## REPL Commands

- `.exit` - Exit the REPL
- Any valid language statement - Execute immediately

## Conclusion

This REPL demonstrates the fundamental principles of interpreter design and shows how high-level programming constructs can be efficiently executed on a minimal computational model.