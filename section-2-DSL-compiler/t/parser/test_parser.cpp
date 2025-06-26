#include "../../include/parser.h"
#include "../../include/lexer.h"
#include "../../include/token.h"
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <sstream>
#include <functional>

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

// Helper function to capture output
std::string captureOutput(std::function<void()> func) {
    std::ostringstream oss;
    std::streambuf* original_cout = std::cout.rdbuf();
    std::cout.rdbuf(oss.rdbuf());
    
    func();
    
    std::cout.rdbuf(original_cout);
    return oss.str();
}

// Helper function to normalize whitespace for comparison
std::string normalizeWhitespace(const std::string& str) {
    std::string result;
    bool lastWasSpace = true;
    
    for (char c : str) {
        if (std::isspace(c)) {
            if (!lastWasSpace) {
                result += ' ';
                lastWasSpace = true;
            }
        } else {
            result += c;
            lastWasSpace = false;
        }
    }
    
    // Remove trailing space
    if (!result.empty() && result.back() == ' ') {
        result.pop_back();
    }
    
    return result;
}

// Helper function to compare IR output
bool compareIR(const std::string& program, const std::vector<std::string>& expectedIR) {
    try {
        Lexer lexer(program);
        Parser parser(lexer);
        parser.parseProgram();
        
        std::string actualOutput = captureOutput([&parser]() {
            parser.printIR();
        });
        
        // Remove debug output lines that start with [DEBUG]
        std::istringstream iss(actualOutput);
        std::string line;
        std::string cleanOutput;
        while (std::getline(iss, line)) {
            if (line.find("[DEBUG]") == std::string::npos) {
                cleanOutput += line + "\n";
            }
        }
        
        std::string normalizedActual = normalizeWhitespace(cleanOutput);
        
        // Build expected output
        std::string expectedOutput;
        for (const auto& ir : expectedIR) {
            expectedOutput += ir + "\n";
        }
        std::string normalizedExpected = normalizeWhitespace(expectedOutput);
        
        // Debug output for failed tests
        if (normalizedActual != normalizedExpected) {
            std::cout << "Expected: '" << normalizedExpected << "'" << std::endl;
            std::cout << "Actual:   '" << normalizedActual << "'" << std::endl;
        }
        
        return normalizedActual == normalizedExpected;
    } catch (const std::exception& e) {
        std::cout << "Exception during parsing: " << e.what() << std::endl;
        return false;
    }
}

// Test functions
bool testLetStatement() {
    std::string program = "let x = 5;";
    std::vector<std::string> expected = {
        "LOAD_CONST 5 -> __temp__0",
        "STORE __temp__0 -> x"
    };
    return compareIR(program, expected);
}

bool testLetStatementWithExpression() {
    std::string program = "let y = 3 + 7;";
    std::vector<std::string> expected = {
        "LOAD_CONST 3 -> __temp__0",
        "LOAD_CONST 7 -> __temp__1",
        "ADD __temp__0 __temp__1 -> __temp__2",
        "STORE __temp__2 -> y"
    };
    return compareIR(program, expected);
}

bool testAssignmentStatement() {
    std::string program = "x = 10;";
    std::vector<std::string> expected = {
        "LOAD_CONST 10 -> __temp__0",
        "STORE __temp__0 -> x"
    };
    return compareIR(program, expected);
}

bool testAssignmentWithExpression() {
    std::string program = "y = x + 5;";
    std::vector<std::string> expected = {
        "LOAD_VAR x -> __temp__0",
        "LOAD_CONST 5 -> __temp__1",
        "ADD __temp__0 __temp__1 -> __temp__2",
        "STORE __temp__2 -> y"
    };
    return compareIR(program, expected);
}

bool testOutStatement() {
    std::string program = "out x;";
    std::vector<std::string> expected = {
        "OUT x"
    };
    return compareIR(program, expected);
}

bool testIfLeqStatement() {
    std::string program = "if x <= y goto loop;";
    std::vector<std::string> expected = {
        "IFLEQ x y loop"
    };
    return compareIR(program, expected);
}

bool testGotoStatement() {
    std::string program = "goto end;";
    std::vector<std::string> expected = {
        "GOTO end"
    };
    return compareIR(program, expected);
}

