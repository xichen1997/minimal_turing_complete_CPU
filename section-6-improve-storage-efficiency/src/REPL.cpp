#include "lexer.h"
#include "parser.h"
#include "cpu.h"
#include "codegen.h"
#include <iostream>
#include <sstream>
#include <fstream>

#include "interpreter.h"

// DEBUG_PRINT is already defined in cpu.h, so we don't need to redefine it here

std::vector<IR> loadedProgram;
std::unordered_map<std::string, size_t> labelMap;
IRInterpreter *interpreterScript = nullptr;
// put the variable map in the interpreter class, the life cycle of the variable map is the same as the interpreter
// we shouldn't use the variable map in global scope because it's will cause the variable conflicts.


// handle command
void handleCommand(const std::string& command) {
    std::istringstream iss(command);
    std::string cmd;
    iss >> cmd;
    if (cmd == ".exit") {
        exit(0);
    }else if(cmd == ".clear") {
        system("clear");
    }else if (cmd == ".help") {
        std::cout << "Available commands: .exit, .clear, .help .runfromCPU" << std::endl;
        std::cout << "Usage: .help to show this message" << std::endl;
        std::cout << "Usage: .runfromCPU <filename> to run the program from the CPU" << std::endl;
        std::cout << "Usage: .load <filename> to load the program from the file" << std::endl;
        std::cout << "Usage: .run to run the program from the REPL" << std::endl;
        std::cout << "Usage: .exit to exit the program" << std::endl;
        std::cout << "Usage: .clear to clear the screen" << std::endl;
    }else if(cmd == ".load"){
        std::string filename;
        iss >> filename;  // Extract filename from the stream
        if(filename.empty()) {
            std::cout << "Error: Please provide a filename after .load" << std::endl;
            return;
        }
        std::ifstream file(filename);
        if(!file.is_open()){
            std::cout << "Error: File not found: " << filename << std::endl;
            std::cout << "Current working directory: ";
            system("pwd");
            return;
        }
        std::string allCode((std::istreambuf_iterator<char>(file)),
                           std::istreambuf_iterator<char>());
        file.close();
        Lexer lexer(allCode);
        Parser parser(lexer);
        parser.parseProgram();
        loadedProgram = parser.getIR();
        for(size_t i = 0; i < loadedProgram.size(); i++){
            if(loadedProgram[i].op == OpCode::LABEL){
                labelMap[loadedProgram[i].result] = i;
            }
        }
        DEBUG_PRINT("Label map contents:");
        for(const auto& label : labelMap) {
            DEBUG_PRINT("  " << label.first << " -> " << label.second);
        }
        DEBUG_PRINT("size of loaded program: " << loadedProgram.size());
        DEBUG_PRINT("Generated IR instructions:");
        for(size_t i = 0; i < loadedProgram.size(); i++){
            const auto& inst = loadedProgram[i];
            DEBUG_PRINT(i << ": ");
            switch(inst.op) {
                case OpCode::LOAD_CONST: DEBUG_PRINT("LOAD_CONST " << inst.arg1 << " -> " << inst.result); break;
                case OpCode::LOAD_VAR: DEBUG_PRINT("LOAD_VAR " << inst.arg1 << " -> " << inst.result); break;
                case OpCode::ADD: DEBUG_PRINT("ADD " << inst.arg1 << " + " << inst.arg2 << " -> " << inst.result); break;
                case OpCode::SUB: DEBUG_PRINT("SUB " << inst.arg1 << " - " << inst.arg2 << " -> " << inst.result); break;
                case OpCode::STORE: DEBUG_PRINT("STORE " << inst.arg1 << " -> " << inst.result); break;
                case OpCode::STORE_CONST: DEBUG_PRINT("STORE_CONST " << inst.arg1 << " -> " << inst.result); break;
                case OpCode::OUT: DEBUG_PRINT("OUT " << inst.arg1); break;
                case OpCode::HALT: DEBUG_PRINT("HALT"); break;
                case OpCode::LABEL: DEBUG_PRINT("LABEL " << inst.arg1); break;
                case OpCode::GOTO: DEBUG_PRINT("GOTO " << inst.arg1); break;
                case OpCode::IFLEQ: DEBUG_PRINT("IFLEQ " << inst.arg1 << " <= " << inst.arg2 << " -> " << inst.result); break;
                case OpCode::IN: DEBUG_PRINT("IN " << inst.arg1); break;
                case OpCode::ARRAY_DECL: DEBUG_PRINT("ARRAY_DECL " << inst.arg1 << "[" << inst.arg2 << "]"); break;
                case OpCode::LOAD_INDEXED: DEBUG_PRINT("LOAD_INDEXED " << inst.arg1 << "[" << inst.arg2 << "] -> " << inst.result); break;
                case OpCode::STORE_INDEXED: DEBUG_PRINT("STORE_INDEXED " << inst.arg1 << "[" << inst.arg2 << "] = " << inst.result); break;
            }
        }
        DEBUG_PRINT("File loaded successfully: " << filename);
    }else if(cmd == ".run"){
        interpreterScript = new IRInterpreter(); // create a new interpreter for the script, which is safer than using the shell interpreter.
        if(loadedProgram.empty()){
            std::cout << "Error: No file loaded, please use .load to load a file first." << std::endl;
            return;
        }
        DEBUG_PRINT("size of loaded program: " << loadedProgram.size());
        interpreterScript->execute(loadedProgram, labelMap);
        delete interpreterScript;
        interpreterScript = nullptr;
    }else if(cmd == ".runfromCPU"){
        std::string filename; 
        uint16_t addr = 0x2000;
        iss >> filename; // Extract filename from the stream
        if(filename.empty()) {
            std::cout << "Error: Please provide a filename after .runfromCPU" << std::endl;
            return;
        }
        std::ifstream file(filename);
        if(!file.is_open()){
            std::cout << "Error: File not found: " << filename << std::endl;
            return;
        }
        file.close();
        
        Codegen gen(filename);
        gen.writeToHex("output.bin", "output.hex");
        gen.writeToFile("output.asm");
        std::vector<uint8_t> code = gen.getCode();
        DEBUG_PRINT("Code size: " << code.size());
        // load the code to the CPU
        MinimalCPU cpu;
        DEBUG_PRINT("Loading code to CPU...");
        DEBUG_PRINT("Code size: " << code.size());
        cpu.loadProgram(code, addr);
        cpu.run();
    }
    else {
        std::cout << "Unknown command: " << cmd << std::endl;
    }
}

int main() {
    IRInterpreter interpreter;

    std::cout << "MiniREPL v0.1\nType .exit to quit.\n";

    while (true) {
        std::cout << ">>> ";
        std::string line;
        if (!std::getline(std::cin, line)) break;
        if (line.empty()) continue;
        if (line[0] == '.') {
            handleCommand(line);
            continue;  // Skip parsing for commands
        }
        try {
            Lexer lexer(line);
            Parser parser(lexer);
            parser.parseStatement(); 

            const auto& ir = parser.getIR();
            // check if there are any control flow instructions
            for (const auto& inst : ir) {
                if (inst.op == OpCode::LABEL || inst.op == OpCode::GOTO || inst.op == OpCode::IFLEQ) {
                    throw std::runtime_error("Control flow instructions not supported in REPL mode");
                }
            }
            DEBUG_PRINT("IR size: " << ir.size());
            // execute the IR
            interpreter.execute(ir); 
        } catch (const std::exception& ex) {
            if (std::string(ex.what()) == "HALT instruction executed") {
                std::cout << "Program halted." << std::endl;
                break;
            }
            std::cerr << "Error: " << ex.what() << std::endl;
        }
    }

    return 0;
}
