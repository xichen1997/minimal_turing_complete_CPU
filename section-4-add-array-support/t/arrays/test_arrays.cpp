#include "../../include/parser.h"
#include "../../include/lexer.h"
#include "../../include/token.h"
#include "../../include/interpreter.h"
#include "../../include/codegen.h"
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <sstream>
#include <functional>
#include <fstream>

// Test framework utilities
class TestFramework {
private:
    int testsRun = 0;
    int testsPassed = 0;
    int testsFailed = 0;
    
public:
    void runTest(const std::string& testName, bool (*testFunc)()) {
        std::cout << "Running test: " << testName << std::endl;
        testsRun++;
        
        try {
            bool result = testFunc();
            if (result) {
                std::cout << "✓ PASSED: " << testName << std::endl;
                testsPassed++;
            } else {
                std::cout << "✗ FAILED: " << testName << std::endl;
                testsFailed++;
            }
        } catch (const std::exception& e) {
            std::cout << "✗ FAILED: " << testName << " (Exception: " << e.what() << ")" << std::endl;
            testsFailed++;
        }
        std::cout << std::endl;
    }
    
    void printSummary() {
        std::cout << "=== Test Summary ===" << std::endl;
        std::cout << "Tests run: " << testsRun << std::endl;
        std::cout << "Passed: " << testsPassed << std::endl;
        std::cout << "Failed: " << testsFailed << std::endl;
        if (testsFailed == 0) {
            std::cout << "🎉 All tests passed!" << std::endl;
        }
    }
    
    int getFailedCount() const { return testsFailed; }
};

// Test helper functions
std::vector<IR> parseAndGetIR(const std::string& code) {
    Lexer lexer(code);
    Parser parser(lexer);
    parser.parseProgram();
    return parser.getIR();
}

bool hasOpCode(const std::vector<IR>& ir, OpCode op) {
    for (const auto& instruction : ir) {
        if (instruction.op == op) return true;
    }
    return false;
}

int countOpCode(const std::vector<IR>& ir, OpCode op) {
    int count = 0;
    for (const auto& instruction : ir) {
        if (instruction.op == op) count++;
    }
    return count;
}

// Test functions
bool test_array_declaration_parsing() {
    std::string code = "let arr[5];";
    auto ir = parseAndGetIR(code);
    
    return ir.size() == 1 && 
           ir[0].op == OpCode::ARRAY_DECL &&
           ir[0].arg1 == "arr" &&
           ir[0].arg2 == "5";
}

bool test_array_assignment_parsing() {
    std::string code = "let arr[3]; arr[0] = 42;";
    auto ir = parseAndGetIR(code);
    
    return ir.size() >= 3 &&
           hasOpCode(ir, OpCode::ARRAY_DECL) &&
           hasOpCode(ir, OpCode::STORE_INDEXED);
}

bool test_array_access_parsing() {
    std::string code = "let arr[3]; arr[0] = 10; let x = arr[0];";
    auto ir = parseAndGetIR(code);
    
    return hasOpCode(ir, OpCode::ARRAY_DECL) &&
           hasOpCode(ir, OpCode::STORE_INDEXED) &&
           hasOpCode(ir, OpCode::LOAD_INDEXED);
}

bool test_array_output_parsing() {
    std::string code = "let arr[3]; arr[1] = 99; out arr[1];";
    auto ir = parseAndGetIR(code);
    
    return hasOpCode(ir, OpCode::ARRAY_DECL) &&
           hasOpCode(ir, OpCode::STORE_INDEXED) &&
           hasOpCode(ir, OpCode::LOAD_INDEXED) &&
           hasOpCode(ir, OpCode::OUT);
}

bool test_array_with_variables() {
    std::string code = "let arr[5]; let i = 2; arr[i] = 100; out arr[i];";
    auto ir = parseAndGetIR(code);
    
    return hasOpCode(ir, OpCode::ARRAY_DECL) &&
           hasOpCode(ir, OpCode::STORE_INDEXED) &&
           hasOpCode(ir, OpCode::LOAD_INDEXED) &&
           countOpCode(ir, OpCode::STORE_CONST) >= 2; // i = 2, value = 100
}

bool test_array_arithmetic() {
    std::string code = "let arr[3]; arr[0] = 10; arr[1] = 20; arr[2] = arr[0] + arr[1];";
    auto ir = parseAndGetIR(code);
    
    return hasOpCode(ir, OpCode::ARRAY_DECL) &&
           countOpCode(ir, OpCode::STORE_INDEXED) >= 3 &&
           countOpCode(ir, OpCode::LOAD_INDEXED) >= 2 &&
           hasOpCode(ir, OpCode::ADD);
}

