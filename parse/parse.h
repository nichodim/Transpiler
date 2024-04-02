#include <vector>
#include <cassert>
#include <map>
#include "../token/token.h"
#include "../emit/emit.h"
#pragma once

class Parser {
private:
    Emitter emit;
    std::vector<Token> tokens; 
    unsigned short curIndex = 0; 
    unsigned short blockDepth = 0;
    std::map<std::string, short> idents; 

    // Ends case helpers
    void print(std::string message);
    void success();
    void error(std::string message);

    // Token vector access points
    Token token();
    void nextToken();
    Token peek();

    // Scope managing ident functions
    bool identExists(std::string ident);
    void addIdent(std::string text);
    void updateIdents(); // Removes idents out of scope

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