#include "analizador_lexico.h"
#include "iostream"
#include "constraints.h"
#include "stdlib.h"
#include <cctype>
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
AnalizadorLexico::AnalizadorLexico(string filename)
{
    file.open(filename);
    pointer = 0;
    currLine = 1;
    line = "";
}
AnalizadorLexico::AnalizadorLexico()
{
    pointer = 0;
    currLine = 1;
}
void AnalizadorLexico::setFile(string filename)
{
    file.open(filename);
    pointer = 0;
}
char AnalizadorLexico::getChar()
{
    char c = file.get();
    pointer++;
    line += c;
    return c;
}
char AnalizadorLexico::peekChar()
{
    char c = file.peek();
    return c;
}
char AnalizadorLexico::peek2Char()
{
    file.seekg(pointer + 1);
    char c = file.peek();
    return c;
}
string AnalizadorLexico::getRestOfLine()
{
    // under construction
    string rest = "";
    char currChar;
    for (int i = pointer; currChar != '\n' || currChar != EOF; i++)
    {
        currChar = file.seekg(i).peek();
        rest += currChar;
        cout << currChar;
    }
    return rest;
}
void AnalizadorLexico::printTokens()
{
    for (int i = 0; i < tokens.size(); i++)
    {
        tokens[i].print();
    }
}
vector<Token> AnalizadorLexico::getTokens()
{
    char c = getChar();
    vector<string> errors;
    while (c != EOF || c == '.')
    {
        if (c == '\n')
        {
            currLine++;
            pointer = 0;
            line = "";
            c = getChar();
            continue;
        }
        if (c == ' ' || c == '\t')
        {
            c = getChar();
            continue;
        }
        // comentarios
        if (c == '{')
        {
            while (c != '}')
            {
                c = getChar();
                if (c == '\n')
                {
                    currLine++;
                    pointer = 0;
                    line = "";
                }
                if(c == EOF){
                    errors.push_back("Error: comentario no cerrado");
                    break;
                }
            }
            c = getChar();
            continue;
        }
        if (c == '(' && peekChar() == '*'){
            c = getChar();
            c = getChar();
            while (c != '*' || peekChar() != ')')
            {
                c = getChar();
                if (c == '\n')
                {
                    currLine++;
                    pointer = 0;
                    line = "";
                }
                if(c == EOF){
                    errors.push_back("Error: comentario no cerrado");
                    break;
                }
            }
            c = getChar();
            c = getChar();
            continue;
        }
        // comentarios de una línea
        // if (c == '(' && peekChar() == '*')
        // {
        //     getChar();
        //     c = getChar();
        //     while (c != '*' || peekChar() != ')')
        //     {
        //         c = getChar();
        //         if (peekChar() == '\n')
        //         {
        //             currLine++;
        //             string linewSpaces(pointer, ' ');
        //             pointer = 0;
        //             errors.push_back("Error: comentario no cerrado en linea " + to_string(currLine) + " columna " + to_string(pointer) + "\n" +
        //                              line + "\n" + linewSpaces + "^" + "\n");
        //             break;
        //         }
        //     }
        //     getChar();
        //     c = getChar();
        //     continue;
        // }
        // strings
        if (c == '\'')
        {
            string str = "'";
            c = getChar();
            while (c != '\'')
            {
                if(peekChar() =='\''){
                    str += c;
                    c=getChar();
                    if(peekChar() =='\''){
                        str += '\'';
                        getChar();
                        c = getChar();
                        continue;
                    }
                    else{
                        break;
                    }
                }
                if(c == '\n' ){
                    currLine++;
                    pointer = 0;
                    line = "";
                }
                if (c == EOF)
                {
                    errors.push_back("Error: string no cerrado en linea " + to_string(currLine) + " columna " + to_string(pointer) + "\n" +
                    line + "\n" + string(pointer, ' ') + "^" + "\n");
                    break;
                }
                str += c;
                c = getChar();
            }
            if (c != '\n' && c != EOF)
            {
                str += c;
            }
            tokens.push_back(Token("STRING", str, currLine, pointer));
            c = getChar();
            continue;
        }
        // números (que dice la miss que solo son unsigned y floats)
        if (c >= '0' && c <= '9')
        {
            string num = "";
            bool isFloat = false;
            bool hasE = false;
            bool foundSpecialOperator =false;
            bool foundSpecialCase = false;
            while (c >= '0' && c <= '9' || c == '.' || c == 'e' || c == 'E')
            {
                if (c == '.')
                {
                    if(peekChar() =='.'){
                        foundSpecialOperator = true;
                        break;
                    }
                    if(isFloat){
                        string linewSpaces(pointer, ' ');
                        errors.push_back("Error: número no válido en linea " + to_string(currLine) + " columna " + to_string(pointer) + "\n" +
                                         line + "\n" + linewSpaces + "^" + "\n");
                        break;
                    }else{
                        isFloat = true;
                    }
                }
                if (c == 'e' || c == 'E')
                {
                    isFloat = true;
                    char temp = peekChar();
                    if(hasE){
                        string linewSpaces(pointer, ' ');
                        errors.push_back("Error: número no válido en linea " + to_string(currLine) + " columna " + to_string(pointer) + "\n" +
                                         line + "\n" + linewSpaces + "^" + "\n");
                    }else{
                        hasE=true;
                    }
                    if (temp == '+' || temp == '-')
                    {
                        
                        if(!(peekChar()>='0' && peekChar()<='9')){
                            
                            string linewSpaces(pointer, ' ');
                            errors.push_back("Error: número no válido en linea " + to_string(currLine) + " columna " + to_string(pointer) + "\n" +
                                             line + "\n" + linewSpaces + "^" + "\n");
                            break;
                        }
                        else{
                            num += c;
                            c = getChar();
                        }
                    }
                    else
                    {
                        if (temp >= '0' && temp <= '9')
                        {
                            num += c;
                            c = getChar();
                        }
                        else
                        {
                            break;
                        }
                    }
                }
                num += c;
                c = getChar();
            }
            if (isFloat)
            {
                tokens.push_back(Token("FLOAT", num, currLine, pointer));
            }
            else
            {
                tokens.push_back(Token("INT", num, currLine, pointer));
            }
            if(c == '\n'){
                currLine++;
                pointer = 0;
                line = "";
            }
            if(!foundSpecialOperator){
                c = getChar();
            }
            if(foundSpecialCase){
                tokens.push_back(Token("ID", "e", currLine, pointer));
            }
            continue;
        }
        // operadores
        string opAux = "";
        opAux += c;
        if (operators.find(opAux) != operators.end() || c == ':')
        {
            string op = "";
            op += c;
            string peak = "";
            peak += peekChar();
            if (opAux != ":" || (opAux == ":" && peak == "="))
            {
                if (operators.find(peak) != operators.end())
                {
                    op += getChar();
                }
                tokens.push_back(Token(operators[op], op, currLine, pointer));
                c = getChar();
                continue;
            }
            //else we go to delimeters
        }
        // delimitadores
        if (delimitadores.find(c) != delimitadores.end())
        {
            if(c =='.'&&peekChar()=='.'){
                tokens.push_back(Token("ITER", "..", currLine, pointer));
                getChar();
                c = getChar();
            }else{
                string charToString = "";
                charToString += c;
                tokens.push_back(Token(delimitadores[c], charToString, currLine, pointer));
                c = getChar();
            }
            continue;
        }
        // identificadores
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
        {
            string id = "";
            while ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c >= '0' && c <= '9')
            {
                id += c;
                c = getChar();
            }
            string tolow = "";
            for (int i = 0; i < id.size(); i++)
            {
                tolow += tolower(id[i]);
            }
            // palabras reservadas
            if (palabras_reservadas.find((tolow)) != palabras_reservadas.end())
            {
                tokens.push_back(Token(palabras_reservadas[tolow], id, currLine, pointer));
            }
            //operadores compuestos
            else if(compound_operators.find(tolow) != compound_operators.end()){
                tokens.push_back(Token(compound_operators[tolow], id, currLine, pointer));
            }
            else
            {
                tokens.push_back(Token("ID", id, currLine, pointer));
            }
            continue;
        }
        // caracteres no reconocidos
        if (c != EOF)
        {
            cout << "Caracter invalido: " << c << endl;
            c = getChar();
            continue;
        }
    }
    for (auto error : errors)
    {
        cout << error;
    }
    return tokens;
}