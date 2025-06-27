#include "../include/codegen.h"
#include "../include/parser.h"
#include "../include/lexer.h"
#include "../include/token.h"
#include <vector>
#include <string>
#include <iostream>
#include <cctype>
#include <iomanip>
#include <stdexcept>

// Memory layout:
// [0x0000 - 0x7FFF] : Code (32KB for program)
// [0x8000 - 0xFF00) : Data (~32KB for variables and temps)
// 0xFF00 : Output register

// Initialize static member
uint16_t Codegen::DATA_CURSOR = 0x0000;

Codegen::Codegen(std::string filename) : filename(filename) { 
    // parse the program and generate the code
    // open the file and convert to string
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    std::string program((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    
    // parse the program
    Lexer lexer(program);
    Parser parser(lexer);
    parser.parseProgram();
    ir = parser.getIR();
    
    // generate the code
    generateCode();
}

// OpCode is defined in parser.h
// enum class OpCode{
//     LOAD_CONST, LOAD_VAR, STORE_CONST, STORE_VAR,
//     ADD, SUB,
//     IFLEQ, GOTO,
//     LABEL, OUT, HALT
// };

uint16_t Codegen::allocateVar(const std::string& name) {
    if (varMap.find(name) == varMap.end()) {
        varMap[name] = DATA_START + DATA_CURSOR; 
        DATA_CURSOR += 1;
    }
    return varMap[name];
}

void Codegen::generateCode() {
    code.push_back(0x02); code.push_back(0x03); code.push_back(1); // LOAD R3, 1 to use JNZ as GOTO.
    
    // use backpatching to patch the address of the label
    // we just need to scan the code once rather than twice because use the:
    // JNZ 1, label(addr placeholder)
    // so we can just scan the code once and patch the address of the label.
    
    for (const auto& instruction : ir) {
        switch (instruction.op) {
            case OpCode::HALT:
                code.push_back(0x00);
                break;
            case OpCode::OUT:
                // STORE 0xFF00, constant
                // or STORE 0xFF00, variable
                // get the first character of the arg1, check if its a number
                if (isdigit(instruction.arg1[0])) {
                    // if it's a number, then put it to the location 0xFF00
                    code.push_back(0x04); // STORE addr, CONST
                    code.push_back(0xFF); // addr high
                    code.push_back(0x00); // addr low
                    code.push_back(uint8_t(std::stoi(instruction.arg1))); // constant
                } else {
                    // it's a variable, then load the variable to the output register
                    // first load to the R0
                    // LOAD_VAR R0, varAddress
                    code.push_back(0x02); // LOAD Rd, addr
                    code.push_back(0x00); // R0
                    uint16_t varAddress = allocateVar(instruction.arg1);
                    code.push_back(varAddress >> 8);
                    code.push_back(varAddress & 0xFF);
                    // then store to the 0xFF00
                    code.push_back(0x03); // STORE addr, Rs
                    code.push_back(0xFF); // addr high
                    code.push_back(0x00); // addr low
                    code.push_back(0x00); // R0
                }
                break;
            case OpCode::LOAD_VAR: {
                // LOAD_VAR Rn, varAddress
                code.push_back(0x01);
                code.push_back(0x00); // R0
                uint16_t varAddress = allocateVar(instruction.arg1);
                code.push_back(varAddress >> 8);
                code.push_back(varAddress & 0xFF);
                break;
            }
            case OpCode::LOAD_CONST:
                // LOAD_CONST R0, const
                code.push_back(0x02);
                code.push_back(0x00); // R0
                code.push_back(uint8_t(std::stoi(instruction.arg1))); // const
                break;
            case OpCode::STORE: {
                // STORE addr1, addr2 store the value of addr2 to addr1
                uint16_t addr1 = allocateVar(instruction.arg1);
                uint16_t addr2 = allocateVar(instruction.arg2);
                // LOAD addr2, R0
                code.push_back(0x02); // LOAD Rd, addr
                code.push_back(0x00); // R0
                code.push_back(addr2 >> 8); // addr high
                code.push_back(addr2 & 0xFF); // addr low
                // STORE addr1, R0
                code.push_back(0x03); // STORE addr, Rs
                code.push_back(addr1 >> 8); // addr high
                code.push_back(addr1 & 0xFF); // addr low
                code.push_back(0x00); // R0
                break;
            }
            case OpCode::ADD: {
                // LOAD R0, var1
                code.push_back(0x02); 
                code.push_back(0x00); // R0
                uint16_t varAddress = allocateVar(instruction.arg1);
                code.push_back(varAddress >> 8);
                code.push_back(varAddress & 0xFF);
                // LOAD R1, var2
                code.push_back(0x02);
                code.push_back(0x01); // R1
                varAddress = allocateVar(instruction.arg2);
                code.push_back(varAddress >> 8);
                code.push_back(varAddress & 0xFF);
                // ADD R0, R1
                code.push_back(0x05); // ADD Rd, Rs
                code.push_back(0x00); // Rd
                code.push_back(0x01); // Rs
                // STORE resultAddress, R0
                code.push_back(0x03); // STORE addr, Rs
                uint16_t resultAddress = allocateVar(instruction.result); // resultAddress
                code.push_back(resultAddress >> 8);
                code.push_back(resultAddress & 0xFF);
                code.push_back(0x00); // R0
                break;
            }
            case OpCode::SUB: {
                // LOAD R0, var1
                code.push_back(0x02);
                code.push_back(0x00); // R0
                uint16_t varAddress = allocateVar(instruction.arg1);
                code.push_back(varAddress >> 8);
                code.push_back(varAddress & 0xFF);
                // LOAD R1, var2
                code.push_back(0x02);
                code.push_back(0x01); // R1
                varAddress = allocateVar(instruction.arg2);
                code.push_back(varAddress >> 8);
                code.push_back(varAddress & 0xFF);
                // SUB R0, R1
                code.push_back(0x06); // SUB Rd, Rs
                code.push_back(0x00); // Rd
                code.push_back(0x01); // Rs
                // STORE resultAddress, R0
                code.push_back(0x03); // STORE addr, Rs
                uint16_t resultAddress = allocateVar(instruction.result); // resultAddress
                code.push_back(resultAddress >> 8);
                code.push_back(resultAddress & 0xFF);
                code.push_back(0x00); // R0
                break;
            }
            case OpCode::IFLEQ: {
                // LOAD R0, var1
                code.push_back(0x02);
                code.push_back(0x00); // R0
                uint16_t varAddress = allocateVar(instruction.arg1);
                code.push_back(varAddress >> 8);
                code.push_back(varAddress & 0xFF);
                // LOAD R1, var2
                code.push_back(0x02);
                code.push_back(0x01); // R1
                varAddress = allocateVar(instruction.arg2);
                code.push_back(varAddress >> 8);
                code.push_back(varAddress & 0xFF);
                // SUB R0, R1
                code.push_back(0x06); // SUB Rd, Rs
                code.push_back(0x00); // Rd
                code.push_back(0x01); // Rs
                // JNZ R0, skip (jump over GOTO if result ≠ 0 and a > b)
                code.push_back(0x07);
                code.push_back(0x00);  // R0
                uint16_t skipLabelAddr = code.size();
                code.push_back(0x00);
                code.push_back(0x00);
                std::string skipLabel = "__ifleq_skip__" + std::to_string(pendingPatches.size());
                pendingPatches.push_back(Patch{skipLabelAddr, skipLabel});

                // GOTO label (jump over skipLabel if result ≠ 0 and a > b)
                code.push_back(0x07); // JNZ Rd, addr
                code.push_back(0x03); // Rd
                uint16_t labelAddress = code.size();
                code.push_back(0x00);
                code.push_back(0x00);
                pendingPatches.push_back(Patch{labelAddress, instruction.result});

                // LABEL skipLabel
                labelMap[skipLabel] = code.size();
                break;
            }
            case OpCode::LABEL: {
                // use the labelMap to get the address of the label
                // get current code address
                size_t currentCodeAddress = code.size();
                labelMap[instruction.result] = currentCodeAddress;
                break;
            }
            case OpCode::GOTO: {
                code.push_back(0x07);       // JNZ
                code.push_back(0x03);       // R3 (always 1)
                size_t patchPos = code.size();
                code.push_back(0x00);       // placeholder high byte
                code.push_back(0x00);       // placeholder low byte

                pendingPatches.push_back(Patch{patchPos, instruction.result});
                break;
            }
        }
    }
    // backpatching
    for (const auto& patch : pendingPatches) {
        if (labelMap.find(patch.labelName) != labelMap.end()) {
            uint16_t labelAddress = labelMap[patch.labelName];
            code[patch.addrPos] = labelAddress >> 8;
            code[patch.addrPos + 1] = labelAddress & 0xFF;
        }
    }
    // write the code to the file
    writeToFile("output.asm");
    writeToHex("output.bin", "output.hex");
}

void Codegen::writeToHex(std::string filenameBin, std::string filenameHex) {
    std::ofstream fileBin(filenameBin, std::ios::binary);
    std::ofstream fileHex(filenameHex);
    
    for (const auto& byte : code) {
        fileBin.write(reinterpret_cast<const char*>(&byte), 1);
        fileHex << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte) << " ";
    }
    
    fileBin.close();
    fileHex.close();
}

void Codegen::writeToFile(std::string filename) {
    std::ofstream file(filename);
    
    // Write the generated code in a readable format
    file << "; Generated assembly code" << std::endl;
    file << "; Code size: " << code.size() << " bytes" << std::endl;
    file << std::endl;
    
    for (size_t i = 0; i < code.size(); i++) {
        file << std::hex << std::setw(4) << std::setfill('0') << i << ": ";
        file << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(code[i]) << std::endl;
    }
    
    file.close();
}

void Codegen::writeToHex(std::string filename) {
    std::ofstream file(filename);
    
    for (const auto& byte : code) {
        file << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte) << " ";
    }
    
    file.close();
}






