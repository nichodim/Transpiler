#include <vector>
#include <iostream>
#include <cassert>
#include <map>
#include "../token/token.h"
#include "../emit/emit.h"
#include "parse.h"

void Parser::print(std::string message) {
    std::cout << message << std::endl; 
}
void Parser::success() {
    emit.pushText();
    std::cout << "Done" << std::endl; 
    std::exit(0);
}
void Parser::error(std::string message) {
    emit.pushText();
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

bool Parser::identExists(std::string ident) {
    if (idents.find(ident) != idents.end()) return true; 
    return false; 
}
void Parser::addIdent(std::string text) {
    if (identExists(text)) error("Invalid declaration - variable already in use");
    idents.insert({ text, blockDepth });
}
void Parser::updateIdents() {
    std::vector<std::string> idents_to_delete;
    for (auto const& ident : idents) {
        if (ident.second > blockDepth) {
            idents_to_delete.push_back(ident.first);
        }
    }
    for (std::string ident : idents_to_delete) {
        idents.erase(ident);
    }
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
            emit.pushToBody("printf(");
            nextToken(); 
            if (token().type == Ttype::STRING) {
                print("STRING"); 
                emit.pushToBody("\"" + token().text + "\"");
                nextToken(); 
            }
            else expression(); 
            emit.pushToBody(");");
            endStatement(); 
            break; 

        case Ttype::IF:
            print("IF");
            emit.pushToBody("if (");
            nextToken(); 
            blockDepth++; 
            comparison(); 
            emit.pushToBody("){");
            if (token().type == Ttype::THEN) print("THEN"); 
            else error("Invalid if statement - Missing THEN"); 
            nextToken(); 
            endStatement(); 
            break; 
        case Ttype::ENDIF:
            print("ENDIF");
            while (token().type == Ttype::ENDIF) {
                emit.pushToBody("}");
                if (blockDepth <= 0) error("Invalid if statement - Extra ENDIF"); 
                blockDepth--; updateIdents(); 
                nextToken(); 
            }
            endStatement(); 
            break; 
        case Ttype::WHILE:
            print("WHILE"); 
            emit.pushToBody("while (");
            nextToken(); 
            blockDepth++;
            comparison(); 
            emit.pushToBody("){");
            if (token().type == Ttype::REPEAT) print("REPEAT"); 
            else error("Invalid while loop - Missing REPEAT"); 
            nextToken(); 
            endStatement(); 
            break; 
        case Ttype::ENDWHILE:
            print("ENDWHILE");
            while (token().type == Ttype::ENDWHILE) {
                emit.pushToBody("}");
                if (blockDepth <= 0) error("Invalid while loop - Extra ENDWHILE"); 
                blockDepth--; updateIdents(); 
                nextToken(); 
            }
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

        case Ttype::LABEL:
            print("LABEL"); 
            emit.pushToBody("float ");
            nextToken(); 
            if (token().type == Ttype::IDENT) print("IDENT"); 
            else error("Invalid declaration - Missing identifier"); 
            emit.pushToBody(token().text + ";");
            addIdent(token().text); 
            nextToken(); 
            endStatement();
            break; 
        case Ttype::LET:
            print("LET"); 
            nextToken(); 
            if (token().type == Ttype::IDENT) print("IDENT"); 
            else error("Invalid assignment - Missing identifier"); 
            if (!identExists(token().text)) {
                addIdent(token().text); emit.pushToBody("float ");
            }
            emit.pushToBody(token().text);
            nextToken(); 
            if (token().type == Ttype::EQ) print("=");
            else error("Invalid assignment - Missing ="); 
            emit.pushToBody(" = ");
            nextToken(); 
            expression(); 
            emit.pushToBody(";");
            endStatement();
            break; 
        case Ttype::INPUT:
            print("INPUT"); 
            emit.pushToBody("scanf(");
            nextToken(); 
            if (token().type == Ttype::IDENT) print("IDENT"); 
            else error("Invalid input - Missing identifier"); 
            if (!identExists(token().text)) addIdent(token().text); 
            emit.pushToBody("\"%v\", &" + token().text + ");");
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
        emit.pushToBody(" " + token().text + " ");
        hasInitialEquality = true; 
        nextToken(); 
        expression(); 
    }
    if (!hasInitialEquality) error("Invalid comparison - Missing equality"); 
}
void Parser::expression() {
    term(); 
    while (token().type == Ttype::PLUS || token().type == Ttype::MINUS) {
        print("PLUS/MINUS"); 
        emit.pushToBody(" " + token().text + " ");
        nextToken(); 
        term(); 
    }
}
void Parser::term() {
    unary(); 
    while (token().type == Ttype::ASTERISK || token().type == Ttype::SLASH) {
        print("MULTIPLY/DIVIDE"); 
        emit.pushToBody(" " + token().text + " ");
        nextToken(); 
        unary(); 
    }
}
void Parser::unary() {
    if (token().type == Ttype::MINUS) {
        print("NEGATIVE"); 
        emit.pushToBody("-");
        nextToken(); 
    }
    primary(); 
}
void Parser::primary() {
    if (token().type == Ttype::INTEGER || token().type == Ttype::FLOAT) {
        print("INTEGER/FLOAT"); 
        emit.pushToBody(token().text);
    }
    else if (token().type == Ttype::IDENT) {
        if (!identExists(token().text)) error("Invalid primary - Using undefined identifier"); 
        print("IDENT"); 
        emit.pushToBody(token().text);
    }
    else error("Invalid primary - Missing number or identifier"); 
    nextToken(); 
}
void Parser::nl() {
    emit.pushToBody("\n");
    if (token().type == Ttype::EOTF) {
        if (blockDepth > 0) error("Invalid block depth - Missing END(s)"); 
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