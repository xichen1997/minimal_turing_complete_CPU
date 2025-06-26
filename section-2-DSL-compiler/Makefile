# Simple Makefile for DSL Compiler Project

# Test directories
LEXER_TEST_DIR = t/lexer
PARSER_TEST_DIR = t/parser

# Default target
test_all: 
	@echo "Building and testing DSL compiler..."
	@echo "=== Testing Lexer ==="
	$(MAKE) -C $(LEXER_TEST_DIR) 
	@echo ""
	@echo "=== Testing Parser ==="
	$(MAKE) -C $(PARSER_TEST_DIR) test
	@echo "All tests completed!"

# Run tests
test: test_all

# Clean
clean:
	@echo "Cleaning..."
	$(MAKE) -C $(LEXER_TEST_DIR) clean
	$(MAKE) -C $(PARSER_TEST_DIR) clean
	rm -rf build bin
	@echo "Clean completed!"

# Help
help:
	@echo "Simple DSL Compiler Makefile:"
	@echo "  make test_all  - Build and run all tests (lexer + parser)"
	@echo "  make test      - Run all tests"
	@echo "  make clean     - Clean build files"
	@echo "  make help      - Show this help"

.PHONY: test_all test clean help 