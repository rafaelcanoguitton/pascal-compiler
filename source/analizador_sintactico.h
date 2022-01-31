#include "parse_tree.h"
#include "analizador_lexico.h"
#include <vector>
#include <string>
#include <iostream>
using namespace std;

class AnalizadorSintactico
{
public:
    ParseTree tree;
    AnalizadorSintactico(){};
    ~AnalizadorSintactico(){};
    void analizar(vector<Token> tokens);
    void parseExpression(vector<Token> tokens, int &pos, ParseTreeNode* &root);//done
    void parseExpression_(vector<Token> tokens, int &pos, ParseTreeNode* &root);//done
    void parseExpression2(vector<Token> tokens, int &pos, ParseTreeNode* &root);//done
    void parseExpression2_(vector<Token> tokens, int &pos, ParseTreeNode* &root);//done
    void parseExpression3(vector<Token> tokens, int &pos, ParseTreeNode* &root);//done
    void parseExpression3_(vector<Token> tokens, int &pos, ParseTreeNode* &root);//done
    void parseFactor(vector<Token> tokens, int &pos, ParseTreeNode* &root);//done
    void parseTerm(vector<Token> tokens, int &pos, ParseTreeNode* &root);//done
    void parseTerm_(vector<Token> tokens, int &pos, ParseTreeNode* &root);//done
    void parseForLoop(vector<Token> tokens, int &pos, ParseTreeNode* &root);//done
    void parseStatementList(vector<Token> tokens, int &pos, ParseTreeNode* &root);//done
    void parseStatement(vector<Token> tokens, int &pos, ParseTreeNode* &root);//done
    void parseIfStatement(vector<Token> tokens, int &pos, ParseTreeNode* &root);//done
    void parseConstBlock(vector<Token> tokens, int &pos, ParseTreeNode* &root);//done
    void parseConstList(vector<Token> tokens, int &pos, ParseTreeNode* &root);//done
    void parseVarBlock(vector<Token> tokens, int &pos, ParseTreeNode* &root);//done
    void parseVarList(vector<Token> tokens, int &pos, ParseTreeNode* &root);//done
    void parseVarDecl(vector<Token> tokens, int &pos, ParseTreeNode* &root);//done
    void parseMainCode(vector<Token> tokens, int &pos, ParseTreeNode* &root);//done
    ParseTree *getParseTree();
    void printParseTree();
    void toJSON(string filename);
};