bool testLabelStatement() {
    std::string program = "loop:";
    std::vector<std::string> expected = {
        "LABEL loop"
    };
    return compareIR(program, expected);
}

bool testHaltStatement() {
    std::string program = "halt;";
    std::vector<std::string> expected = {
        "HALT"
    };
    return compareIR(program, expected);
}

bool testComplexExpression() {
    std::string program = "let result = a + b - 3;";
    std::vector<std::string> expected = {
        "LOAD_VAR a -> __temp__0",
        "LOAD_VAR b -> __temp__1",
        "ADD __temp__0 __temp__1 -> __temp__2",
        "LOAD_CONST 3 -> __temp__3",
        "SUB __temp__2 __temp__3 -> __temp__4",
        "STORE __temp__4 -> result"
    };
    return compareIR(program, expected);
}

bool testMultipleStatements() {
    std::string program = R"(
        let x = 5;
        let y = 10;
        out x;
        if x <= y goto loop;
        loop:
        halt;
    )";
    std::vector<std::string> expected = {
        "LOAD_CONST 5 -> __temp__0",
        "STORE __temp__0 -> x",
        "LOAD_CONST 10 -> __temp__1",
        "STORE __temp__1 -> y",
        "OUT x",
        "IFLEQ x y loop",
        "LABEL loop",
        "HALT"
    };
    return compareIR(program, expected);
}

bool testSimpleLoop() {
    std::string program = R"(
        let counter = 0;
        loop:
        counter = counter + 1;
        if counter <= 5 goto loop;
        out counter;
        halt;
    )";
    std::vector<std::string> expected = {
        "LOAD_CONST 0 -> __temp__0",
        "STORE __temp__0 -> counter",
        "LABEL loop",
        "LOAD_VAR counter -> __temp__1",
        "LOAD_CONST 1 -> __temp__2",
        "ADD __temp__1 __temp__2 -> __temp__3",
        "STORE __temp__3 -> counter",
        "IFLEQ counter 5 loop",
        "OUT counter",
        "HALT"
    };
    return compareIR(program, expected);
}

bool testErrorHandling() {
    // Test that invalid syntax throws exceptions
    try {
        std::string program = "let x = ;"; // Missing expression
        Lexer lexer(program);
        Parser parser(lexer);
        parser.parseProgram();
        return false; // Should not reach here
    } catch (const std::exception&) {
        return true; // Expected exception
    }
}

bool testErrorHandling2() {
    try {
        std::string program = "out ;"; // Missing identifier
        Lexer lexer(program);
        Parser parser(lexer);
        parser.parseProgram();
        return false; // Should not reach here
    } catch (const std::exception&) {
        return true; // Expected exception
    }
}

bool testIRSize() {
    std::string program = "let x = 5; let y = 10;";
    Lexer lexer(program);
    Parser parser(lexer);
    parser.parseProgram();
    return parser.getIRSize() == 4; // 2 LOAD_CONST + 2 STORE instructions
}

int main() {
    TestFramework testFramework;
    
    std::cout << "=== PARSER TESTS ===" << std::endl << std::endl;
    
    // Basic statement tests
    testFramework.runTest("Let Statement", testLetStatement);
    testFramework.runTest("Let Statement with Expression", testLetStatementWithExpression);
    testFramework.runTest("Assignment Statement", testAssignmentStatement);
    testFramework.runTest("Assignment with Expression", testAssignmentWithExpression);
    testFramework.runTest("Out Statement", testOutStatement);
    testFramework.runTest("If-Leq Statement", testIfLeqStatement);
    testFramework.runTest("Goto Statement", testGotoStatement);
    testFramework.runTest("Label Statement", testLabelStatement);
    testFramework.runTest("Halt Statement", testHaltStatement);
    
    // Complex tests
    testFramework.runTest("Complex Expression", testComplexExpression);
    testFramework.runTest("Multiple Statements", testMultipleStatements);
    testFramework.runTest("Simple Loop", testSimpleLoop);
    
    // Error handling tests
    testFramework.runTest("Error Handling - Missing Expression", testErrorHandling);
    testFramework.runTest("Error Handling - Missing Identifier", testErrorHandling2);
    
    // Utility tests
    testFramework.runTest("IR Size Check", testIRSize);
    
    testFramework.printSummary();
    
    return testFramework.getExitCode();
} 