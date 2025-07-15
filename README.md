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
├── section-2.1-Pratt-parser/         # Advanced parser with operator precedence
├── section-3-REPL/                   # Basic REPL (Read-Eval-Print Loop)
├── section-3.1-REPL-input/           # REPL with input support
├── section-3.2-REPL-with-simulated-cpu-backend/  # REPL with CPU backend
├── section-4-minimal-OS/             # Minimal operating system implementation
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

## REPL (Read-Eval-Print Loop)

The project includes two REPL implementations that provide interactive programming environments for the DSL.

### Section 3: Basic REPL

A fundamental REPL implementation that supports interactive DSL programming without input operations.

#### Features
- **Interactive Programming**: Execute DSL statements line by line
- **File Loading**: Load and execute complete programs from `.dsl` files
- **Variable Persistence**: Variables persist across statements in the same session
- **Error Handling**: Comprehensive error reporting with line/column information
- **Debug Output**: Optional debug information for program analysis

#### Commands
```
.exit          - Quit the REPL
.clear         - Clear the screen
.help          - Show available commands
.load file.dsl - Load a program from file
.run           - Execute the loaded program
```

#### Usage Example
```bash
cd section-3-REPL
make
./build/REPL

>>> let x = 10;
>>> let y = 20;
>>> let result = x + y;
>>> out result;
30
>>> .load demo.dsl
>>> .run
```

### Section 3.1: REPL with Input Support

An enhanced REPL that extends the basic functionality with user input capabilities.

#### New Features
- **Input Operations**: `in variable;` instruction for reading user input
- **Interactive Programs**: Support for programs that require user interaction
- **Input Validation**: Clear prompts and error handling for input operations
- **Enhanced Debugging**: Improved debug output with input tracking

#### Input Syntax
```dsl
let x = 0;
in x;        // Read a number from user input
out x;       // Display the value
```

#### Interactive Example
```bash
cd section-3.1-REPL-input
make
./build/REPL

>>> let x = 0;
>>> in x;
Enter a number: 42
>>> out x;
42
>>> .load interactive.dsl
>>> .run
Enter a number: 15
15
```

#### Advanced Input Programs
```dsl
// Interactive calculator
let a = 0;
let b = 0;
in a;
in b;
let result = a + b;
out result;
halt;
```

### Section 3.2: REPL with Simulated CPU Backend

A sophisticated hybrid REPL that combines the convenience of interactive programming with the authenticity of running programs on the actual CPU simulator. This represents the most advanced REPL implementation in the project, offering a complete development-to-execution pipeline.

#### Key Innovation: Dual Execution Modes

This REPL introduces a novel approach by providing **two different execution backends** within the same interactive environment:

1. **Interpreter Mode** (`.run`): Fast interpretation for development and testing
2. **CPU Mode** (`.runfromCPU`): Authentic execution on the simulated CPU

#### Why This Matters

**Educational Value**: 
- Students can develop programs interactively using the interpreter
- Then run the same programs on the actual CPU to see "real" machine execution
- Bridges the gap between high-level programming and low-level execution
- Provides hands-on experience with the complete compilation pipeline

**Debugging Benefits**:
- Interpreter provides fast feedback and detailed error messages
- CPU mode shows exactly how the program runs on the actual hardware
- Helps identify differences between interpreted and compiled execution
- Enables debugging at both high-level and machine-level

**Performance Comparison**:
- Interpreter: Fast development cycle, detailed debugging
- CPU: Authentic performance characteristics, real machine behavior
- Students can observe the performance trade-offs between interpretation and compilation

**Learning Outcomes**:
- Understanding of compilation vs interpretation
- Appreciation of machine-level execution
- Debugging skills at multiple abstraction levels
- Performance analysis and optimization awareness

#### New Command: `.runfromCPU`

```bash
>>> .runfromCPU hello.dsl
```

This command orchestrates a complete compilation and execution pipeline:

1. **Compilation Phase**:
   - Parses the DSL program using the full compiler pipeline
   - Generates intermediate representation (IR)
   - Converts IR to machine code with proper instruction encoding
   - Resolves all labels and addresses through backpatching

