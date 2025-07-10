# Makefile for Parser Testing
# Author: Generated for DSL Compiler Project

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g -DDEBUG
INCLUDES = -I../../include

# Directories
SRC_DIR = ../../src
TEST_DIR = .
BUILD_DIR = build
COMPREHENSIVE_TEST = $(BUILD_DIR)/test_parser_comprehensive


all: test
# create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# build the comprehensive test file
$(COMPREHENSIVE_TEST): $(SRC_DIR)/parser.cpp $(SRC_DIR)/lexer.cpp $(TEST_DIR)/test_parser_comprehensive.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SRC_DIR)/parser.cpp $(SRC_DIR)/lexer.cpp $(TEST_DIR)/test_parser_comprehensive.cpp -o $(COMPREHENSIVE_TEST)

# run the comprehensive parser test
test: $(COMPREHENSIVE_TEST)
	@echo "Running comprehensive Pratt parser test..."
	@$(COMPREHENSIVE_TEST)
	@echo ""

# clean the build directory
clean:
	rm -rf $(BUILD_DIR)
	rm -f *.o

# help
help:
	@echo "Available targets:"
	@echo "  test - Run the comprehensive Pratt parser test"
	@echo "  clean         - Clean build files"
	@echo "  help          - Show this help"