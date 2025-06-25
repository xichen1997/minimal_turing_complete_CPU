#include "lexer.h"
#include <cctype>
#include <unordered_map>
#include <stdexcept>

// private functions
char Lexer::peek() const { 
    return pos < src.size() ? src[pos] : '\0'; 
}
char Lexer::get()  { 
    char c = peek(); ++pos;
    c == '\n' ? (++line, column = 1) : ++column;
    return c; 
}
TokenType Lexer::keyWord(const std::string& s) {
    // keyword, operator and identifier includes in the class.
    // only focus on the keyword for this function.
    static const std::unordered_map<std::string,TokenType> tb = {
        {"let",TokenType::KW_LET}, {"if",TokenType::KW_IF}, {"goto",TokenType::KW_GOTO},
        {"out",TokenType::KW_OUT}, {"halt",TokenType::KW_HALT}};
    auto it = tb.find(s);
    return it==tb.end()?TokenType::ID : it->second; // means it's a identifier
}

// public functions

Token Lexer::genNextToken(){
    // scnning the first character of the string and skip the comments until the end of the line.
    // also skip the white spaces.
    while (std::isspace(peek()) || (peek()=='/' && pos+1<src.size() && src[pos+1]=='/')) {
        if (peek()=='/') {                 // skip all of the comments until the end of the line
            while (peek() && peek()!='\n') get();
        } else get();                      // skip white spaces
    }
    int startline = line, startcol = column;

    char c = peek();
    if(c == '\0') return {TokenType::TOKEN_EOF, "", startline, startcol};

    // for the number
    if(std::isdigit(c)) {
        std::string num;
        while(std::isdigit(peek())) num += get();
        return {TokenType::NUMBER, num, startline, startcol};
    }

    // for the identifier and keyword
    if (std::isalpha(c) || c=='_') {
        std::string id;
        while (std::isalnum(peek()) || peek()=='_') id.push_back(get());
        return {keyWord(id), id, startline, startcol}; // return identifier or keyword
    }

    // for the operator
    get();
    switch(c) {
        case '+': return {TokenType::OP_PLUS, "+", startline, startcol};
        case '-': return {TokenType::OP_MINUS, "-", startline, startcol};
        case ':': return {TokenType::COLON,   ":", startline, startcol};
        case ';': return {TokenType::SEMICOLON,";",startline, startcol};
        case '=': return {TokenType::EQUAL,   "=", startline, startcol};
        case '<':
            if (peek()=='=') { 
                get();
                return {TokenType::OP_LEQ,"<=",startline,startcol}; 
            }
            break;
        default:
            throw std::runtime_error("Unexpected character: " + std::string(1, c) + " at line " + std::to_string(startline) + " column " + std::to_string(startcol));
    }
    // if the first character is not a number or identifier, it's an error, throw an exception about the line and column number
    throw std::runtime_error("Unexpected character: " + std::string(1, c) + " at line " + std::to_string(startline) + " column " + std::to_string(startcol));
}