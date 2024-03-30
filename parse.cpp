#include <vector>
#include <iostream>
#include "token/token.h"

class Parser {
private:
    std::vector<Token> tokens;

public:
    Parser(std::vector<Token> _tokens) {
        tokens = _tokens;
        std::cout << std::endl;
        for (Token token : tokens) {
            std::cout << token.text << " "; 
        }
    }
}; 