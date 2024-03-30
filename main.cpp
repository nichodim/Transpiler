#include <fstream>
#include "lex/lex.h"

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
    std::string fileText = readFile(); 
    Lexer lexer(fileText);

    return 0; 
}