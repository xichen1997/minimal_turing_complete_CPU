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
// [0x2000 - 0x7FFF] : Code (<32KB for program)
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
//     LABEL, OUT, HALT, IN
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
                    code.push_back(0x01); // LOAD Rd, addr
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
            case OpCode::LOAD_CONST:{
                // LOAD_CONST R0, const
                code.push_back(0x02);
                code.push_back(0x00); // R0
                code.push_back(uint8_t(std::stoi(instruction.arg1))); // const
                break;
            }
            case OpCode::STORE: {
                // STORE addr1, addr2 store the value of addr2 to addr1
                uint16_t addr1 = allocateVar(instruction.result);
                uint16_t addr2 = allocateVar(instruction.arg1);
                // LOAD addr2, R0
                code.push_back(0x01); // LOAD Rd, addr
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
                code.push_back(0x01); 
                code.push_back(0x00); // R0
                uint16_t varAddress = allocateVar(instruction.arg1);
                code.push_back(varAddress >> 8);
                code.push_back(varAddress & 0xFF);
                // LOAD R1, var2
                code.push_back(0x01);
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
                code.push_back(0x01);
                code.push_back(0x00); // R0
                uint16_t varAddress = allocateVar(instruction.arg1);
                code.push_back(varAddress >> 8);
                code.push_back(varAddress & 0xFF);
                // LOAD R1, var2
                code.push_back(0x01);
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
            case OpCode::STORE_CONST: {
                // STORE addr, const
                uint16_t varAddress = allocateVar(instruction.result);
                code.push_back(0x04); // STORE addr, const
                code.push_back(varAddress >> 8); // addr high
                code.push_back(varAddress & 0xFF); // addr low
                code.push_back(uint8_t(std::stoi(instruction.arg1))); // const
                break;
            }
            case OpCode::IFLEQ: {
                // LOAD R0, var1
                code.push_back(0x01);
                code.push_back(0x00); // R0
                uint16_t varAddress = allocateVar(instruction.arg1);
                code.push_back(varAddress >> 8);
                code.push_back(varAddress & 0xFF);
                // LOAD R1, var2
                code.push_back(0x01);
                code.push_back(0x01); // R1
                varAddress = allocateVar(instruction.arg2);
                code.push_back(varAddress >> 8);
                code.push_back(varAddress & 0xFF);
                // SUB R1, R0
                code.push_back(0x06); // SUB Rd, Rs
                code.push_back(0x01); // Rd Var2
                code.push_back(0x00); // Rs Var1
                
                // JZ R2, skip (jump to label if a <= b)
                code.push_back(0x08);
                code.push_back(0x02); // R2
                size_t patchPos = code.size();
                code.push_back(0x00);
                code.push_back(0x00);
                pendingPatches.push_back(Patch{patchPos, instruction.result});
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
            case OpCode::IN: {
                // IN addr
                // can be parsed into instructions:
                // in R0 // this will put the value from the input buffer to R0 and store it to 0xFF01
                // STORE 0xFF01, addr // this will store the value of 0xFF01 to the addr

                // get the Rd from the std input and store it to the Rd
                code.push_back(0x09); // IN Rd
                code.push_back(0x00); // Rd
                // STORE 0xFF01, addr
                code.push_back(0x04); // STORE addr, R0
                uint16_t addr = allocateVar(instruction.arg1);
                code.push_back(addr >> 8);
                code.push_back(addr & 0xFF);
                code.push_back(0x00); // R0
                break;
            }
        }
    }
    // backpatching
    // std::cout << "Backpatching..." << std::endl;
    // std::cout << "Label map:" << std::endl;
    for (const auto& label : labelMap) {
        std::cout << "  " << label.first << " -> 0x" << std::hex << label.second << std::endl;
    }
    // std::cout << "Pending patches:" << std::endl;
    for (const auto& patch : pendingPatches) {
        std::cout << "  " << patch.labelName << " at position " << patch.addrPos << std::endl;
        if (labelMap.find(patch.labelName) != labelMap.end()) {
            uint16_t labelAddress = labelMap[patch.labelName];
            std::cout << "    Patching with address 0x" << std::hex << labelAddress << std::endl;
            code[patch.addrPos] = labelAddress >> 8;
            code[patch.addrPos + 1] = labelAddress & 0xFF;
        } else {
            std::cout << "    Label not found!" << std::endl;
        }
    }
    // write the code to the file
    writeToFile("output.asm");
    writeToHex("output.bin", "output.hex");
}

