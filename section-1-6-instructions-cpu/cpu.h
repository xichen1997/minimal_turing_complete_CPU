#pragma once
#include <iostream>
#include <vector>
#include <cstdint>
#include <iomanip>

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

    void run() {
        while (!halted) {
            uint8_t op = fetch();
            switch (op) {
                case 0x00: // HALT
                    halted = true;
                    break;
                case 0x01: { // LOAD Rd, addr
                    uint8_t rd = fetch(); // use temp variable to replace the rd. If tempID is <=3, then use R[tempID]
                    uint16_t addr = (fetch() << 8) | fetch();
                    R[rd] = RAM[addr];
                    break;
                }
                case 0x02: { // LOAD Rd, CONST
                    uint8_t rd = fetch();
                    uint8_t const = fetch();
                    R[rd] = const;
                    break;
                }
                case 0x03: { // STORE addr, Rs
                    uint16_t addr = (fetch() << 8) | fetch();
                    uint8_t rs = fetch();
                    RAM[addr] = R[rs];
                    break;
                }
                case 0x04: { // STORE_CONST addr, CONST
                    uint16_t addr = (fetch() << 8) | fetch();
                    uint8_t conVar = fetch();
                    RAM[addr] = conVar;
                    break;
                }
                case 0x05: { // ADD Rd, Rs
                    uint8_t rd = fetch();
                    uint8_t rs = fetch();
                    R[rd] += R[rs];
                    break;
                }
                case 0x06: { // SUB Rd, Rs
                    uint8_t rd = fetch();
                    uint8_t rs = fetch();
                    R[rd] -= R[rs];
                    break;
                }
                case 0x07: { // JNZ Rd, addr
                    uint8_t rd = fetch();
                    uint16_t addr = (fetch() << 8) | fetch();
                    if (R[rd] != 0) {
                        PC = addr;
                    }
                    break;
                }
                default:
                    std::cerr << "Unknown opcode: " << std::hex << static_cast<int>(op) << "\n";
                    halted = true;
                    break;
            }
        }
    }

private:
    uint8_t fetch() {
        return RAM[PC++];
    }
};