#include <iostream>
#include "token.h"

template <typename Enumeration>
auto as_integer(Enumeration const value)
    -> typename std::underlying_type<Enumeration>::type
{
    return static_cast<typename std::underlying_type<Enumeration>::type>(value);
}
std::string TtypeInttoString[] = {
    "EOTF", "NEWLINE", "NUMBER", "IDENT", "STRING",
    // Keywords.
    "LABEL", "GOTO", "PRINT", "INPUT", "LET", "IF", 
    "THEN", "ENDIF", "WHILE", "REPEAT", "ENDWHILE",
    // Operators.
    "EQ", "PLUS", "MINUS", "ASTERISK", "SLASH", 
    "EQEQ", "NOTEQ", "LT", "LTEQ", "GT", "GTEQ", "NOT"
};

Token::Token(std::string _text, Ttype _type) {
    text = _text;
    type = _type;
}
void Token::print() {
    std::string outputText = text;
    if (type == Ttype::NEWLINE) outputText = "/n";
    if (type == Ttype::EOTF) outputText = "/0";
    std::string typeAsString = TtypeInttoString[as_integer(type)]; 
    std::cout << outputText << " : " << typeAsString << std::endl; 
}