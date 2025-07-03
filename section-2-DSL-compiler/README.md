
## How It Works

- **Lexer**:  
  Tokenizes the input DSL program into a stream of tokens (keywords, identifiers, numbers, operators, etc.).

- **Parser**:  
  Consumes the token stream and builds an intermediate representation (IR) of the program, suitable for execution or further compilation. Use linear scanning and memory write back method to keep the generation simpler, but will generate more code:

  For example:
  ```
  let a = 1 + 2 + 3;
  ```
  will be parsed into:
  ```
  1 + 2 -> __temp__0
  __temp0__ + 3 -> __temp1__ 
  __temp1 -> a
  ```

- **Intermediate Representation (IR)**:  
  The IR is a vector of instructions, each corresponding to a CPU operation (e.g., `LOAD_CONST`, `ADD`, `STORE`, `IFLEQ`, `GOTO`, `LABEL`, `OUT`, `HALT`).

## Building and Testing

### Prerequisites

- A C++11-compatible compiler (e.g., `g++`)
- `make`

### Build and Run Parser Tests

```sh
cd section-2-DSL-compiler/t/parser
make test
```

### Build and Run Lexer Tests

```sh
cd section-2-DSL-compiler/t/lexer
make test
```

### Build and Run Codegen Tests

```sh
cd section-2-DSL-compiler/t/codegen
make test
```

### Test Output

Both test suites will print a summary of passed and failed tests, covering all language features and error handling.

## Example DSL Program

```dsl
let a = 3 + 5;
out a;
a = 10;
if a <= 7 goto loop;
loop:
goto end;
halt;
end:
```

## Example IR Output
