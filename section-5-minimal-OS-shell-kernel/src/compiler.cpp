#include "../include/codegen.h"
#include "../include/lexer.h"
#include "../include/parser.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <iomanip>
#include <cctype>
#include <stdexcept>


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    std::string inputFile = argv[1];
    Codegen codegen(inputFile);
    codegen.writeToHex("output.bin", "output.hex");
    codegen.writeToFile("output.asm");
    std::cout << "Compiler completed!" << std::endl;
    return 0;
}