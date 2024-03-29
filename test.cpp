#include <iostream>

using namespace std;

int main ()
{
    string Word, Sentence;

    cout << "Please enter a word: "; getline(cin, Word); 
    cout << endl << Word;

    cout << "\nPlease enter a sentence: "; getline(cin, Sentence); 
    cout << endl << Sentence;
}