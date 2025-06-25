# Simple Makefile for DSL Compiler Project

# Test directory
TEST_DIR = t/lexer

# Default target
test_all: 
	@echo "Building DSL compiler..."
	$(MAKE) -C $(TEST_DIR) 
	@echo "Build completed!"

# Run tests
test:
	@echo "Running tests..."
	$(MAKE) -C $(TEST_DIR) test
	@echo "Tests completed!"

# Clean
clean:
	@echo "Cleaning..."
	$(MAKE) -C $(TEST_DIR) clean
	rm -rf build bin
	@echo "Clean completed!"

# Help
help:
	@echo "Simple DSL Compiler Makefile:"
	@echo "  make build  - Build the project"
	@echo "  make test   - Run tests"
	@echo "  make clean  - Clean build files"
	@echo "  make help   - Show this help"

.PHONY: test_all build test clean help 