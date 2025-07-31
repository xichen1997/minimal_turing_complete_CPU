# Shell-OS Usage Guide

## Overview
This minimal Shell-OS runs entirely on your custom CPU architecture using only registers R0-R7 and direct memory access. No stack pointer required.

Currently, our DSL is not low-level enough to operate the memory effectively, so we need to use the machine code to write a bootloader, a OS-shell program and customized programs.

The design is pretty simple, the program is being writen in c++ code `enhanced_integrated_shell.cpp`, which assemble the bootloader, kernel, and customized programs. And it will be loaded into CPU.

The bootloader will jump to the shell-os kernel area, and our programs can be choisen from the CLI interface. It looks like the embedding system chips, we burns a pre-built target image into the memory area and the system will run forever. 

But it can be called a operating system, though very simple and stupid.

### Memory Layout
```
0x0000-0x0FFF : System/Boot area (reserved)
0x1000-0x1FFF : Shell/OS kernel code 
kernelEND-0x8000: customize programs, after running will return to shell-os
0x8000        : Free memory/heap (for data)
0xFF00-0xFFFF : I/O and system area (4KB)
```

### 2. Run Shell-OS on CPU
```bash
# Method 1: Using REPL
make os

# Method 2: Using test harness
make run
```

## Shell Commands

When the Shell-OS starts, you'll see:
```bash
./build/REPL
========================================
    Enhanced Integrated Shell v6.0     
  Interactive Calculator & Memory!      
========================================
Enhanced system size: 2643 bytes
Memory constraint: Must not exceed 0x8000 (32768 bytes)
✓ Enhanced system fits within memory constraint
End address: 0x1a53

New Features:
• Interactive Calculator (command 'c')
• Memory Viewer (command 'm')
• Enhanced error messages
• Improved help system
========================================

===========================
| Enhanced OS v6.0 |
===========================
Commands: h,1,c,m,q (calc & memory!)
```

```bash
$ ./build/REPL
MiniREPL v0.1
Type .exit to quit.
>>> .runfromCPU shell_os.txt
Starting Shell-OS...
MiniOS
>> Input for R0: h
Commands: h,r,q
>> Input for R0: r  
Running...
>> Input for R0: q
Goodbye
```

Now you can use ASCII characters directly and get properly formatted character output!

## Technical Details

### Shell-OS Features:
- **Size**: 755 bytes (0x2F3) - very memory efficient
- **No Stack**: Uses register-only architecture
- **Command Loop**: Continuous input/command processing
- **Modular**: Easy to extend with new commands

### Current Limitations:
- Single character commands only
- No file system (programs loaded via compiler)
- No user program loading yet (placeholder)
- Input requires ASCII codes

### Extending the Shell:
1. Add new command checks in `shell_os.txt`
2. Create corresponding label handlers
3. Recompile with `./build/compiler shell_os.txt`

## Development Workflow

### Adding New Commands:
```
// In shell_os.txt, add after existing command checks:
if cmd <= [ASCII_VALUE] goto new_cmd;

// Add handler:
new_cmd:
// Your command implementation
goto command_loop;
```

### Loading User Programs:
The `run_cmd` section is prepared for loading programs at 0x3000. You can extend this to:
1. Load compiled user programs to 0x3000
2. Jump to user program entry point
3. Return to shell when program completes

## Troubleshooting

### Common Issues:
1. **Compilation errors**: Check DSL syntax in `.txt` files
2. **Memory conflicts**: Ensure programs don't exceed memory bounds
3. **Input hanging**: Shell waits for user input - provide ASCII values

### Debug Mode:
Compile with DEBUG flag to see detailed CPU execution:
```bash
make DEBUG=1 all
```

## Next Steps
1. Implement user program loading in `run_cmd`
2. Add file system simulation
3. Create more built-in commands
4. Implement bootloader integration