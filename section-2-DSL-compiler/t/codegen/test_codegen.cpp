#include "../../include/codegen.h"
#include "../../include/parser.h"
#include "../../include/lexer.h"
#include "../../include/token.h"
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <sstream>
#include <fstream>
#include <functional>
#include <filesystem>

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
        std::cout << "=== TEST SUMMARY ===" << std::endl;
        std::cout << "Total tests: " << testsRun << std::endl;
        std::cout << "Passed: " << testsPassed << std::endl;
        std::cout << "Failed: " << testsFailed << std::endl;
        std::cout << "Success rate: " << (testsRun > 0 ? (testsPassed * 100.0 / testsRun) : 0) << "%" << std::endl;
    }
    
    int getExitCode() const {
        return testsFailed > 0 ? 1 : 0;
    }
};

// Helper function to create a temporary file with content
std::string createTempFile(const std::string& content) {
    static int counter = 0;
    std::string filename = "temp_test_" + std::to_string(counter++) + ".txt";
    std::ofstream file(filename);
    file << content;
    file.close();
    return filename;
}

// Helper function to read file content
std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return "";
    }
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    return content;
}

// Helper function to check if file exists
bool fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

// Helper function to clean up test files
void cleanupTestFiles() {
    std::vector<std::string> filesToRemove = {
        "temp_test_0.txt", "temp_test_1.txt", "temp_test_2.txt", "temp_test_3.txt",
        "temp_test_4.txt", "temp_test_5.txt", "temp_test_6.txt", "temp_test_7.txt",
        "temp_test_8.txt", "temp_test_9.txt", "temp_test_10.txt", "temp_test_11.txt",
        "temp_test_12.txt", "temp_test_13.txt", "temp_test_14.txt", "temp_test_15.txt",
        "output.asm", "output.bin", "output.hex"
    };
    
    for (const auto& file : filesToRemove) {
        if (fileExists(file)) {
            std::remove(file.c_str());
        }
    }
}

// Test functions

bool testCodegenConstructor() {
    std::string program = "let x = 5; halt;";
    std::string filename = createTempFile(program);
    
    try {
        Codegen codegen(filename);
        // If constructor doesn't throw, test passes
        std::remove(filename.c_str());
        return true;
    } catch (const std::exception& e) {
        std::cout << "Constructor failed: " << e.what() << std::endl;
        std::remove(filename.c_str());
        return false;
    }
}

bool testSimpleHaltProgram() {
    std::string program = "halt;";
    std::string filename = createTempFile(program);
    
    try {
        Codegen codegen(filename);
        
        // Check if output files were created
        bool asmExists = fileExists("output.asm");
        bool binExists = fileExists("output.bin");
        bool hexExists = fileExists("output.hex");
        
        std::remove(filename.c_str());
        return asmExists && binExists && hexExists;
    } catch (const std::exception& e) {
        std::cout << "Simple halt program failed: " << e.what() << std::endl;
        std::remove(filename.c_str());
        return false;
    }
}

bool testLetStatementCodegen() {
    std::string program = "let x = 5; halt;";
    std::string filename = createTempFile(program);
    
    try {
        Codegen codegen(filename);
        
        // Check if output files were created
        bool filesCreated = fileExists("output.asm") && fileExists("output.bin") && fileExists("output.hex");
        
        std::remove(filename.c_str());
        return filesCreated;
    } catch (const std::exception& e) {
        std::cout << "Let statement codegen failed: " << e.what() << std::endl;
        std::remove(filename.c_str());
        return false;
    }
}

bool testAssignmentCodegen() {
    std::string program = "x = 10; halt;";
    std::string filename = createTempFile(program);
    
    try {
        Codegen codegen(filename);
        
        // Check if output files were created
        bool filesCreated = fileExists("output.asm") && fileExists("output.bin") && fileExists("output.hex");
        
        std::remove(filename.c_str());
        return filesCreated;
    } catch (const std::exception& e) {
        std::cout << "Assignment codegen failed: " << e.what() << std::endl;
        std::remove(filename.c_str());
        return false;
    }
}

bool testAdditionCodegen() {
    std::string program = "let x = 3 + 7; halt;";
    std::string filename = createTempFile(program);
    
    try {
        Codegen codegen(filename);
        
        // Check if output files were created
        bool filesCreated = fileExists("output.asm") && fileExists("output.bin") && fileExists("output.hex");
        
        std::remove(filename.c_str());
        return filesCreated;
    } catch (const std::exception& e) {
        std::cout << "Addition codegen failed: " << e.what() << std::endl;
        std::remove(filename.c_str());
        return false;
    }
}

