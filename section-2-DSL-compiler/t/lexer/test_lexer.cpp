#include "../../include/lexer.h"
#include <iostream>
#include <vector>
#include <string>
#include <cassert>

// Test case structure
struct TestCase {
    std::string name;
    std::string input;
    std::vector<std::pair<TokenType, std::string>> expected_tokens;
    bool should_throw;
    
    TestCase(const std::string& n, const std::string& i, 
             const std::vector<std::pair<TokenType, std::string>>& exp, bool throw_ex = false)
        : name(n), input(i), expected_tokens(exp), should_throw(throw_ex) {}
};

// Helper function to convert TokenType to string for output
std::string tokenTypeToString(TokenType type) {
    switch(type) {
        case TokenType::KW_LET: return "KW_LET";
        case TokenType::KW_IF: return "KW_IF";
        case TokenType::KW_GOTO: return "KW_GOTO";
        case TokenType::KW_OUT: return "KW_OUT";
        case TokenType::KW_HALT: return "KW_HALT";
        case TokenType::ID: return "ID";
        case TokenType::NUMBER: return "NUMBER";
        case TokenType::OP_PLUS: return "OP_PLUS";
        case TokenType::OP_MINUS: return "OP_MINUS";
        case TokenType::OP_LEQ: return "OP_LEQ";
        case TokenType::EQUAL: return "EQUAL";
        case TokenType::COLON: return "COLON";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::TOKEN_EOF: return "TOKEN_EOF";
        default: return "UNKNOWN";
    }
}

// Test function
bool runTest(const TestCase& test) {
    std::cout << "Running test: " << test.name << std::endl;
    
    try {
        Lexer lexer(test.input);
        std::vector<Token> tokens;
        
        // Collect all tokens
        while (true) {
            Token token = lexer.genNextToken();
            tokens.push_back(token);
            if (token.type == TokenType::TOKEN_EOF) break;
        }
        
        // Check if we expected an exception
        if (test.should_throw) {
            std::cout << "  FAILED: Expected exception but none was thrown" << std::endl;
            return false;
        }
        
        // Verify token count (excluding EOF)
        if (tokens.size() - 1 != test.expected_tokens.size()) {
            std::cout << "  FAILED: Expected " << test.expected_tokens.size() 
                      << " tokens, got " << (tokens.size() - 1) << std::endl;
            return false;
        }
        
        // Verify each token
        for (size_t i = 0; i < test.expected_tokens.size(); ++i) {
            if (tokens[i].type != test.expected_tokens[i].first ||
                tokens[i].value != test.expected_tokens[i].second) {
                std::cout << "  FAILED: Token " << i << " mismatch" << std::endl;
                std::cout << "    Expected: (" << tokenTypeToString(test.expected_tokens[i].first) 
                          << ", \"" << test.expected_tokens[i].second << "\")" << std::endl;
                std::cout << "    Got: (" << tokenTypeToString(tokens[i].type) 
                          << ", \"" << tokens[i].value << "\")" << std::endl;
                return false;
            }
        }
        
        std::cout << "  PASSED" << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        if (test.should_throw) {
            std::cout << "  PASSED (expected exception: " << e.what() << ")" << std::endl;
            return true;
        } else {
            std::cout << "  FAILED: Unexpected exception: " << e.what() << std::endl;
            return false;
        }
    }
}

int main() {
    std::vector<TestCase> tests = {
        // Basic keywords
        TestCase("Keywords", "let if goto out halt", {
            {TokenType::KW_LET, "let"},
            {TokenType::KW_IF, "if"},
            {TokenType::KW_GOTO, "goto"},
            {TokenType::KW_OUT, "out"},
            {TokenType::KW_HALT, "halt"}
        }),
        
        // Numbers
        TestCase("Numbers", "123 456 789", {
            {TokenType::NUMBER, "123"},
            {TokenType::NUMBER, "456"},
            {TokenType::NUMBER, "789"}
        }),
        
        // Identifiers
        TestCase("Identifiers", "abc _def x123", {
            {TokenType::ID, "abc"},
            {TokenType::ID, "_def"},
            {TokenType::ID, "x123"}
        }),
        
        // Operators
        TestCase("Operators", "+ - = : ; <=", {
            {TokenType::OP_PLUS, "+"},
            {TokenType::OP_MINUS, "-"},
            {TokenType::EQUAL, "="},
            {TokenType::COLON, ":"},
            {TokenType::SEMICOLON, ";"},
            {TokenType::OP_LEQ, "<="}
        }),
        
        // Assignment statement
        TestCase("Assignment", "let x = 42;", {
            {TokenType::KW_LET, "let"},
            {TokenType::ID, "x"},
            {TokenType::EQUAL, "="},
            {TokenType::NUMBER, "42"},
            {TokenType::SEMICOLON, ";"}
        }),
        
        // Comments
        TestCase("Comments", "let x = 5; // this is a comment\nout x;", {
            {TokenType::KW_LET, "let"},
            {TokenType::ID, "x"},
            {TokenType::EQUAL, "="},
            {TokenType::NUMBER, "5"},
            {TokenType::SEMICOLON, ";"},
            {TokenType::KW_OUT, "out"},
            {TokenType::ID, "x"},
            {TokenType::SEMICOLON, ";"}
        }),
        
        // Whitespace handling
        TestCase("Whitespace", "  let   x   =   10   ;  ", {
            {TokenType::KW_LET, "let"},
            {TokenType::ID, "x"},
            {TokenType::EQUAL, "="},
            {TokenType::NUMBER, "10"},
            {TokenType::SEMICOLON, ";"}
        }),
        
        // Complex expression
        TestCase("Complex Expression", "let result = a + b - 5;", {
            {TokenType::KW_LET, "let"},
            {TokenType::ID, "result"},
            {TokenType::EQUAL, "="},
            {TokenType::ID, "a"},
            {TokenType::OP_PLUS, "+"},
            {TokenType::ID, "b"},
            {TokenType::OP_MINUS, "-"},
            {TokenType::NUMBER, "5"},
            {TokenType::SEMICOLON, ";"}
        }),
        
        // Error case - invalid character
        TestCase("Invalid Character", "let x = @;", {}, true),
        
        // Empty input
        TestCase("Empty Input", "", {
            // Should only return EOF token
        }),
        
        // Only comments
        TestCase("Only Comments", "// This is a comment\n// Another comment", {
            // Should only return EOF token
        })
    };
    
    int passed = 0;
    int total = tests.size();
    
    std::cout << "=== Lexer Test Suite ===" << std::endl;
    std::cout << "Running " << total << " tests..." << std::endl << std::endl;
    
    for (const auto& test : tests) {
        if (runTest(test)) {
            passed++;
        }
        std::cout << std::endl;
    }
    
    std::cout << "=== Test Results ===" << std::endl;
    std::cout << "Passed: " << passed << "/" << total << std::endl;
    
    if (passed == total) {
        std::cout << "All tests passed! ✓" << std::endl;
        return 0;
    } else {
        std::cout << "Some tests failed! ✗" << std::endl;
        return 1;
    }
} 