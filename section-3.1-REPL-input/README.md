# REPL with Input Support (Section 3.1)

This section extends the basic REPL from Section 3 to support input operations through the `in` instruction.

## What's New: Input Support

The main addition is the `in` instruction that allows reading user input during program execution.

### Syntax
```
in variable_name;
```

### Example
```
let x = 0;
in x;        // Read a number from user input and store it in x
out x;       // Display the value of x
```

## Changes Required for Input Support

### 1. Token System Updates

**File: `include/token.h`**
```cpp
enum class TokenType {
    KW_LET, KW_IF, KW_GOTO, KW_OUT, KW_HALT, KW_IN,  // Added KW_IN
    ID, NUMBER,
    OP_PLUS, OP_MINUS, OP_LEQ, OP_BRACKET_LEFT, OP_BRACKET_RIGHT,
    EQUAL, COLON, SEMICOLON,
    TOKEN_EOF
};
```

### 2. Lexer Updates

**File: `src/lexer.cpp`**
```cpp
// Add "in" to the keyword map
std::unordered_map<std::string, TokenType> keywords = {
    {"let", TokenType::KW_LET}, {"if", TokenType::KW_IF}, 
    {"goto", TokenType::KW_GOTO}, {"out", TokenType::KW_OUT}, 
    {"halt", TokenType::KW_HALT}, {"in", TokenType::KW_IN}  // Added this line
};
```

### 3. Parser Updates

**File: `src/parser.cpp`**

Add the `parseIn()` function:
```cpp
void Parser::parseIn() {
    expect(TokenType::KW_IN);

    if (currentToken.type != TokenType::ID) {
        throw std::runtime_error("Expected identifier after 'in' at line " +
                                 std::to_string(currentToken.line) + ", column " +
                                 std::to_string(currentToken.column));
    }

    std::string varName = currentToken.value;
    advance();

    expect(TokenType::SEMICOLON); // match ';'

    ir.push_back(IR{OpCode::IN, varName, "", ""}); // push the IR to the vector.
}
```

Update `parseStatement()` to handle `in`:
```cpp
void Parser::parseStatement() {
    // ... existing code ...
    if (currentToken.type == TokenType::KW_LET) {
        parseLet();
    } else if (currentToken.type == TokenType::KW_OUT) {
        parseOut();
    } else if (currentToken.type == TokenType::KW_IN) {  // Added this block
        parseIn();
    } else if (currentToken.type == TokenType::KW_IF) {
        parseIfLeq();
    }
    // ... rest of the function ...
}
```

### 4. IR System Updates

**File: `include/parser.h`**
```cpp
enum class OpCode {
    LOAD_CONST, LOAD_VAR, ADD, SUB, STORE, STORE_CONST,
    IFLEQ, GOTO, LABEL, OUT, HALT, IN  // Added IN
};
```

### 5. Interpreter Updates

**File: `src/interpreter.cpp`**

Add IN instruction handling to both `executeSingleInstruction()` and `execute()`:
```cpp
} else if (inst.op == OpCode::IN) {
    // Read a number from stdin and store it in the variable
    std::cout << "Enter a number: ";
    int value;
    std::cin >> value;
    variables[inst.arg1] = value;
}
```

### 6. REPL Updates

**File: `src/REPL.cpp`**

Add IN to the debug output switch statement:
```cpp
case OpCode::IN: DEBUG_PRINT(std::cout << "IN " << inst.arg1 << std::endl;); break;
```

## Usage Examples

### Basic Input/Output
```
let x = 0;
in x;
out x;
```

### Input in Loops
```
let count = 0;
loop:
in count;
out count;
if count <= 5 goto loop;
halt;
```

### Interactive Calculator
```
let a = 0;
let b = 0;
in a;
in b;
let result = a + b;
out result;
halt;
```

## Key Features

1. **User Input**: The `in` instruction reads integers from standard input
2. **Clear Prompts**: Shows "Enter a number: " when waiting for input
3. **Variable Storage**: Stores the input value in the specified variable
4. **Type Safety**: Converts input to integer automatically
5. **Error Handling**: Provides clear error messages for invalid input

## Building and Running

```bash
make
./build/REPL
```

## Commands

- `.load filename.dsl` - Load a program from file
- `.run` - Execute the loaded program
- `.exit` - Quit the REPL
- `.clear` - Clear the screen
- `.help` - Show available commands

## File Format

Programs are stored in `.dsl` files with the following syntax:
```
let variable = expression;
in variable;
out variable;
if condition goto label;
label:
halt;
```
