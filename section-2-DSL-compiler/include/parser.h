#pragma once
#include "token.h"
#include "lexer.h"
#include <iostream>
#include <vector>
#include <string>

enum class OpCode{
    LOAD_CONST, LOAD_VAR,
    ADD, SUB,
    STORE, STORE_CONST, IFLEQ, GOTO,
    LABEL, OUT, HALT
};

struct IR{
    OpCode op; // operation code
    std::string arg1, arg2, result; // arg1: variable name, arg2: constant value, result: temporary variable name
};

class Parser{
    Lexer lexer;
    Token currentToken;

    int tempVarCount = 0;

    std::vector<IR> ir;
    Token expect(TokenType type); // check if the current token is the expected type
    Token peek(); // get current token
    void advance(); // move pointer to the next token;
    std::string genTempVar(); // generate a temporary variable name

    std::string parseTerm();  // return a temporary variable name or temperary variable name
    std::string parseExpr();  // parse an expression and 
    void parseLet();
    void parseAssignment(); // parse variable assignment like 'a = 2;'
    void parseIfLeq();
    void parseGoto();
    void parseOut();
    void parseLabel();
    void parseHalt();

public:
    explicit Parser(Lexer lexer) : lexer(lexer) { advance();}
    void parseStatement();
    void parseProgram();
    // for debug and test
    void printIR();
    size_t getIRSize() const { return ir.size(); }
    const std::vector<IR>& getIR() const { return ir; }
};











