#include <fstream>
#pragma once

class Emitter {
    std::string head;
    std::string body;

public:
    void pushToHead(std::string text) {
        head += text;
    }
    void pushToBody(std::string text) {
        body += text;
    }
    void pushBoilerPlate() {
        pushToHead("#include <stdio.h>\nint main(void){\n");
        pushToBody("return 0;\n}");
    }
    void pushText() {
        pushBoilerPlate(); 
        std::ofstream file("result.txt"); 
        file << head;
        file << body;
        file.close(); 
    }
};