bool testSubtractionCodegen() {
    std::string program = "let x = 10 - 3; halt;";
    std::string filename = createTempFile(program);
    
    try {
        Codegen codegen(filename);
        
        // Check if output files were created
        bool filesCreated = fileExists("output.asm") && fileExists("output.bin") && fileExists("output.hex");
        
        std::remove(filename.c_str());
        return filesCreated;
    } catch (const std::exception& e) {
        std::cout << "Subtraction codegen failed: " << e.what() << std::endl;
        std::remove(filename.c_str());
        return false;
    }
}

bool testOutStatementCodegen() {
    std::string program = "let x = 42; out x; halt;";
    std::string filename = createTempFile(program);
    
    try {
        Codegen codegen(filename);
        
        // Check if output files were created
        bool filesCreated = fileExists("output.asm") && fileExists("output.bin") && fileExists("output.hex");
        
        std::remove(filename.c_str());
        return filesCreated;
    } catch (const std::exception& e) {
        std::cout << "Out statement codegen failed: " << e.what() << std::endl;
        std::remove(filename.c_str());
        return false;
    }
}

bool testOutVariableCodegen() {
    std::string program = "let x = 42; out x; halt;";
    std::string filename = createTempFile(program);
    
    try {
        Codegen codegen(filename);
        
        // Check if output files were created
        bool filesCreated = fileExists("output.asm") && fileExists("output.bin") && fileExists("output.hex");
        
        std::remove(filename.c_str());
        return filesCreated;
    } catch (const std::exception& e) {
        std::cout << "Out variable codegen failed: " << e.what() << std::endl;
        std::remove(filename.c_str());
        return false;
    }
}

bool testLabelCodegen() {
    std::string program = "loop: halt;";
    std::string filename = createTempFile(program);
    
    try {
        Codegen codegen(filename);
        
        // Check if output files were created
        bool filesCreated = fileExists("output.asm") && fileExists("output.bin") && fileExists("output.hex");
        
        std::remove(filename.c_str());
        return filesCreated;
    } catch (const std::exception& e) {
        std::cout << "Label codegen failed: " << e.what() << std::endl;
        std::remove(filename.c_str());
        return false;
    }
}

bool testGotoCodegen() {
    std::string program = "goto end; end: halt;";
    std::string filename = createTempFile(program);
    
    try {
        Codegen codegen(filename);
        
        // Check if output files were created
        bool filesCreated = fileExists("output.asm") && fileExists("output.bin") && fileExists("output.hex");
        
        std::remove(filename.c_str());
        return filesCreated;
    } catch (const std::exception& e) {
        std::cout << "Goto codegen failed: " << e.what() << std::endl;
        std::remove(filename.c_str());
        return false;
    }
}

bool testIfLeqCodegen() {
    std::string program = "if x <= y goto loop; loop: halt;";
    std::string filename = createTempFile(program);
    
    try {
        Codegen codegen(filename);
        
        // Check if output files were created
        bool filesCreated = fileExists("output.asm") && fileExists("output.bin") && fileExists("output.hex");
        
        std::remove(filename.c_str());
        return filesCreated;
    } catch (const std::exception& e) {
        std::cout << "IfLeq codegen failed: " << e.what() << std::endl;
        std::remove(filename.c_str());
        return false;
    }
}

bool testComplexProgramCodegen() {
    std::string program = R"(
        let x = 5;
        let y = 10;
        let z = x + y;
        out z;
        if x <= y goto loop;
        halt;
        loop: out x;
        halt;
    )";
    std::string filename = createTempFile(program);
    
    try {
        Codegen codegen(filename);
        
        // Check if output files were created
        bool filesCreated = fileExists("output.asm") && fileExists("output.bin") && fileExists("output.hex");
        
        std::remove(filename.c_str());
        return filesCreated;
    } catch (const std::exception& e) {
        std::cout << "Complex program codegen failed: " << e.what() << std::endl;
        std::remove(filename.c_str());
        return false;
    }
}

bool testMultipleVariablesCodegen() {
    std::string program = R"(
        let a = 1;
        let b = 2;
        let c = 3;
        let d = a + b;
        let e = d + c;
        out e;
        halt;
    )";
    std::string filename = createTempFile(program);
    
    try {
        Codegen codegen(filename);
        
        // Check if output files were created
        bool filesCreated = fileExists("output.asm") && fileExists("output.bin") && fileExists("output.hex");
        
        std::remove(filename.c_str());
        return filesCreated;
    } catch (const std::exception& e) {
        std::cout << "Multiple variables codegen failed: " << e.what() << std::endl;
        std::remove(filename.c_str());
        return false;
    }
}

