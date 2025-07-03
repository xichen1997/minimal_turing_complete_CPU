# Minimal CPU Implementation

A simple CPU implementation that demonstrates basic computer architecture concepts through a minimal instruction set.

## Design

The CPU implements a simple von Neumann architecture with the following components:

- **Memory**: 64KB (65536 bytes) of RAM
- **Registers**: 4 general-purpose registers (R0-R3)
- **Program Counter (PC)**: 16-bit register for instruction fetching
- **Instruction Set**:
  - `HALT (0x00)`: Stops program execution
  - `LOAD (0x01)`: Loads data from memory into register
  - `LOAD_CONST (0x02)` : Loads data from constant
  - `STORE (0x03)`: Stores register value into memory
  - `STORE_CONST (0x04)`: Stores constant
  - `ADD (0x03)`: Adds two register values
  - `SUB (0x04)`: Subtracts two register values
  - `JNZ (0x05)`: Jump if register is not zero

The CPU uses a simple fetch-execute cycle:
1. Fetch instruction from memory at PC
2. Decode instruction
3. Execute instruction
4. Update PC
5. Repeat until HALT

## Code

The implementation consists of two main files:

### cpu.h
Contains the `MinimalCPU` class that implements the CPU functionality:
- Memory and register definitions
- Program loading mechanism
- Instruction execution logic
- Fetch-execute cycle implementation

### main.cpp
Demonstrates CPU usage with a simple program:
```cpp
// Example program:
// LOAD R0, 0x00FA    ; Load value from memory address 0x00FA into R0
// STORE 0xFF00, R0   ; Store R0's value to memory address 0xFF00
// HALT              ; Stop execution
```

The program shows how to:
1. Initialize the CPU
2. Load a program into memory
3. Set initial memory values
4. Run the program

### Building and Running

To build the project:
```bash
make
make debug
```
make debug to get DEBUG output and the each step printing.

To run the cpu test program:
```bash
./build/test
```

To run the program(generated from section 2):
```bash
./bulid/cpu build/output.hex 
```

To clean build artifacts:
```bash
make clean
``` 