#include <vector>
#include <iostream>
#include "../token/token.h"
#include "parse.h"

void Parser::print(std::string message) {
    std::cout << message << std::endl; 
}
void Parser::success() {
    std::cout << "Done" << std::endl; 
    std::exit(0);
}
void Parser::error(std::string message) {
    std::cout << "Parsing Error: " << message << std::endl; 
    token().print(); 
    std::exit(0);
}

Token Parser::token() {
    return tokens[curIndex]; 
}
void Parser::nextToken() {
    curIndex++; 
    if (token().type == Ttype::EOTF) success(); 
}
Token Parser::peek() {
    Token nextToken = tokens[curIndex + 1]; 
    if (nextToken.type == Ttype::EOTF) success(); 
    return nextToken; 
}

void Parser::endStatement() {
    nl(); 
    statement(); 
}

void Parser::program() {
    if (token().type == Ttype::NEWLINE) nl(); 
    statement(); 
}
void Parser::statement() {
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
            activeIfs++; 
            comparison(); 
            if (token().type == Ttype::THEN) print("THEN"); 
            else error("Invalid if statement - Missing THEN"); 
            nextToken(); 
            endStatement(); 
            break; 
        case Ttype::ENDIF:
            print("ENDIF");
            while (token().type == Ttype::ENDIF) {
                if (activeIfs <= 0) error("Invalid if statement - Extra ENDIF"); 
                activeIfs--;
                nextToken(); 
            }
            endStatement(); 
            break; 
        case Ttype::WHILE:
            print("WHILE"); 
            nextToken(); 
            activeWhiles++; 
            comparison(); 
            if (token().type == Ttype::REPEAT) print("REPEAT"); 
            else error("Invalid while loop - Missing REPEAT"); 
            nextToken(); 
            endStatement(); 
            break; 
        case Ttype::ENDWHILE:
            print("ENDWHILE");
            while (token().type == Ttype::ENDWHILE) {
                if (activeWhiles <= 0) error("Invalid while loop - Extra ENDWHILE"); 
                activeWhiles--;
                nextToken(); 
            }
            endStatement(); 
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
            if (token().type == Ttype::IDENT) print("IDENT"); 
            else error("Invalid input - Missing identifier"); 
            nextToken(); 
            endStatement();
            break; 
        default:
            error("Invalid statement"); 
    }
}
bool Parser::isEquality() {
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
void Parser::comparison() {
    expression(); 
    bool hasInitialEquality = false; 
    while (isEquality()) {
        hasInitialEquality = true; 
        nextToken(); 
        expression(); 
    }
    if (!hasInitialEquality) error("Invalid comparison - Missing equality"); 
}
void Parser::expression() {
    term(); 
    if (token().type == Ttype::PLUS) print("PLUS"); 
    else if (token().type == Ttype::MINUS) print("MINUS"); 
    else return; 
    nextToken(); 
    term(); 
}
void Parser::term() {
    unary(); 
    if (token().type == Ttype::ASTERISK) print("MULTIPLY"); 
    else if (token().type == Ttype::SLASH) print("DIVIDE"); 
    else return; 
    nextToken(); 
    unary(); 
}
void Parser::unary() {
    if (token().type == Ttype::MINUS) {
        print("NEGATIVE"); 
        nextToken(); 
    }
    primary(); 
}
void Parser::primary() {
    if (token().type == Ttype::NUMBER) print("NUMBER"); 
    else if (token().type == Ttype::IDENT) print("IDENT"); 
    else error("Invalid primary - Missing number or identifier"); 
    nextToken(); 
}
void Parser::nl() {
    if (token().type == Ttype::EOTF) {
        if (activeIfs > 0) error("Invalid if statement(s) - Missing ENDIF"); 
        if (activeWhiles > 0) error("Invalid while loop(s) - Missing ENDWHILE"); 
        success(); 
    }
    if (token().type != Ttype::NEWLINE) error("Invalid formatting: Missing new line"); 
    do {
        nextToken(); 
    } while (token().type == Ttype::NEWLINE); 
    print("NEWLINE"); 
}

Parser::Parser(std::vector<Token> _tokens) {
    tokens = _tokens;
    program(); 
}