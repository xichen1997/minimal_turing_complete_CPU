# Codegen Test Suite

This directory contains comprehensive tests for the code generation functionality of the DSL compiler.

## Overview

The test suite validates the `Codegen` class functionality, which is responsible for:
- Converting IR (Intermediate Representation) instructions to machine code
- Managing variable allocation and memory layout
- Handling label resolution and backpatching
- Generating output files in various formats (binary, hex, assembly)

## Test Coverage

The test suite covers the following areas:

### Basic Functionality
- **Codegen Constructor**: Tests proper initialization and file parsing
- **Simple Halt Program**: Tests basic program compilation
- **Let Statement Codegen**: Tests variable declaration code generation
- **Assignment Codegen**: Tests variable assignment code generation

### Arithmetic Operations
- **Addition Codegen**: Tests addition operation code generation
- **Subtraction Codegen**: Tests subtraction operation code generation

### Output Operations
- **Out Statement Codegen**: Tests output statement code generation
- **Out Variable Codegen**: Tests output of variable values

### Control Flow
- **Label Codegen**: Tests label definition code generation
- **Goto Codegen**: Tests unconditional jump code generation
- **IfLeq Codegen**: Tests conditional jump code generation

### Complex Programs
- **Complex Program Codegen**: Tests multi-statement programs
- **Multiple Variables Codegen**: Tests programs with many variables
- **Loop Program Codegen**: Tests loop constructs

### File Generation
- **Binary File Generation**: Tests binary output file creation
- **Hex File Generation**: Tests hexadecimal output file creation
- **ASM File Generation**: Tests assembly output file creation

### Error Handling
- **Error Handling - Invalid File**: Tests handling of non-existent files
- **Error Handling - Invalid Syntax**: Tests handling of invalid syntax

## Running the Tests

```bash
# Build and run all tests
make test

# Clean build artifacts
make clean

# Show help
make help
```

## Test Structure

Each test function:
1. Creates a temporary source file with test code
2. Instantiates a `Codegen` object with the test file
3. Verifies that output files are generated correctly
4. Cleans up temporary files
5. Returns success/failure status

## Expected Output Files

The codegen generates three output files:
- `output.asm`: Human-readable assembly code with addresses and hex values
- `output.bin`: Binary machine code file
- `output.hex`: Hexadecimal representation of the machine code

## Memory Layout

The codegen uses the following memory layout:
- `[0x0000 - 0x7FFF]`: Code section (32KB for program)
- `[0x8000 - 0xFF00)`: Data section (~32KB for variables and temps)
- `0xFF00`: Output register

## Dependencies

- `codegen.h/cpp`: Main codegen implementation
- `parser.h/cpp`: Parser for generating IR
- `lexer.h/cpp`: Lexer for tokenization
- `token.h`: Token definitions 