#include <iostream>
#include <string>

class Lexer {
    std::string source;
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

    Lexer lexer(source);

    return 0; 
}