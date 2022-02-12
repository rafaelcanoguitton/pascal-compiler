#include "analizador_semantico.h"

/** Objective:
 * Literally simply generate python equivalent code
 * thats it
 */
class CodeGenerator{
    public:
        ParseTree *tree;
        string* code;
        CodeGenerator(ParseTree *tree);
        void generateIf(ParseTreeNode *ifNode,string tabs);
        void generateFor(ParseTreeNode *forNode,string tabs);
        void generateWriteLn(ParseTreeNode *writeLnNode,string tabs);
        void generateWrite(ParseTreeNode *writeNode,string tabs);
        void generateAssign(ParseTreeNode *assignNode,string tabs);
        void generateStatementList(ParseTreeNode *statementList,string tabs);
        void generateExpression(ParseTreeNode *expression,string tabs);
        void generateConstBlock(ParseTreeNode *ConstBlock,string tabs);
        void printCode();
        void saveToFile(string fileName);
        void runCode(string filename);
        void generate();
};