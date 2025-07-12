#include "lexer.h"
#include "parser.h"
#include <iostream>
#include <sstream>

#include "interpreter.h"
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
