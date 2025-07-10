# Pratt Parser

## What's the difference of the Pratt parser?

Currently, our parser scans from left to right, which can handle different expressions like:

```
1 + 2
a
a + b
a + c + d
```

However, in order to compute complicated expressions, we have to use more symbols like '()', '*', '/'.
Each symbol has different binding priority to the terms.

Except for these symbols, there are other needs like parsing functions and their parameters. 
Linear scanning will be terrible with all of these symbols added.

So the Pratt parser is introduced, which is easy to write (just a recursive procedure) and maintain (if we need more symbols in it, just change the priority of association).

## How it works?

The Pratt parser takes each operator with different association priority. Like '+' and '-' will be assigned priority 1, and '*' will be assigned priority 2. The higher priority will be calculated first in the AST.

Here is a very good introduction about how it works and visualizes the binding process:

https://matklad.github.io/2020/04/13/simple-but-powerful-pratt-parsing.html

## Implementation

The implementation is relatively easy once you really understand the recursive process. In simple words, we divide the expression into two parts: left and right.

When parsing from the lexer, the parser will read the left part. Here are the situations:
* A number or symbol => take it as output.
* A `(` => refresh the binding priority to 0 and parse from the right of it.

Then parse the operator in between and read the priority. 

Here is the key: **If the current operator priority is larger than the last operator, then parse the right part.**
An example: `1 + 2 * 3`.
When we parse the `2` and `*`, `*` has larger priority than `+`, so we parse `2 * 3` first and take it as a whole to return. Then calculate `1 + (2 * 3)`.

*What will happen at the beginning?* Actually, it's easy. The empty should not bind with any term, so we assume the beginning priority is 0. And the priority of the end of the expression is also 0. When we parse to `1` and `+`, we have to parse the right part first because there is no association between `1` and the beginning.

Here is the draft of the parse recursive function:

```c++
// Pratt parser for expression parsing
std::string Parser::parseExpr(int precedence) {
    // Parse the left-hand side first
    std::string left = parsePrefixExpr();
    
    // Process operators while they have higher precedence than the minimum
    while(precedence < getPrecedence(currentToken.type)) {
        TokenType opType = currentToken.type;  // Save the operator
        advance();  // Consume the operator
        
        // Parse the right-hand side with the operator's precedence
        std::string right = parseExpr(getPrecedence(opType));
        std::string tmpVariable = genTempVar();
        
        // Generate IR based on the operator
        if(opType == TokenType::OP_PLUS) {
            ir.push_back(IR{OpCode::ADD, left, right, tmpVariable});
        } else if(opType == TokenType::OP_MINUS) {
            ir.push_back(IR{OpCode::SUB, left, right, tmpVariable});
        } else {
            throw std::runtime_error("Unexpected operator: " + currentToken.value + " at line " + std::to_string(currentToken.line) + ", column " + std::to_string(currentToken.column));
        }
        
        left = tmpVariable;  // Result becomes new left operand
    }
    return left;
}
```

Notice we start from the left empty which has priority 0, so every operator which has priority > 0 will be parsed. 

The prefix parser part is easy:

```c++
std::string Parser::parsePrefixExpr() {
    std::string temp = genTempVar();
    if(currentToken.type == TokenType::NUMBER) {
        ir.push_back(IR{OpCode::STORE_CONST, currentToken.value, "", temp});
        advance();
    } else if(currentToken.type == TokenType::ID) {
        ir.push_back(IR{OpCode::STORE, currentToken.value, "", temp});
        advance();
    } else if(currentToken.type == TokenType::OP_BRACKET_LEFT) {
        advance(); // consume '('
        std::string innerExpr = parseExpr(0); // parse inner expression with lowest precedence
        expect(TokenType::OP_BRACKET_RIGHT); // consume ')'
        return innerExpr; // return inner expression result directly
    } else {
        throw std::runtime_error("Expected identifier or number in expression at line " +
                                 std::to_string(currentToken.line) + " column " +
                                 std::to_string(currentToken.column));
    } 
    return temp;
}
```

And the priority:

```c++
// Get the precedence of the operator
int Parser::getPrecedence(TokenType op) {
    switch(op) {
        case TokenType::OP_PLUS:
        case TokenType::OP_MINUS:
            return 1;
        case TokenType::OP_BRACKET_LEFT:
        case TokenType::OP_BRACKET_RIGHT:
            return 0; 
        default:
            return 0;
    }
}
```

## Generate IR

How to generate IR? Because the Pratt parser is designed to generate an AST, every time the parser has finished the right part (we have operator, left and right now), we can push the IR: ADD or SUB. 

Because we are lazy and use memory write-back, when the prefix (left) part is finished, we can store it in a temporary variable. The same situation applies to the right part. PS: This makes life much easier even though it's inefficient. `:-)`

The recursive order(stack) is the same as the final IR generation process.