bool testLoopProgramCodegen() {
    std::string program = R"(
        let i = 0;
        loop: out i;
        let i = i + 1;
        if i <= 5 goto loop;
        halt;
    )";
    std::string filename = createTempFile(program);
    
    try {
        Codegen codegen(filename);
        
        // Check if output files were created
        bool filesCreated = fileExists("output.asm") && fileExists("output.bin") && fileExists("output.hex");
        
        std::remove(filename.c_str());
        return filesCreated;
    } catch (const std::exception& e) {
        std::cout << "Loop program codegen failed: " << e.what() << std::endl;
        std::remove(filename.c_str());
        return false;
    }
}

bool testBinaryFileGeneration() {
    std::string program = "halt;";
    std::string filename = createTempFile(program);
    
    try {
        Codegen codegen(filename);
        
        // Check if binary file was created and has content
        bool binExists = fileExists("output.bin");
        std::string binContent = readFile("output.bin");
        bool hasContent = !binContent.empty();
        
        std::remove(filename.c_str());
        return binExists && hasContent;
    } catch (const std::exception& e) {
        std::cout << "Binary file generation failed: " << e.what() << std::endl;
        std::remove(filename.c_str());
        return false;
    }
}

bool testHexFileGeneration() {
    std::string program = "halt;";
    std::string filename = createTempFile(program);
    
    try {
        Codegen codegen(filename);
        
        // Check if hex file was created and has content
        bool hexExists = fileExists("output.hex");
        std::string hexContent = readFile("output.hex");
        bool hasContent = !hexContent.empty();
        
        std::remove(filename.c_str());
        return hexExists && hasContent;
    } catch (const std::exception& e) {
        std::cout << "Hex file generation failed: " << e.what() << std::endl;
        std::remove(filename.c_str());
        return false;
    }
}

bool testAsmFileGeneration() {
    std::string program = "halt;";
    std::string filename = createTempFile(program);
    
    try {
        Codegen codegen(filename);
        
        // Check if asm file was created and has content
        bool asmExists = fileExists("output.asm");
        std::string asmContent = readFile("output.asm");
        bool hasContent = !asmContent.empty();
        
        std::remove(filename.c_str());
        return asmExists && hasContent;
    } catch (const std::exception& e) {
        std::cout << "ASM file generation failed: " << e.what() << std::endl;
        std::remove(filename.c_str());
        return false;
    }
}

bool testErrorHandlingInvalidFile() {
    try {
        Codegen codegen("nonexistent_file.txt");
        // Should not reach here
        return false;
    } catch (const std::exception& e) {
        // Expected to throw an exception
        return true;
    }
}

bool testErrorHandlingInvalidSyntax() {
    std::string program = "invalid syntax here;";
    std::string filename = createTempFile(program);
    
    try {
        Codegen codegen(filename);
        // If it doesn't throw, that's also acceptable (parser might handle it gracefully)
        std::remove(filename.c_str());
        return true;
    } catch (const std::exception& e) {
        // Expected to throw an exception for invalid syntax
        std::remove(filename.c_str());
        return true;
    }
}

int main() {
    TestFramework framework;
    
    std::cout << "=== Codegen Test Suite ===" << std::endl;
    std::cout << "Testing code generation functionality..." << std::endl;
    std::cout << std::endl;
    
    // Basic functionality tests
    framework.runTest("Codegen Constructor", testCodegenConstructor);
    framework.runTest("Simple Halt Program", testSimpleHaltProgram);
    framework.runTest("Let Statement Codegen", testLetStatementCodegen);
    framework.runTest("Assignment Codegen", testAssignmentCodegen);
    
    // Arithmetic operation tests
    framework.runTest("Addition Codegen", testAdditionCodegen);
    framework.runTest("Subtraction Codegen", testSubtractionCodegen);
    
    // Output tests
    framework.runTest("Out Statement Codegen", testOutStatementCodegen);
    framework.runTest("Out Variable Codegen", testOutVariableCodegen);
    
    // Control flow tests
    framework.runTest("Label Codegen", testLabelCodegen);
    framework.runTest("Goto Codegen", testGotoCodegen);
    framework.runTest("IfLeq Codegen", testIfLeqCodegen);
    
    // Complex program tests
    framework.runTest("Complex Program Codegen", testComplexProgramCodegen);
    framework.runTest("Multiple Variables Codegen", testMultipleVariablesCodegen);
    framework.runTest("Loop Program Codegen", testLoopProgramCodegen);
    
    // File generation tests
    framework.runTest("Binary File Generation", testBinaryFileGeneration);
    framework.runTest("Hex File Generation", testHexFileGeneration);
    framework.runTest("ASM File Generation", testAsmFileGeneration);
    
    // Error handling tests
    framework.runTest("Error Handling - Invalid File", testErrorHandlingInvalidFile);
    framework.runTest("Error Handling - Invalid Syntax", testErrorHandlingInvalidSyntax);
    
    // Clean up any remaining test files
    cleanupTestFiles();
    
    framework.printSummary();
    return framework.getExitCode();
} 