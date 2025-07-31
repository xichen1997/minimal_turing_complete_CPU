#pragma once
#include <iostream>
#include <vector>
#include <cstdint>
#include <iomanip>
#include <algorithm>
#ifdef DEBUG
#define DEBUG_PRINT(x) std::cout << x << std::endl;
#else
#define DEBUG_PRINT(x)
#endif

class MinimalCPU {
public:
    uint8_t RAM[65536]{};
    uint8_t R[8] = {0};  // R0 ~ R7 // R3 = 1 for JNZ // R2 = carry register
    uint16_t PC = 0;
    bool halted = false;

    void loadProgram(const std::vector<uint8_t>& program, uint16_t start = 0) {
        reset();
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
                    if (addr == static_cast<uint16_t>(0xFF00)) {
                        std::cout << static_cast<int>(R[rs]) << std::endl;
                    }
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
                    uint8_t original_rd = R[rd];
                    R[rd] -= R[rs];
                    // Set R2 as carry register: 1 if underflow occurred (Rd < Rs), 0 otherwise
                    R[2] = (original_rd < R[rs]) ? 1 : 0;
                    DEBUG_PRINT("SUB Rd: " << std::hex << static_cast<int>(rd) << " Rs: " << std::hex << static_cast<int>(rs) << " result: " << std::hex << static_cast<int>(R[rd]) << " carry: " << std::hex << static_cast<int>(R[2]));
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
                case 0x09: { // IN Rd
                    uint8_t rd = fetch();
                    std::cout << "Input for R" << int(rd) << ": ";
                    int val;
                    std::cin >> val;
                    if (val < 0 || val > 255) {
                        std::cerr << "Warning: input out of 8-bit range, truncating." << std::endl;
                        val = std::clamp(val, 0, 255);
                    }
                    R[rd] = static_cast<uint8_t>(val);
                    // put the value to the input buffer 0xFF01
                    // STORE 0xFF01, Rd
                    // RAM[0xFF01] = R[rd];
                    DEBUG_PRINT("IN Rd: " << std::hex << static_cast<int>(rd) << " value: " << std::hex << static_cast<int>(R[rd]));
                    break;
                }
                case 0x0A: { // 
                    // LOAD_INDIRECT R0, R1, R2 -> R4
                    // R0 = RAM[R0<<8 | R1 + R2]
                    uint8_t hi = R[0], lo = R[1], idx = R[2];
                    uint16_t addr = (hi << 8 | lo) + idx;
                    R[4] = RAM[addr];
                    DEBUG_PRINT("LOAD_INDIRECT R0: " << std::hex << static_cast<int>(R[0]) << " R1: " << std::hex << static_cast<int>(R[1]) << " R2: " << std::hex << static_cast<int>(R[2]) << " addr: " << std::hex << addr << " value: " << std::hex << static_cast<int>(R[4]));
                    break;
                }
                case 0x0B: { // STORE_INDEXED
                    // STORE_INDIRECT R0, R1, R2 -> R4
                    // RAM[R0<<8 | R1 + R2] = R[4]
                    uint8_t hi = R[0], lo = R[1], src = R[2];
                    uint16_t addr = (hi << 8 | lo) + src;
                    RAM[addr] = R[4];
                    DEBUG_PRINT("STORE_INDIRECT R0: " << std::hex << static_cast<int>(R[0]) << " R1: " << std::hex << static_cast<int>(R[1]) << " R2: " << std::hex << static_cast<int>(R[2]) << " addr: " << std::hex << addr << " value: " << std::hex << static_cast<int>(R[0]));
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
    void reset() {
        halted = false;
        PC = 0;
        for(int i = 0; i < 4; i++) {
            R[i] = 0;
        }
    }
};