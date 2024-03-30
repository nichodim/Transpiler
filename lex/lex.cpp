#include <bits/stdc++.h> 
#include <iostream>
#include <vector> 
#include <algorithm>
#include "../token/token.h"
#include "lex.h"

bool Lexer::tryNextChar() {
    curIndex++;
    if (curIndex >= source.length()) return false; 
    curChar = source[curIndex]; 
    return true; 
}
void Lexer::nextChar() {
    curIndex++;
    if (curIndex >= source.length()) error("Invalid Syntax");
    curChar = source[curIndex]; 
}

std::string Lexer::peek(unsigned short depth) {
    if (curIndex + depth >= source.length()) return "\0";
    return source.substr(curIndex, depth);
}
char Lexer::peek() {
    if (curIndex + 1 >= source.length()) return '\0';
    return source[curIndex + 1];
}

bool Lexer::isSpace(char c) {
    if (c == ' ' || c == '\f' || c == '\r' || c == '\t' || c == '\v') {
        return true;
    } return false;
}
void Lexer::removeWhiteSpace() {
    std::string newSource;
    for (unsigned short i = 0; i < source.size(); i++) {
        char c = source[i];
        if (!isSpace(c)) newSource += c;
    }
    source = newSource;
}

void Lexer::error(std::string message) {
    std::cout << "Lexing Error: " << message << std::endl; 
    std::exit(0);
}

void Lexer::pushToken(std::string text, Ttype type) {
    Token token(text, type); 
    tokens.push_back(token);
}

void Lexer::pushMultiToken(std::string text, std::vector<Ttype> types) {
    if (types.size() == 0 || types.size() != text.length()) return;

    unsigned short newLength = text.length() - 1; unsigned short curLast = newLength; 
    std::string realText = peek(text.length()); 

    // Token match, push and send success signal
    if (text == realText) {
        pushToken(text, types[curLast]); 
        for (unsigned short i = 0; i < text.length() - 1; i++) {
            nextChar(); 
        }
        return;
    }

    // Didnt match, recall with shortened token
    std::string newText = text.substr(0, newLength);
    std::vector<Ttype> newTypes; 
    for (unsigned short i = 0; i < types.size() - 1; i++) {
        newTypes.push_back(types[i]); 
    }
    pushMultiToken(newText, newTypes);
}

void Lexer::pushStringToken() {
    nextChar();
    std::string text;
    while (curChar != '"') {
        text += curChar; 
        nextChar(); 
    }
    pushToken(text, Ttype::STRING);
}

void Lexer::pushNumberToken() {
    std::string text; 
    text += curChar;
    while (std::isdigit(peek())) {
        nextChar();
        text += curChar;
    }
    pushToken(text, Ttype::NUMBER); 
}

bool Lexer::tryPushKeywordToken(std::string text) {
    if (text == "LABEL") pushToken(text, Ttype::LABEL);
    else if (text == "GOTO") pushToken(text, Ttype::GOTO); 
    else if (text == "PRINT") pushToken(text, Ttype::PRINT); 
    else if (text == "INPUT") pushToken(text, Ttype::INPUT); 
    else if (text == "LET") pushToken(text, Ttype::LET); 
    else if (text == "IF") pushToken(text, Ttype::IF); 
    else if (text == "THEN") pushToken(text, Ttype::THEN); 
    else if (text == "ENDIF") pushToken(text, Ttype::ENDIF); 
    else if (text == "WHILE") pushToken(text, Ttype::WHILE); 
    else if (text == "REPEAT") pushToken(text, Ttype::REPEAT); 
    else if (text == "ENDWHILE") pushToken(text, Ttype::ENDWHILE); 
    else return false; 

    return true; 
}

void Lexer::pushIdentKeyToken() {
    std::string text; 
    bool canBeKeyword = true; 

    text += curChar; 
    while (std::isalpha(peek())) {
        nextChar(); 
        text += curChar; 

        if (std::islower(curChar) || std::isdigit(curChar) || curChar == '_') canBeKeyword = false; 

        if (canBeKeyword) {
            if (tryPushKeywordToken(text)) return; 
        }
    }
    pushToken(text, Ttype::IDENT); 
}

void Lexer::check() {
    // Next character, base case if done with source string
    bool hasMore = tryNextChar();
    if (!hasMore) {
        pushToken("\0", Ttype::EOTF); 

        // std::cout << source << std::endl;
        // std::cout << "Tokens: " << tokens.size() << " -> ";
        // for (unsigned short i = 0; i < tokens.size(); i++) {
        //     std::cout << i << ": " << tokens[i].text << " ";
        // }

        return;
    }

    bool foundToken = true; 
    switch (curChar)  {
        // Truly single character tokens
        case '\n': 
            pushToken("\n", Ttype::NEWLINE); 
            break;
        case '+':
            pushToken("+", Ttype::PLUS); 
            break; 
        case '-':
            pushToken("-", Ttype::MINUS); 
            break; 
        case '*':
            pushToken("*", Ttype::ASTERISK); 
            break; 
        case '/':
            pushToken("/", Ttype::SLASH); 
            break; 

        // Single/double if next char corresponds
        case '!': 
            pushMultiToken("!=", {Ttype::NOT, Ttype::NOTEQ}); 
            break; 
        case '=': 
            pushMultiToken("==", {Ttype::EQ, Ttype::EQEQ}); 
            break; 
        case '<': 
            pushMultiToken("<=", {Ttype::LT, Ttype::LTEQ}); 
            break; 
        case '>': 
            pushMultiToken(">=", {Ttype::GT, Ttype::GTEQ}); 
            break; 
        
        // String case
        case '"': 
            pushStringToken(); 
            break; 

        default:
            foundToken = false; 
            break;
        
    }
    if (foundToken) {
        check(); return; 
    }

    // Number and Ident/Keyword special cases
    if (std::isdigit(curChar)) pushNumberToken(); 
    else if (std::isalpha(curChar)) pushIdentKeyToken(); 
    else error("Invalid syntax"); 

    check();
}

Lexer::Lexer(std::string _source) {
    source = _source;

    removeWhiteSpace();
    check();
}

std::vector<Token> Lexer::getTokens() {
    return tokens; 
}