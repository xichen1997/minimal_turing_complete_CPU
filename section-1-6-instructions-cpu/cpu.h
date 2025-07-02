#pragma once
#include <iostream>
#include <vector>
#include <cstdint>
#include <iomanip>
#ifdef DEBUG
#define DEBUG_PRINT(x) std::cout << x << std::endl;
#else
#define DEBUG_PRINT(x)
#endif

class MinimalCPU {
public:
    uint8_t RAM[65536]{};
    uint8_t R[4] = {0};  // R0 ~ R3
    uint16_t PC = 0;
    bool halted = false;

    void loadProgram(const std::vector<uint8_t>& program, uint16_t start = 0) {
        for (size_t i = 0; i < program.size(); ++i) {
            RAM[start + i] = program[i];
        }
        PC = start;
    }
    // add a debug function to print the state of the CPU
    void run() {
        while (!halted) {   
            uint8_t op = fetch();
            DEBUG_PRINT("PC: " << std::hex << PC << " Op: " << std::hex << static_cast<int>(op));
            switch (op) {
                case 0x00: // HALT
                    halted = true;
                    DEBUG_PRINT("Halted");
                    break;
                case 0x01: { // LOAD Rd, addr
                    uint8_t rd = fetch(); // use temp variable to replace the rd. If tempID is <=3, then use R[tempID]
                    uint16_t addr = (fetch() << 8) | fetch();
                    R[rd] = RAM[addr];
                    DEBUG_PRINT("LOAD Rd: " << std::hex << static_cast<int>(rd) << " addr: " << std::hex << addr << " value: " << std::hex << static_cast<int>(R[rd]));
                    break;
                }
                case 0x02: { // LOAD Rd, CONST
                    uint8_t rd = fetch();
                    uint8_t constVar = fetch();
                    R[rd] = constVar;
                    DEBUG_PRINT("LOAD Rd: " << std::hex << static_cast<int>(rd) << " const: " << std::hex << static_cast<int>(constVar));
                    break;
                }
                case 0x03: { // STORE addr, Rs
                    uint16_t addr = (fetch() << 8) | fetch();
                    uint8_t rs = fetch();
                    RAM[addr] = R[rs];
                    DEBUG_PRINT("STORE addr: " << std::hex << addr << " Rs: " << std::hex << static_cast<int>(rs) << " value: " << std::hex << static_cast<int>(R[rs]));
                    break;
                }
                case 0x04: { // STORE_CONST addr, CONST
                    uint16_t addr = (fetch() << 8) | fetch();
                    uint8_t conVar = fetch();
                    RAM[addr] = conVar;
                    DEBUG_PRINT("STORE_CONST addr: " << std::hex << addr << " const: " << std::hex << static_cast<int>(conVar));
                    break;
                }
                case 0x05: { // ADD Rd, Rs
                    uint8_t rd = fetch();
                    uint8_t rs = fetch();
                    R[rd] += R[rs];
                    DEBUG_PRINT("ADD Rd: " << std::hex << static_cast<int>(rd) << " Rs: " << std::hex << static_cast<int>(rs) << " result: " << std::hex << static_cast<int>(R[rd]));
                    break;
                }
                case 0x06: { // SUB Rd, Rs
                    uint8_t rd = fetch();
                    uint8_t rs = fetch();
                    R[rd] -= R[rs];
                    DEBUG_PRINT("SUB Rd: " << std::hex << static_cast<int>(rd) << " Rs: " << std::hex << static_cast<int>(rs) << " result: " << std::hex << static_cast<int>(R[rd]));
                    break;
                }
                case 0x07: { // JNZ Rd, addr
                    uint8_t rd = fetch();
                    uint16_t addr = (fetch() << 8) | fetch();
                    if (R[rd] != 0) {
                        PC = addr;
                        DEBUG_PRINT("JNZ Rd: " << std::hex << static_cast<int>(rd) << " addr: " << std::hex << addr);
                    }
                    break;
                }
                case 0x08: { // JZ Rd, addr
                    uint8_t rd = fetch();
                    uint16_t addr = (fetch() << 8) | fetch();
                    if (R[rd] == 0) {
                        PC = addr;
                        DEBUG_PRINT("JZ Rd: " << std::hex << static_cast<int>(rd) << " addr: " << std::hex << addr);
                    }
                    break;
                }
                default:
                    std::cerr << "Unknown opcode: " << std::hex << static_cast<int>(op) << "\n";
                    halted = true;
                    DEBUG_PRINT("Unknown opcode: " << std::hex << static_cast<int>(op));
                    break;
            }
        }
    }

private:
    uint8_t fetch() {
        return RAM[PC++];
    }
};