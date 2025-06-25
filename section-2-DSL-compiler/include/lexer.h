#pragma once

#include <iostream>
#include <unordered_map>
#include "token.h"

class Lexer{
    const std::string src;
    int pos = 0; // current position in the input string
    int line = 1; // current line number
    int column = 1; // current column number
    char peek() const;
    char get();
    static TokenType keyWord(const std::string& s);
public:
    explicit Lexer(std::string text):src(std::move(text)) {} // doesn't allow copy or move
    Token genNextToken();   
};

