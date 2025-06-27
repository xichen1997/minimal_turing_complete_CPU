#pragma once
#include "parser.h"
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <fstream>

// use backpatching to patch the address of the label
// we just need to scan the code once rather than twice because use the:
// JNZ 1, label(addr placeholder)
// so we can just scan the code once and patch the address of the label.
struct Patch {
    size_t addrPos;       // offset in `code` vector where address needs patching
    std::string labelName;
};

class Codegen {
    public:
        Codegen(std::string filename);
        void generateCode();
        void writeToFile(std::string outputFile);
        void writeToHex(std::string outputFileBin, std::string outputFileHex);
        void writeToHex(std::string outputFile);
    private:
        std::string filename;
        std::vector<IR> ir; // IR vector
        std::vector<uint8_t> code; // code vector
        std::vector<uint8_t> data; // data vector
        static uint16_t DATA_CURSOR; // current data cursor
        static const uint16_t CODE_START = 0x0100; // start of code
        static const uint16_t CODE_END = 0x7FFF; // end of code
        static const uint16_t DATA_START = 0x8000; // start of data
        static const uint16_t DATA_END = 0xFF00; // end of data, output register is at 0xFF00

        // map the label and variable to the address
        std::unordered_map<std::string, uint16_t> labelMap; // map the label to the address
        std::unordered_map<std::string, uint16_t> varMap; // map the variable to the address

        // for the output file
        std::vector<std::string> asmCode;
        uint16_t allocateVar(const std::string& name);

        // for the backpatching
        std::vector<Patch> pendingPatches;
};