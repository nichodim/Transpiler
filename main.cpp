#include <bits/stdc++.h> 
#include <iostream>
#include <vector> 
#include <algorithm>

enum class Ttype {
    NEWLINE, NUMBER, IDENT, STRING,
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

class Lexer {
private:
    std::string source;
    std::vector<Token> tokens;
    unsigned short curIndex = -1;
    char curChar;

    // Mutate to next character, returns false when all processed
    bool tryNextChar() {
        curIndex++;
        if (curIndex >= source.length()) return false; 
        curChar = source[curIndex]; 
        return true; 
    }
    // Same but returns error instead
    void nextChar() {
        curIndex++;
        if (curIndex >= source.length()) error("Invalid Syntax");
        curChar = source[curIndex]; 
    }

    std::string peek(unsigned short depth) {
        if (curIndex + depth >= source.length()) return "\0";
        return source.substr(curIndex, depth);
    }
    char peek() {
        if (curIndex + 1 >= source.length()) return '\0';
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

    void pushToken(std::string text, Ttype type) {
        Token token(text, type); 
        tokens.push_back(token);
    }

    // When tokens build on previous tokens, pass culminated string of tokens and each type short -> big
    void pushMultiToken(std::string text, std::vector<Ttype> types) {
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
        std::vector<Ttype> newTypes(newLength); std::copy_n(types.begin(), curLast - 1, newTypes.begin()); 
        pushMultiToken(newText, newTypes);
    }

    void pushStringToken() {
        nextChar();
        std::string text;
        while (curChar != '"') {
            text += curChar; 
            nextChar(); 
        }
        pushToken(text, Ttype::STRING);
    }

    void pushNumberToken() {
        std::string text; 
        text += curChar;
        while (std::isdigit(peek())) {
            nextChar();
            text += curChar;
        }
        pushToken(text, Ttype::NUMBER); 
    }

    bool tryPushKeywordToken(std::string text) {
        for (char c : text) {
            if (std::islower(c)) return false;
        }

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

    void pushIdentKeyToken() {
        std::string text; 
        text += curChar; 
        while (std::isalpha(peek())) {
            nextChar(); 
            text += curChar; 
        }

        bool success = tryPushKeywordToken(text);
        if (!success) pushToken(text, Ttype::IDENT); 
    }

    void check() {
        // Next character, base case if done with source string
        bool hasMore = tryNextChar();
        if (!hasMore) {
            std::cout << source << std::endl;
            std::cout << std::endl;
            std::cout << "Tokens: " << tokens.size() << " -> ";
            for (unsigned short i = 0; i < tokens.size(); i++) {
                std::cout << i << ": " << tokens[i].text << " ";
            }
            return;
        }

        bool foundToken = true; 
        switch (curChar)  {
            // Truly single character tokens
            case '\n': 
                std::cout << "new lineeee" << std::endl;
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

public:
    Lexer(std::string _source) {
        source = _source;

        removeWhiteSpace();
        check();
    }
};

int main() {
    // std::cout << "Give source: ";
    // std::string source;
    // std::getline(std::cin, source);

    Lexer lexer("LET 5 = 9 IF 6 != 5 + 45");

    return 0; 
}