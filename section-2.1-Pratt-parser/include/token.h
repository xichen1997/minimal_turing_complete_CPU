#pragma once
#include<string>
// generate a parser for the DSL for minimal CPU
// lexer: tokenize the input string, generate a stream of tokens, for now, we only support +, -, <=, =, (, )
enum class TokenType {
    KW_LET, KW_IF, KW_GOTO, KW_OUT, KW_HALT,
    ID, NUMBER,
    OP_PLUS, OP_MINUS, OP_LEQ, OP_BRACKET_LEFT, OP_BRACKET_RIGHT,
    EQUAL, COLON, SEMICOLON,
    TOKEN_EOF
};

struct Token{
    TokenType type;
    std::string value; // for ID and NUMBER, need to store the original string.
    int line;
    int column; // need to store the position of the token in the input string
};