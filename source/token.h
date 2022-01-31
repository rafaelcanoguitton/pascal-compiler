
#include <iostream>
#include <string>
class Token
{
public:
    Token(std::string tokentype, std::string tokenval, int ln, int col);
    std::string getType();
    std::string getValue();
    int getLine();
    int getColumn();
    void print();
    void verbosePrint();

private:
    std::string type;
    std::string value;
    int line;
    int column;
};