bool test_interpreter_array_basic() {
    std::string code = "let arr[3]; arr[0] = 42; out arr[0];";
    Lexer lexer(code);
    Parser parser(lexer);
    parser.parseProgram();
    auto ir = parser.getIR();
    
    // Capture output
    std::ostringstream output;
    std::streambuf* old_cout = std::cout.rdbuf(output.rdbuf());
    
    IRInterpreter interpreter;
    interpreter.execute(ir);
    
    std::cout.rdbuf(old_cout);
    
    return output.str() == "42\n";
}

bool test_interpreter_array_multiple_elements() {
    std::string code = "let arr[3]; arr[0] = 10; arr[1] = 20; arr[2] = 30; out arr[0]; out arr[1]; out arr[2];";
    Lexer lexer(code);
    Parser parser(lexer);
    parser.parseProgram();
    auto ir = parser.getIR();
    
    std::ostringstream output;
    std::streambuf* old_cout = std::cout.rdbuf(output.rdbuf());
    
    IRInterpreter interpreter;
    interpreter.execute(ir);
    
    std::cout.rdbuf(old_cout);
    
    return output.str() == "10\n20\n30\n";
}

bool test_interpreter_array_arithmetic() {
    std::string code = "let arr[3]; arr[0] = 5; arr[1] = 7; arr[2] = arr[0] + arr[1]; out arr[2];";
    Lexer lexer(code);
    Parser parser(lexer);
    parser.parseProgram();
    auto ir = parser.getIR();
    
    std::ostringstream output;
    std::streambuf* old_cout = std::cout.rdbuf(output.rdbuf());
    
    IRInterpreter interpreter;
    interpreter.execute(ir);
    
    std::cout.rdbuf(old_cout);
    
    return output.str() == "12\n";
}

bool test_interpreter_array_variable_index() {
    std::string code = "let arr[5]; let i = 2; arr[i] = 99; out arr[2];";
    Lexer lexer(code);
    Parser parser(lexer);
    parser.parseProgram();
    auto ir = parser.getIR();
    
    std::ostringstream output;
    std::streambuf* old_cout = std::cout.rdbuf(output.rdbuf());
    
    IRInterpreter interpreter;
    interpreter.execute(ir);
    
    std::cout.rdbuf(old_cout);
    
    return output.str() == "99\n";
}

bool test_codegen_array_basic() {
    // Create a test DSL file
    std::ofstream testFile("test_codegen_array.dsl");
    testFile << "let arr[3];\narr[0] = 42;\nout arr[0];\nhalt;\n";
    testFile.close();
    
    try {
        Codegen gen("test_codegen_array.dsl");
        auto code = gen.getCode();
        
        // Clean up
        std::remove("test_codegen_array.dsl");
        std::remove("output.asm");
        std::remove("output.bin");
        std::remove("output.hex");
        
        // Check that code was generated and contains expected opcodes
        bool hasLoadIndexed = false;
        bool hasStoreIndexed = false;
        
        for (size_t i = 0; i < code.size(); i++) {
            if (code[i] == 0x0A) hasLoadIndexed = true;
            if (code[i] == 0x0B) hasStoreIndexed = true;
        }
        
        return code.size() > 0 && hasLoadIndexed && hasStoreIndexed;
    } catch (const std::exception& e) {
        std::remove("test_codegen_array.dsl");
        return false;
    }
}

bool test_codegen_no_spurious_halt() {
    // Create a test DSL file
    std::ofstream testFile("test_codegen_halt.dsl");
    testFile << "let arr[2];\narr[0] = 1;\nout arr[0];\nhalt;\n";
    testFile.close();
    
    try {
        Codegen gen("test_codegen_halt.dsl");
        auto code = gen.getCode();
        
        // Clean up
        std::remove("test_codegen_halt.dsl");
        std::remove("output.asm");
        std::remove("output.bin");
        std::remove("output.hex");
        
        // Count HALT instructions (0x00) in the middle of the code
        // Ignore potential padding or final HALT
        int spuriousHaltCount = 0;
        
        // Look for HALT instructions that appear in suspicious patterns
        // The issue was HALT appearing where we expected array instruction operands
        for (size_t i = 0; i < code.size() - 1; i++) {
            if (code[i] == 0x00) {
                // Check if this is a spurious HALT by looking at context
                // A spurious HALT typically appears after incomplete LOAD_CONST instructions
                if (i >= 3 && code[i-3] == 0x02 && code[i-2] == 0x00 && code[i-1] == 0x80) {
                    spuriousHaltCount++;
                }
            }
        }
        
        // Should have no spurious HALTs in the middle of array operations
        return spuriousHaltCount == 0 && code.size() > 0;
    } catch (const std::exception& e) {
        std::remove("test_codegen_halt.dsl");
        return false;
    }
}

