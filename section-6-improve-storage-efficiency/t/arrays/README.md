# Array Support Test Suite

This directory contains comprehensive tests for the array functionality implemented in section-4.

## Test Coverage

### 📝 Lexer Tests
- **Bracket Token Recognition**: Verifies `[` and `]` tokens are properly tokenized
- **Array Declaration Tokenization**: Tests complete tokenization of `let arr[size];` syntax

### 🔍 Parser Tests  
- **Array Declaration Parsing**: Tests `let arr[5];` produces correct `ARRAY_DECL` IR
- **Array Assignment Parsing**: Tests `arr[0] = value;` produces `STORE_INDEXED` IR
- **Array Access Parsing**: Tests `let x = arr[0];` produces `LOAD_INDEXED` IR
- **Array Output Parsing**: Tests `out arr[0];` generates proper IR sequence
- **Array with Variables**: Tests variable indices like `arr[i] = value;`
- **Array Arithmetic**: Tests `arr[2] = arr[0] + arr[1];` expressions

### ⚙️ Interpreter Tests
- **Basic Array Operations**: End-to-end execution of array store/load/output
- **Multiple Array Elements**: Tests multiple array operations in sequence
- **Array Arithmetic**: Tests arithmetic expressions with array elements
- **Variable Index Access**: Tests using variables as array indices

### 🔧 Code Generation Tests
- **Basic Array Codegen**: Verifies proper machine code generation for arrays
- **No Spurious HALT Instructions**: Ensures the HALT bug is fixed in generated code

### ❌ Error Handling Tests
- **Array Bounds Checking**: Verifies runtime bounds checking works correctly

## Running Tests

```bash
# Build and run all tests
make test

# Or step by step:
make all      # Build test executable
make run      # Run the tests
make clean    # Clean up build files
```

## Test Results

The test suite provides detailed output showing:
- ✓ Passed tests in green
- ✗ Failed tests with error details  
- Summary statistics
- Return code (0 for success, >0 for failures)

## Integration with Main Makefile

The main project Makefile includes a `test-arrays` target that runs this test suite:

```bash
# From project root:
make test-arrays
```

## Test Framework

Uses a simple custom test framework that:
- Captures exceptions and provides clear error reporting
- Redirects stdout for output verification tests
- Provides utility functions for IR analysis
- Generates comprehensive test reports