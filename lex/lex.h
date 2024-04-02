#include <vector> 
#include "../token/token.h"
#pragma once

class Lexer {
private:
    std::string source;
    std::vector<Token> tokens;
    unsigned short curIndex = -1;
    char curChar;

    bool isSpace(char c); // All spacing except \n
    void error(std::string message); // Lexer Error, ends program
    void pushToken(std::string text, Ttype type);

    // Source text access points
    // Spacing is not deleted but skipped for use in keyword/ident logic
    // NextChar iterates the state of accessed source
    bool tryNextChar();
    void nextChar(); // returns errors
    std::string peek(unsigned short depth);
    char peek();

    // When tokens build on previous tokens, pass culminated string of tokens and each type short -> big
    void pushMultiToken(std::string text, std::vector<Ttype> types);
    void pushStringToken();
    void pushNumberToken(); 
    bool tryPushKeywordToken(std::string text); 
    void pushIdentKeyToken(); 

    void check();

public:
    Lexer(std::string _source);

    std::vector<Token> getTokens(); 
};