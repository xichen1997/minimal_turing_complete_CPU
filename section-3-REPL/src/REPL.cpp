#include "lexer.h"
#include "parser.h"
#include <iostream>
#include <sstream>
#include <fstream>

#include "interpreter.h"

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
        parser.parseStatement();
        loadedProgram = parser.getIR();
        for(size_t i = 0; i < loadedProgram.size(); i++){
            if(loadedProgram[i].op == OpCode::LABEL){
                labelMap[loadedProgram[i].arg1] = i;
            }
        }
        std::cout << "File loaded successfully: " << filename << std::endl;
    }else if(cmd == ".run"){
        interpreterScript = new IRInterpreter(); // create a new interpreter for the script, which is safer than using the shell interpreter.
        if(loadedProgram.empty()){
            std::cout << "Error: No file loaded, please use .load to load a file first." << std::endl;
            return;
        }
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

    std::cout << "MiniOS REPL v0.1\nType .exit to quit.\n";

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
