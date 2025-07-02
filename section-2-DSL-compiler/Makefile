# Simple Makefile for DSL Compiler Project

# Test directories
LEXER_TEST_DIR = t/lexer
PARSER_TEST_DIR = t/parser
CODEGEN_TEST_DIR = t/codegen

BUILD_DIR = build

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g
INCLUDES = -Iinclude

# compile the compiler
compiler: $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(BUILD_DIR)/compiler src/compiler.cpp src/codegen.cpp src/parser.cpp src/lexer.cpp

# make the build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# test all targets
test: 
	@echo "Building and testing DSL compiler..."
	@echo "=== Testing Lexer ==="
	$(MAKE) -C $(LEXER_TEST_DIR) 
	@echo ""
	@echo "=== Testing Parser ==="
	$(MAKE) -C $(PARSER_TEST_DIR) test
	@echo ""
	@echo "=== Testing Codegen ==="
	$(MAKE) -C $(CODEGEN_TEST_DIR) test
	@echo "All tests completed!"

# Clean
clean:
	@echo "Cleaning..."
	$(MAKE) -C $(LEXER_TEST_DIR) clean
	$(MAKE) -C $(PARSER_TEST_DIR) clean
	$(MAKE) -C $(CODEGEN_TEST_DIR) clean
	rm -rf build bin
	@echo "Clean completed!"

# Help
help:
	@echo "Simple DSL Compiler Makefile:"
	@echo "  make test      - Build and run all tests (lexer + parser + codegen)"
	@echo "  make clean     - Clean build files"
	@echo "  make help      - Show this help"
	@echo "  make compiler  - Build the main compiler executable"

.PHONY: test clean help 