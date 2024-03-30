#include <fstream>
#include <iostream>

void outputToFile() {
    std::ofstream file("testfile.txt"); 
    file << " hello";
    file.close(); 
}

void inputToFile() {
    std::ifstream file("testfile.txt"); 
    std::string text; 
    file >> text;
    std::cout << text << std::endl; 
    file.close(); 
}

int main ()
{
    // // output
    // std::ofstream file("testfile.txt"); 
    // file << " hello";
    // file.close(); 

    // // input
    // std::ifstream file("testfile.txt"); 
    // std::string text; 
    // file >> text;
    // std::cout << text << std::endl; 
    // file.close(); 

    outputToFile(); 
    inputToFile(); 

    return 0; 
}