// Lexer tests for bracket tokens
bool test_lexer_bracket_tokens() {
    std::string code = "arr[0]";
    Lexer lexer(code);
    
    auto tok1 = lexer.genNextToken(); // arr
    auto tok2 = lexer.genNextToken(); // [
    auto tok3 = lexer.genNextToken(); // 0
    auto tok4 = lexer.genNextToken(); // ]
    
    return tok1.type == TokenType::ID && tok1.value == "arr" &&
           tok2.type == TokenType::OP_LBRACKET && tok2.value == "[" &&
           tok3.type == TokenType::NUMBER && tok3.value == "0" &&
           tok4.type == TokenType::OP_RBRACKET && tok4.value == "]";
}

bool test_lexer_array_declaration() {
    std::string code = "let buffer[32];";
    Lexer lexer(code);
    
    auto tok1 = lexer.genNextToken(); // let
    auto tok2 = lexer.genNextToken(); // buffer
    auto tok3 = lexer.genNextToken(); // [
    auto tok4 = lexer.genNextToken(); // 32
    auto tok5 = lexer.genNextToken(); // ]
    auto tok6 = lexer.genNextToken(); // ;
    
    return tok1.type == TokenType::KW_LET &&
           tok2.type == TokenType::ID && tok2.value == "buffer" &&
           tok3.type == TokenType::OP_LBRACKET &&
           tok4.type == TokenType::NUMBER && tok4.value == "32" &&
           tok5.type == TokenType::OP_RBRACKET &&
           tok6.type == TokenType::SEMICOLON;
}

// Error handling tests
bool test_array_bounds_error() {
    std::string code = "let arr[2]; arr[5] = 10; out arr[5];";
    Lexer lexer(code);
    Parser parser(lexer);
    parser.parseProgram();
    auto ir = parser.getIR();
    
    IRInterpreter interpreter;
    try {
        interpreter.execute(ir);
        return false; // Should have thrown an exception
    } catch (const std::runtime_error& e) {
        std::string error = e.what();
        return error.find("bounds") != std::string::npos;
    }
}

int main() {
    TestFramework framework;
    
    std::cout << "🧪 Array Support Test Suite" << std::endl;
    std::cout << "============================" << std::endl << std::endl;
    
    // Lexer tests
    std::cout << "📝 Lexer Tests:" << std::endl;
    framework.runTest("Bracket Token Recognition", test_lexer_bracket_tokens);
    framework.runTest("Array Declaration Tokenization", test_lexer_array_declaration);
    
    // Parser tests  
    std::cout << "🔍 Parser Tests:" << std::endl;
    framework.runTest("Array Declaration Parsing", test_array_declaration_parsing);
    framework.runTest("Array Assignment Parsing", test_array_assignment_parsing);
    framework.runTest("Array Access Parsing", test_array_access_parsing);
    framework.runTest("Array Output Parsing", test_array_output_parsing);
    framework.runTest("Array with Variables", test_array_with_variables);
    framework.runTest("Array Arithmetic", test_array_arithmetic);
    
    // Interpreter tests
    std::cout << "⚙️  Interpreter Tests:" << std::endl;
    framework.runTest("Basic Array Operations", test_interpreter_array_basic);
    framework.runTest("Multiple Array Elements", test_interpreter_array_multiple_elements);
    framework.runTest("Array Arithmetic", test_interpreter_array_arithmetic);
    framework.runTest("Variable Index Access", test_interpreter_array_variable_index);
    
    // Codegen tests
    std::cout << "🔧 Code Generation Tests:" << std::endl;
    framework.runTest("Basic Array Codegen", test_codegen_array_basic);
    framework.runTest("No Spurious HALT Instructions", test_codegen_no_spurious_halt);
    
    // Error handling tests
    std::cout << "❌ Error Handling Tests:" << std::endl;
    framework.runTest("Array Bounds Checking", test_array_bounds_error);
    
    framework.printSummary();
    return framework.getFailedCount();
}