# Minimal Turing Complete CPU Project

A comprehensive implementation of a minimal Turing complete CPU with a custom Domain Specific Language (DSL) compiler, demonstrating fundamental computer architecture concepts.

## Minimal Instruction Set Philosophy

This CPU demonstrates that **Turing completeness can be achieved with just 6 core instructions**. The design philosophy follows the principle of minimalism while maintaining full computational capability.

### Core Instructions (6 fundamental operations)
```
LOAD_VAR    - Load data from memory address to register
LOAD_CONST  - Load immediate constant value to register  
STORE       - Store register value to memory address
STORE_CONST - Store immediate constant to memory address
ADD         - Arithmetic addition of two registers
SUB         - Arithmetic subtraction with carry flag
JNZ         - Conditional jump (Jump if Not Zero)
JZ          - Conditional jump (Jump if Zero)
HALT        - Program termination
```

### Instruction Variations (3 convenience forms)
The 9 total instructions include 3 variations for programmer convenience:
- **LOAD_VAR vs LOAD_CONST**: Separate instructions for memory vs immediate addressing
- **STORE vs STORE_CONST**: Separate instructions for register vs immediate values
- **JNZ vs JZ**: Complementary conditional jump instructions

This design choice simplifies the compiler while maintaining a clean instruction set architecture.

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
  - Automatically set by SUB instruction
  - R2 = 1 if underflow occurred (Rd < Rs)
  - R2 = 0 if no underflow (Rd >= Rs)
  - Enables efficient comparison operations

- **Comparison Logic**: SUB sets R2=1 if underflow (Rd < Rs), R2=0 otherwise
  - Eliminates need for separate comparison instruction
  - Enables complex conditional logic with minimal instructions
  - Supports all comparison operators through arithmetic operations

- **Conditional Jumps**: JZ/JNZ use R2 for comparison results
  - JZ R2, addr: Jump if R2 == 0 (meaning Rd >= Rs)
  - JNZ R2, addr: Jump if R2 != 0 (meaning Rd < Rs)
  - Enables if-then-else constructs and loops

### Turing Completeness Proof
This instruction set is Turing complete because it provides:
1. **Data manipulation**: LOAD/STORE for memory access
2. **Arithmetic operations**: ADD/SUB for computation
3. **Control flow**: JZ/JNZ for conditional branching
4. **Program termination**: HALT for stopping execution
5. **Unbounded memory**: 64KB addressable space
6. **Conditional logic**: Carry flag enables complex decisions

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
   - Scans input character by character
   - Recognizes keywords, identifiers, numbers, operators
   - Handles whitespace and comments
   - Generates token stream with line/column information

2. **Parsing**: Linear scanning with memory write-back approach
   - **No AST construction**: Uses direct linear scanning for simplicity
   - **Memory write-back**: Variables and labels are resolved during scanning
   - **Single-pass parsing**: Generates IR directly without intermediate tree structures
   - **Backpatching**: Labels are resolved in a second pass after code generation

3. **Code Generation**: Generate intermediate representation (IR)
   - Converts parsed statements to IR instructions
   - Handles variable allocation and memory management
   - Generates temporary variables for complex expressions
   - Optimizes instruction sequences where possible

4. **Assembly**: Convert IR to machine code
   - Maps IR instructions to CPU opcodes
   - Generates binary machine code
   - Handles instruction encoding and operand placement
   - Creates assembly listing for debugging

5. **Linking**: Resolve labels and addresses
   - Backpatching: Fills in label addresses after code generation
   - Address calculation: Converts symbolic labels to absolute addresses
   - Final binary generation: Produces executable machine code

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

