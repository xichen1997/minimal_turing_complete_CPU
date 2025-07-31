#include "cpu.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <iomanip>

// Helper to read a hex file into a vector<uint8_t>
std::vector<uint8_t> readHexFile(const std::string& filename) {
    std::vector<uint8_t> program;
    std::ifstream infile(filename);
    if (!infile) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        exit(1);
    }
    std::string byteStr;
    while (infile >> byteStr) {
        // Accepts 0xXX or XX format
        uint8_t byte = 0;
        if (byteStr.rfind("0x", 0) == 0 || byteStr.rfind("0X", 0) == 0) {
            byte = static_cast<uint8_t>(std::stoul(byteStr, nullptr, 16));
        } else {
            byte = static_cast<uint8_t>(std::stoul(byteStr, nullptr, 16));
        }
        program.push_back(byte);
    }
    return program;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <hexfile>" << std::endl;
        return 1;
    }
    std::string filename = argv[1];
    std::vector<uint8_t> program = readHexFile(filename);
    MinimalCPU cpu;
    cpu.loadProgram(program);
    cpu.run();
    return 0;
} 