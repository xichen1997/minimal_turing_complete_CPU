# Memory-Based MiniOS Shell v4.0

## Overview

This is an advanced implementation of the MiniOS shell that stores programs in dedicated memory sections and loads them to execution areas when needed. This mimics real operating system behavior where programs are stored on disk/memory and loaded into RAM for execution.

## Memory Architecture

### Memory Layout
```
0x0000-0x0FFF : System/Boot area (reserved)
0x1000-0x1FFF : Shell/OS kernel code 
0x2000-0x2FFF : Program execution area (4KB)
0x3000-0x4FFF : Program storage slot 1 (8KB)
0x5000-0x6FFF : Program storage slot 2 (8KB)
0x7000-0x8FFF : Program storage slot 3 (8KB)
0x9000-0xEFFF : Free memory/heap (24KB)
0xF000-0xFFFF : I/O and system area (4KB)
```

### Key Features

1. **Program Storage**: Programs are stored in dedicated 8KB memory sections
2. **Program Loading**: Programs are copied from storage to execution area (0x2000)
3. **Memory Isolation**: Shell and programs use separate memory regions
4. **State Preservation**: CPU state is saved/restored when switching contexts
5. **Metadata Management**: Program information stored in memory tables

## Implementation Details

### MemoryManagedCPU Class
Extends the basic MinimalCPU with:
- **Program metadata table**: Stores program info, addresses, sizes
- **Built-in program loader**: Loads programs into memory at startup
- **Dynamic program execution**: Copies and runs programs on demand
- **Memory management**: Handles program storage and loading

### Program Metadata Structure
```cpp
struct ProgramMetadata {
    uint16_t startAddr;     // Storage address in memory
    uint16_t size;          // Program size in bytes
    char name[32];          // Program name
    char description[64];   // Program description  
    bool loaded;            // Load status
};
```

### Program Loading Process
1. **Storage**: Programs stored at fixed memory addresses (0x3000+)
2. **Selection**: User selects program by number
3. **Loading**: Program copied from storage to execution area (0x2000)
4. **Execution**: CPU jumps to execution area and runs program
5. **Cleanup**: CPU state restored, ready for next program

## Built-in Programs

### 1. Hello World
- **Storage**: 0x3000-0x3FFF
- **Size**: 53 bytes
- **Function**: Prints "Hello World!" message

### 2. Counter  
- **Storage**: 0x5000-0x5FFF
- **Size**: 73 bytes
- **Function**: Displays "Counting: 1, 2, 3"

### 3. Calculator
- **Storage**: 0x7000-0x7FFF
- **Size**: 80 bytes
- **Function**: Shows simple arithmetic (5+7=12)

## Shell Commands

- `h` - Show help menu
- `l` - List programs stored in memory
- `r` - Run a program (with loading details)
- `m` - Show complete memory map
- `c` - Clear screen  
- `q` - Quit shell

## Usage

### Compilation
```bash
g++ -o memory_os_shell memory_os_shell.cpp
```

### Running
```bash
./memory_os_shell
```

### Example Session
```
[MemoryOS]$ m                    # Show memory map
[MemoryOS]$ l                    # List programs
[MemoryOS]$ r                    # Run program
Enter program number: 1          # Select Hello World
Loading program 'Hello World'... # Loading info
Source address: 0x3000          # Storage location
Target address: 0x2000          # Execution location
Program loaded successfully!     
--- Program Output ---
Hello World!                     # Program runs
--- Program Ended ---
Memory returned to shell.        # Cleanup
[MemoryOS]$ q                    # Quit
```

## Technical Features

### Memory Management
- **Isolation**: Programs can't interfere with shell operation
- **Bounds checking**: Programs limited to their allocated sections
- **Dynamic loading**: Programs loaded only when executed
- **State management**: CPU registers saved/restored per program

### Program Loading
- **Copy-on-execute**: Programs copied to execution area when run
- **Address translation**: Programs run at consistent execution address
- **Size validation**: Program size checked against storage limits
- **Metadata tracking**: Complete program information maintained

### Error Handling
- **Invalid programs**: Graceful handling of missing programs
- **Memory bounds**: Protection against memory overflow
- **Load failures**: Proper error reporting for failed loads
- **User input validation**: Safe handling of user selections

## Advantages Over File-Based Systems

1. **Performance**: No file I/O overhead, programs in memory
2. **Reliability**: No disk access failures or file corruption
3. **Simplicity**: Direct memory operations, no filesystem complexity
4. **Control**: Complete control over program placement and loading
5. **Real-time**: Instant program loading and execution

## Testing

Run the comprehensive test suite:
```bash
./test_memory_os.sh        # Full functionality tests
./test_fixed_programs.sh   # Individual program tests  
./demo_memory_os.sh        # Complete demonstration
```

## Extending the System

### Adding New Programs
1. Create program bytecode array in `loadBuiltInPrograms()`
2. Call `storeProgram()` with slot number, name, description, bytecode
3. Ensure program uses standard instruction set
4. Update `MAX_PROGRAMS` if needed

### Memory Layout Changes
1. Modify memory constants at top of file
2. Update `ProgramMetadata` initialization
3. Adjust program storage slot calculations
4. Update memory map display function

This implementation demonstrates advanced OS concepts including memory management, program loading, and process isolation within the minimal CPU architecture.