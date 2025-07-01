#include "../include/parser.h"
#include "../include/token.h"
#include "../include/lexer.h"
#include<iostream>

#ifdef DEBUG
#define DEBUG_PRINT(x) do { x; } while (0)
#else
#define DEBUG_PRINT(x) do {} while (0)
#endif

void Parser::advance(){
    currentToken = lexer.genNextToken();
}

Token Parser::peek(){
    return currentToken;
}

std::string Parser::genTempVar(){
    return "__temp__" + std::to_string(tempVarCount++);
}

Token Parser::expect(TokenType type){
    if(currentToken.type == type){
        Token result = currentToken;
        advance();
        return result;
    }else{
        // throw error with line and column
        throw std::runtime_error("Error: expected " + std::to_string(static_cast<int>(type)) + " but got " + std::to_string(static_cast<int>(currentToken.type)) + " at line " + std::to_string(currentToken.line) + " column " + std::to_string(currentToken.column));
    }
}

std::string Parser::parseTerm() {
    std::string temp = genTempVar();
    if (currentToken.type == TokenType::NUMBER) {
        ir.push_back(IR{OpCode::STORE_CONST, currentToken.value, "", temp});
        advance();
    } else if (currentToken.type == TokenType::ID) {
        ir.push_back(IR{OpCode::STORE, currentToken.value, "", temp});
        advance();
    } else {
        throw std::runtime_error("Expected identifier or number in expression at line " +
                                 std::to_string(currentToken.line) + " column " +
                                 std::to_string(currentToken.column));
    }
    return temp;
}

// left associative linear expression parser
std::string Parser::parseExpr() {
    // we always make the left most term as the left term.
    // like (a + b) + c, we make (a + b) as the left term.
    // and we parse the right term and add the result to the left term.

    std::string left = parseTerm();  // left most term.
    while (currentToken.type == TokenType::OP_PLUS || currentToken.type == TokenType::OP_MINUS) {
        TokenType op = currentToken.type;
        advance();                    // get a '+' or '-'
        std::string right = parseTerm(); // parse the right term
        std::string tmpVariable = genTempVar();  // get a temporary variable. and spill back to RAM.
        if (op == TokenType::OP_PLUS) {
            ir.push_back(IR{OpCode::ADD, left, right, tmpVariable});
        } else {
            ir.push_back(IR{OpCode::SUB, left, right, tmpVariable});
        }
        left = tmpVariable;             // next left term is the current result.

    }
    return left; // the final result is stored in left. 
}

// parse let statement
// ex: let a = b + 3;
void Parser::parseLet() {
    DEBUG_PRINT(std::cout << "[DEBUG] parseLet called" << std::endl;);
    expect(TokenType::KW_LET);                    //let
    if (currentToken.type != TokenType::ID) {
        throw std::runtime_error("Expected identifier after 'let'");
    }
    std::string varName = currentToken.value;          // get the variable name
    advance();
    expect(TokenType::EQUAL);                     // match =
    std::string valueTemp = parseExpr();          //  parse the expression and return the temperory variable name.
    expect(TokenType::SEMICOLON);                 //  match ;
    ir.push_back(IR{OpCode::STORE, valueTemp, "", varName});
    DEBUG_PRINT(std::cout << "[DEBUG] Added STORE IR, vector size now: " << ir.size() << std::endl;);
}

void Parser::parseOut() {
    expect(TokenType::KW_OUT);  // match out

    if (currentToken.type != TokenType::ID) {
        throw std::runtime_error("Expected identifier after 'out' at line " +
                                 std::to_string(currentToken.line) + ", column " +
                                 std::to_string(currentToken.column));
    }

    std::string varName = currentToken.value;
    advance();                  // next token

    expect(TokenType::SEMICOLON); // match ';'

    ir.push_back(IR{OpCode::OUT, varName, "", ""}); // push the IR to the vector.
}

void Parser::parseIfLeq() {
    expect(TokenType::KW_IF);  // match 'if'

    if (currentToken.type != TokenType::ID) {
        throw std::runtime_error("Expected identifier after 'if' at line " +
                                 std::to_string(currentToken.line) + ", column " +
                                 std::to_string(currentToken.column));
    }
    std::string lhs = currentToken.value;
    advance();

    expect(TokenType::OP_LEQ); // match '<='

    // Accept either an identifier or a number for the right-hand side
    std::string rhs;
    if (currentToken.type == TokenType::ID || currentToken.type == TokenType::NUMBER) {
        rhs = currentToken.value;
        advance();
    } else {
        throw std::runtime_error("Expected identifier or number after '<=' at line " +
                                 std::to_string(currentToken.line) + ", column " +
                                 std::to_string(currentToken.column));
    }

    expect(TokenType::KW_GOTO); // 3. match 'goto'

    if (currentToken.type != TokenType::ID) {
        throw std::runtime_error("Expected label after 'goto' at line " +
                                 std::to_string(currentToken.line) + ", column " +
                                 std::to_string(currentToken.column));
    }
    std::string label = currentToken.value;
    advance();

    expect(TokenType::SEMICOLON); // 4. match ';'

    ir.push_back(IR{OpCode::IFLEQ, lhs, rhs, label}); // generate IR
}

void Parser::parseGoto() {
    expect(TokenType::KW_GOTO);

    if (currentToken.type != TokenType::ID) {
        throw std::runtime_error("Expected label after 'goto' at line " +
                                 std::to_string(currentToken.line) + ", column " +
                                 std::to_string(currentToken.column));
    }
    std::string label = currentToken.value;
    advance();

    expect(TokenType::SEMICOLON); // match ';'

    ir.push_back(IR{OpCode::GOTO, "", "", label});
}

