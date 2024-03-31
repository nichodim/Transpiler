#include <vector>
#include "../token/token.h"
#pragma once

class Parser {
private:
    std::vector<Token> tokens; 
    unsigned short curIndex = 0; 
    unsigned short activeIfs = 0, activeWhiles = 0; 

    // Ends case helpers
    void print(std::string message);
    void success();
    void error(std::string message);

    // Token vector access points
    Token token();
    void nextToken();
    Token peek();

    // Convenient line reset in statements
    void endStatement();

    // Parse tree that trickles down directives
    void program();
    void statement();
    bool isEquality();
    void comparison();
    void expression();
    void term();
    void unary();
    void primary();
    void nl();

public:
    Parser(std::vector<Token> _tokens);
}; 