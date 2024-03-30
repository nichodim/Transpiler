#include <vector> 
#include "../token/token.h"
#pragma once

class Lexer {
private:
    std::string source;
    std::vector<Token> tokens;
    unsigned short curIndex = -1;
    char curChar;

    // Mutate to next character, returns false when all processed
    bool tryNextChar();
    // Same but returns error instead
    void nextChar();

    std::string peek(unsigned short depth);
    char peek();

    // Removes whitespace except \n
    bool isSpace(char c);
    void removeWhiteSpace();

    // Lexer Error, ends program
    void error(std::string message);

    void pushToken(std::string text, Ttype type);

    // When tokens build on previous tokens, pass culminated string of tokens and each type short -> big
    void pushMultiToken(std::string text, std::vector<Ttype> types);
    void pushStringToken();
    void pushNumberToken(); 
    bool tryPushKeywordToken(std::string text); 
    void pushIdentKeyToken(); 

    void check();

public:
    Lexer(std::string _source);
};