2. **Code Generation Phase**:
   - Creates assembly listing for human-readable analysis
   - Generates binary machine code file
   - Produces hex dump for debugging and verification
   - Validates code integrity and completeness

3. **CPU Loading Phase**:
   - Loads machine code into the CPU simulator at a specific memory address
   - Initializes CPU registers and memory state
   - Sets up program counter for execution
   - Prepares output handling mechanisms

4. **Execution Phase**:
   - Runs the program on the actual CPU simulator
   - Executes each instruction in the fetch-execute cycle
   - Handles memory access, register operations, and control flow
   - Manages program termination and output

5. **Output Phase**:
   - Captures output from CPU's output register (0xFF00)
   - Displays results in a user-friendly format
   - Provides execution statistics and debugging information

#### Technical Implementation

**Code Generation Pipeline**:
```cpp
// Full compilation pipeline integration
Codegen gen(filename);           // Parse DSL and generate IR
gen.writeToHex("output.bin", "output.hex");  // Generate binary files
gen.writeToFile("output.asm");   // Generate assembly listing
std::vector<uint8_t> code = gen.getCode();  // Get machine code
```

**CPU Integration**:
```cpp
// Direct CPU simulator integration
MinimalCPU cpu;
cpu.loadProgram(code, 0x2000);   // Load at specific address
cpu.run();                       // Execute on CPU
```

**Output Handling**:
```cpp
// CPU detects writes to output register 0xFF00
if (addr == 0xFF00) {
    std::cout << static_cast<int>(R[rs]) << std::endl;
}
```

**Memory Management**:
```cpp
// Proper memory layout and addressing
const uint16_t CODE_START = 0x2000;  // Program loading address
const uint16_t DATA_START = 0x8000;  // Variable storage area
const uint16_t OUTPUT_REG = 0xFF00;  // Output register address
```

#### Advanced Features

**Debug Output Control**:
- Configurable debug output through Makefile switches
- Assembly listing generation for code analysis
- Hex dump generation for binary inspection
- Execution trace capabilities

**Error Handling**:
- Comprehensive error reporting during compilation
- CPU execution error detection and reporting
- Memory access violation detection
- Invalid instruction handling

**File Management**:
- Automatic file generation and cleanup
- Temporary file handling for intermediate results
- Output file organization and naming conventions
- Cross-platform file path handling

#### Usage Example

```bash
cd section-3.2-REPL-with-simulated-cpu-backend
make
./build/REPL

>>> .runfromCPU hello.dsl
# This compiles hello.dsl to machine code and runs it on the CPU
# Output: 2

>>> .load hello.dsl
>>> .run
# This runs the same program using the interpreter
# Output: 2
```

**Complete Workflow Example**:
```bash
>>> let x = 10;
>>> let y = 20;
>>> let result = x + y;
>>> out result;
30

>>> .runfromCPU complex_program.dsl
# Compiles and runs complex_program.dsl on the CPU
# Shows the complete pipeline from DSL to machine execution
```

#### Generated Files

When using `.runfromCPU`, the system generates a comprehensive set of files for analysis:

- **`output.asm`**: Human-readable assembly code
  - Shows the complete instruction sequence
  - Includes memory addresses and labels
  - Provides comments for clarity
  - Useful for understanding code generation

- **`output.bin`**: Binary machine code
  - Raw binary data for CPU execution
  - Exact byte sequence loaded into CPU memory
  - Used for program loading and execution

- **`output.hex`**: Hex dump for analysis
  - Hexadecimal representation of machine code
  - Useful for debugging and verification
  - Shows byte-by-byte instruction encoding

#### Comparison: Interpreter vs CPU Execution

| Aspect | Interpreter (`.run`) | CPU (`.runfromCPU`) |
|--------|---------------------|---------------------|
| **Speed** | Fast (immediate) | Slower (compilation + execution) |
| **Memory** | High-level variables | Actual CPU memory layout |
| **Debugging** | Rich error messages | CPU-level debugging |
| **Authenticity** | Simulated execution | Real CPU behavior |
| **Use Case** | Development, testing | Final verification, learning |
| **Memory Layout** | Abstract variables | Physical memory addresses |
| **Instruction Execution** | High-level interpretation | Actual CPU fetch-execute cycle |
| **Performance Characteristics** | Optimized for development | Real hardware simulation |
| **Error Detection** | Syntax and semantic errors | Hardware-level errors |
| **Output Format** | Direct console output | CPU register-based output |

