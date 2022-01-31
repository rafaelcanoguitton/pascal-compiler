#include "token.h"
#include <iostream>
#include <string>
using namespace std;
const std::string red("\033[0;31m");
const std::string green("\033[1;32m");
const std::string yellow("\033[1;33m");
const std::string cyan("\033[0;36m");
const std::string magenta("\033[0;35m");
const std::string reset("\033[0m");
Token::Token(string tokentype, string tokenval, int ln, int col)
{
    type = tokentype;
    value = tokenval;
    line = ln;
    column = col;
}
int Token::getColumn()
{
    return column;
}
int Token::getLine()
{
    return line;
}
string Token::getType()
{
    return type;
}
string Token::getValue()
{
    return value;
}
void Token::print()
{

    cout << magenta << "<";
    cout << red << type;
    cout << reset << ",";
    cout << green << value;
    cout << magenta << ">" << reset << endl;
}
void Token::verbosePrint()
{
    cout << magenta << "<";
    cout << red << type;
    cout << reset << ",";
    cout << green << value;
    cout << magenta << ">" << reset;

    cout << cyan << " at line " + to_string(line) + " and column " + to_string(column) << reset << endl;
}