#include "analizador_sintactico.h"
#include <map>
/** Objective:
  * Type checking
  * Label checking
  * Flow Control Checking
  */

class AnalizadorSemantico {
    public:
      ParseTree *tree;
      std::vector<std::string> *errors;
      AnalizadorSemantico(ParseTree *tree);
      void ParseConstBlock(ParseTreeNode* constList, std::map<std::string, string> *table);
      void ParseVarBlock(ParseTreeNode* varList, std::map<std::string, string> *table);
      void ParseStatementList(ParseTreeNode* statementList, std::map<std::string, string> *table);
      void ParseAssign(ParseTreeNode* assign, std::map<std::string, string> *table);
      void ParseIf(ParseTreeNode* ifNode, std::map<std::string, string> *table);
      void parseFor(ParseTreeNode* forNode, std::map<std::string,string>*table);
      void parseWrite(ParseTreeNode* writeNode, std::map<std::string,string>*table);
      void parseWriteLn(ParseTreeNode* writeLnNode, std::map<std::string,string>*table);
      void checkEveryLeafForDataType(ParseTreeNode* root, string datatype, std::map<std::string, string> *table);
      void findDatatypeAndCheckEveryLeaf(ParseTreeNode* root, std::map<std::string, string> *table);
      void check();
};