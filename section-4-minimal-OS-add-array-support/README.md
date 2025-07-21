# Simulate a OS

This implementation is pretty simple, we just need to write a DSL program, which has the already programmed jobs:

```
:start:
    in cmd;
    if cmd <= 1 goto cmd1;
    if cmd <= 2 goto cmd2;
    goto start;

:cmd1:
    out 100;
    goto start;

:cmd2:
    in x;
    out x;
    goto start;
```

Use `.runfromCPU kernel.dsl` can launch this program and give an interactive shell to user to choose two program cmd1 and cmd2. 

Congratulation, here is a mileston, the CPU can run a very simple operating system, but it's pretty unconvenient though(The flow is fixed can't add new program.)

## Double thoughts about the implementation

Currently the OS highly rely on the interpreter and REPL. Which is just a simulation of the CPU and provide a lot of convenience, however, the real CPU need to boot itself and run the OS, and the shell should also supported by the CPU simulator directly rather than rely on the C++ REPL. (However, the IO I will rely on the C++ because it will be over complicated if we implement every details in the CPU chip, it's not the purpose of the project)

What we could do next?

### Currently missing components

#### interactive shell run on CPU directly

#### in order to support the shell and other programs, OS is needed, then we have to implement a minimal kernel which allow us to run the program in order(not multiprogramming, that's will be kept in the future)

#### Bootloader. The loading of the program rely on the C++ REPL loadprogram function, which is the high level abstraction. It should run on 0x0000.

#### File system, which allow the bootloader find the kernel.hex and shell.hex to load them in the correct location in the CPU and RAM.

#### In order to read the file, we need a "string", thus we have to implement the input of a line directly and store them in an array.



## Time to action

<!-- DSL 写法	出现位置	所需 Parser 行为
let x = buffer[i] + 2;	表达式右值	parseExpr() 调用 parsePrefixExpr() 处理 buffer[i] 为一个 temp
buffer[i] = x;	赋值语句左值	parseStatement() 调用 parseArrayAssignment() 或分支中处理
if buffer[i] <= x goto L1;	条件判断	parseExpr() 能处理 buffer[i] 为一个变量一样 -->

### Array Support Implementation

This section introduces comprehensive array support to our DSL, enabling programs to work with collections of data - a crucial feature for building more sophisticated systems like file systems, buffers, and data structures.

#### Why Arrays Are Essential

1. **String/Text Processing**: To implement a file system and shell commands, we need to handle strings as character arrays
2. **Buffer Management**: Operating systems require buffers for I/O operations and data transfer
3. **Data Structures**: Arrays are fundamental building blocks for more complex data structures
4. **Memory Management**: Proper array support lays groundwork for dynamic memory allocation

#### Supported Array Operations

```dsl
let buffer[32];           // Array declaration with fixed size
buffer[0] = 42;          // Array element assignment
let x = buffer[i] + 2;   // Array element access in expressions
out buffer[0];           // Output array elements
buffer[i] = buffer[j];   // Copy between array elements
```

#### Implementation Details

##### 1. Lexer Extensions (`src/lexer.cpp`)
- Added `OP_LBRACKET` (`[`) and `OP_RBRACKET` (`]`) token types
- Both `genNextToken()` and `peekNextToken()` functions handle bracket tokens
- Proper error reporting for unexpected bracket characters

##### 2. Parser Enhancements (`src/parser.cpp`)
- **Array Declaration**: `parseLet()` detects array syntax via `peekNextToken()` lookahead
- **Array Assignment**: `parseArrayAssignment()` handles `arr[index] = value` syntax
- **Array Access**: `parsePrefixExpr()` handles `arr[index]` in expressions
- **Output Support**: `parseOut()` enhanced to support `out arr[index]` statements
- **Expression Parsing**: Fixed `parseExpr(0)` calls to include precedence parameter

##### 3. Intermediate Representation (IR)
Three new opcodes added to `parser.h`:
- `ARRAY_DECL`: Declares array with specified size
- `LOAD_INDEXED`: Loads value from array[index] into temporary variable
- `STORE_INDEXED`: Stores value into array[index]

##### 4. Interpreter Infrastructure (`src/interpreter.cpp`, `include/interpreter.h`)
- **Memory Management**: 64KB simulated memory (`std::vector<int> memory`)
- **Address Allocation**: `allocate()` function manages memory addresses starting at 0x1000
- **Variable Resolution**: `resolve()` function handles both constants and variables
- **Array Mapping**: `arrayMap` tracks array base addresses and sizes
- **Bounds Checking**: Runtime verification of array index validity

##### 5. REPL Integration (`src/REPL.cpp`)
- Added debug output for array opcodes in switch statement
- Proper IR instruction logging for `ARRAY_DECL`, `LOAD_INDEXED`, `STORE_INDEXED`

#### Technical Challenges Solved

1. **Token Precedence**: Bracket tokens needed proper precedence handling in expression parsing
2. **Memory Simulation**: Implemented virtual memory system for array storage
3. **Address Management**: Created allocation system for consecutive array elements
4. **Parse Context**: Distinguished between array declaration, assignment, and access contexts
5. **Expression Integration**: Seamlessly integrated array access into existing expression system

#### Usage Examples

```dsl
# Basic array operations
let arr[5];
arr[0] = 10;
arr[1] = 20;
out arr[0];        # Outputs: 10

# Array with variable indices
let i = 1;
let x = arr[i];    # x = 20
arr[i] = arr[0] + 5; # arr[1] = 15

# Complex expressions
let buffer[100];
let size = 10;
buffer[0] = size * 2;
out buffer[0];     # Outputs: 20
```

#### Memory Layout

Arrays are allocated consecutive memory addresses starting from 0x1000:
- `arr[0]` → address 0x1000
- `arr[1]` → address 0x1001  
- `arr[2]` → address 0x1002
- etc.

##### 6. Code Generation Extensions (`src/codegen.cpp`, `include/codegen.h`)
- **New CPU Instructions**: Added support for two new CPU opcodes:
  - `0x0A`: `LOAD_INDEXED` - Loads value from memory address calculated as base + offset
  - `0x0B`: `STORE_INDEXED` - Stores value to memory address calculated as base + offset
- **Array Memory Management**: `allocateArrayViaVar()` function allocates consecutive memory for array elements
- **Array Mapping**: `arrMap` tracks array base addresses and sizes for code generation
- **Register Usage Protocol**: 
  - R0: Base address for array operations
  - R2: Array index/offset
  - R4: Source/destination for array values

#### Code Generation Details

##### Array Declaration (`ARRAY_DECL`)
```cpp
case OpCode::ARRAY_DECL: {
    uint16_t arraySize = std::stoi(instruction.arg2);
    uint16_t arrayAddress = allocateArrayViaVar(instruction.arg1, arraySize);
    arrMap[instruction.arg1] = {arrayAddress, arraySize};
    break;
}
```
- Allocates consecutive memory addresses for array elements
- Records array metadata in `arrMap` for later access operations

##### Array Element Loading (`LOAD_INDEXED`)
```assembly
LOAD R2, index_addr     ; Load index into R2
LOAD R0, base_addr      ; Load array base address into R0  
LOAD_INDEXED           ; CPU instruction 0x0A: R4 = memory[R0 + R2]
STORE result_addr, R4   ; Store loaded value to result variable
```

##### Array Element Storing (`STORE_INDEXED`)
```assembly
LOAD R4, value_addr     ; Load value to store into R4
LOAD R2, index_addr     ; Load index into R2
LOAD R0, base_addr      ; Load array base address into R0
STORE_INDEXED          ; CPU instruction 0x0B: memory[R0 + R2] = R4
```

#### CPU Architecture Enhancements Required

To support arrays, the CPU simulator must implement two new instructions:

1. **`LOAD_INDEXED` (0x0A)**: 
   - Calculates effective address: `R0 + R2`
   - Loads value from memory at effective address into R4
   - Enables: `R4 = memory[R0 + R2]`

2. **`STORE_INDEXED` (0x0B)**:
   - Calculates effective address: `R0 + R2` 
   - Stores value from R4 to memory at effective address
   - Enables: `memory[R0 + R2] = R4`

#### Memory Layout for Arrays

Arrays are allocated in the data segment starting at 0x8000:
```
Memory Layout:
[0x2000 - 0x7FFF]: Code segment (32KB)
[0x8000 - 0xFF00]: Data segment (~32KB)
  └── Arrays allocated consecutively:
      arr[0] → 0x8000
      arr[1] → 0x8001  
      arr[2] → 0x8002
      ... 
[0xFF00]: Output register
[0xFF01]: Input register
```

This foundation enables future implementations of:
- String manipulation for file system operations
- Command-line argument parsing
- Buffer management for I/O operations
- Dynamic data structures for the operating system