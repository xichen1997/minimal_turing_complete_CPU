#include "lexer.h"
#include "parser.h"
#include <iostream>
#include <sstream>
#include <fstream>

#include "interpreter.h"

#ifdef DEBUG
#define DEBUG_PRINT(x) do { x; } while (0)
#else
#define DEBUG_PRINT(x) do {} while (0)
#endif

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
        std::cout << "Available commands: .exit, .clear, .help" << std::endl;
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
        DEBUG_PRINT(std::cout << "Label map contents:" << std::endl;);
        for(const auto& label : labelMap) {
            DEBUG_PRINT(std::cout << "  " << label.first << " -> " << label.second << std::endl;);
        }
        DEBUG_PRINT(std::cout << "size of loaded program: " << loadedProgram.size() << std::endl;);
        DEBUG_PRINT(std::cout << "Generated IR instructions:" << std::endl;);
        for(size_t i = 0; i < loadedProgram.size(); i++){
            const auto& inst = loadedProgram[i];
            DEBUG_PRINT(std::cout << i << ": ");
            switch(inst.op) {
                case OpCode::LOAD_CONST: DEBUG_PRINT(std::cout << "LOAD_CONST " << inst.arg1 << " -> " << inst.result << std::endl;); break;
                case OpCode::LOAD_VAR: DEBUG_PRINT(std::cout << "LOAD_VAR " << inst.arg1 << " -> " << inst.result << std::endl;); break;
                case OpCode::ADD: DEBUG_PRINT(std::cout << "ADD " << inst.arg1 << " + " << inst.arg2 << " -> " << inst.result << std::endl;); break;
                case OpCode::SUB: DEBUG_PRINT(std::cout << "SUB " << inst.arg1 << " - " << inst.arg2 << " -> " << inst.result << std::endl;); break;
                case OpCode::STORE: DEBUG_PRINT(std::cout << "STORE " << inst.arg1 << " -> " << inst.result << std::endl;); break;
                case OpCode::STORE_CONST: DEBUG_PRINT(std::cout << "STORE_CONST " << inst.arg1 << " -> " << inst.result << std::endl;); break;
                case OpCode::OUT: DEBUG_PRINT(std::cout << "OUT " << inst.arg1 << std::endl;); break;
                case OpCode::HALT: DEBUG_PRINT(std::cout << "HALT" << std::endl;); break;
                case OpCode::LABEL: DEBUG_PRINT(std::cout << "LABEL " << inst.arg1 << std::endl;); break;
                case OpCode::GOTO: DEBUG_PRINT(std::cout << "GOTO " << inst.arg1 << std::endl;); break;
                case OpCode::IFLEQ: DEBUG_PRINT(std::cout << "IFLEQ " << inst.arg1 << " <= " << inst.arg2 << " -> " << inst.result << std::endl;); break;
                case OpCode::IN: DEBUG_PRINT(std::cout << "IN " << inst.arg1 << std::endl;); break;
            }
        }
        DEBUG_PRINT(std::cout << "File loaded successfully: " << filename << std::endl;);
    }else if(cmd == ".run"){
        interpreterScript = new IRInterpreter(); // create a new interpreter for the script, which is safer than using the shell interpreter.
        if(loadedProgram.empty()){
            std::cout << "Error: No file loaded, please use .load to load a file first." << std::endl;
            return;
        }
        DEBUG_PRINT(std::cout << "size of loaded program: " << loadedProgram.size() << std::endl;);
        interpreterScript->execute(loadedProgram, labelMap);
        delete interpreterScript;
        interpreterScript = nullptr;
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
            DEBUG_PRINT(std::cout << "IR size: " << ir.size() << std::endl;);
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
