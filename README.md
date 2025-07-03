# Minimal Turing Complete CPU Project

A comprehensive implementation of a minimal Turing complete CPU with a custom Domain Specific Language (DSL) compiler, demonstrating fundamental computer architecture concepts.

## Project Overview

This project implements a complete computer system from the ground up:
- **CPU**: A minimal but Turing complete CPU with a custom instruction set
- **DSL Compiler**: A compiler that translates high-level code to machine code
- **Assembler**: Code generation and linking system
- **Runtime**: CPU simulator that executes the generated machine code

## Project Structure

```
minimal_turing_complete_CPU/
├── section-1-6-instructions-cpu/     # Basic CPU implementation
├── section-2-DSL-compiler/           # DSL compiler and code generation
└── README.md                         # This file
```

## Architecture Overview

### CPU Design
- **Memory**: 64KB (65536 bytes) of RAM
- **Registers**: 4 general-purpose registers (R0-R3)
- **Program Counter (PC)**: 16-bit register for instruction fetching
- **Instruction Set**: 9 instructions for Turing completeness

### Memory Layout
- **Code Section**: 0x0000 - 0x7FFF (32KB for program)
- **Data Section**: 0x8000 - 0xFF00 (~32KB for variables)
- **Output Register**: 0xFF00 (for program output)

## Instruction Set Architecture (ISA)

### Core Instructions
| Opcode | Mnemonic | Description | Format |
|--------|----------|-------------|---------|
| 0x00 | HALT | Stop program execution | HALT |
| 0x01 | LOAD | Load from memory to register | LOAD Rd, addr |
| 0x02 | LOAD_CONST | Load constant to register | LOAD Rd, const |
| 0x03 | STORE | Store register to memory | STORE addr, Rs |
| 0x04 | STORE_CONST | Store constant to memory | STORE addr, const |
| 0x05 | ADD | Add two registers | ADD Rd, Rs |
| 0x06 | SUB | Subtract two registers | SUB Rd, Rs |
| 0x07 | JNZ | Jump if register not zero | JNZ Rd, addr |
| 0x08 | JZ | Jump if register zero | JZ Rd, addr |

### Special Features
- **Carry Register**: R2 serves as carry flag for comparisons
- **Comparison Logic**: SUB sets R2=1 if underflow (Rd < Rs), R2=0 otherwise
- **Conditional Jumps**: JZ/JNZ use R2 for comparison results

## Domain Specific Language (DSL)

### Syntax
The DSL provides a high-level interface for programming the CPU:

```dsl
let a = 10;
let b = 20;
if a <= b goto end;
label end;
halt;
```

### Language Features
- **Variable Declaration**: `let variable = value;`
- **Conditional Logic**: `if variable1 <= variable2 goto label;`
- **Labels**: `label_name:`
- **Unconditional Jumps**: `goto label;`
- **Output**: `out variable;`
- **Program Termination**: `halt;`

### Compilation Process
1. **Lexical Analysis**: Tokenize input into tokens
2. **Parsing**: Use the linear scanning and memory write back method to simplify parsing.
3. **Code Generation**: Generate intermediate representation (IR)
4. **Assembly**: Convert IR to machine code
5. **Linking**: Resolve labels and addresses

## Implementation Details

### Section 1-6: Basic CPU
- **cpu.h**: CPU class with fetch-execute cycle
- **main.cpp**: Program loader and runner
- **test.cpp**: Unit tests for CPU functionality

### Section 2: DSL Compiler
- **lexer.cpp**: Tokenizes DSL input
- **parser.cpp**: Parses tokens into IR
- **codegen.cpp**: Generates machine code from IR
- **compiler.cpp**: Main compiler driver

### Key Components

#### Lexer
- Handles whitespace and comments
- Recognizes keywords, identifiers, numbers, operators
- Provides token stream for parser

#### Parser
- Implements recursive descent parsing
- Generates intermediate representation (IR)
- Handles variable scoping and label resolution

#### Code Generator
- Converts IR to machine code
- Implements backpatching for label resolution
- Generates optimized instruction sequences

## Usage Examples

### Basic Program
```dsl
let x = 5;
let y = 10;
if x <= y goto continue;
halt;
label continue;
out x;
halt;
```

### Compilation and Execution
```bash
# Compile DSL to machine code
cd section-2-DSL-compiler
make
./build/compiler input.dsl

# Run on CPU simulator
cd ../section-1-6-instructions-cpu
make
./build/cpu ../section-2-DSL-compiler/build/output.hex
```

## Development Status

### Completed
- ✅ Basic CPU implementation with 9 instructions
- ✅ DSL lexer and parser
- ✅ Code generation with backpatching
- ✅ CPU simulator with debug output
- ✅ Complete compilation pipeline

### Future Enhancements
- 🔄 Assembler with macro support
- 🔄 Runtime environment with I/O
- 🔄 Optimizing compiler
- 🔄 Extended instruction set
- 🔄 Memory management
- 🔄 Interrupt handling

## Technical Specifications

### CPU Architecture
- **Word Size**: 8-bit data, 16-bit addresses
- **Endianness**: Little-endian
- **Instruction Format**: Variable length (1-4 bytes)
- **Addressing Modes**: Direct, immediate, register

### Compiler Features
- **Error Handling**: Comprehensive error reporting
- **Debug Output**: Assembly listing and hex dump
- **Optimization**: Basic instruction optimization
- **Cross-Platform**: C++ implementation

## Contributing

This project serves as an educational tool for understanding:
- Computer architecture fundamentals
- Compiler design principles
- Assembly language concepts
- CPU design and implementation

## License

Educational project - feel free to use and modify for learning purposes.

---

*This project demonstrates the complete pipeline from high-level language to machine execution, providing hands-on experience with computer architecture and compiler design.*
