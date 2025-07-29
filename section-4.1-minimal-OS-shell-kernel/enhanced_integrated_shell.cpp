#include "include/cpu.h"
#include <vector>
#include <iostream>
#include <iomanip>
#include <map>
#include <string>

// Enhanced shell OS with Interactive Calculator and Memory Viewer
std::vector<uint8_t> createEnhancedIntegratedShellOS() {
    std::vector<uint8_t> code;
    
    // ASCII Art Banner
    // Print top border: "═══════════════════════════"
    for(int i = 0; i < 27; i++) {
        code.insert(code.end(), {0x04, 0xFF, 0x00, 61}); // '='
    }
    code.insert(code.end(), {0x04, 0xFF, 0x00, 10}); // newline
    
    // Print "║  Enhanced Integrated OS  ║"
    code.insert(code.end(), {0x04, 0xFF, 0x00, 124}); // '|'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 32});  // ' '
    code.insert(code.end(), {0x04, 0xFF, 0x00, 69});  // 'E'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 110}); // 'n'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 104}); // 'h'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 97});  // 'a'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 110}); // 'n'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 99});  // 'c'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 101}); // 'e'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 100}); // 'd'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 32});  // ' '
    code.insert(code.end(), {0x04, 0xFF, 0x00, 79});  // 'O'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 83});  // 'S'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 32});  // ' '
    code.insert(code.end(), {0x04, 0xFF, 0x00, 118}); // 'v'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 54});  // '6'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 46});  // '.'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 48});  // '0'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 32});  // ' '
    code.insert(code.end(), {0x04, 0xFF, 0x00, 124}); // '|'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 10}); // newline
    
    // Print bottom border
    for(int i = 0; i < 27; i++) {
        code.insert(code.end(), {0x04, 0xFF, 0x00, 61});
    }
    code.insert(code.end(), {0x04, 0xFF, 0x00, 10});
    
    // Print available commands
    std::string commands = "Commands: h,1,c,m,q (calc & memory!)";
    for (char c : commands) {
        code.insert(code.end(), {0x04, 0xFF, 0x00, static_cast<uint8_t>(c)});
    }
    code.insert(code.end(), {0x04, 0xFF, 0x00, 10}); // newline
    code.insert(code.end(), {0x04, 0xFF, 0x00, 10}); // newline
    
    // ***** MAIN LOOP START *****
    uint16_t main_loop = code.size();
    
    // Print shell prompt: "[Enhanced]$ "
    code.insert(code.end(), {0x04, 0xFF, 0x00, 91});  // '['
    code.insert(code.end(), {0x04, 0xFF, 0x00, 69});  // 'E'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 79});  // 'O'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 83});  // 'S'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 93});  // ']'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 36});  // '$'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 32});  // ' '
    
    // Read command and store in R4
    code.insert(code.end(), {0x09, 0x00}); // IN R0
    code.insert(code.end(), {0x04, 0xFF, 0x00, 10}); // newline
    code.insert(code.end(), {0x02, 0x04, 0x00}); // LOAD R4, 0
    code.insert(code.end(), {0x05, 0x04, 0x00}); // ADD R4, R0
    
    // Check for 'h' (104) - Help
    code.insert(code.end(), {0x02, 0x01, 104}); // LOAD R1, 104
    code.insert(code.end(), {0x06, 0x00, 0x01}); // SUB R0, R1
    uint16_t help_jump_pos = code.size() + 2;
    code.insert(code.end(), {0x08, 0x00, 0x00, 0x00}); // JZ R0, help_addr
    
    // Restore and check for '1' (49) - Hello World
    code.insert(code.end(), {0x02, 0x00, 0x00}); // LOAD R0, 0
    code.insert(code.end(), {0x05, 0x00, 0x04}); // ADD R0, R4
    code.insert(code.end(), {0x02, 0x01, 49});  // LOAD R1, 49
    code.insert(code.end(), {0x06, 0x00, 0x01}); // SUB R0, R1
    uint16_t hello_jump_pos = code.size() + 2;
    code.insert(code.end(), {0x08, 0x00, 0x00, 0x00}); // JZ R0, hello_addr
    
    // Restore and check for 'c' (99) - Interactive Calculator
    code.insert(code.end(), {0x02, 0x00, 0x00}); // LOAD R0, 0
    code.insert(code.end(), {0x05, 0x00, 0x04}); // ADD R0, R4
    code.insert(code.end(), {0x02, 0x01, 99});  // LOAD R1, 99
    code.insert(code.end(), {0x06, 0x00, 0x01}); // SUB R0, R1
    uint16_t calc_jump_pos = code.size() + 2;
    code.insert(code.end(), {0x08, 0x00, 0x00, 0x00}); // JZ R0, calc_addr
    
    // Restore and check for 'm' (109) - Memory Viewer
    code.insert(code.end(), {0x02, 0x00, 0x00}); // LOAD R0, 0
    code.insert(code.end(), {0x05, 0x00, 0x04}); // ADD R0, R4
    code.insert(code.end(), {0x02, 0x01, 109}); // LOAD R1, 109
    code.insert(code.end(), {0x06, 0x00, 0x01}); // SUB R0, R1
    uint16_t mem_jump_pos = code.size() + 2;
    code.insert(code.end(), {0x08, 0x00, 0x00, 0x00}); // JZ R0, mem_addr
    
    // Check for 'q' (113) - Quit
    code.insert(code.end(), {0x02, 0x00, 0x00}); // LOAD R0, 0
    code.insert(code.end(), {0x05, 0x00, 0x04}); // ADD R0, R4
    code.insert(code.end(), {0x02, 0x01, 113}); // LOAD R1, 113
    code.insert(code.end(), {0x06, 0x00, 0x01}); // SUB R0, R1
    uint16_t quit_jump_pos = code.size() + 2;
    code.insert(code.end(), {0x08, 0x00, 0x00, 0x00}); // JZ R0, quit_addr
    
    // Unknown command
    code.insert(code.end(), {0x04, 0xFF, 0x00, 69});  // 'E'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 114}); // 'r'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 114}); // 'r'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 111}); // 'o'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 114}); // 'r'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 58});  // ':'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 32});  // ' '
    code.insert(code.end(), {0x03, 0xFF, 0x00, 0x04}); // STORE 0xFF00, R4
    code.insert(code.end(), {0x04, 0xFF, 0x00, 32});  // ' '
    code.insert(code.end(), {0x04, 0xFF, 0x00, 110}); // 'n'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 111}); // 'o'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 116}); // 't'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 32});  // ' '
    code.insert(code.end(), {0x04, 0xFF, 0x00, 102}); // 'f'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 111}); // 'o'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 117}); // 'u'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 110}); // 'n'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 100}); // 'd'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 10});  // newline
    
    // Jump back to main loop
    uint16_t abs_main_loop = 0x1000 + main_loop;
    uint8_t main_high = (abs_main_loop >> 8) & 0xFF;
    uint8_t main_low = abs_main_loop & 0xFF;
    code.insert(code.end(), {0x02, 0x03, 1}); // LOAD R3, 1
    code.insert(code.end(), {0x07, 0x03, main_high, main_low}); // JNZ R3, main_loop
    
    // ***** HELP COMMAND *****
    uint16_t help_addr = code.size();
    code.insert(code.end(), {0x04, 0xFF, 0x00, 61});  // '='
    code.insert(code.end(), {0x04, 0xFF, 0x00, 61});  // '='
    code.insert(code.end(), {0x04, 0xFF, 0x00, 61});  // '='
    code.insert(code.end(), {0x04, 0xFF, 0x00, 32});  // ' '
    code.insert(code.end(), {0x04, 0xFF, 0x00, 72});  // 'H'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 69});  // 'E'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 76});  // 'L'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 80});  // 'P'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 32});  // ' '
    code.insert(code.end(), {0x04, 0xFF, 0x00, 61});  // '='
    code.insert(code.end(), {0x04, 0xFF, 0x00, 61});  // '='
    code.insert(code.end(), {0x04, 0xFF, 0x00, 61});  // '='
    code.insert(code.end(), {0x04, 0xFF, 0x00, 10});  // newline
    
    // Help content
    std::string help_lines[] = {
        "h - Show this help",
        "1 - Hello World demo", 
        "c - Interactive Calculator",
        "m - Memory Viewer",
        "q - Quit system"
    };
    
    for (const auto& line : help_lines) {
        for (char c : line) {
            code.insert(code.end(), {0x04, 0xFF, 0x00, static_cast<uint8_t>(c)});
        }
        code.insert(code.end(), {0x04, 0xFF, 0x00, 10}); // newline
    }
    code.insert(code.end(), {0x04, 0xFF, 0x00, 10}); // extra newline
    
    // Back to main loop
    code.insert(code.end(), {0x02, 0x03, 1}); // LOAD R3, 1
    code.insert(code.end(), {0x07, 0x03, main_high, main_low}); // JNZ R3, main_loop
    
    // ***** HELLO WORLD PROGRAM *****
    uint16_t hello_addr = code.size();
    std::string hello_msg = "Hello from Enhanced OS!";
    for (char c : hello_msg) {
        code.insert(code.end(), {0x04, 0xFF, 0x00, static_cast<uint8_t>(c)});
    }
    code.insert(code.end(), {0x04, 0xFF, 0x00, 10}); // newline
    
    // Jump back to main loop
    code.insert(code.end(), {0x02, 0x03, 1}); // LOAD R3, 1
    code.insert(code.end(), {0x07, 0x03, main_high, main_low}); // JNZ R3, main_loop
    
    // ***** INTERACTIVE CALCULATOR *****
    uint16_t calc_addr = code.size();
    
    // Calculator banner
    std::string calc_banner = "=== INTERACTIVE CALCULATOR ===";
    for (char c : calc_banner) {
        code.insert(code.end(), {0x04, 0xFF, 0x00, static_cast<uint8_t>(c)});
    }
    code.insert(code.end(), {0x04, 0xFF, 0x00, 10}); // newline
    
    // Instructions
    std::string calc_inst = "Enter first number (0-9):";
    for (char c : calc_inst) {
        code.insert(code.end(), {0x04, 0xFF, 0x00, static_cast<uint8_t>(c)});
    }
    code.insert(code.end(), {0x04, 0xFF, 0x00, 32}); // space
    
    // Read first number
    code.insert(code.end(), {0x09, 0x05}); // IN R5 (first number)
    code.insert(code.end(), {0x04, 0xFF, 0x00, 10}); // newline
    
    // Convert ASCII to number (subtract 48)
    code.insert(code.end(), {0x02, 0x06, 48}); // LOAD R6, 48 ('0')
    code.insert(code.end(), {0x06, 0x05, 0x06}); // SUB R5, R6 (R5 = R5 - 48)
    
    // Ask for operation
    std::string op_inst = "Enter operation (+,-): ";
    for (char c : op_inst) {
        code.insert(code.end(), {0x04, 0xFF, 0x00, static_cast<uint8_t>(c)});
    }
    
    // Read operation
    code.insert(code.end(), {0x09, 0x06}); // IN R6 (operation)
    code.insert(code.end(), {0x04, 0xFF, 0x00, 10}); // newline
    
    // Ask for second number
    std::string num2_inst = "Enter second number (0-9): ";
    for (char c : num2_inst) {
        code.insert(code.end(), {0x04, 0xFF, 0x00, static_cast<uint8_t>(c)});
    }
    
    // Read second number
    code.insert(code.end(), {0x09, 0x07}); // IN R7 (second number)
    code.insert(code.end(), {0x04, 0xFF, 0x00, 10}); // newline
    
    // Convert second number ASCII to number
    code.insert(code.end(), {0x02, 0x01, 48}); // LOAD R1, 48
    code.insert(code.end(), {0x06, 0x07, 0x01}); // SUB R7, R1 (R7 = R7 - 48)
    
    // Check operation: '+' = 43, '-' = 45
    code.insert(code.end(), {0x02, 0x01, 43}); // LOAD R1, 43 ('+')
    code.insert(code.end(), {0x02, 0x02, 0x00}); // LOAD R2, 0 (temp for comparison)
    code.insert(code.end(), {0x05, 0x02, 0x06}); // ADD R2, R6 (R2 = R6)
    code.insert(code.end(), {0x06, 0x02, 0x01}); // SUB R2, R1 (R2 = R6 - 43)
    
    uint16_t add_jump_pos = code.size() + 2;
    code.insert(code.end(), {0x08, 0x02, 0x00, 0x00}); // JZ R2, add_operation
    
    // Must be subtraction
    code.insert(code.end(), {0x06, 0x05, 0x07}); // SUB R5, R7 (R5 = R5 - R7)
    uint16_t result_jump_pos = code.size() + 2;
    code.insert(code.end(), {0x02, 0x03, 1}); // LOAD R3, 1
    code.insert(code.end(), {0x07, 0x03, 0x00, 0x00}); // JNZ R3, show_result
    
    // Addition operation
    uint16_t add_operation_addr = code.size();
    code.insert(code.end(), {0x05, 0x05, 0x07}); // ADD R5, R7 (R5 = R5 + R7)
    
    // Show result
    uint16_t show_result_addr = code.size();
    std::string result_msg = "Result: ";
    for (char c : result_msg) {
        code.insert(code.end(), {0x04, 0xFF, 0x00, static_cast<uint8_t>(c)});
    }
    
    // Convert result back to ASCII and display
    code.insert(code.end(), {0x02, 0x01, 48}); // LOAD R1, 48 ('0')
    code.insert(code.end(), {0x05, 0x05, 0x01}); // ADD R5, R1 (convert to ASCII)
    code.insert(code.end(), {0x03, 0xFF, 0x00, 0x05}); // STORE 0xFF00, R5 (print result)
    code.insert(code.end(), {0x04, 0xFF, 0x00, 10}); // newline
    code.insert(code.end(), {0x04, 0xFF, 0x00, 10}); // newline
    
    // Back to main loop
    code.insert(code.end(), {0x02, 0x03, 1}); // LOAD R3, 1
    code.insert(code.end(), {0x07, 0x03, main_high, main_low}); // JNZ R3, main_loop
    
    // ***** MEMORY VIEWER *****
    uint16_t mem_addr = code.size();
    
    // Memory viewer banner
    std::string mem_banner = "=== MEMORY VIEWER ===";
    for (char c : mem_banner) {
        code.insert(code.end(), {0x04, 0xFF, 0x00, static_cast<uint8_t>(c)});
    }
    code.insert(code.end(), {0x04, 0xFF, 0x00, 10}); // newline
    
    // Show system info
    std::string sys_info = "System Memory Layout:";
    for (char c : sys_info) {
        code.insert(code.end(), {0x04, 0xFF, 0x00, static_cast<uint8_t>(c)});
    }
    code.insert(code.end(), {0x04, 0xFF, 0x00, 10}); // newline
    
    std::string mem_lines[] = {
        "0x1000: OS Start",
        "0x1044: Main Loop", 
        "0x13XX: Programs",
        "0x8000: Data Section",
        "0xFF00: I/O Output"
    };
    
    for (const auto& line : mem_lines) {
        for (char c : line) {
            code.insert(code.end(), {0x04, 0xFF, 0x00, static_cast<uint8_t>(c)});
        }
        code.insert(code.end(), {0x04, 0xFF, 0x00, 10}); // newline
    }
    
    // Show some actual memory contents
    code.insert(code.end(), {0x04, 0xFF, 0x00, 10}); // newline
    std::string mem_sample = "Sample Memory at 0x1000: ";
    for (char c : mem_sample) {
        code.insert(code.end(), {0x04, 0xFF, 0x00, static_cast<uint8_t>(c)});
    }
    
    // Read and display memory at 0x1000 (first few bytes of our program)
    code.insert(code.end(), {0x01, 0x05, 0x10, 0x00}); // LOAD R5, [0x1000]
    code.insert(code.end(), {0x02, 0x06, 48}); // LOAD R6, 48 ('0' base)
    code.insert(code.end(), {0x05, 0x05, 0x06}); // ADD R5, R6 (convert to rough ASCII)
    code.insert(code.end(), {0x03, 0xFF, 0x00, 0x05}); // STORE 0xFF00, R5 (display)
    code.insert(code.end(), {0x04, 0xFF, 0x00, 32}); // space
    
    // Read next byte
    code.insert(code.end(), {0x01, 0x05, 0x10, 0x01}); // LOAD R5, [0x1001]
    code.insert(code.end(), {0x05, 0x05, 0x06}); // ADD R5, R6
    code.insert(code.end(), {0x03, 0xFF, 0x00, 0x05}); // STORE 0xFF00, R5
    code.insert(code.end(), {0x04, 0xFF, 0x00, 32}); // space
    
    // Read third byte
    code.insert(code.end(), {0x01, 0x05, 0x10, 0x02}); // LOAD R5, [0x1002]  
    code.insert(code.end(), {0x05, 0x05, 0x06}); // ADD R5, R6
    code.insert(code.end(), {0x03, 0xFF, 0x00, 0x05}); // STORE 0xFF00, R5
    code.insert(code.end(), {0x04, 0xFF, 0x00, 46}); // '.'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 46}); // '.'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 46}); // '.'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 10}); // newline
    code.insert(code.end(), {0x04, 0xFF, 0x00, 10}); // newline
    
    // Back to main loop
    code.insert(code.end(), {0x02, 0x03, 1}); // LOAD R3, 1
    code.insert(code.end(), {0x07, 0x03, main_high, main_low}); // JNZ R3, main_loop
    
    // ***** QUIT COMMAND *****
    uint16_t quit_addr = code.size();
    std::string goodbye = "Enhanced OS shutting down...";
    for (char c : goodbye) {
        code.insert(code.end(), {0x04, 0xFF, 0x00, static_cast<uint8_t>(c)});
    }
    code.insert(code.end(), {0x04, 0xFF, 0x00, 10}); // newline
    code.insert(code.end(), {0x04, 0xFF, 0x00, 71}); // 'G'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 111}); // 'o'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 111}); // 'o'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 100}); // 'd'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 98}); // 'b'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 121}); // 'y'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 101}); // 'e'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 33}); // '!'
    code.insert(code.end(), {0x04, 0xFF, 0x00, 10}); // newline
    code.insert(code.end(), {0x00}); // HALT
    
    // ***** PATCH JUMP ADDRESSES *****
    uint16_t base_addr = 0x1000;
    
    // Patch help jump
    uint16_t abs_help_addr = base_addr + help_addr;
    code[help_jump_pos] = (abs_help_addr >> 8) & 0xFF;
    code[help_jump_pos + 1] = abs_help_addr & 0xFF;
    
    // Patch hello jump
    uint16_t abs_hello_addr = base_addr + hello_addr;
    code[hello_jump_pos] = (abs_hello_addr >> 8) & 0xFF;
    code[hello_jump_pos + 1] = abs_hello_addr & 0xFF;
    
    // Patch calculator jump
    uint16_t abs_calc_addr = base_addr + calc_addr;
    code[calc_jump_pos] = (abs_calc_addr >> 8) & 0xFF;
    code[calc_jump_pos + 1] = abs_calc_addr & 0xFF;
    
    // Patch memory viewer jump
    uint16_t abs_mem_addr = base_addr + mem_addr;
    code[mem_jump_pos] = (abs_mem_addr >> 8) & 0xFF;
    code[mem_jump_pos + 1] = abs_mem_addr & 0xFF;
    
    // Patch quit jump
    uint16_t abs_quit_addr = base_addr + quit_addr;
    code[quit_jump_pos] = (abs_quit_addr >> 8) & 0xFF;
    code[quit_jump_pos + 1] = abs_quit_addr & 0xFF;
    
    // Patch calculator internal jumps
    uint16_t abs_add_operation = base_addr + add_operation_addr;
    code[add_jump_pos] = (abs_add_operation >> 8) & 0xFF;
    code[add_jump_pos + 1] = abs_add_operation & 0xFF;
    
    uint16_t abs_show_result = base_addr + show_result_addr;
    code[result_jump_pos] = (abs_show_result >> 8) & 0xFF;
    code[result_jump_pos + 1] = abs_show_result & 0xFF;
    
    return code;
}

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "    Enhanced Integrated Shell v6.0     " << std::endl;
    std::cout << "  Interactive Calculator & Memory!      " << std::endl;
    std::cout << "========================================" << std::endl;
    
    // Generate the enhanced system
    std::vector<uint8_t> enhancedCode = createEnhancedIntegratedShellOS();
    
    std::cout << "Enhanced system size: " << enhancedCode.size() << " bytes" << std::endl;
    std::cout << "Memory constraint: Must not exceed 0x8000 (32768 bytes)" << std::endl;
    
    // Check memory constraint
    if (0x1000 + enhancedCode.size() > 0x8000) {
        std::cout << "WARNING: System exceeds 0x8000 memory limit!" << std::endl;
        std::cout << "Current end address: 0x" << std::hex << (0x1000 + enhancedCode.size()) << std::endl;
    } else {
        std::cout << "✓ Enhanced system fits within memory constraint" << std::endl;
        std::cout << "End address: 0x" << std::hex << (0x1000 + enhancedCode.size()) << std::endl;
    }
    
    std::cout << std::dec << std::endl;
    std::cout << "New Features:" << std::endl;
    std::cout << "• Interactive Calculator (command 'c')" << std::endl;
    std::cout << "• Memory Viewer (command 'm')" << std::endl;
    std::cout << "• Enhanced error messages" << std::endl;
    std::cout << "• Improved help system" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;
    
    // Run the enhanced system
    MinimalCPU cpu;
    cpu.loadProgram(enhancedCode, 0x1000);
    cpu.run();
    
    std::cout << std::endl << "Enhanced system halted." << std::endl;
    return 0;
}