// Lexical analizer for pascal language
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include "token.h"
using namespace std;

class AnalizadorLexico
{
public:
    AnalizadorLexico();
    AnalizadorLexico(string filename);
    void setFile(string filename);
    char getChar();
    char peekChar();
    char peek2Char();
    string getRestOfLine();//for debugging purposes
    vector<Token> getTokens();
    void printTokens();

private:
    ifstream file;
    string line;
    int pointer;
    int currLine;
    vector<Token> tokens;
};