void Parser::parseLabel() {
    DEBUG_PRINT(std::cout << "[DEBUG] Entering parseLabel: TokenType=" << static_cast<int>(currentToken.type)
              << ", value='" << currentToken.value << "', line=" << currentToken.line
              << ", column=" << currentToken.column << std::endl;);
    if (currentToken.type != TokenType::ID) {
        throw std::runtime_error("Expected label identifier at line " +
                                 std::to_string(currentToken.line) + ", column " +
                                 std::to_string(currentToken.column));
    }
    std::string label = currentToken.value;
    advance();
    expect(TokenType::COLON); // match ':'
    
    ir.push_back(IR{OpCode::LABEL, "", "", label});
}

void Parser::parseHalt() {
    expect(TokenType::KW_HALT);       // match halt
    expect(TokenType::SEMICOLON);     // match semincolon

    ir.push_back(IR{OpCode::HALT, "", "", ""});  // halt
}

// parse assignment statement
// ex: a = 2;
void Parser::parseAssignment() {
    if (currentToken.type != TokenType::ID) {
        throw std::runtime_error("Expected identifier for assignment at line " +
                                 std::to_string(currentToken.line) + ", column " +
                                 std::to_string(currentToken.column));
    }
    std::string varName = currentToken.value;
    advance();
    expect(TokenType::EQUAL);                     // match =
    std::string valueTemp = parseExpr();          // parse the expression
    expect(TokenType::SEMICOLON);                 // match ;
    ir.push_back(IR{OpCode::STORE, valueTemp, "", varName});
}

void Parser::parseStatement() {
    DEBUG_PRINT(std::cout << "[DEBUG] Entering parseStatement: TokenType=" << static_cast<int>(currentToken.type)
              << ", value='" << currentToken.value << "', line=" << currentToken.line
              << ", column=" << currentToken.column << std::endl;);
    
    if (currentToken.type == TokenType::KW_LET) {
        parseLet();
    } else if (currentToken.type == TokenType::KW_OUT) {
        parseOut();
    } else if (currentToken.type == TokenType::KW_IF) {
        parseIfLeq();
    } else if (currentToken.type == TokenType::KW_GOTO) {
        parseGoto();
    } else if (currentToken.type == TokenType::KW_HALT) {
        parseHalt();
    } else if (currentToken.type == TokenType::ID) {
        // Check if it's a label (ID followed by colon) or assignment (ID followed by equals)
        Token nextToken = lexer.peekNextToken();
        DEBUG_PRINT(std::cout << "[DEBUG] Next token: TokenType=" << static_cast<int>(nextToken.type)
                  << ", value='" << nextToken.value << "'" << std::endl;);
        
        if (nextToken.type == TokenType::COLON) {
            // It's a label
            parseLabel();
        } else if (nextToken.type == TokenType::EQUAL) {
            // It's a variable assignment
            parseAssignment();
        } else {
            throw std::runtime_error("Unexpected token: " + currentToken.value + " at line " + std::to_string(currentToken.line) + ", column " + std::to_string(currentToken.column));
        }
    } else {
        throw std::runtime_error("Unexpected token: " + currentToken.value + " at line " + std::to_string(currentToken.line) + ", column " + std::to_string(currentToken.column));
    }
}

void Parser::parseProgram() {
    while (currentToken.type != TokenType::TOKEN_EOF) {
        DEBUG_PRINT(std::cout << "[DEBUG] About to parse statement: TokenType=" << static_cast<int>(currentToken.type)
                  << ", value='" << currentToken.value << "', line=" << currentToken.line
                  << ", column=" << currentToken.column << std::endl;);
        parseStatement();
    }
}

void Parser::printIR() {
    std::cout << "[DEBUG] printIR called, IR vector size: " << ir.size() << std::endl;
    for (const auto& instruction : ir) {
        std::string opStr;
        switch (instruction.op) {
            case OpCode::LOAD_CONST: opStr = "LOAD_CONST"; break;
            case OpCode::LOAD_VAR: opStr = "LOAD_VAR"; break;
            case OpCode::ADD: opStr = "ADD"; break;
            case OpCode::SUB: opStr = "SUB"; break;
            case OpCode::STORE: opStr = "STORE"; break;
            case OpCode::IFLEQ: opStr = "IFLEQ"; break;
            case OpCode::GOTO: opStr = "GOTO"; break;
            case OpCode::LABEL: opStr = "LABEL"; break;
            case OpCode::OUT: opStr = "OUT"; break;
            case OpCode::HALT: opStr = "HALT"; break;
        }
        
        if (instruction.op == OpCode::STORE) {
            std::cout << opStr << " " << instruction.arg1 << " -> " << instruction.result << std::endl;
        } else if (instruction.op == OpCode::LOAD_CONST || instruction.op == OpCode::LOAD_VAR) {
            std::cout << opStr << " " << instruction.arg1 << " -> " << instruction.result << std::endl;
        } else if (instruction.op == OpCode::ADD || instruction.op == OpCode::SUB) {
            std::cout << opStr << " " << instruction.arg1 << " " << instruction.arg2 << " -> " << instruction.result << std::endl;
        } else if (instruction.op == OpCode::IFLEQ) {
            std::cout << opStr << " " << instruction.arg1 << " " << instruction.arg2 << " " << instruction.result << std::endl;
        } else if (instruction.op == OpCode::GOTO || instruction.op == OpCode::LABEL) {
            std::cout << opStr << " " << instruction.result << std::endl;
        } else if (instruction.op == OpCode::OUT) {
            std::cout << opStr << " " << instruction.arg1 << std::endl;
        } else {
            std::cout << opStr << std::endl;
        }
    }
}

