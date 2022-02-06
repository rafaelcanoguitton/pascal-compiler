#include "parse_tree.h"
#include "analizador_lexico.h"
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
using namespace std;

class AnalizadorSintactico
{
public:
    ParseTree tree;
    vector<string> errors;
    AnalizadorSintactico(){};
    ~AnalizadorSintactico(){};
    void analizar(vector<Token> tokens);
    void parseExpression(vector<Token> tokens, int &pos, ParseTreeNode* &root);
    void parseExpression_(vector<Token> tokens, int &pos, ParseTreeNode* &root);
    void parseExpression2(vector<Token> tokens, int &pos, ParseTreeNode* &root);
    void parseExpression2_(vector<Token> tokens, int &pos, ParseTreeNode* &root);
    void parseExpression3(vector<Token> tokens, int &pos, ParseTreeNode* &root);
    void parseExpression3_(vector<Token> tokens, int &pos, ParseTreeNode* &root);
    void parseFactor(vector<Token> tokens, int &pos, ParseTreeNode* &root);
    void parseTerm(vector<Token> tokens, int &pos, ParseTreeNode* &root);
    void parseTerm_(vector<Token> tokens, int &pos, ParseTreeNode* &root);
    void parseForLoop(vector<Token> tokens, int &pos, ParseTreeNode* &root);
    void parseStatementList(vector<Token> tokens, int &pos, ParseTreeNode* &root);
    void parseStatement(vector<Token> tokens, int &pos, ParseTreeNode* &root);
    void parseIfStatement(vector<Token> tokens, int &pos, ParseTreeNode* &root);
    void parseConstBlock(vector<Token> tokens, int &pos, ParseTreeNode* &root);
    void parseConstList(vector<Token> tokens, int &pos, ParseTreeNode* &root);
    void parseVarBlock(vector<Token> tokens, int &pos, ParseTreeNode* &root);
    void parseVarList(vector<Token> tokens, int &pos, ParseTreeNode* &root);
    void parseVarDecl(vector<Token> tokens, int &pos, ParseTreeNode* &root);
    void parseMainCode(vector<Token> tokens, int &pos, ParseTreeNode* &root);
    void panicModeRecovery(vector<Token> tokens, int &pos, ParseTreeNode* &root, vector<string> expected);
    ParseTree *getParseTree();
    void printParseTree();
    void toJSON(string filename);
};