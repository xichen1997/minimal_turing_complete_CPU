
CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -g
TARGET = REPL

BUILD_DIR = build

# compile the compiler
compiler: $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(BUILD_DIR)/compiler src/compiler.cpp src/codegen.cpp src/parser.cpp src/lexer.cpp

# make the build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

ifdef DEBUG
	CXXFLAGS += -DDEBUG
endif

SRCS = src/REPL.cpp src/lexer.cpp src/parser.cpp src/interpreter.cpp src/codegen.cpp

all: $(BUILD_DIR) $(TARGET)

run:
	./$(BUILD_DIR)/$(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/$(TARGET) $(SRCS)

clean:
	rm -f $(BUILD_DIR)/$(TARGET)

# Test targets
test-arrays:
	@echo "🧪 Running Array Support Tests..."
	@cd t/arrays && $(MAKE) test

test: test-arrays

# Clean test artifacts
clean-tests:
	@cd t/arrays && $(MAKE) clean

clean-all: clean clean-tests

.PHONY: test test-arrays clean-tests clean-all