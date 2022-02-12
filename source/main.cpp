#include "iostream"
#include <vector>
#include <string.h>
#include "compiler.h"
#include <stdio.h>
using namespace std;

void invokePythonAndOpenInCode(string filename,string program){
    //navigate to source directory
    FILE *fp;
    //delete file extension
    string fileAndCommand ="cd .. && cd source && python parseTreeVisualization.py && cd .. && cd visualizations && "+ program + " "+ filename.substr(0, filename.find(".")) + ".png";
    char command[fileAndCommand.length()+1];
    strcpy(command, fileAndCommand.c_str());
    //open file in code
    char buffer[1234];
    fp = popen(
        command, "r");
    while(fgets(buffer, sizeof(buffer), fp) != NULL){
        printf("%s", buffer);
    }
    pclose(fp);
    // fp = popen(
    //     fileAndCommand.c_str(), "r");
}
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
             << "--verbose: Show verbose messages\n"
             << "--visualize <program name>: Visualize the parse tree of the program <program name>\n"
             << "--run : Run the program\n"
             << "--open : Open the program in code\n"
             << "Thank you for using this compiler!\n";
        return 1;
    }
    string filename = "";
    string output = "";
    string program = "";
    vector<string> temps;
    bool visualization=false;
    bool run=false;
    bool outputFile=false;
    bool openInCode=false;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--compile") == 0)
        {
            if (i + 1 < argc){
                filename = argv[i + 1];
                i++;
            }
            else
            {
                cout << "←(ಠ_ಠ)→" << endl;
                return 1;
            }
        }
        else if (strcmp(argv[i], "--output") == 0)
        {
            if (i + 1 < argc){
                output = argv[i + 1];
                i++;
            }
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
        else if (strcmp(argv[i],"--visualize")==0){
            visualization=true;
            program = argv[i+1];
            i++;
        }
        else if (strcmp(argv[i],"--run")==0){
            run=true;
        }
        else if (strcmp(argv[i],"--open")==0){
            openInCode=true;
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
    if(sint.errors.size()>0){
        for(auto error:sint.errors){
            cout<<error<<endl;
        }
    } else {
        AnalizadorSemantico sem(sint.getParseTree());
        sem.check();
        for(int i= 0; i<sem.errors->size();i++){
             cout<<sem.errors->at(i)<<endl;
        }
        if(sem.errors->size()!=0) return 1;
        sint.toJSON(filename);
        if( visualization ){
            invokePythonAndOpenInCode(filename,program);
        }
        CodeGenerator gen(sint.getParseTree());
        gen.generate();
        cout<<"\033[0;36m";
        gen.printCode();
        cout<<"\033[0m";
        if(outputFile){
            gen.saveToFile(output);
            if(run){
                cout<<"\033[0;31m"<<"Running python file: "<<output<<"\033[0m"<<endl;
                gen.runCode(output);
            }
            if(openInCode){
                string command = "code "+output;
                system(command.c_str());
            }
        } else {
            string pythonFilename = filename.substr(0, filename.find(".")) + ".py";
            gen.saveToFile(pythonFilename);
            if(run){
                cout<<"\033[0;31m"<<"Running python file: "<<pythonFilename<<"\033[0m"<<endl;
                gen.runCode(pythonFilename);
            }
            if(openInCode){
                string command = "code "+pythonFilename;
                system(command.c_str());
            }
        }
    }
    return 0;
}