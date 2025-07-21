
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
  1 -> __temp__0
  2 -> __temp__1
  __temp__0 + __temp__1 -> __temp__2
  3 -> __temp__3
  __temp__2 + __temp__3 -> temp__4
  __temp__4 -> a
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
a = 0;
loop:
a = a + 1;
if a <= 7 goto loop;
halt;
```

## Example IR Output

```dsl
; IR Instructions:
; IR[0]: STORE_CONST 3 -> __temp__0
; IR[1]: STORE_CONST 5 -> __temp__1
; IR[2]: ADD __temp__0 __temp__1 -> __temp__2
; IR[3]: STORE __temp__2 -> a
; IR[4]: OUT a
; IR[5]: STORE_CONST 0 -> __temp__3
; IR[6]: STORE __temp__3 -> a
; IR[7]: LABEL -> loop
; IR[8]: STORE a -> __temp__4
; IR[9]: STORE_CONST 1 -> __temp__5
; IR[10]: ADD __temp__4 __temp__5 -> __temp__6
; IR[11]: STORE __temp__6 -> a
; IR[12]: IFLEQ a 7 -> loop
; IR[13]: HALT
```
## Machine and asm code

```
; Machine Code:
0000: 0x02 0x03 0x01 ; LOAD_CONST (LOAD Rd, const) R3, 1
0003: 0x04 0x80 0x00 0x03 ; STORE_CONST (STORE addr, const) 0x8000, 3
0007: 0x04 0x80 0x01 0x05 ; STORE_CONST (STORE addr, const) 0x8001, 5
000b: 0x01 0x00 0x80 0x00 ; LOAD_VAR (LOAD Rd, addr) R0, 0x8000
000f: 0x01 0x01 0x80 0x01 ; LOAD_VAR (LOAD Rd, addr) R1, 0x8001
0013: 0x05 0x00 0x01 ; ADD (ADD Rd, Rs) R0, R1
0016: 0x03 0x80 0x02 0x00 ; STORE (STORE addr, Rs) 0x8002, R0
001a: 0x01 0x00 0x80 0x02 ; LOAD_VAR (LOAD Rd, addr) R0, 0x8002
001e: 0x03 0x80 0x03 0x00 ; STORE (STORE addr, Rs) 0x8003, R0
0022: 0x01 0x00 0x80 0x03 ; LOAD_VAR (LOAD Rd, addr) R0, 0x8003
0026: 0x03 0xff 0x00 0x00 ; STORE (STORE addr, Rs) 0xff00, R0
002a: 0x04 0x80 0x04 0x00 ; STORE_CONST (STORE addr, const) 0x8004, 0
002e: 0x01 0x00 0x80 0x04 ; LOAD_VAR (LOAD Rd, addr) R0, 0x8004
0032: 0x03 0x80 0x03 0x00 ; STORE (STORE addr, Rs) 0x8003, R0
0036: 0x01 0x00 0x80 0x03 ; LOAD_VAR (LOAD Rd, addr) R0, 0x8003
003a: 0x03 0x80 0x05 0x00 ; STORE (STORE addr, Rs) 0x8005, R0
003e: 0x04 0x80 0x06 0x01 ; STORE_CONST (STORE addr, const) 0x8006, 1
0042: 0x01 0x00 0x80 0x05 ; LOAD_VAR (LOAD Rd, addr) R0, 0x8005
0046: 0x01 0x01 0x80 0x06 ; LOAD_VAR (LOAD Rd, addr) R1, 0x8006
004a: 0x05 0x00 0x01 ; ADD (ADD Rd, Rs) R0, R1
004d: 0x03 0x80 0x07 0x00 ; STORE (STORE addr, Rs) 0x8007, R0
0051: 0x01 0x00 0x80 0x07 ; LOAD_VAR (LOAD Rd, addr) R0, 0x8007
0055: 0x03 0x80 0x03 0x00 ; STORE (STORE addr, Rs) 0x8003, R0
0059: 0x01 0x00 0x80 0x03 ; LOAD_VAR (LOAD Rd, addr) R0, 0x8003
005d: 0x01 0x01 0x80 0x08 ; LOAD_VAR (LOAD Rd, addr) R1, 0x8008
0061: 0x06 0x01 0x00 ; SUB (SUB Rd, Rs) R1, R0
0064: 0x08 0x02 0x00 0x36 ; JZ (JZ Rd, addr) 0x02 0x00 0x36
0068: 0x00 ; HALT (HALT)
```

