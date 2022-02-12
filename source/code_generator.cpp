#include "code_generator.h"

CodeGenerator::CodeGenerator(ParseTree *tree){
    this->tree = tree;
    this->code = new string();
}
void CodeGenerator::generate(){
    if(tree->root->children[0]->children[2]->children[0]->name!=""){
        generateConstBlock(tree->root->children[0]->children[2]->children[1], "");
    }
    generateStatementList(tree->root->children[0]->children[4]->children[1], "");
}
void CodeGenerator::generateStatementList(ParseTreeNode *statementList,string tabs){
    if(statementList->children[0]->children[0]->name=="forStatement"){
        generateFor(statementList->children[0]->children[0],tabs);
    }else if(statementList->children[0]->children[0]->name=="if"){
        generateIf(statementList->children[0]->children[0],tabs);
    }
    else if (statementList->children[0]->children[0]->name=="ASSIGN"){
        generateAssign(statementList->children[0]->children[0],tabs);
    }
    else if (statementList->children[0]->children[0]->name=="WRITELN"){
        generateWriteLn(statementList->children[0]->children[0],tabs);
    }
    else if (statementList->children[0]->children[0]->name=="WRITE"){
        generateWrite(statementList->children[0]->children[0],tabs);
    }
    if(statementList->children.size()>1){
        generateStatementList(statementList->children[1],tabs);
    }
}
void CodeGenerator::generateIf(ParseTreeNode *ifNode,string tabs){
    *code += tabs + "if ";
    generateExpression(ifNode->children[1],tabs);
    *code += ":\n";
    generateStatementList(ifNode->children[5],tabs+"\t");
    *code += tabs + "else:\n";
    generateStatementList(ifNode->children[10],tabs+"\t");
}
void CodeGenerator::generateFor(ParseTreeNode *forNode,string tabs){
    *code += tabs + "for " + forNode->children[1]->name + " in range(" + forNode->children[3]->children[0]->name + ", ";
    generateExpression(forNode->children[5]->children[0],tabs);
    *code += "):\n";
    generateStatementList(forNode->children[8],tabs+"\t");
}
void CodeGenerator::generateWriteLn(ParseTreeNode *writeLnNode,string tabs){
    *code += tabs + "print("+writeLnNode->children[2]->children[0]->name+")\n";
}
void CodeGenerator::generateWrite(ParseTreeNode *writeNode,string tabs){
    //*code += tabs + "print("+writeNode->children[2]->children[0]->name+")\n";
    *code += tabs + "print(";
    generateExpression(writeNode->children[2],tabs);
    *code +=")\n";
}
void CodeGenerator::generateAssign(ParseTreeNode *assignNode,string tabs){
    *code += tabs + assignNode->children[0]->name + " = ";
    generateExpression(assignNode->children[2]->children[0],tabs);
    *code += "\n";
}
void CodeGenerator::generateExpression(ParseTreeNode *expression,string tabs){
    // we simply add every leaf to the code
    if(expression->children.size()==0){
        *code +=expression->name;
    } else{
        for(int i=0;i<expression->children.size();i++){
            generateExpression(expression->children[i],tabs);
        }
    }
}
void CodeGenerator::generateConstBlock(ParseTreeNode *ConstBlock,string tabs){
    if(ConstBlock->children[0]->name==""){
        return;
    }
    *code += tabs + ConstBlock->children[0]->name + " = " + ConstBlock->children[2]->children[0]->name + "\n";
    generateConstBlock(ConstBlock->children[4],tabs);
}
void CodeGenerator::printCode(){
    cout << *code << endl;
}
void CodeGenerator::saveToFile(string fileName){
    ofstream file;
    file.open(fileName);
    file << *code;
    file.close();
}
void CodeGenerator::runCode(string filename){
    string command = "python " + filename;
    system(command.c_str());
}