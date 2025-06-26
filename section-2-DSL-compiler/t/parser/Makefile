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
PARSER_TEST = $(BUILD_DIR)/test_parser
BUILD_TARGET = $(BUILD_DIR)/test_parser

# create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# build the test file
$(PARSER_TEST): $(SRC_DIR)/parser.cpp $(SRC_DIR)/lexer.cpp $(TEST_DIR)/test_parser.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SRC_DIR)/parser.cpp $(SRC_DIR)/lexer.cpp $(TEST_DIR)/test_parser.cpp -o $(PARSER_TEST)
# run the test file
test: $(PARSER_TEST)
	@echo "Running parser test..."
	@$(PARSER_TEST)
	@echo ""

# clean the build directory
clean:
	rm -rf $(BUILD_DIR)
	rm -f *.o

# help
help:
	@echo "Available targets:"
	@echo "  test        - Run the parser test"
	@echo "  clean       - Clean build files"
	@echo "  help        - Show this help"