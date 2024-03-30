#pragma once

enum class Ttype {
    EOTF, NEWLINE, NUMBER, IDENT, STRING,
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
};