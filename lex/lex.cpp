#include <bits/stdc++.h> 
#include <iostream>
#include <vector> 
#include <algorithm>
#include "../token/token.h"
#include "lex.h"

bool Lexer::isSpace(char c) {
    if (c == ' ' || c == '\f' || c == '\r' || c == '\t' || c == '\v') {
        return true;
    } return false;
}

// Source text access points
// Spacing is not deleted but skipped for use in keyword/ident logic
// NextChar iterates the state of accessed source
bool Lexer::tryNextChar() {
    if (++curIndex >= source.length()) return false; 
    while (isSpace(source[curIndex])) {
        if (++curIndex >= source.length()) return false; 
    }
    curChar = source[curIndex]; 
    return true; 
}
void Lexer::nextChar() {
    if (++curIndex >= source.length()) error("Invalid Syntax");
    while (isSpace(source[curIndex])) {
        if (++curIndex >= source.length()) error("Invalid Syntax");
    }
    curChar = source[curIndex]; 
}
// Peek finds the string or character later the the source
std::string Lexer::peek(unsigned short depth) {
    unsigned short peekIndex = curIndex + depth; 
    if (peekIndex >= source.length()) return "\0";
    while (isSpace(source[peekIndex])) {
        if (++peekIndex >= source.length()) return "\0";
    }
    return source.substr(curIndex, peekIndex - curIndex - 1);
}
char Lexer::peek() {
    unsigned short peekIndex = curIndex + 1; 
    if (peekIndex >= source.length()) return '\0';
    while (isSpace(source[peekIndex])) {
        if (++peekIndex >= source.length()) return '\0';
    }
    return source[peekIndex];
}

void Lexer::error(std::string message) {
    std::cout << "Lexing Error: " << message << std::endl; 
    std::cout << "Last text read: " << tokens[tokens.size() - 1].text << std::endl;
    std::exit(0);
}


// Token pushing
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
    bool hasDecimal = false; 
    text += curChar;
    while (std::isdigit(peek()) || peek() == '.') {
        if (curChar == '.') {
            if (hasDecimal) error("Invalid number syntax");
            hasDecimal = true;
        }
        nextChar();
        text += curChar;
    }
    if (text == ".") error("Invalid syntax"); 
    if (hasDecimal) pushToken(text, Ttype::FLOAT); 
    else pushToken(text, Ttype::INTEGER); 
}
bool Lexer::tryPushKeywordToken(std::string text) {
    if (text == "IF") pushToken(text, Ttype::IF); 
    else if (text == "LET") pushToken(text, Ttype::LET); 
    else if (text == "THEN") pushToken(text, Ttype::THEN); 
    else if (text == "GOTO") pushToken(text, Ttype::GOTO); 
    else if (text == "LABEL") pushToken(text, Ttype::LABEL); 
    else if (text == "PRINT") pushToken(text, Ttype::PRINT); 
    else if (text == "INPUT") pushToken(text, Ttype::INPUT); 
    else if (text == "ENDIF") pushToken(text, Ttype::ENDIF); 
    else if (text == "WHILE") pushToken(text, Ttype::WHILE); 
    else if (text == "REPEAT") pushToken(text, Ttype::REPEAT); 
    else if (text == "ENDWHILE") pushToken(text, Ttype::ENDWHILE); 
    else return false; 
    return true; 
}
void Lexer::pushIdentKeyToken() {
    std::string text;
    text += curChar; 
    while (std::isalpha(peek()) || std::isdigit(peek()) || peek() == '_') {
        if (source[curIndex + 1] != peek()) break; // passes if actual next char is space
        nextChar(); 
        text += curChar; 
    }
    if (!tryPushKeywordToken(text)) pushToken(text, Ttype::IDENT);
}

// Continually finds what type of token to push
void Lexer::check() {
    bool hasMore = tryNextChar();
    if (!hasMore) {
        pushToken("\0", Ttype::EOTF); 
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
    if (std::isdigit(curChar) || curChar == '.') pushNumberToken(); 
    else if (std::isalpha(curChar) || curChar == '_') pushIdentKeyToken(); 
    else error("Invalid syntax"); 

    check();
}

Lexer::Lexer(std::string _source) {
    source = _source;
    check();
}

std::vector<Token> Lexer::getTokens() {
    return tokens; 
}