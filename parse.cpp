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
    void print(std::string message) {
        std::cout << message << std::endl; 
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

    void program(); 
    void statement(); 
    void comparison(); 
    void expression(); 
    void term(); 
    void unary(); 
    void primary(); 
    void nl(); 

    void program() {
        if (token().type == Ttype::NEWLINE) nl(); 
        statement(); 
    }
    void statement() {
        switch(token().type) {
            case Ttype::PRINT:
                print("PRINT"); 
                nextToken(); 
                if (token().type == Ttype::STRING) print("STRING"); 
                else statement(); 
                nl(); 
                break; 
            case Ttype::IF:
                print("IF");
                nextToken(); 
                comparison(); 
                if (token().type != Ttype::THEN) error("Invalid if statement - Missing THEN"); 
                nextToken(); 
                nl(); 
                statement(); 
                if (token().type != Ttype::ENDIF) error("Invalid if statement - Missing ENDIF"); 
                nl(); 
                break; 
            case Ttype::WHILE:
                print("WHILE"); 
                nextToken(); 
                comparison(); 
                if (token().type != Ttype::REPEAT) error("Invalid while loop - Missing REPEAT"); 
                nl(); 
                statement(); 
                if (type().type != Ttype::ENDWHILE) error("Invalid while loop - Missing ENDWHILE"); 
                nl(); 
                break; 
            case Ttype::LABEL:
                print("LABEL"); 
                nextToken(); 
                if (token().type != Ttype::IDENT) error("Invalid declaration - Missing identifier"); 
                nl(); 
                break; 
            case Ttype::GOTO:
                print("GOTO"); 
                nextToken(); 
                if (token().type != Ttype::IDENT) error("Invalid goto - Missing identifier");
                nl(); 
                break; 
            case Ttype::LET:
                print("LET"); 
                nextToken(); 
                if (token().type != Ttype::IDENT) error("Invalid assignment - Missing identifier"); 
                nextToken(); 
                if (token().type != Ttype::EQ) error("Invalid assignment - Missing ="); 
                nextToken(); 
                expression(); 
                nl(); 
                break; 
            case Ttype::INPUT:
                print("INPUT"); 
                nextToken(); 
                if (token().type != Ttype::IDENT) error("Invalid input - Missing identifier"); 
                nextToken(); 
                nl(); 
                break; 
            default:
                error("Invalid statement")
        }
    }
    bool isEquality() {
        switch (token().type) {
            Ttype::EQEQ: 
            Ttype::NOTEQ: 
            Ttype::GT:
            Ttype::GTEQ:
            Ttype::LT:
            Ttype::LTEQ:
                return true; 
            default:
                return false; 
        }
    }
    void comparison() {
        expression(); 
        nextToken(); 
        bool hasInitialEquality = false; 
        while (isEquality()) {
            hasInitialEquality = true; 
            expression(); 
            nextToken(); 
        }
    }
    void expression() {
        term(); 
        nextToken(); 
        if (token().type == Ttype::PLUS) print("PLUS"); 
        else if (token().type == Ttype::MINUS) print("MINUS"); 
        else error("Invalid expression - Missing + or -"); 
        nextToken(); 
        term(); 
        nextToken(); 
    }
    void term() {
        unary(); 
        nextToken(); 
        if (token().type == Ttype::ASTERISK) print("MULTIPLY"); 
        else if (token().type == Ttype::SLASH) print("DIVIDE"); 
        else error("Invalid expression - Missing * or /"); 
        nextToken(); 
        unary(); 
        nextToken(); 
    }
    void unary() {
        if (token().type == Ttype::MINUS) {
            print("NEGATIVE"); 
            nextToken(); 
        }
        primary(); 
    }
    void primary() {
        if (token().type == Ttype::NUMBER) print("NUMBER"); 
        else if (token().type == Ttype::IDENT) print("IDENT"); 
        else error("Invalid primary - Missing number or identifier"); 
    }
    void nl() {
        if (token().type != Ttype::NEWLINE) error("Invalid formatting: Missing new line"); 
        do {
            nextToken(); 
        } while (token().type == Ttype::NEWLINE); 
    }

public:
    Parser(std::vector<Token> _tokens) {
        tokens = _tokens;
        program(); 
    }
}; 