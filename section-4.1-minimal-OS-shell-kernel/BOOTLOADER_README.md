# MiniOS Bootloader System v1.0

## Overview

This is a complete bootloader implementation that mimics real computer boot processes. The system starts at 0x0000 with a bootloader that copies the OS from storage to its runtime location and then transfers control to the OS.

## Boot Process Architecture

### Complete Boot Sequence
1. **Power On**: CPU starts execution at 0x0000 (bootloader)
2. **Bootloader Initialization**: Display boot messages and system info
3. **OS Loading**: Copy OS code from storage (0xC000) to runtime (0x1000)
4. **Control Transfer**: Jump to OS at 0x1000 to begin OS execution
5. **OS Operation**: OS runs and can execute user programs
6. **Program Execution**: OS can load and run programs from memory sections

### Memory Layout
```
0x0000-0x0FFF : Bootloader (4KB)
    ├── Boot messages and initialization
    ├── OS copy routine  
    └── Jump to OS code

0x1000-0x1FFF : OS Runtime Location (4KB)
    ├── OS kernel code (copied by bootloader)
    ├── Shell interface
    └── Program management

0x2000-0x2FFF : Program Execution Area (4KB)
    └── Where programs run (isolated from OS)

0x3000-0x4FFF : Program Storage Slot 1 (8KB)
    └── Hello World program

0x5000-0x6FFF : Program Storage Slot 2 (8KB)  
    └── Boot Info program

0x7000-0x8FFF : Program Storage Slot 3 (8KB)
    └── System Status program

0x9000-0xBFFF : Free Memory/Heap (12KB)
    └── Available for dynamic allocation

0xC000-0xCFFF : OS Storage (Boot Image) (4KB)
    └── Where OS is initially stored

0xD000-0xEFFF : Reserved/System (8KB)
    └── Reserved for system use

0xF000-0xFFFF : I/O and System Area (4KB)
    └── Memory-mapped I/O (0xFF00 for output)
```

## Implementation Details

### BootloaderCPU Class
Extends MinimalCPU with bootloader capabilities:

#### Key Methods
- **`createBootloader()`**: Generates bootloader machine code
- **`createOSCode()`**: Creates OS kernel code 
- **`storeOSInMemory()`**: Places OS at storage location (0xC000)
- **`performBootSequence()`**: Executes complete boot process
- **`runSimpleOS()`**: Runs OS after successful boot

#### Boot Sequence Implementation
```cpp
void performBootSequence() {
    1. Load bootloader at 0x0000
    2. Display boot messages
    3. Copy OS from 0xC000 to 0x1000
    4. Transfer control to OS at 0x1000
}
```

### Bootloader Code Structure
The bootloader generates machine code that:
1. **Displays Boot Messages**: Welcome and initialization messages
2. **Performs OS Copy**: Copies OS bytecode from storage to runtime location
3. **Status Updates**: Shows progress during boot process
4. **Control Transfer**: Jumps to OS entry point

### OS Code Structure
The OS kernel includes:
- **Initialization Messages**: OS startup notification
- **Simple Shell**: Basic command interface  
- **Input Handling**: Command processing
- **Program Interface**: Ability to run stored programs

## Boot Messages and Flow

### Typical Boot Output
```
PRE-BOOT MEMORY LAYOUT:
[Memory map showing initial state]

========================================
        MINIOS BOOT SEQUENCE           
========================================

Loading bootloader at 0x0000...
Bootloader size: 562 bytes

Starting boot process...
----------------------------------------
Bootloader running...
MINIOS BOOTLOADER v1.0

Initializing system...
Loading OS from 0xC000 to 0x1000...
OS loaded successfully!
Jumping to OS at 0x1000...

----------------------------------------
BOOT COMPLETE - OS STARTING
----------------------------------------
[BootedOS]$
```

### OS Commands
After successful boot, the OS supports:
- **`m`**: Show memory map
- **`r`**: Run a program (lists available programs)
- **`q`**: Shutdown OS

