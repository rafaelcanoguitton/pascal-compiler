// Lexical analizer for pascal language
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
using namespace std;

class Token
{
public:
    Token(string tokentype, string tokenval, int ln, int col);
    string getType();
    string getValue();
    int getLine();
    int getColumn();
    void print();
    void verbosePrint();

private:
    string type;
    string value;
    int line;
    int column;
};
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
