#include <bits/stdc++.h> 
#include <iostream>
#include <vector> 

enum class Ttype {
    NEWLINE, NUMBER, IDENT, STRING,
    // Keywords.
    LABEL, GOTO, PRINT, INPUT, LET, IF, 
    THEN, ENDIF, WHILE, REPEAT, ENDWHILE,
    // Operators.
    EQ, PLUS, MINUS, ASTERISK, SLASH, 
    EQEQ, NOTEQ, LT, LTEQ, GT, GTEQ
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

class Lexer {
private:
    std::string source;
    std::vector<Token> tokens;
    unsigned short curIndex = -1;
    char curChar;

    void print() {
        std::cout << curIndex << " : " << curChar << std::endl;
    }

    // Mutate to next character, returns false when all processed
    bool nextChar() {
        curIndex++;
        if (curIndex >= source.length()) {
            return false; 
        }

        curChar = source[curIndex]; 
        return true; 
    }
    char peek() {
        return source[curIndex + 1];
    }

    // Removes whitespace except \n
    bool isSpace(char c) {
        if (c == ' ' || c == '\f' || c == '\r' || c == '\t' || c == '\v') {
            return true;
        } return false;
    }
    void removeWhiteSpace() {
        std::string newSource;
        for (unsigned short i = 0; i < source.size(); i++) {
            char c = source[i];
            if (!isSpace(c)) newSource += c;
        }
        source = newSource;
    }

    // Lexer Error, ends program
    void error(std::string message) {
        std::cout << "Lexing Error: " << message << std::endl; 
        std::exit(0);
    }

    void check() {
        bool hasMore = nextChar();
        if (!hasMore) {
            std::cout << std::endl;
            std::cout << "Tokens: ";
            for (unsigned short i = 0; i < tokens.size(); i++) {
                std::cout << tokens[i].text << " ";
            }
            std::cout << std::endl;
            return;
        }
        std::cout << curChar << " ";

        switch (curChar)  {
            // Truly single character tokens
            case '\n': 
                { Token token("\n", Ttype::NEWLINE); tokens.push_back(token); }
                break;
            case '+':
                { Token token("+", Ttype::PLUS); tokens.push_back(token); }
                break; 
            case '-':
                { Token token("-", Ttype::MINUS); tokens.push_back(token); }
                break; 
            case '*':
                { Token token("*", Ttype::ASTERISK); tokens.push_back(token); }
                break; 
            case '/':
                { Token token("/", Ttype::SLASH); tokens.push_back(token); }
                break; 
            default:
                // error("Invalid syntax");
                break;
        }
        check();
    }

public:
    Lexer(std::string _source) {
        source = _source;

        removeWhiteSpace();
        check();
    }
};

int main() {
    std::cout << "Give source: ";
    std::string source;
    std::getline(std::cin, source);

    Lexer lexer(source);

    return 0; 
}