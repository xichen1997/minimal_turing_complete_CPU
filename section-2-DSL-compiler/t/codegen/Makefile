# Makefile for Codegen Testing
# Author: Generated for DSL Compiler Project

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g -DDEBUG
INCLUDES = -I../../include

# Directories
SRC_DIR = ../../src
TEST_DIR = .
BUILD_DIR = build
CODEGEN_TEST = $(BUILD_DIR)/test_codegen
BUILD_TARGET = $(BUILD_DIR)/test_codegen

all: test

# build the test file
$(CODEGEN_TEST): $(SRC_DIR)/codegen.cpp $(SRC_DIR)/parser.cpp $(SRC_DIR)/lexer.cpp $(TEST_DIR)/test_codegen.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SRC_DIR)/codegen.cpp $(SRC_DIR)/parser.cpp $(SRC_DIR)/lexer.cpp $(TEST_DIR)/test_codegen.cpp -o $(CODEGEN_TEST)

# create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# run the test file
test: $(CODEGEN_TEST)
	@echo "Running codegen test..."
	@$(CODEGEN_TEST)
	@echo ""

# clean the build directory
clean:
	rm -rf $(BUILD_DIR)
	rm -f *.o
	rm -f temp_test_*.txt
	rm -f output.asm output.bin output.hex

# help
help:
	@echo "Available targets:"
	@echo "  test        - Run the codegen test"
	@echo "  clean       - Clean build files and test artifacts"
	@echo "  help        - Show this help" 