## Program Management

### Built-in Programs
The system includes three demonstration programs:

1. **Hello World**
   - Location: 0x3000-0x4FFF
   - Function: Simple greeting message
   - Size: ~53 bytes

2. **Boot Info**
   - Location: 0x5000-0x6FFF  
   - Function: Shows boot information
   - Size: ~84 bytes

3. **System Status**
   - Location: 0x7000-0x8FFF
   - Function: System status check
   - Size: ~44 bytes

### Program Execution Process
1. **Program Selection**: User chooses program number
2. **Memory Copy**: Program copied from storage to execution area (0x2000)
3. **Context Switch**: Save OS state, prepare program environment
4. **Execution**: Run program with clean register state
5. **Return**: Restore OS state, return control to OS

## Technical Features

### Memory Management
- **Complete Isolation**: Bootloader, OS, and programs use separate memory regions
- **Safe Copying**: OS copied safely from storage to runtime location
- **State Preservation**: CPU state managed across bootloader → OS → program transitions
- **Bounds Checking**: All memory operations respect section boundaries

### Boot Safety
- **Initialization Verification**: Bootloader verifies successful initialization
- **Copy Validation**: OS copy operation includes size and address validation
- **Error Handling**: Graceful handling of boot failures
- **Status Reporting**: Clear progress indication throughout boot process

### Performance Characteristics
- **Fast Boot**: In-memory operations for instant boot times
- **Efficient Copy**: Direct memory-to-memory OS transfer
- **Minimal Overhead**: Lightweight bootloader with focused functionality
- **Quick Program Loading**: Programs load instantly from memory sections

## Usage

### Compilation
```bash
g++ -o bootloader_os bootloader_os.cpp
```

### Running the System
```bash
./bootloader_os
```

### Interactive Session
```
# System boots automatically
[BootedOS]$ m          # Show memory map
[BootedOS]$ r          # Run a program
Enter program number: 1
Running program: Hello World
--- Program Output ---
Hello World!
--- Program Ended ---
[BootedOS]$ q          # Shutdown
```

### Testing
```bash
./test_bootloader.sh        # Comprehensive bootloader tests
./demo_complete_boot.sh     # Complete boot demonstration
```

## Real-World Parallels

### Similar to Real Systems
This bootloader system mirrors real computer boot processes:

1. **BIOS/UEFI Stage**: Our bootloader at 0x0000
2. **OS Loading**: Copying OS from disk to RAM  
3. **Kernel Initialization**: OS startup at 0x1000
4. **Program Execution**: Running user programs in isolated memory
5. **Memory Management**: Organized memory layout with specific purposes

### Key Differences from Real Systems
- **Simplified Storage**: Uses memory instead of disk storage
- **No Hardware Init**: Skips hardware initialization complexity
- **Single-User**: No multi-user or multi-process support
- **Fixed Layout**: Static memory layout vs. dynamic allocation

## Advantages

### Educational Value
- **Complete Boot Process**: Shows full boot-to-program execution
- **Memory Management**: Demonstrates organized memory layout
- **System Design**: Illustrates OS architecture principles
- **State Management**: Shows context switching concepts

### Practical Benefits
- **Reliability**: No external dependencies or disk I/O
- **Performance**: Instant boot and program loading
- **Simplicity**: Easy to understand and modify
- **Control**: Complete system control and visibility

## Extending the System

### Adding Boot Features
- Modify `createBootloader()` to add new boot functionality
- Update boot messages in bootloader code generation
- Add boot-time hardware checks or initialization

### Expanding OS Capabilities
- Enhance `createOSCode()` for more OS features
- Add new OS commands to the shell interface
- Implement additional system services

### Adding Programs
- Use `storeProgram()` to add new programs at boot time
- Create program bytecode arrays
- Update program storage slots if needed

This bootloader system provides a complete, educational implementation of computer boot processes while maintaining the simplicity of the minimal CPU architecture.