#### Lexer (Tokenization Engine)
- **Character-by-character scanning**: Processes input stream sequentially
- **Token recognition**: Identifies keywords, identifiers, numbers, operators
- **Error handling**: Reports syntax errors with precise line/column locations
- **Comment support**: Handles single-line comments (//)
- **Whitespace handling**: Skips spaces, tabs, newlines
- **Token classification**: Categorizes tokens by type (keyword, identifier, number, operator)

#### Parser (Linear Scanning Parser)
- **Single-pass parsing**: No AST construction, direct IR generation
- **Memory write-back**: Variables and labels resolved during scanning
- **Statement recognition**: Identifies let, if, goto, label, out, halt statements
- **Expression parsing**: Handles arithmetic expressions with operator precedence
- **Label resolution**: Maps symbolic labels to memory addresses
- **Variable management**: Allocates memory addresses for variables

#### Code Generator (IR to Machine Code)
- **Instruction mapping**: Converts IR operations to CPU opcodes
- **Memory allocation**: Manages variable storage in data section
- **Backpatching system**: Resolves forward references to labels
- **Optimization**: Eliminates redundant instructions where possible
- **Binary generation**: Produces executable machine code
- **Debug output**: Generates assembly listing and hex dump

#### Backpatching Algorithm
```
1. First pass: Generate code with placeholder addresses for labels
2. Build label map: Record actual addresses of label definitions
3. Second pass: Replace placeholders with actual label addresses
4. Final output: Complete machine code with resolved addresses
```

## Usage Examples

### Basic Program Examples

#### Simple Comparison
```dsl
let x = 5;
let y = 10;
if x <= y goto continue;
halt;
label continue;
out x;
halt;
```

#### Loop Example
```dsl
let counter = 0;
let limit = 5;
label loop:
if counter <= limit goto print;
halt;
label print:
out counter;
let counter = counter + 1;
goto loop;
```

#### Conditional Logic
```dsl
let a = 15;
let b = 10;
if a <= b goto less_equal;
out a;  // a > b
halt;
label less_equal:
out b;  // a <= b
halt;
```

#### Generated Machine Code (for simple comparison)
```
0000: 0x02 0x03 0x01    ; LOAD R3, 1 (for JNZ as GOTO)
0003: 0x04 0x80 0x00 0x05 ; STORE_CONST 0x8000, 5 (x)
0007: 0x04 0x80 0x01 0x0a ; STORE_CONST 0x8001, 10 (y)
000b: 0x01 0x00 0x80 0x00 ; LOAD R0, 0x8000 (x)
000f: 0x01 0x01 0x80 0x01 ; LOAD R1, 0x8001 (y)
0013: 0x06 0x01 0x00    ; SUB R1, R0 (compare y-x)
0016: 0x08 0x02 0x00 0x1a ; JZ R2, 0x001a (if x<=y)
001a: 0x01 0x00 0x80 0x00 ; LOAD R0, 0x8000 (x)
001e: 0x03 0xff 0x00 0x00 ; STORE 0xFF00, R0 (output x)
0022: 0x00               ; HALT
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
- **Clock**: Single-cycle execution model
- **Pipeline**: No pipelining (simple fetch-execute cycle)

### Instruction Encoding
```
HALT:           [0x00]                           (1 byte)
LOAD_CONST:     [0x02][Rd][const]               (3 bytes)
LOAD_VAR:       [0x01][Rd][addr_high][addr_low] (4 bytes)
STORE_CONST:    [0x04][addr_high][addr_low][const] (4 bytes)
STORE:          [0x03][addr_high][addr_low][Rs] (4 bytes)
ADD:            [0x05][Rd][Rs]                  (3 bytes)
SUB:            [0x06][Rd][Rs]                  (3 bytes)
JNZ:            [0x07][Rd][addr_high][addr_low] (4 bytes)
JZ:             [0x08][Rd][addr_high][addr_low] (4 bytes)
```

### Memory Organization
```
Address Range    Purpose                    Size
0x0000-0x7FFF   Code Section              32KB
0x8000-0xFEFF   Data Section (Variables)  ~30KB
0xFF00          Output Register           1 byte
0xFF01-0xFFFF   Reserved                  255 bytes
```

### Register Usage Convention
- **R0**: Primary accumulator, temporary storage
- **R1**: Secondary operand, temporary storage  
- **R2**: Carry flag for comparisons (automatically set by SUB)
- **R3**: Constant 1 (used for unconditional jumps via JNZ)

### Compiler Features
- **Error Handling**: Comprehensive error reporting
  - Line and column number tracking
  - Descriptive error messages
  - Graceful error recovery
  - Syntax validation

- **Debug Output**: Assembly listing and hex dump
  - Human-readable assembly code
  - Hex byte dump for binary analysis
  - Instruction-by-instruction breakdown
  - Memory address mapping

- **Optimization**: Basic instruction optimization
  - Dead code elimination
  - Constant folding
  - Register allocation optimization
  - Instruction sequence optimization

- **Cross-Platform**: C++ implementation
  - Standard C++11 compliance
  - Portable across operating systems
  - No external dependencies
  - Easy compilation with standard tools

### Performance Characteristics
- **Compilation Speed**: Single-pass compilation for fast builds
- **Memory Usage**: Minimal memory footprint during compilation
- **Code Size**: Optimized instruction sequences
- **Execution Speed**: Direct machine code execution

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
