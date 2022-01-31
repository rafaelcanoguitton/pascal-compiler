#include "iostream"
#include <vector>
#include <string.h>
#include "compiler.h"
using namespace std;
// main driver function parsing file name and calling the function to read the file
int main(int argc, char *argv[])
{
    bool verbose = false;
    if (argc < 2)
    {
        cout << "Please provide a filename to compile or a flag" << endl;
        return 1;
    }
    if (strcmp(argv[1], "--help") == 0)
    {
        cout << "¡Welcome to the Rafael's Pascal Compiler!\n"
             << "This is a simple compiler for the Pascal programming language.\n"
             << "This project is a part (main part) of the course 'Compilers' at Universidad Católica San Pablo.\n\n"
             << "--help: Show this help message\n"
             << "--compile <filename>: Compile the file <filename>\n"
             << "--output <filename>: Output the compiled file to <filename>\n\n"
             << "Thank you for using this compiler!\n";
        return 1;
    }
    string filename = "";
    string output = "";
    vector<string> temps;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--compile") == 0)
        {
            if (i + 1 < argc)
                filename = argv[i + 1];
            else
            {
                cout << "←(ಠ_ಠ)→" << endl;
                return 1;
            }
        }
        else if (strcmp(argv[i], "--output") == 0)
        {
            if (i + 1 < argc)
                output = argv[i + 1];
            else
            {
                cout << "←(ಠ_ಠ)→" << endl;
                return 1;
            }
        }
        else if (strcmp(argv[i], "--verbose") == 0)
        {
            verbose = true;
        }
        else
        {
            temps.push_back(argv[i]);
        }
    }
    AnalizadorLexico lex;
    if (filename != "")
    {
        lex.setFile(filename);
    }
    else
    {
        lex.setFile(temps[0]);
        filename = temps[0];
    }
    vector<Token> tokens = lex.getTokens();
    cout<<"Tokens: "<<endl;
    cout<<"<"<<"Tipo"<<","<<"Valor"<<">"<<endl<<endl;
    for (auto token : tokens)
    {
        if (verbose)
        {
            token.verbosePrint();
        }
        else
        {
            token.print();
        }
    }
    AnalizadorSintactico sint;
    sint.analizar(tokens);
    sint.toJSON(filename);
    return 0;
}