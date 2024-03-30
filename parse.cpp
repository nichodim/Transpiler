#include <vector>
#include <iostream>
#include "token/token.h"

class Parser {
private:
    std::vector<Token> tokens; 
    unsigned short curIndex = 0; 

    void success() {
        std::cout << "Done" << std::endl; 
        std::exit(0);
    }
    void error(std::string message) {
        std::cout << "Parsing Error: " << message << std::endl; 
        std::exit(0);
    }

    Token token() {
        return tokens[curIndex]; 
    }
    void nextToken() {
        curIndex++; 
        if (token().type == Ttype::EOTF) success(); 
    }
    Token peek() {
        Token nextToken = tokens[curIndex + 1]; 
        if (nextToken.type == Ttype::EOTF) success(); 
        return nextToken; 
    }

    // void program() {
    //     switch (curToken.type) {
    //         case 
    //     }
    // }

public:
    Parser(std::vector<Token> _tokens) {
        tokens = _tokens;
        for (Token token : tokens) {
            token.print(); 
        }
    }
}; 