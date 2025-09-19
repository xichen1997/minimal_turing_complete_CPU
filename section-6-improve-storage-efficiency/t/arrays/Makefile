CXX = g++
CXXFLAGS = -std=c++17 -I../../include -g -Wall -Wextra
TARGET = test_arrays
BUILD_DIR = build

# Source files
SRCS = test_arrays.cpp ../../src/lexer.cpp ../../src/parser.cpp ../../src/interpreter.cpp ../../src/codegen.cpp

# Object files
OBJS = $(SRCS:%.cpp=$(BUILD_DIR)/%.o)

.PHONY: all clean test run

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/../../src

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/$(TARGET) $(OBJS)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: $(TARGET)
	cd $(BUILD_DIR) && ./$(TARGET)

run: test

clean:
	rm -rf $(BUILD_DIR)
	rm -f *.dsl *.asm *.bin *.hex

help:
	@echo "Available targets:"
	@echo "  all   - Build the test executable"
	@echo "  test  - Run the array tests"
	@echo "  run   - Alias for test"
	@echo "  clean - Remove build files and temporary files"
	@echo "  help  - Show this help message"