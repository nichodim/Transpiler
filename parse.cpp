#include <vector>
#include <iostream>
#include "token/token.h"

class Parser {
private:
    std::vector<Token> tokens; 
    unsigned short curIndex = 0; 

    void print(std::string message) {
        std::cout << message << std::endl; 
    }
    void success() {
        std::cout << "Done" << std::endl; 
        std::exit(0);
    }
    void error(std::string message) {
        std::cout << "Parsing Error: " << message << std::endl; 
        token().print(); 
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

    // void program(); 
    // void statement(); 
    // void comparison(); 
    // void expression(); 
    // void term(); 
    // void unary(); 
    // void primary(); 
    // void nl(); 

    void endStatement() {
        if (token().type != Ttype::ENDWHILE) {
            nl(); 
            statement(); 
        }
    }

    void program() {
        if (token().type == Ttype::NEWLINE) nl(); 
        statement(); 
    }
    void statement() {
        switch(token().type) {
            case Ttype::PRINT:
                print("PRINT"); 
                nextToken(); 
                if (token().type == Ttype::STRING) {
                    print("STRING"); 
                    nextToken(); 
                }
                else expression(); 
                endStatement(); 
                break; 
            case Ttype::IF:
                print("IF");
                nextToken(); 
                comparison(); 
                if (token().type == Ttype::THEN) print("THEN"); 
                else error("Invalid if statement - Missing THEN"); 
                nextToken(); 
                nl(); 
                statement(); 
                if (token().type == Ttype::ENDIF) print("ENDIF"); 
                else error("Invalid if statement - Missing ENDIF"); 
                nextToken(); 
                endStatement(); 
                break; 
            case Ttype::WHILE:
                print("WHILE"); 
                nextToken(); 
                comparison(); 
                if (token().type == Ttype::REPEAT) print("REPEAT"); 
                else error("Invalid while loop - Missing REPEAT"); 
                nextToken(); 
                nl(); 
                statement(); 
                if (token().type == Ttype::ENDWHILE) print("ENDWHILE"); 
                else error("Invalid while loop - Missing ENDWHILE"); 
                nextToken(); 
                nl(); 
                statement(); 
                break; 
            case Ttype::LABEL:
                print("LABEL"); 
                nextToken(); 
                if (token().type == Ttype::IDENT) print("IDENT"); 
                else error("Invalid declaration - Missing identifier"); 
                nextToken(); 
                endStatement();
                break; 
            // case Ttype::GOTO:
            //     print("GOTO"); 
            //     nextToken(); 
            //     if (token().type == Ttype::IDENT) print("IDENT"); 
            //     else error("Invalid goto - Missing identifier");
            //     nl(); 
            //     statement(); 
            //     break; 
            case Ttype::LET:
                print("LET"); 
                nextToken(); 
                if (token().type == Ttype::IDENT) print("IDENT"); 
                else error("Invalid assignment - Missing identifier"); 
                nextToken(); 
                if (token().type == Ttype::EQ) print("=");
                else error("Invalid assignment - Missing ="); 
                nextToken(); 
                expression(); 
                endStatement();
                break; 
            case Ttype::INPUT:
                print("INPUT"); 
                nextToken(); 
                if (token().type != Ttype::IDENT) print("IDENT"); 
                else error("Invalid input - Missing identifier"); 
                nextToken(); 
                endStatement();
                break; 
            default:
                error("Invalid statement"); 
        }
    }
    bool isEquality() {
        switch (token().type) {
            case Ttype::EQEQ: 
            case Ttype::NOTEQ: 
            case Ttype::GT:
            case Ttype::GTEQ:
            case Ttype::LT:
            case Ttype::LTEQ:
                return true; 
            default:
                return false; 
        }
    }
    void comparison() {
        expression(); 
        bool hasInitialEquality = false; 
        while (isEquality()) {
            hasInitialEquality = true; 
            nextToken(); 
            expression(); 
        }
        if (!hasInitialEquality) error("Invalid comparison - Missing equality"); 
    }
    void expression() {
        term(); 
        if (token().type == Ttype::PLUS) print("PLUS"); 
        else if (token().type == Ttype::MINUS) print("MINUS"); 
        else return; 
        nextToken(); 
        term(); 
    }
    void term() {
        unary(); 
        if (token().type == Ttype::ASTERISK) print("MULTIPLY"); 
        else if (token().type == Ttype::SLASH) print("DIVIDE"); 
        else return; 
        nextToken(); 
        unary(); 
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
        nextToken(); 
    }
    void nl() {
        if (token().type == Ttype::EOTF) success(); 
        if (token().type != Ttype::NEWLINE) error("Invalid formatting: Missing new line"); 
        do {
            nextToken(); 
        } while (token().type == Ttype::NEWLINE); 
        print("NEWLINE"); 
    }

public:
    Parser(std::vector<Token> _tokens) {
        tokens = _tokens;
        program(); 
    }
}; 