#### Educational Benefits

1. **Complete Pipeline Experience**: 
   - Students see the full journey from DSL to machine execution
   - Understanding of compilation phases and their purposes
   - Appreciation of the complexity of modern compilers

2. **Two Perspectives**: 
   - High-level programming vs low-level execution
   - Abstract vs concrete program representation
   - Interpretation vs compilation trade-offs

3. **Debugging Skills**: 
   - Learn to debug at both interpreter and CPU levels
   - Understanding of different error types and their sources
   - Tools and techniques for each abstraction level

4. **Performance Understanding**: 
   - See the difference between interpreted and compiled execution
   - Understanding of performance trade-offs
   - Awareness of optimization opportunities

5. **Hardware Awareness**: 
   - Understand how programs actually run on the CPU
   - Memory layout and addressing concepts
   - Instruction execution and control flow

6. **System Integration**: 
   - How different components work together
   - Interface design between software layers
   - Error handling across system boundaries

#### Advanced Usage Scenarios

**Performance Analysis**:
```bash
>>> .runfromCPU benchmark.dsl
# Compare execution times between interpreter and CPU modes
# Analyze performance characteristics of different algorithms
```

**Debugging Complex Programs**:
```bash
>>> .runfromCPU buggy_program.dsl
# Use CPU mode to identify hardware-level issues
# Compare with interpreter mode for high-level debugging
```

**Educational Demonstrations**:
```bash
>>> .runfromCPU tutorial_program.dsl
# Show students the complete compilation pipeline
# Demonstrate the relationship between DSL and machine code
```

#### Technical Architecture

**Component Integration**:
- **REPL Interface**: User interaction and command processing
- **Compiler Pipeline**: DSL parsing and code generation
- **CPU Simulator**: Machine code execution
- **File System**: Output file management
- **Debug System**: Error reporting and analysis

**Memory Management**:
- **Code Section**: Program instructions loaded at 0x2000
- **Data Section**: Variables allocated starting at 0x8000
- **Output Register**: Program output at 0xFF00
- **Stack Management**: Automatic memory allocation and cleanup

**Error Handling Strategy**:
- **Compilation Errors**: Detailed syntax and semantic error reporting
- **Runtime Errors**: CPU-level error detection and reporting
- **System Errors**: File I/O and memory management error handling
- **User Errors**: Input validation and command error reporting

This hybrid approach provides the best of both worlds: the convenience of interactive development with the authenticity of real CPU execution, making it an invaluable tool for computer architecture education and system programming understanding.

## Section 4: Minimal Operating System

A groundbreaking implementation that demonstrates how a simple operating system can be built using the minimal Turing complete CPU and DSL. This section represents the pinnacle of the project, showing how high-level system concepts can be implemented using the fundamental building blocks developed in previous sections.

### Concept: OS as a DSL Program

The minimal OS is implemented as a **DSL program that runs on the CPU**, demonstrating that an operating system is fundamentally just another program that manages resources and provides services to other programs.

#### Core Philosophy

**"An OS is just a program that never exits"** - This implementation embodies this principle by creating a continuous loop that:
- Accepts user commands
- Executes different programs based on command input
- Returns control to the main loop
- Provides a basic shell-like interface

### Implementation Overview

#### Kernel Structure
```dsl
:start:
    in cmd;           // Read command from user
    if cmd <= 1 goto cmd1;  // Route to program 1
    if cmd <= 2 goto cmd2;  // Route to program 2
    goto start;       // Return to main loop

:cmd1:
    out 100;          // Execute program 1
    goto start;       // Return to OS

:cmd2:
    in x;             // Execute program 2
    out x;
    goto start;       // Return to OS
```

#### Key Features

