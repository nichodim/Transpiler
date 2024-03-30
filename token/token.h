#include <iostream>
#pragma once

template <typename Enumeration>
auto as_integer(Enumeration const value)
    -> typename std::underlying_type<Enumeration>::type
{
    return static_cast<typename std::underlying_type<Enumeration>::type>(value);
}

enum class Ttype {
    EOTF = 0, NEWLINE, NUMBER, IDENT, STRING,
    // Keywords.
    LABEL, GOTO, PRINT, INPUT, LET, IF, 
    THEN, ENDIF, WHILE, REPEAT, ENDWHILE,
    // Operators.
    EQ, PLUS, MINUS, ASTERISK, SLASH, 
    EQEQ, NOTEQ, LT, LTEQ, GT, GTEQ, NOT
}; 

class Token {
public:
    std::string text;
    Ttype type;
    Token(std::string _text, Ttype _type) {
        text = _text;
        type = _type;
    }
    void print() {
        std::string TtypeInttoString[] =
        {
            "EOTF", "NEWLINE", "NUMBER", "IDENT", "STRING",
            // Keywords.
            "LABEL", "GOTO", "PRINT", "INPUT", "LET", "IF", 
            "THEN", "ENDIF", "WHILE", "REPEAT", "ENDWHILE",
            // Operators.
            "EQ", "PLUS", "MINUS", "ASTERISK", "SLASH", 
            "EQEQ", "NOTEQ", "LT", "LTEQ", "GT", "GTEQ", "NOT"
        };

        std::string typeAsString = TtypeInttoString[as_integer(type)]; 
        std::cout << "text: " << text << " : " << typeAsString << std::endl; 
    }
};