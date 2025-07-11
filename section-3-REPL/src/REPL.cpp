#include "lexer.h"
#include "parser.h"
#include <iostream>
#include <sstream>

#include "interpreter.h"

int main() {
    IRInterpreter interpreter;

    std::cout << "MiniOS REPL v0.1\nType .exit to quit.\n";

    while (true) {
        std::cout << ">>> ";
        std::string line;
        if (!std::getline(std::cin, line)) break;
        if (line.empty()) continue;
        if (line == ".exit") break;

        try {
            Lexer lexer(line);
            Parser parser(lexer);
            parser.parseStatement(); 

            const auto& ir = parser.getIR();
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
