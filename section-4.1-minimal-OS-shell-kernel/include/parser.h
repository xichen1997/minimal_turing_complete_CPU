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
    LABEL, OUT, HALT, IN,
    ARRAY_DECL,     // arg1 = array name, arg2 = length
    LOAD_INDEXED,   // arg1 = array name, arg2 = index, result = result
    STORE_INDEXED   // arg1 = array name, arg2 = index, arg3 = value
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
    int getPrecedence(TokenType op); // get the precedence of the operator
    
    std::string parseTerm();  // return a temporary variable name or temperary variable name
    std::string parseExpr(int precedence = 0);  // parse an expression and
    std::string parsePrefixExpr();  // parse prefix expression
    std::string parsePostfixExpr();  // parse postfix expression 
    void parseLet();
    void parseAssignment(); // parse variable assignment like 'a = 2;'
    void parseArrayAssignment();
    void parseArrayAccess();
    void parseArrayDecl();
    void parseIn();
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











