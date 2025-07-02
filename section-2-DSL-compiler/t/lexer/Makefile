# Makefile for Lexer Testing
# Author: Generated for DSL Compiler Project

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g
INCLUDES = -I../../include

# Directories
SRC_DIR = ../../src
TEST_DIR = .
BUILD_DIR = build

# Source files

# Executables
LEXER_TEST = $(BUILD_DIR)/test_lexer


# Default target
all: test

# Build the comprehensive test suite
$(LEXER_TEST): $(SRC_DIR)/lexer.cpp $(TEST_DIR)/test_lexer.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SRC_DIR)/lexer.cpp $(TEST_DIR)/test_lexer.cpp -o $(LEXER_TEST)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Run the comprehensive test suite
test: $(LEXER_TEST)
	@echo "Running comprehensive lexer test suite..."
	@$(LEXER_TEST)
	@echo ""


# Run both tests
check: test 


# Build everything
build: $(LEXER_TEST) 

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)
	rm -f *.o

# Show help
help:
	@echo "Available targets:"
	@echo "  all           - Run comprehensive tests (default)"
	@echo "  test          - Run the comprehensive test suite"
	@echo "  check         - Run test"
	@echo "  build         - Build all executables"
	@echo "  clean         - Remove build artifacts"
	@echo "  help          - Show this help message"
	@echo ""
	@echo "Test files:"

# Phony targets
.PHONY: all test check build clean help 