CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++20 -O2

TARGET = cpu
SRCS = main.cpp
TEST_SRCS = test.cpp

all: $(TARGET)

build: 
	mkdir -p build

debug: CXXFLAGS += -g -O0 -DDEBUG
debug: all test

release: CXXFLAGS += -O3
release: all test

$(TARGET): $(SRCS) | build
	$(CXX) $(CXXFLAGS) $(SRCS) -o build/$(TARGET)

test: $(TEST_SRCS) | build
	$(CXX) $(CXXFLAGS) $(TEST_SRCS) -o build/test

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf build

help:
	@echo "Usage: make [TARGET]"
	@echo "Targets:"
	@echo "  all       Build the main program"
	@echo "  test      Build the test program"
	@echo "  debug     Build the main program with debug symbols"
	@echo "  release   Build the main program with optimizations"
	@echo "  clean     Clean the build directory"
	@echo "  help      Show this help message"

.PHONY: all clean build debug release test help