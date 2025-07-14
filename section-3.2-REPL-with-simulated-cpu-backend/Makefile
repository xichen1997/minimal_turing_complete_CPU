
CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -g
TARGET = REPL

ifdef DEBUG
	CXXFLAGS += -DDEBUG
endif

SRCS = src/REPL.cpp src/lexer.cpp src/parser.cpp src/interpreter.cpp src/codegen.cpp

all: build $(TARGET)

build:
	mkdir -p build

run:
	./build/$(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o build/$(TARGET) $(SRCS)

clean:
	rm -f build/$(TARGET)