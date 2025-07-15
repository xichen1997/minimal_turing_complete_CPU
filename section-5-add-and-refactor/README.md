# Expand the CPU instruction to allow advanced feature

## Current limitations:
1) The bootloader and kernel loading rely on the REPL, not real instruction level of simulation.

2) No real kernel and kernel mode.

2) Need more compare logic like == != < >.

3) lack of file system, which make the bootloader impossible.

4) ASCII input and output support.

5) Doesn't Support SP, and system level instruction `ret`, which make the function and stack structure impossible.


## Furthur plan for the OS.

1) implement a real kernel with more system level instruction like return and system call. The kernel program should start with 0x1000.

2) Use bootloader which start from 0x0000 to load kernel.

3) Run a shell which is based on the CPU program, which can handle ASCII characters.

4) Kernel will have to sechdule multiple programs.

5) Implement file system to load/save files and edit them online.

6) Partite the memory section again.
