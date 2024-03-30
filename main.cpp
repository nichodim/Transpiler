#include <fstream>
#include <vector>
#include "lex/lex.h"
#include "token/token.h"
#include "parse.cpp"

#define fileName ("testfile.txt") 

std::string readFile() {
    std::string fileText, curLine; 
    std::ifstream file(fileName); 
    while (std::getline(file, curLine)) {
        fileText += curLine; 
        fileText += '\n'; 
    }
    file.close(); 
    return fileText; 
}

int main() {
    Lexer lexer(readFile());
    Parser parser(lexer.getTokens()); 

    return 0; 
}