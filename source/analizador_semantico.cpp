#include "analizador_semantico.h"
AnalizadorSemantico::AnalizadorSemantico(ParseTree *tree) {
    this->tree = tree;
    this->errors = new std::vector<std::string>();
}
void AnalizadorSemantico::ParseConstBlock(ParseTreeNode* constList, std::map<std::string, string> *table) {
    if(constList->children.size()>1){
        table->insert(std::pair<std::string, string>(constList->children[0]->name, constList->children[2]->value));
    }
    if (constList->children.size() > 4) {
        ParseConstBlock(constList->children[4], table);
    }
}
void AnalizadorSemantico::ParseVarBlock(ParseTreeNode* varList, std::map<std::string, string> *table) {
    if(varList->children.size()>1){
        vector<string> vars;
        ParseTreeNode *varDecl = varList->children[0];
        while(true){
            vars.push_back(varDecl->children[0]->name);
            if(varDecl->children.size()>1){
                varDecl = varDecl->children[2];
            }else{
                break;
            }
        }
        string type;
        if(varList->children[2]->children[0]->name == "INTEGER_TYPE")
            type = "INT";
        else if(varList->children[2]->children[0]->name == "FLOAT_TYPE")
            type = "FLOAT";
        else if(varList->children[2]->children[0]->name == "STRING_TYPE")
            type = "STRING";
        for(int i=0; i<vars.size(); i++){
            table->insert(std::pair<std::string, string>(vars[i], type));
        }
        if (varList->children.size() > 4) {
            ParseVarBlock(varList->children[4], table);
        }
    }
}
void AnalizadorSemantico::ParseStatementList(ParseTreeNode* statementList, std::map<std::string, string> *table) {
    if(statementList->children[0]->children[0]->name == "forStatement"){
        parseFor(statementList->children[0]->children[0], table);
    }
    else if(statementList->children[0]->children[0]->name=="if"){
        ParseIf(statementList->children[0]->children[0], table);
    }
    else if (statementList->children[0]->children[0]->name=="ASSIGN"){
        ParseAssign(statementList->children[0]->children[0], table);
    }
    else if (statementList->children[0]->children[0]->name=="WRITELN"){
        parseWriteLn(statementList->children[0]->children[0], table);
    }
    else if (statementList->children[0]->children[0]->name=="WRITE"){
        parseWrite(statementList->children[0]->children[0], table);
    }
    if(statementList->children.size()>1){
        ParseStatementList(statementList->children[1], table);
    }
}
void AnalizadorSemantico::ParseAssign(ParseTreeNode* assign, std::map<std::string, string> *table) {
    string datatype = table->find(assign->children[0]->name)->second;
    checkEveryLeafForDataType(assign->children[2],datatype, table);
}
void AnalizadorSemantico::ParseIf(ParseTreeNode* ifNode, std::map<std::string, string> *table) {
    findDatatypeAndCheckEveryLeaf(ifNode->children[1], table);
    ParseStatementList(ifNode->children[5],table);
    ParseStatementList(ifNode->children[10],table);
}
void AnalizadorSemantico::parseFor(ParseTreeNode* forNode, std::map<std::string,string>*table){
    string datatype = forNode->children[1]->value;
    checkEveryLeafForDataType(forNode->children[5],datatype, table);
}
void AnalizadorSemantico::parseWrite(ParseTreeNode* writeNode, std::map<std::string,string>*table){
    //literally don't do anything bc the parser takes care of this
}
void AnalizadorSemantico::parseWriteLn(ParseTreeNode* writeLnNode, std::map<std::string,string>*table){
    //literally don't do anything bc the parser takes care of this
}
void AnalizadorSemantico::checkEveryLeafForDataType(ParseTreeNode* root, string datatype,std::map<std::string,string>*table){
    //first check if the root is a leaf
    if(root->children.size()==0){
        // this also accounts for undeclared variables or constants im so smart
        string type = table->find(root->name)->second;
        //cout<<"name: "<<root->name<<" type: "<<type<<endl;
        if(!operators.count(root->name) && root->value != datatype && type!=datatype && root->name!=""){
            errors->push_back("Error on line "+to_string(root->line)+" column "+to_string(root->column)+" a "+datatype+" datatype was expected but got "+root->value);
        }
    } else{
        for(int i=0; i<root->children.size(); i++){
            checkEveryLeafForDataType(root->children[i], datatype, table);
        }
    }
}
void AnalizadorSemantico::findDatatypeAndCheckEveryLeaf(ParseTreeNode* root, std::map<std::string,string>*table){
    //go to leftmost leaf to find prevalent datatype
    ParseTreeNode* temp=root;
    while(temp->children.size()>=1){
        temp=temp->children[0];
    }
    string datatype=table->find(temp->name)->second;
    checkEveryLeafForDataType(root,datatype,table);
}
void AnalizadorSemantico::check() {
    std::map<std::string, std::string> *table = new std::map<std::string, std::string>();
    //first we reach ConstBlock and add every variable to the map
    ParseTreeNode *node = tree->root->children[0]->children[2];
    if(node->children.size() != 0 && node->children[0]->name != "") {
        ParseConstBlock(node->children[1], table);
    }
    //Now we do the same for VarBlock
    node = tree->root->children[0]->children[3]->children[1];
    if(node->children.size() != 0 && node->children[1]->name != "") {
        ParseVarBlock(node, table);
    }
    //Now we do the same for StatementList
    node = tree->root->children[0]->children[4];
    if(node->children.size() != 0 && node->children[1]->name != "") {
        ParseStatementList(node->children[1], table);
    }
    //print keys and values
    for(auto it = table->begin(); it != table->end(); ++it) {
        std::cout << it->first << " => " << it->second << '\n';
    }
}