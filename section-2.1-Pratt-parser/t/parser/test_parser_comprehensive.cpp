#include "../../include/parser.h"
#include "../../include/lexer.h"
#include <iostream>
#include <string>
#include <vector>

struct TestCase {
    std::string name;
    std::string expression;
    std::string description;
};

void testExpression(const TestCase& test) {
    std::cout << "\n=== " << test.name << " ===" << std::endl;
    std::cout << "Expression: " << test.expression << std::endl;
    std::cout << "Purpose: " << test.description << std::endl;
    
    try {
        Lexer lexer(test.expression);
        Parser parser(lexer);
        parser.parseProgram();
        
        std::cout << "Generated IR:" << std::endl;
        parser.printIR();
        std::cout << "IR Instructions: " << parser.getIRSize() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}

int main() {
    std::cout << "=== COMPREHENSIVE PRATT PARSER TESTING ===" << std::endl;
    
    std::vector<TestCase> tests = {
        // Basic Expression Tests
        {"Basic Addition", "let x = 5 + 3;", "Simple binary operation"},
        {"Basic Subtraction", "let x = 10 - 7;", "Simple subtraction"},
        
        // Left Associativity Tests
        {"Left Associative Add", "let x = 1 + 2 + 3 + 4;", "Multiple same-precedence operators (left-to-right)"},
        {"Left Associative Sub", "let x = 20 - 5 - 3 - 2;", "Multiple subtractions (left-to-right)"},
        {"Mixed Same Precedence", "let x = 10 + 5 - 3 + 2 - 1;", "Mixed addition/subtraction"},
        
        // Parentheses Grouping Tests
        {"Simple Grouping", "let x = (5 + 3) - 2;", "Basic parentheses override"},
        {"Right Grouping", "let x = 5 + (3 - 2);", "Right-side parentheses"},
        {"Nested Grouping", "let x = ((1 + 2) + 3) + 4;", "Nested parentheses (left)"},
        {"Complex Nested", "let x = (1 + (2 + 3)) + 4;", "Nested parentheses (right)"},
        {"Deep Nesting", "let x = (((a + b) - c) + d) - e;", "Deep nested parentheses"},
        
        // Variable and Constant Mix
        {"Var-Const Mix", "let result = var1 + 10 - var2 + 5;", "Variables and constants mixed"},
        {"All Variables", "let result = a + b - c + d - e + f;", "All variable expression"},
        {"All Constants", "let result = 1 + 2 - 3 + 4 - 5 + 6;", "All constant expression"},
        
        // Complex Expression Patterns
        {"Alternating Ops", "let x = a - b + c - d + e - f;", "Alternating operators"},
        {"Grouped Alternating", "let x = (a - b) + (c - d) + (e - f);", "Grouped alternating operations"},
        {"Mixed Grouping", "let x = a + (b - c) + d - (e + f);", "Mixed grouping patterns"},
        
        // Edge Case Tests
        {"Single Variable", "let x = a;", "Single variable assignment"},
        {"Single Constant", "let x = 42;", "Single constant assignment"},
        {"Parentheses Only", "let x = (((5)));", "Multiple nested parentheses around single value"},
        
        // Long Expression Chain
        {"Long Chain", "let result = a + b + c + d + e + f + g + h + i + j;", "Long chain of additions"},
        {"Long Alternating", "let result = a - b + c - d + e - f + g - h + i - j;", "Long alternating operations"},
        
        // Complex Realistic Expressions
        {"Formula-like", "let area = length + width - margin + padding;", "Realistic formula"},
        {"Calculation", "let total = base + (extra1 - discount) + (extra2 - tax);", "Complex calculation"},
        {"Nested Formula", "let result = ((start + increment) - overhead) + ((bonus - penalty) + adjustment);", "Complex nested formula"},
        
        // Stress Tests
        {"Deep Left", "let x = ((((a + b) + c) + d) + e) + f;", "Deep left-associative nesting"},
        {"Deep Right", "let x = a + (b + (c + (d + (e + f))));", "Deep right-associative nesting"},
        {"Balanced Tree", "let x = (a + b) + (c + d) + (e + f) + (g + h);", "Balanced expression tree"},
        
        // Multiple Statement Tests
        {"Multi Statement", 
         "let x = a + b; let y = x - c; let z = (x + y) - d;", 
         "Multiple interdependent statements"},
    };
    
    for (const auto& test : tests) {
        testExpression(test);
    }
    
    std::cout << "\n=== TESTING COMPLETE ===" << std::endl;
    std::cout << "Total test cases: " << tests.size() << std::endl;
    
    return 0;
}