std::vector<uint8_t> Codegen::getCode() {
    return code;
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
    
    // Write IR instructions for debugging
    file << "; IR Instructions:" << std::endl;
    for (size_t i = 0; i < ir.size(); i++) {
        const auto& instruction = ir[i];
        std::string opStr;
        switch (instruction.op) {
            case OpCode::LOAD_CONST: opStr = "LOAD_CONST"; break;
            case OpCode::LOAD_VAR: opStr = "LOAD_VAR"; break;
            case OpCode::ADD: opStr = "ADD"; break;
            case OpCode::SUB: opStr = "SUB"; break;
            case OpCode::STORE: opStr = "STORE"; break;
            case OpCode::IFLEQ: opStr = "IFLEQ"; break;
            case OpCode::GOTO: opStr = "GOTO"; break;
            case OpCode::LABEL: opStr = "LABEL"; break;
            case OpCode::OUT: opStr = "OUT"; break;
            case OpCode::HALT: opStr = "HALT"; break;
            case OpCode::STORE_CONST: opStr = "STORE_CONST"; break;
            case OpCode::IN: opStr = "IN"; break;
            default: opStr = "UNKNOWN"; break;
        }
        
        file << "; IR[" << i << "]: " << opStr;
        if (!instruction.arg1.empty()) file << " " << instruction.arg1;
        if (!instruction.arg2.empty()) file << " " << instruction.arg2;
        if (!instruction.result.empty()) file << " -> " << instruction.result;
        file << std::endl;
    }
    file << std::endl;
    
    // Write machine code
    file << "; Machine Code:" << std::endl;
    size_t i = 0;
    while (i < code.size()) {
        file << std::hex << std::setw(4) << std::setfill('0') << i << ": ";
        
        uint8_t opcode = code[i];
        
        // Add instruction name and opcode as comment
        std::string instruction_name;
        std::string opcode_desc;
        
        switch (opcode) {
            case 0x00:
                instruction_name = "HALT";
                opcode_desc = "HALT";
                break;
            case 0x01:
                instruction_name = "LOAD_VAR";
                opcode_desc = "LOAD Rd, addr";
                break;
            case 0x02:
                instruction_name = "LOAD_CONST";
                opcode_desc = "LOAD Rd, const";
                break;
            case 0x03:
                instruction_name = "STORE";
                opcode_desc = "STORE addr, Rs";
                break;
            case 0x04:
                instruction_name = "STORE_CONST";
                opcode_desc = "STORE addr, const";
                break;
            case 0x05:
                instruction_name = "ADD";
                opcode_desc = "ADD Rd, Rs";
                break;
            case 0x06:
                instruction_name = "SUB";
                opcode_desc = "SUB Rd, Rs";
                break;
            case 0x07:
                instruction_name = "JNZ";
                opcode_desc = "JNZ Rd, addr";
                break;
            case 0x08:
                instruction_name = "JZ";
                opcode_desc = "JZ Rd, addr";
                break;
            case 0x09:
                instruction_name = "IN";
                opcode_desc = "IN Rd"; //not only load the value to the Rd, but also store it to 0xFF01
                break;
            default:
                instruction_name = "UNKNOWN";
                opcode_desc = "Unknown opcode";
                break;
        }
        
        // Print all bytes of the instruction
        file << "0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode);
        
        // Add operand information for multi-byte instructions
        if (opcode == 0x01 || opcode == 0x02) { // LOAD_VAR, LOAD_CONST
            if (i + 1 < code.size()) {
                uint8_t rd = code[i + 1];
                file << " 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(rd);
                if (opcode == 0x01 && i + 3 < code.size()) { // LOAD_VAR with address
                    uint8_t addr_high = code[i + 2];
                    uint8_t addr_low = code[i + 3];
                    file << " 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(addr_high);
                    file << " 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(addr_low);
                } else if (opcode == 0x02 && i + 2 < code.size()) { // LOAD_CONST with constant
                    uint8_t const_val = code[i + 2];
                    file << " 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(const_val);
                }
            }
        } else if (opcode == 0x03) { // STORE
            if (i + 3 < code.size()) {
                uint8_t addr_high = code[i + 1];
                uint8_t addr_low = code[i + 2];
                uint8_t rs = code[i + 3];
                file << " 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(addr_high);
                file << " 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(addr_low);
                file << " 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(rs);
            }
        } else if (opcode == 0x04) { // STORE_CONST
            if (i + 3 < code.size()) {
                uint8_t addr_high = code[i + 1];
                uint8_t addr_low = code[i + 2];
                uint8_t const_val = code[i + 3];
                file << " 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(addr_high);
                file << " 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(addr_low);
                file << " 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(const_val);
            }
        } else if (opcode == 0x05 || opcode == 0x06) { // ADD, SUB
            if (i + 2 < code.size()) {
                uint8_t rd = code[i + 1];
                uint8_t rs = code[i + 2];
                file << " 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(rd);
                file << " 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(rs);
            }
        } else if (opcode == 0x07) { // JNZ
            if (i + 1 < code.size()) {
                uint8_t rd = code[i + 1];
                file << " 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(rd);
                if (i + 3 < code.size()) {
                    uint8_t addr_high = code[i + 2];
                    uint8_t addr_low = code[i + 3];
                    file << " 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(addr_high);
                    file << " 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(addr_low);
                }
            }
        } else if (opcode == 0x08) { // JZ
            if (i + 1 < code.size()) {
                uint8_t rd = code[i + 1];
                file << " 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(rd);
                if (i + 3 < code.size()) {
                    uint8_t addr_high = code[i + 2];
                    uint8_t addr_low = code[i + 3];
                    file << " 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(addr_high);
                    file << " 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(addr_low);
                }
            }
        } else if (opcode == 0x09) { // IN
            if (i + 1 < code.size()) {
                uint8_t addr_high = code[i + 1];
                uint8_t addr_low = code[i + 2];
                file << " 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(addr_high);
                file << " 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(addr_low);
            }
        }
        
        file << " ; " << instruction_name << " (" << opcode_desc << ")";
        
        // Add operand details to the comment
        if (opcode == 0x01 || opcode == 0x02) { // LOAD_VAR, LOAD_CONST
            if (i + 1 < code.size()) {
                uint8_t rd = code[i + 1];
                file << " R" << static_cast<int>(rd);
                if (opcode == 0x01 && i + 3 < code.size()) { // LOAD_VAR with address
                    uint16_t addr = (code[i + 2] << 8) | code[i + 3];
                    file << ", 0x" << std::hex << std::setw(4) << std::setfill('0') << addr;
                } else if (opcode == 0x02 && i + 2 < code.size()) { // LOAD_CONST with constant
                    uint8_t const_val = code[i + 2];
                    file << ", " << static_cast<int>(const_val);
                }
            }
        } else if (opcode == 0x03) { // STORE
            if (i + 3 < code.size()) {
                uint16_t addr = (code[i + 1] << 8) | code[i + 2];
                uint8_t rs = code[i + 3];
                file << " 0x" << std::hex << std::setw(4) << std::setfill('0') << addr << ", R" << static_cast<int>(rs);
            }
        } else if (opcode == 0x04) { // STORE_CONST
            if (i + 3 < code.size()) {
                uint16_t addr = (code[i + 1] << 8) | code[i + 2];
                uint8_t const_val = code[i + 3];
                file << " 0x" << std::hex << std::setw(4) << std::setfill('0') << addr << ", " << static_cast<int>(const_val);
            }
        } else if (opcode == 0x05 || opcode == 0x06) { // ADD, SUB
            if (i + 2 < code.size()) {
                uint8_t rd = code[i + 1];
                uint8_t rs = code[i + 2];
                file << " R" << static_cast<int>(rd) << ", R" << static_cast<int>(rs);
            }
        } else if (opcode == 0x07) { // JNZ
            if (i + 1 < code.size()) {
                uint8_t rd = code[i + 1];
                file << " R" << static_cast<int>(rd);
                if (i + 3 < code.size()) {
                    uint16_t addr = (code[i + 2] << 8) | code[i + 3];
                    file << ", 0x" << std::hex << std::setw(4) << std::setfill('0') << addr;
                }
            }
        } else if (opcode == 0x08) { // JZ
            if (i + 1 < code.size()) {
                uint8_t rd = code[i + 1];
                file << " 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(rd);
                if (i + 3 < code.size()) {
                    uint8_t addr_high = code[i + 2];
                    uint8_t addr_low = code[i + 3];
                    file << " 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(addr_high);
                    file << " 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(addr_low);
                }
            }
        }   
        
        file << std::endl;
        
        // Advance to next instruction based on opcode
        switch (opcode) {
            case 0x00: // HALT
                i += 1;
                break;
            case 0x01: // LOAD_VAR
                i += 4; // opcode + rd + addr_high + addr_low
                break;
            case 0x02: // LOAD_CONST
                i += 3; // opcode + rd + const
                break;
            case 0x03: // STORE
                i += 4; // opcode + addr_high + addr_low + rs
                break;
            case 0x04: // STORE_CONST
                i += 4; // opcode + addr_high + addr_low + const
                break;
            case 0x05: // ADD
            case 0x06: // SUB
                i += 3; // opcode + rd + rs
                break;
            case 0x07: // JNZ
                i += 4; // opcode + rd + addr_high + addr_low
                break;
            case 0x08: // JZ
                i += 4; // opcode + rd + addr_high + addr_low
                break;
            case 0x09: // IN
                i += 3; // opcode + addr_high + addr_low
                break;
            default:
                i += 1; // Unknown opcode, advance by 1
                break;
        }
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






