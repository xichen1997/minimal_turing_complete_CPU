#include "cpu.h"
#include <iostream>

int main() {
    MinimalCPU cpu;

    // Program: LOAD R0, 0x00FA; STORE 0xFF00, R0; HALT
    std::vector<uint8_t> program = {
        0x01, 0x00, 0x00, 0xFA,   // LOAD R0, 0x00FA
        0x02, 0xFF, 0x00, 0x00,   // STORE 0xFF00, R0
        0x00                      // HALT
    };

    cpu.RAM[0xFF00] = 'A';  // output data
    cpu.loadProgram(program);
    cpu.run();
    
    return 0;
}