1. **Command Interpreter**: 
   - Reads numeric commands from user input
   - Routes execution to different programs based on command value
   - Provides a simple but functional command interface

2. **Program Management**:
   - Each command corresponds to a different "program"
   - Programs are implemented as labeled sections in the DSL
   - Automatic return to OS after program completion

3. **Continuous Operation**:
   - The OS runs in an infinite loop
   - Never exits unless explicitly halted
   - Maintains control flow between programs

4. **Resource Management**:
   - Manages input/output operations
   - Controls program execution flow
   - Provides basic system services

### Usage

#### Running the Minimal OS

```bash
cd section-4-minimal-OS
# Use the REPL from section-3.2 to run the OS
../section-3.2-REPL-with-simulated-cpu-backend/build/REPL

>>> .runfromCPU kernel.dsl
```

#### Interactive Session

When the OS is running, it provides an interactive shell:

```
Enter command (1 or 2): 1
100
Enter command (1 or 2): 2
Enter a number: 42
42
Enter command (1 or 2): 1
100
```

#### Available Commands

- **Command 1**: Executes a simple program that outputs the value 100
- **Command 2**: Executes an interactive program that reads a number and outputs it back

### Technical Implementation

#### Memory Layout for OS

```
Address Range    Purpose                    Size
0x0000-0x1FFF   Bootloader Area           8KB
0x2000-0x7FFF   Kernel Code               24KB
0x8000-0xFEFF   System Data               ~30KB
0xFF00          Output Register           1 byte
0xFF01-0xFFFF   Reserved                  255 bytes
```

#### Execution Flow

1. **Boot Process**:
   - Kernel loads at address 0x2000
   - Initializes system state
   - Enters main command loop

2. **Command Processing**:
   - Reads command from user input
   - Validates command range
   - Routes to appropriate program

3. **Program Execution**:
   - Executes selected program
   - Manages program state
   - Returns control to OS

4. **System Services**:
   - Input/output management
   - Program scheduling
   - Resource allocation

#### Generated Machine Code

The kernel.dsl compiles to machine code that implements:

```assembly
2000: LOAD_CONST R0, 0        ; Initialize command register
2003: IN R0                   ; Read command from user
2004: LOAD_CONST R1, 1        ; Compare with command 1
2007: SUB R1, R0              ; Check if cmd <= 1
2009: JZ R2, 2010             ; Jump to cmd1 if true
200C: LOAD_CONST R1, 2        ; Compare with command 2
200F: SUB R1, R0              ; Check if cmd <= 2
2011: JZ R2, 2015             ; Jump to cmd2 if true
2014: JMP 2003                ; Return to start
2015: LOAD_CONST R0, 100      ; cmd1: output 100
2018: OUT R0
2019: JMP 2003                ; Return to OS
201A: IN R0                   ; cmd2: read input
201B: OUT R0                  ; Output input value
201C: JMP 2003                ; Return to OS
```

### Educational Value

#### Operating System Concepts

1. **Kernel Design**:
   - Understanding of what constitutes an OS kernel
   - Simple process management and scheduling
   - System call interface design

2. **Command Interpreter**:
   - Shell implementation principles
   - Command parsing and routing
   - User interface design

3. **Program Management**:
   - How programs are loaded and executed
   - Process control and scheduling
   - Resource management basics

4. **System Architecture**:
   - Layered system design
   - Hardware abstraction
   - Service-oriented architecture

#### Computer Science Principles

1. **Abstraction Layers**:
   - How high-level concepts map to low-level implementation
   - Interface design between system layers
   - Separation of concerns

2. **Control Flow**:
   - Interrupt handling and return mechanisms
   - Program state management
   - Context switching concepts

3. **Resource Management**:
   - Input/output management
   - Memory allocation strategies
   - System resource coordination

### Future Development Plans

The minimal OS implementation provides a foundation for more advanced features:

#### Phase 1: Enhanced Kernel
1. **System Calls**: Implement proper system call interface
   - Return instruction support
   - Privileged vs user mode
   - System call table implementation

2. **Bootloader**: Create a proper boot sequence
   - Bootloader starting at 0x0000
   - Kernel loading at 0x1000
   - System initialization procedures

