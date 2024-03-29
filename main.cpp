#include <bits/stdc++.h> 
#include <iostream>
#include <string>
#include <vector> 

class TokenTypes {
    public:
        enum tokens {
            EOF, NEWLINE, NUMBER, IDENT, STRING,
            // Keywords.
            LABEL, GOTO, PRINT, INPUT, LET, IF, 
            THEN, ENDIF, WHILE, REPEAT, ENDWHILE
            // Operators.
            EQ, PLUS, MINUS, ASTERISK, SLASH, 
            EQEQ, NOTEQ, LT, LTEQ, GT, GTEQ
        }; 
}; 

class Lexer {
    std::string source;
    std::vector<char> tokens = [];
    int curIndex = -1;
    char curChar;

    // Mutate to next character, returns false when processed source
    bool nextChar() {
        curIndex++;
        if (curIndex >= source.length()) {
            return False; 
        }

        curChar = source[curIndex]; 
        return True; 
    }

    // See next character for token checks
    char peek() {
        return source[curIndex + 1]
    }

    // Lexer Error, ends program
    void error(std::string message) {
        std::cout << message << std::endl; 
        std::exit();
    }

    public:
        Lexer(std::string _source) {
            source = _source;
            nextChar();
        }

        void print() {
            std::cout << curIndex << curChar << std::endl;
        }
};

int main() {
    std::cout << "Give source:\n";
    std::string source;
    std::cin >> source;

    TokenTypes tokentypes;
    Lexer lexer(source);

    // std::cout << tokentypes << std::endl

    return 0; 
}