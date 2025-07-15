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

## Furthur plan for the OS.

1) implement a real kernel with more system level instruction like return and system call. The kernel program should start with 0x1000.

2) Use bootloader which start from 0x0000 to load kernel.

3) Run a shell which is based on the CPU program, which can handle ASCII characters.

4) Kernel will have to sechdule multiple programs.

5) Implement file system to load/save files and edit them online.