#### Phase 2: Advanced Shell
1. **ASCII Support**: Handle character-based input/output
   - Character encoding and decoding
   - String processing capabilities
   - Text-based user interface

2. **Command Processing**: Enhanced command interpreter
   - Multiple command support
   - Parameter passing
   - Command history

#### Phase 3: Multi-Program Support
1. **Process Scheduling**: Manage multiple programs
   - Round-robin scheduling
   - Process state management
   - Context switching implementation

2. **Memory Management**: Advanced memory handling
   - Program isolation
   - Memory protection
   - Dynamic allocation

#### Phase 4: File System
1. **File Operations**: Basic file system implementation
   - File creation and deletion
   - Read/write operations
   - Directory structure

2. **Online Editing**: Interactive file editing
   - Text editor functionality
   - File save/load operations
   - Real-time editing capabilities


### Conclusion

The minimal OS implementation demonstrates that the fundamental concepts of operating systems can be understood and implemented using simple building blocks. By creating an OS as a DSL program running on the minimal CPU, students gain:

1. **Deep Understanding**: How OS concepts map to actual implementation
2. **Practical Experience**: Hands-on system programming
3. **Architecture Insight**: System design and layering principles
4. **Foundation Knowledge**: Basis for understanding real operating systems

This implementation serves as an excellent educational tool for computer science students, providing a bridge between theoretical OS concepts and practical implementation, while working within the constraints of the minimal Turing complete CPU architecture.

### REPL Architecture

#### Core Components
1. **Command Parser**: Handles REPL-specific commands (`.load`, `.run`, etc.)
2. **Statement Parser**: Parses individual DSL statements
3. **Interpreter**: Executes parsed statements immediately
4. **Variable Manager**: Maintains variable state across statements
5. **File Loader**: Loads and parses complete programs from files

#### Execution Modes
- **Interactive Mode**: Execute statements one at a time
- **File Mode**: Load and execute complete programs
- **Debug Mode**: Show detailed execution information

#### Error Handling
- **Syntax Errors**: Detailed error messages with line/column information
- **Runtime Errors**: Graceful handling of execution errors
- **File Errors**: Clear reporting of file loading issues
- **Input Errors**: Validation and error handling for user input

### REPL vs Compiler Comparison

| Feature | REPL | Compiler |
|---------|------|----------|
| **Execution Speed** | Immediate interpretation | Compiled to machine code |
| **Memory Usage** | Higher (interpreter overhead) | Lower (direct execution) |
| **Development Speed** | Fast (no compilation step) | Slower (compile-run cycle) |
| **Debugging** | Easy (step-by-step execution) | More complex (requires debugger) |
| **Performance** | Slower (interpreted) | Faster (native execution) |
| **Use Case** | Development, testing, learning | Production, performance-critical |

### REPL Implementation Details

#### Statement Processing
```cpp
// Parse and execute single statement
Lexer lexer(line);
Parser parser(lexer);
parser.parseStatement();
interpreter.execute(parser.getIR());
```

#### Variable Management
```cpp
// Variables persist across statements
std::unordered_map<std::string, int> variables;
```

#### File Loading
```cpp
// Load complete program from file
std::string allCode = readFile(filename);
Parser parser(lexer);
parser.parseProgram();  // Parse all statements
```

#### Input Handling
```cpp
// Handle user input with clear prompts
std::cout << "Enter a number: ";
std::cin >> value;
variables[varName] = value;
```

## Development Status

### Completed
- ✅ Basic CPU implementation with 9 instructions
- ✅ DSL lexer and parser
- ✅ Code generation with backpatching
- ✅ CPU simulator with debug output
- ✅ Complete compilation pipeline
- ✅ Basic REPL (section-3)
- ✅ REPL with input support (section-3.1)
- ✅ REPL with simulated CPU backend (section-3.2)
- ✅ Minimal operating system (section-4)

### Future Enhancements
- Advanced REPL features
- Enhanced OS support with system calls
- GUI interface
- File system implementation
- Multi-program scheduling 

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
