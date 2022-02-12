#include "analizador_sintactico.h"
void AnalizadorSintactico::parseExpression(vector<Token> tokens, int &pos, ParseTreeNode* &root){
    //first(expr):
	//not
	//e
	//id
	//INT
	//STRING
	//FLOAT
	//(
    if(tokens[pos].getType() == "OP_NOT" ){
        ParseTreeNode* expr = new ParseTreeNode("expression", "", tokens[pos].getLine(), tokens[pos].getColumn());
        root->addChild(expr);
        expr->addChild(new ParseTreeNode("not", "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
        parseExpression(tokens, pos, expr);
        parseExpression_(tokens, pos, expr);
    }
    else{
        ParseTreeNode* expr = new ParseTreeNode("expression", "", tokens[pos].getLine(), tokens[pos].getColumn());
        root->addChild(expr);
        parseExpression2(tokens, pos, expr);
        parseExpression_(tokens, pos, expr);
    }
}
void AnalizadorSintactico::parseFactor(vector<Token> tokens, int &pos, ParseTreeNode* &root){
    if( tokens[pos].getType() == "INT" || tokens[pos].getType() =="STRING" || tokens[pos].getType() == "FLOAT"){
        ParseTreeNode* factor = new ParseTreeNode("factor", "", tokens[pos].getLine(), tokens[pos].getColumn());
        root->addChild(factor);
        ParseTreeNode* Value = new ParseTreeNode("value", tokens[pos].getType(), tokens[pos].getLine(), tokens[pos].getColumn());
        factor->addChild(Value);
        Value->addChild(new ParseTreeNode(tokens[pos].getValue(), tokens[pos].getType() , tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
    }
    else if (tokens[pos].getType()== "ID"){
        ParseTreeNode * factor = new ParseTreeNode("factor", "", tokens[pos].getLine(), tokens[pos].getColumn());
        root->addChild(factor);
        factor->addChild(new ParseTreeNode(tokens[pos].getValue(),tokens[pos].getValue(),tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
    }
    else if(tokens[pos].getType() == "("){
        ParseTreeNode* factor = new ParseTreeNode("factor", "", tokens[pos].getLine(), tokens[pos].getColumn());
        root->addChild(factor);
        factor->addChild(new ParseTreeNode("(", "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
        parseExpression(tokens, pos, factor);
        if(tokens[pos].getType() == ")"){
            factor->addChild(new ParseTreeNode(")", "", tokens[pos].getLine(), tokens[pos].getColumn()));
            pos++;
        }
        else{
            errors.push_back("Error: ) expected on line " +to_string(tokens[pos].getLine())+ " column "+ to_string(tokens[pos].getColumn()));
            vector<string> expected = {"OP_MULT","OP_DIV","OP_DIV_","OP_MOD"};
            panicModeRecovery(tokens,pos,factor,expected);
        }
    }
}
void AnalizadorSintactico::parseTerm(vector<Token> tokens, int &pos, ParseTreeNode* &root){
    ParseTreeNode* term = new ParseTreeNode("term", "", tokens[pos].getLine(), tokens[pos].getColumn());
    if(tokens[pos].getType() == "ID" || tokens[pos].getType() == "INT" || tokens[pos].getType() == "STRING" || tokens[pos].getType() == "FLOAT"|| tokens[pos].getType() == "("){
        root->addChild(term);
        parseFactor(tokens, pos, term);
    }
    else{
        errors.push_back("Error: an ID, INT, STRING, FLOAT or ( was expected on line "+ to_string(tokens[pos].getLine()) + " column "+ to_string(tokens[pos].getColumn()));
        vector<string> expected = {"OP_SUM","OP_RES"};
        panicModeRecovery(tokens,pos,term,expected);
    }
    parseTerm_(tokens, pos, term);
}
void AnalizadorSintactico::parseTerm_(vector<Token> tokens, int &pos, ParseTreeNode* &root){
    if(tokens[pos].getType() == "OP_MULT" || tokens[pos].getType() == "OP_DIV" || tokens[pos].getType() == "OP_DIV_" || tokens[pos].getType() == "OP_MOD"){
        ParseTreeNode* term_ = new ParseTreeNode("term_", "", tokens[pos].getLine(), tokens[pos].getColumn());
        root->addChild(term_);
        term_->addChild(new ParseTreeNode(tokens[pos].getValue(), "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
        parseFactor(tokens, pos, term_);
        parseTerm_(tokens, pos, term_);
    }
    else{
        ParseTreeNode* term_ = new ParseTreeNode("term_", "", tokens[pos].getLine(), tokens[pos].getColumn());
        root->addChild(term_);
        term_->addChild(new ParseTreeNode("", "", tokens[pos].getLine(), tokens[pos].getColumn()));
    }
}
void AnalizadorSintactico::parseExpression3_(vector<Token> tokens, int &pos, ParseTreeNode* &root){
    if(tokens[pos].getType() == "OP_SUM" || tokens[pos].getType() == "OP_RES"){
        ParseTreeNode* expression3_ = new ParseTreeNode("expression3_", "", tokens[pos].getLine(), tokens[pos].getColumn());
        root->addChild(expression3_);
        expression3_->addChild(new ParseTreeNode(tokens[pos].getValue(), "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
        parseTerm(tokens, pos, expression3_);
        parseExpression3_(tokens, pos, expression3_);
    }
    else{
        ParseTreeNode* expression3_ = new ParseTreeNode("expression3_", "", tokens[pos].getLine(), tokens[pos].getColumn());
        root->addChild(expression3_);
        expression3_->addChild(new ParseTreeNode("", "", tokens[pos].getLine(), tokens[pos].getColumn()));
    }
}
void AnalizadorSintactico::parseExpression3(vector<Token> tokens, int &pos, ParseTreeNode* &root){
    ParseTreeNode* expression3 = new ParseTreeNode("expression3", "", tokens[pos].getLine(), tokens[pos].getColumn());
    root->addChild(expression3);
    parseTerm(tokens, pos, expression3);
    parseExpression3_(tokens, pos, expression3);
}
void AnalizadorSintactico::parseExpression2_(vector<Token> tokens, int &pos, ParseTreeNode* &root){
    ParseTreeNode* expression2_ = new ParseTreeNode("expression2_", "", tokens[pos].getLine(), tokens[pos].getColumn());
    root->addChild(expression2_);
    if(tokens[pos].getType() == "LESS" || tokens[pos].getType() == "GREATER" || tokens[pos].getType() == "LESS_EQUAL" || tokens[pos].getType() == "GREATER_EQUAL"){
        ParseTreeNode* RelOp = new ParseTreeNode("RelOp", "", tokens[pos].getLine(), tokens[pos].getColumn());
        expression2_->addChild(RelOp);
        RelOp->addChild(new ParseTreeNode(tokens[pos].getValue(), "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
        parseExpression3(tokens, pos, expression2_);
        parseExpression2_(tokens, pos, expression2_);
    }
    else{
        expression2_->addChild(new ParseTreeNode("", "", tokens[pos].getLine(), tokens[pos].getColumn()));
    }
}
void AnalizadorSintactico::parseExpression2(vector<Token> tokens, int &pos, ParseTreeNode* &root){
    ParseTreeNode* expression2 = new ParseTreeNode("expression2", "", tokens[pos].getLine(), tokens[pos].getColumn());
    root->addChild(expression2);
    parseExpression3(tokens, pos, expression2);
    parseExpression2_(tokens, pos, expression2);
}
void AnalizadorSintactico::parseExpression_(vector<Token> tokens, int &pos, ParseTreeNode* &root){
    ParseTreeNode* expression_ = new ParseTreeNode("expression_", "", tokens[pos].getLine(), tokens[pos].getColumn());
    root->addChild(expression_);
    if(tokens[pos].getType() == "OP_AND" || tokens[pos].getType() == "OP_OR"){
        ParseTreeNode* BooleanOp = new ParseTreeNode("BooleanOp", "", tokens[pos].getLine(), tokens[pos].getColumn());
        expression_->addChild(BooleanOp);
        BooleanOp->addChild(new ParseTreeNode(tokens[pos].getValue(), "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
        parseExpression2(tokens, pos, expression_);
        parseExpression_(tokens, pos, expression_);
    }
    else{
        expression_->addChild(new ParseTreeNode("", "", tokens[pos].getLine(), tokens[pos].getColumn()));
    }
}
void AnalizadorSintactico::parseForLoop(vector<Token> tokens, int &pos, ParseTreeNode* &root){
    ParseTreeNode* temp2 = new ParseTreeNode("forStatement", "", tokens[pos].getLine(), tokens[pos].getColumn());
    root->addChild(temp2);
    temp2->addChild(new ParseTreeNode("for", "", tokens[pos].getLine(), tokens[pos].getColumn()));
    pos++;
    if(tokens[pos].getType() != "ID"){
        errors.push_back("Error on line "+  to_string(tokens[pos].getLine())+ " column " + to_string(tokens[pos].getColumn()) + ": ID expected but got " + tokens[pos].getValue());
        vector<string> expected = {"ASSIGN","INT","STRING","FLOAT","TO","DOWNTO","NOT","ID","PARENTESIS_IZQ","DO","BEGIN","FOR","IF","ID","WRITELN","WRITE","BREAK","END","PUNTO_COMA"};
        panicModeRecovery(tokens,pos,temp2,expected);
        
    }else{
        temp2->addChild(new ParseTreeNode(tokens[pos].getValue(), "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
    }
    if(tokens[pos].getType() != "ASSIGN"){
        errors.push_back("Error on line " + to_string(tokens[pos].getLine())+ " column " + to_string(tokens[pos].getColumn())+ ": ASSIGN expected but got "+ tokens[pos].getValue());
        vector<string> expected = {"INT","STRING","FLOAT","TO","DOWNTO","NOT","ID","PARENTESIS_IZQ","DO","BEGIN","FOR","IF","ID","WRITELN","WRITE","BREAK","END","PUNTO_COMA"};
        panicModeRecovery(tokens,pos,temp2,expected);
    }else{
        temp2->addChild(new ParseTreeNode("assign", "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
    }
    ParseTreeNode* temp3 = new ParseTreeNode("VALUE", tokens[pos].getValue(), tokens[pos].getLine(), tokens[pos].getColumn());
    temp2->addChild(temp3);
    if(tokens[pos].getType() != "INT" && tokens[pos].getType() != "STRING" && tokens[pos].getType() != "FLOAT"){
        errors.push_back("Error on line " + to_string(tokens[pos].getLine()) + " column " + to_string(tokens[pos].getColumn()) + ": INT, STRING or FLOAT expected but got " + tokens[pos].getValue());
        vector<string> expected = {"TO","DOWNTO","INT","STRING","FLOAT","NOT","ID","PARENTESIS_IZQ","DO","BEGIN","FOR","IF","ID","WRITELN","WRITE","BREAK","END","PUNTO_COMA"};
        panicModeRecovery(tokens,pos,temp2,expected);
    }
    else{
        temp3 -> addChild(new ParseTreeNode(tokens[pos].getValue(), "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
    }
    if(tokens[pos].getType() != "TO" && tokens[pos].getType() != "DOWNTO"){
        errors.push_back("Error on line " + to_string(tokens[pos].getLine()) + " column " + to_string(tokens[pos].getColumn()) + ": TO expected but got " + tokens[pos].getValue());
        vector<string> expected = {"INT","STRING","FLOAT","NOT","ID","PARENTESIS_IZQ","DO","BEGIN","FOR","IF","ID","WRITELN","WRITE","BREAK","END","PUNTO_COMA"};
        panicModeRecovery(tokens,pos,temp2,expected);
    }
    else{
        temp2->addChild(new ParseTreeNode("To", "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
    }
    parseExpression(tokens, pos, temp2);
    if(tokens[pos].getType() != "DO"){
        errors.push_back("Error on line " + to_string(tokens[pos].getLine()) + " column " + to_string(tokens[pos].getColumn())+ ": DO expected but got " + tokens[pos].getValue());
        vector<string> expected = {"BEGIN","FOR","IF","ID","WRITELN","WRITE","BREAK","END","PUNTO_COMA"};
        panicModeRecovery(tokens,pos,temp2,expected);
    }
    else{
        temp2->addChild(new ParseTreeNode("do", "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
    }
    if(tokens[pos].getType() != "BEGIN"){
        errors.push_back("Error on line " + to_string(tokens[pos].getLine()) + " column " + to_string(tokens[pos].getColumn()) + ": BEGIN expected but got " + tokens[pos].getValue());
        vector<string> expected = {"FOR","IF","ID","WRITELN","WRITE","BREAK","END","PUNTO_COMA"};
        panicModeRecovery(tokens,pos,temp2,expected);
    }else{
        temp2->addChild(new ParseTreeNode("begin", "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
    }
    parseStatementList(tokens, pos, temp2);
    if(tokens[pos].getType() != "END"){
        errors.push_back("Error on line " + to_string(tokens[pos].getLine()) + " column " + to_string(tokens[pos].getColumn()) + ": END expected but got " + tokens[pos].getValue());
        vector<string> expected = {"FOR","IF","ID","WRITELN","WRITE","BREAK","PUNTO_COMA"};
        panicModeRecovery(tokens,pos,temp2,expected);
    }else{
        temp2->addChild(new ParseTreeNode("end", "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
    }
    if(tokens[pos].getType() != "PUNTO_COMA"){
        errors.push_back("Error on line " + to_string(tokens[pos].getLine()) + " column " + to_string(tokens[pos].getColumn()) + ": PUNTO_COMA expected but got " + tokens[pos].getValue());
        vector<string> expected = {"FOR","IF","ID","WRITELN","WRITE"};
        panicModeRecovery(tokens,pos,temp2,expected);
    }
    else{
        temp2->addChild(new ParseTreeNode("PUNTO_COMA", "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
    }
}
void AnalizadorSintactico::parseStatementList(vector<Token> tokens, int &pos, ParseTreeNode* &root){
    ParseTreeNode* statementList = new ParseTreeNode("statementList", "", tokens[pos].getLine(), tokens[pos].getColumn());
    root->addChild(statementList);
    parseStatement(tokens, pos, statementList);
    if(tokens[pos].getType() == "FOR" || tokens[pos].getType() == "IF" || tokens[pos].getType() == "ID" || tokens[pos].getType() == "WRITELN" || tokens[pos].getType() == "WRITE" || tokens[pos].getType() == "BREAK" || tokens[pos].getType() == "CONTINUE" ){
        parseStatementList(tokens, pos, statementList);
    }
}
void AnalizadorSintactico::parseStatement(vector<Token> tokens, int &pos, ParseTreeNode* &root){
    ParseTreeNode* temp = new ParseTreeNode("statement", "", tokens[pos].getLine(), tokens[pos].getColumn());
    root->addChild(temp);
    if(tokens[pos].getType() != "FOR" && tokens[pos].getType() != "IF" && tokens[pos].getType() != "ID" && tokens[pos].getType() != "WRITELN" && tokens[pos].getType() != "WRITE" && tokens[pos].getType() != "BREAK" && tokens[pos].getType() != "CONTINUE"){
        errors.push_back("Error on line "+to_string(tokens[pos].getLine())+" column "+to_string(tokens[pos].getColumn())+"Expected Statement but got "+tokens[pos].getValue());
        vector<string> expected = {"FOR","IF","ID","WRITELN","WRITE","BREAK","CONTINUE","PUNTO_COMA"};
        panicModeRecovery(tokens,pos,temp,expected);
    }
    if(tokens[pos].getType() == "FOR"){
        parseForLoop(tokens, pos, temp);
    }
    else if(tokens[pos].getType() == "IF"){
        parseIfStatement(tokens, pos, temp);
    }
    else if(tokens[pos].getType() == "ID"){
        ParseTreeNode* temp2 = new ParseTreeNode("ASSIGN", "", tokens[pos].getLine(), tokens[pos].getColumn());
        temp->addChild(temp2);
        //parsing the id
        temp2->addChild(new ParseTreeNode(tokens[pos].getValue(), tokens[pos].getValue(), tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
        if(tokens[pos].getType() != "ASSIGN"){
            errors.push_back("Error on line " + to_string(tokens[pos].getLine()) + " column " + to_string(tokens[pos].getColumn()) + ": ASSIGN expected but got " +tokens[pos].getValue());
            vector<string> expected = {"NOT","ID","PARENTESIS_IZQ","INT","FLOAT","STRING","FOR","IF","WRITELN","WRITE","BREAK","CONTINUE","PUNTO_COMA"};
            panicModeRecovery(tokens,pos,temp2,expected);
        } else {
            temp2->addChild(new ParseTreeNode(tokens[pos].getValue(), "", tokens[pos].getLine(), tokens[pos].getColumn()));
            pos++;
        }
        parseExpression(tokens, pos, temp2);
        if(tokens[pos].getType() != "PUNTO_COMA"){
            errors.push_back("Error on line " + to_string(tokens[pos].getLine()) + " column " + to_string(tokens[pos].getColumn()) + ": ; expected but got " + tokens[pos].getValue());
            vector<string> expected = {"INT","FLOAT","STRING","FOR","IF","WRITELN","WRITE","BREAK","CONTINUE"};
            panicModeRecovery(tokens,pos,temp2,expected);
        }
        else{
            temp2->addChild(new ParseTreeNode("PUNTO_COMA", "", tokens[pos].getLine(), tokens[pos].getColumn()));
            pos++;
        }
    }
    else if(tokens[pos].getType() == "WRITELN"){
        ParseTreeNode* temp2 = new ParseTreeNode("WRITELN", "", tokens[pos].getLine(), tokens[pos].getColumn());
        temp->addChild(temp2);
        temp2->addChild(new ParseTreeNode("writeLn", "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
        if(tokens[pos].getType() != "PARENTESIS_IZQ"){
            errors.push_back("Error on line "+ to_string(tokens[pos].getLine()) + " column " + to_string(tokens[pos].getColumn()) + ": ( expected but got " + tokens[pos].getValue());
            vector<string> expected = {"INT","FLOAT","STRING","PARENTESIS_IZQ","NOT","ID","FOR","IF","WRITELN","WRITE","BREAK","CONTINUE","PARENTESIS_DER", "PUNTO_COMA"};
            panicModeRecovery(tokens,pos,temp2,expected);
        }else{
            temp2->addChild(new ParseTreeNode("PARENTESIS_IZQ", "", tokens[pos].getLine(), tokens[pos].getColumn()));
            pos++;
        }
        if (tokens[pos].getType() == "STRING" || tokens[pos].getType() == "INT" || tokens[pos].getType() == "FLOAT" || tokens[pos].getType() == "ID") {
            ParseTreeNode* Value = new ParseTreeNode("Value", tokens[pos].getType(), tokens[pos].getLine(), tokens[pos].getColumn());
            temp2->addChild(Value);
            Value->addChild(new ParseTreeNode(tokens[pos].getValue(), tokens[pos].getValue(), tokens[pos].getLine(), tokens[pos].getColumn()));
            pos++;
            if (tokens[pos].getType() == "COMA" ){
                temp2->addChild(new ParseTreeNode("COMA", "", tokens[pos].getLine(), tokens[pos].getColumn()));
                pos++;
                if (tokens[pos].getType() == "STRING" || tokens[pos].getType() == "INT" || tokens[pos].getType() == "FLOAT" || tokens[pos].getType() == "ID") {
                    Value = new ParseTreeNode("Value", tokens[pos].getType(), tokens[pos].getLine(), tokens[pos].getColumn());
                    temp2->addChild(Value);
                    Value->addChild(new ParseTreeNode(tokens[pos].getValue(), tokens[pos].getValue(), tokens[pos].getLine(), tokens[pos].getColumn()));
                    pos++;
                    if (tokens[pos].getType() == "COMA" ){
                        temp2->addChild(new ParseTreeNode("COMA", "", tokens[pos].getLine(), tokens[pos].getColumn()));
                        pos++;
                        if (tokens[pos].getType() == "STRING" || tokens[pos].getType() == "INT" || tokens[pos].getType() == "FLOAT" || tokens[pos].getType() == "ID") {
                            Value = new ParseTreeNode("Value", tokens[pos].getType(), tokens[pos].getLine(), tokens[pos].getColumn());
                            temp2->addChild(Value);
                            Value->addChild(new ParseTreeNode(tokens[pos].getValue(), tokens[pos].getValue(), tokens[pos].getLine(), tokens[pos].getColumn()));
                            pos++;
                        }
                        else{
                            errors.push_back("Error on line " + to_string(tokens[pos].getLine()) + " column " + to_string(tokens[pos].getColumn()) + ": STRING, INT, FLOAT or ID expected but got " + tokens[pos].getValue());
                            vector<string> expected = {"PARENTESIS_IZQ","NOT","ID","FOR","IF","WRITELN","WRITE","BREAK","CONTINUE","PARENTESIS_DER", "PUNTO_COMA"};
                            panicModeRecovery(tokens,pos,temp2,expected);
                        }
                    }
                }
                else{
                    errors.push_back("Error on line " + to_string(tokens[pos].getLine()) + " column " + to_string(tokens[pos].getColumn()) + ": STRING, INT, FLOAT or ID expected but got " + tokens[pos].getValue());
                    vector<string> expected = {"PARENTESIS_IZQ","NOT","ID","FOR","IF","WRITELN","WRITE","BREAK","CONTINUE","PARENTESIS_DER", "PUNTO_COMA"};
                    panicModeRecovery(tokens,pos,temp2,expected);
                }
            }
        }
        if(tokens[pos].getType() != "PARENTESIS_DER"){
            errors.push_back( "Error on line " + to_string(tokens[pos].getLine()) + " column " + to_string(tokens[pos].getColumn()) + ": ) expected but got " + tokens[pos].getValue());
            vector<string> expected = {"PARENTESIS_IZQ","NOT","ID","FOR","IF","WRITELN","WRITE","BREAK","CONTINUE","PARENTESIS_DER", "PUNTO_COMA"};
            panicModeRecovery(tokens,pos,temp2,expected);
        }else{
            temp2->addChild(new ParseTreeNode("PARENTESIS_DER", "", tokens[pos].getLine(), tokens[pos].getColumn()));
            pos++;  
        }
        if(tokens[pos].getType() != "PUNTO_COMA"){
            errors.push_back("Error on line " + to_string(tokens[pos].getLine()) + " column " + to_string(tokens[pos].getColumn()) + ": ; expected but got " + tokens[pos].getValue());
            vector<string> expected = {"INT","FLOAT","STRING","FOR","IF","WRITELN","WRITE","BREAK","CONTINUE", "PUNTO_COMA"};
        }else{
            temp2->addChild(new ParseTreeNode("PUNTO_COMA", "", tokens[pos].getLine(), tokens[pos].getColumn()));
            pos++;
        }
    }
    else if(tokens[pos].getType() == "WRITE"){
        ParseTreeNode* temp2 = new ParseTreeNode("WRITE", "", tokens[pos].getLine(), tokens[pos].getColumn());
        temp->addChild(temp2);
        temp2->addChild(new ParseTreeNode("write", "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
        if(tokens[pos].getType() != "PARENTESIS_IZQ"){
            errors.push_back("Error on line " + to_string(tokens[pos].getLine()) + " column " + to_string(tokens[pos].getColumn()) + ": ( expected but got " + tokens[pos].getValue());
            vector<string> expected = {"INT","FLOAT","STRING","NOT","ID","FOR","IF","WRITELN","WRITE","BREAK","CONTINUE","PARENTESIS_DER", "PUNTO_COMA"};
            panicModeRecovery(tokens,pos,temp2,expected);
        } else {
            temp2->addChild(new ParseTreeNode("PARENTESIS_IZQ", "", tokens[pos].getLine(), tokens[pos].getColumn()));
            pos++;
        }        
        parseExpression(tokens, pos, temp2);
        if(tokens[pos].getType() != "PARENTESIS_DER"){
            errors.push_back("Error on line " + to_string(tokens[pos].getLine()) + " column " + to_string(tokens[pos].getColumn()) + ": ) expected but got " + tokens[pos].getValue());
            vector<string> expected = {"INT","FLOAT","STRING","PARENTESIS_IZQ","NOT","ID","FOR","IF","WRITELN","WRITE","BREAK","CONTINUE", "PUNTO_COMA"};
        } else {
            temp2->addChild(new ParseTreeNode("PARENTESIS_DER", "", tokens[pos].getLine(), tokens[pos].getColumn()));
            pos++;
        }
        if(tokens[pos].getType() != "PUNTO_COMA"){
            errors.push_back("Error on line " + to_string(tokens[pos].getLine()) + " column " + to_string(tokens[pos].getColumn()) + ": ; expected but got " + tokens[pos].getValue());
            vector<string> expected = {"INT","FLOAT","STRING","PARENTESIS_IZQ","NOT","ID","FOR","IF","WRITELN","WRITE","BREAK","CONTINUE", "PUNTO_COMA","END","PUNTO"};
            panicModeRecovery(tokens,pos,temp2,expected);
        } else {
            temp2->addChild(new ParseTreeNode("PUNTO_COMA", "", tokens[pos].getLine(), tokens[pos].getColumn()));
            pos++;
        }
    }
    else if(tokens[pos].getType() == "BREAK"){
        ParseTreeNode* temp2 = new ParseTreeNode("break", "", tokens[pos].getLine(), tokens[pos].getColumn());
        temp->addChild(temp2);
        pos++;
    }
    else if(tokens[pos].getType() == "CONTINUE"){
        ParseTreeNode* temp2 = new ParseTreeNode("continue", "", tokens[pos].getLine(), tokens[pos].getColumn());
        temp->addChild(temp2);
        pos++;
    }      
}
void AnalizadorSintactico::parseIfStatement(vector<Token> tokens, int &pos, ParseTreeNode* &root){
    ParseTreeNode* temp = new ParseTreeNode("if", "", tokens[pos].getLine(), tokens[pos].getColumn());
    if(tokens[pos].getType() != "IF"){
        errors.push_back("Error on line "+to_string(tokens[pos].getLine())+" column "+to_string(tokens[pos].getColumn())+" Expected IF but got "+tokens[pos].getValue());
        vector<string> expected = {"PARENTESIS_IZQ","NOT","ID","THEN","BEGIN","FOR","WRITELN","WRITE","BREAK","CONTINUE","PARENTESIS_DER", "PUNTO_COMA","ELSE"};
        panicModeRecovery(tokens,pos,temp,expected);
    }else{
        root->addChild(temp);       
        pos++;
    }
    if(tokens[pos].getType() != "PARENTESIS_IZQ"){
        errors.push_back("Error on line "+to_string(tokens[pos].getLine())+" column "+to_string(tokens[pos].getColumn())+" Expected PARENTESIS_IZQ but got "+tokens[pos].getValue());
        vector<string> expected = {"NOT","ID","THEN","BEGIN","FOR","WRITELN","WRITE","BREAK","CONTINUE","PARENTESIS_DER", "PUNTO_COMA","ELSE"};
        panicModeRecovery(tokens,pos,temp,expected);
    }else{
        temp->addChild(new ParseTreeNode("PARENTESIS_IZQ", "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
    }
    parseExpression(tokens, pos, temp);
    if(tokens[pos].getType() != "PARENTESIS_DER"){
        errors.push_back("Error on line "+to_string(tokens[pos].getLine())+" column "+to_string(tokens[pos].getColumn())+" Expected PARENTESIS_DER but got "+tokens[pos].getValue());
        vector<string> expected = {"THEN","BEGIN","FOR","WRITELN","WRITE","BREAK","CONTINUE","PARENTESIS_DER", "PUNTO_COMA","ELSE"};
        panicModeRecovery(tokens,pos,temp,expected);
        
    }else{
        temp->addChild(new ParseTreeNode("PARENTESIS_DER", "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
    }
    if(tokens[pos].getType() != "THEN"){
        errors.push_back("Error on line "+to_string(tokens[pos].getLine())+" column "+to_string(tokens[pos].getColumn())+" Expected THEN but got "+tokens[pos].getValue());
        vector<string> expected = {"BEGIN","FOR","WRITELN","WRITE","BREAK","CONTINUE","PARENTESIS_DER", "PUNTO_COMA","ELSE"};
        panicModeRecovery(tokens,pos,temp,expected);
    }else{
        temp->addChild(new ParseTreeNode("then", "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
    }
    if(tokens[pos].getType() != "BEGIN"){
        errors.push_back("Error on line "+to_string(tokens[pos].getLine())+" column "+to_string(tokens[pos].getColumn())+" Expected BEGIN but got "+tokens[pos].getValue());
        vector<string> expected = {"FOR","WRITELN","WRITE","BREAK","CONTINUE","PARENTESIS_DER", "PUNTO_COMA","ELSE"};
        panicModeRecovery(tokens,pos,temp,expected);
    }else{
        temp->addChild(new ParseTreeNode("begin", "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
    }
    parseStatementList(tokens, pos, temp);
    if(tokens[pos].getType() != "END"){
        errors.push_back("Error on line "+to_string(tokens[pos].getLine())+" column "+to_string(tokens[pos].getColumn())+" Expected END but got "+tokens[pos].getValue());
        vector<string> expected = {"BEGIN","FOR","WRITELN","WRITE","BREAK","CONTINUE","PARENTESIS_DER", "PUNTO_COMA","ELSE"};
        panicModeRecovery(tokens,pos,temp,expected);
    }else{
        temp->addChild(new ParseTreeNode("end", "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
    }
    if(tokens[pos].getType() != "PUNTO_COMA"){
        errors.push_back("Error on line "+to_string(tokens[pos].getLine())+" column "+to_string(tokens[pos].getColumn())+" Expected PUNTO_COMA but got "+tokens[pos].getValue());
        vector<string> expected = {"FOR","WRITELN","WRITE","BREAK","CONTINUE","PARENTESIS_DER","ELSE"};
        panicModeRecovery(tokens,pos,temp,expected);
    }else{
        temp->addChild(new ParseTreeNode("PUNTO_COMA", "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
    }
    if(tokens[pos].getType() != "ELSE"){
        errors.push_back("Error on line "+to_string(tokens[pos].getLine())+" column "+to_string(tokens[pos].getColumn())+" Expected ELSE but got "+tokens[pos].getValue());
        vector<string> expected = {"BEGIN","FOR","WRITELN","WRITE","BREAK","CONTINUE","PARENTESIS_DER", "PUNTO_COMA","END"};
        panicModeRecovery(tokens,pos,temp,expected);
    }else{
        temp->addChild(new ParseTreeNode("ELSE", "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
    }
    if(tokens[pos].getType() != "BEGIN"){
        errors.push_back("Error on line "+to_string(tokens[pos].getLine())+" column "+to_string(tokens[pos].getColumn())+" Expected BEGIN but got "+tokens[pos].getValue());
        vector<string> expected = {"FOR","WRITELN","WRITE","BREAK","CONTINUE","PARENTESIS_DER", "PUNTO_COMA","END"};
        panicModeRecovery(tokens,pos,temp,expected);
    } else{
        temp->addChild(new ParseTreeNode("begin", "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
    }
    parseStatementList(tokens, pos, temp);
    if(tokens[pos].getType() != "END"){
        errors.push_back("Error on line "+to_string(tokens[pos].getLine())+" column "+to_string(tokens[pos].getColumn())+" Expected END but got "+tokens[pos].getValue());
        vector<string> expected = {"BEGIN","FOR","WRITELN","WRITE","BREAK","CONTINUE","PARENTESIS_DER", "PUNTO_COMA"};
        panicModeRecovery(tokens,pos,temp,expected);
    } else{
        temp->addChild(new ParseTreeNode("end", "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
    }
    if(tokens[pos].getType() != "PUNTO_COMA"){
        errors.push_back("Error on line "+to_string(tokens[pos].getLine())+" column "+to_string(tokens[pos].getColumn())+" Expected PUNTO_COMA but got "+tokens[pos].getValue());
        vector<string> expected = {"BEGIN","FOR","WRITELN","WRITE","BREAK","CONTINUE"};
        panicModeRecovery(tokens,pos,temp,expected);
    } else{
        temp->addChild(new ParseTreeNode("PUNTO_COMA", "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
    }
}
void AnalizadorSintactico::parseConstBlock(vector<Token> tokens, int &pos, ParseTreeNode* &root){
    //refactor this since it can be empty
    if(tokens[pos].getType() == "CONST"){
        ParseTreeNode* constBlock = new ParseTreeNode("ConstBlock", "", tokens[pos].getLine(), tokens[pos].getColumn());
        root->addChild(constBlock);
        constBlock->addChild(new ParseTreeNode("CONST", "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
        if(tokens[pos].getType() != "ID"){
            errors.push_back("Error on line "+to_string(tokens[pos].getLine())+" column "+to_string(tokens[pos].getColumn())+" Expected ID but got "+tokens[pos].getValue());
            vector<string> expected = {"VAR"};
            panicModeRecovery(tokens,pos,constBlock,expected);
        }
        parseConstList(tokens, pos, constBlock);
    } else{
        ParseTreeNode* constBlock = new ParseTreeNode("ConstBlock", "", tokens[pos].getLine(), tokens[pos].getColumn());
        root->addChild(constBlock);
        constBlock->addChild(new ParseTreeNode("", "", tokens[pos].getLine(), tokens[pos].getColumn()));
    }
}
void AnalizadorSintactico::parseConstList(vector<Token> tokens, int &pos, ParseTreeNode* &root){
    if(tokens[pos].getType() == "ID"){
        ParseTreeNode* constList = new ParseTreeNode("ConstList", "", tokens[pos].getLine(), tokens[pos].getColumn());
        root->addChild(constList);
        constList->addChild(new ParseTreeNode(tokens[pos].getValue(), "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
        if(tokens[pos].getType() != "EQUAL"){
            errors.push_back("Error on line "+to_string(tokens[pos].getLine())+" column "+to_string(tokens[pos].getColumn())+" Expected EQUAL but got "+tokens[pos].getValue());
            vector<string> expected = {"INT","STRING","FLOAT","PUNTO_COMA","VAR"};
            panicModeRecovery(tokens,pos,constList,expected);
        } else{
            constList->addChild(new ParseTreeNode("EQUAL", "", tokens[pos].getLine(), tokens[pos].getColumn()));
            pos++;
        }
        if(tokens[pos].getType() != "INT" && tokens[pos].getType() != "FLOAT" && tokens[pos].getType() != "STRING"){
            errors.push_back("Error on line "+to_string(tokens[pos].getLine())+" column "+to_string(tokens[pos].getColumn())+" Expected INT, FLOAT or STRING but got "+tokens[pos].getValue());
            vector<string> expected = {"PUNTO_COMA","VAR"};
            panicModeRecovery(tokens,pos,constList,expected);
        }else{
            ParseTreeNode* Value = new ParseTreeNode("Value", tokens[pos].getType(), tokens[pos].getLine(), tokens[pos].getColumn());
            constList->addChild(Value);
            Value->addChild(new ParseTreeNode(tokens[pos].getValue(), tokens[pos].getType(), tokens[pos].getLine(), tokens[pos].getColumn()));
            pos++;
        }
        if(tokens[pos].getType() != "PUNTO_COMA"){
            errors.push_back("Error on line "+to_string(tokens[pos].getLine())+" column "+to_string(tokens[pos].getColumn())+" Expected PUNTO_COMA but got "+tokens[pos].getValue());
            vector<string> expected = {"INT","FLOAT","STRING","PUNTO_COMA","VAR","ID"};
            panicModeRecovery(tokens,pos,constList,expected);
        }else{
            constList->addChild(new ParseTreeNode("PUNTO_COMA", "", tokens[pos].getLine(), tokens[pos].getColumn()));
            pos++;
        }
        parseConstList(tokens, pos, constList);
    } else {
        ParseTreeNode* constList = new ParseTreeNode("ConstList", "", tokens[pos].getLine(), tokens[pos].getColumn());
        root->addChild(constList);
        constList->addChild(new ParseTreeNode("", "", tokens[pos].getLine(), tokens[pos].getColumn()));
    }
}
void AnalizadorSintactico::parseVarBlock(vector<Token> tokens, int &pos, ParseTreeNode* &root){
    if(tokens[pos].getType() == "VAR"){
        ParseTreeNode* varBlock = new ParseTreeNode("VarBlock", "", tokens[pos].getLine(), tokens[pos].getColumn());
        root->addChild(varBlock);
        varBlock->addChild(new ParseTreeNode("VAR", "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
        parseVarList(tokens, pos, varBlock);
    } else{
        ParseTreeNode* varBlock = new ParseTreeNode("VarBlock", "", tokens[pos].getLine(), tokens[pos].getColumn());
        root->addChild(varBlock);
        varBlock->addChild(new ParseTreeNode("", "", tokens[pos].getLine(), tokens[pos].getColumn()));
    }
}
void AnalizadorSintactico::parseVarList(vector<Token> tokens, int &pos, ParseTreeNode* &root){
    ParseTreeNode* varList = new ParseTreeNode("VarList", "", tokens[pos].getLine(), tokens[pos].getColumn());
    root->addChild(varList);
    if(tokens[pos].getType() != "ID"){
        // no error since varlist can be empty
        varList->addChild(new ParseTreeNode("", "", tokens[pos].getLine(), tokens[pos].getColumn()));
        return;
    }
    parseVarDecl(tokens, pos, varList);
    if(tokens[pos].getType() != "DOS_PUNTOS"){
        errors.push_back("Error on line "+to_string(tokens[pos].getLine())+" column "+to_string(tokens[pos].getColumn())+" Expected DOS_PUNTOS but got "+tokens[pos].getValue());
        vector<string> expected = {"STRING_TYPE","INTEGER_TYPE","FLOAT_TYPE","PUNTO_COMA","VAR","BEGIN"};
        panicModeRecovery(tokens,pos,varList,expected);
    }else{
        varList->addChild(new ParseTreeNode("DOS_PUNTOS", "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
    }
    //revisit since i don't understand
    if(tokens[pos].getType() != "STRING_TYPE"&& tokens[pos].getType() != "INTEGER_TYPE" && tokens[pos].getType() != "FLOAT_TYPE"){
        errors.push_back("Error on line "+to_string(tokens[pos].getLine())+" column "+to_string(tokens[pos].getColumn())+" Expected STRING_TYPE, INTEGER_TYPE or FLOAT_TYPE but got "+tokens[pos].getValue());
        vector<string> expected = {"PUNTO_COMA","VAR","BEGIN"};
        panicModeRecovery(tokens,pos,varList,expected);
    } else {
        ParseTreeNode* type = new ParseTreeNode("Type", "", tokens[pos].getLine(), tokens[pos].getColumn());
        varList->addChild(type);
        type->addChild(new ParseTreeNode(tokens[pos].getType(), "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
    }
    if(tokens[pos].getType() != "PUNTO_COMA"){
        errors.push_back("Error on line "+to_string(tokens[pos].getLine())+" column "+to_string(tokens[pos].getColumn())+" Expected PUNTO_COMA but got "+tokens[pos].getValue());
        vector<string> expected = {"STRING_TYPE","INTEGER_TYPE","FLOAT_TYPE","PUNTO_COMA","VAR","BEGIN"};
        panicModeRecovery(tokens,pos,varList,expected);
    } else {
        varList->addChild(new ParseTreeNode("PUNTO_COMA", "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
    }
    parseVarList(tokens, pos, varList);
}
void AnalizadorSintactico::parseVarDecl(vector<Token> tokens, int &pos, ParseTreeNode* &root){
    if(tokens[pos].getType() == "ID"){
        ParseTreeNode* varDecl = new ParseTreeNode("VarDecl", "", tokens[pos].getLine(), tokens[pos].getColumn());
        root->addChild(varDecl);
        varDecl->addChild(new ParseTreeNode(tokens[pos].getValue(), "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
        if(tokens[pos].getType() == "COMA"){
            varDecl->addChild(new ParseTreeNode("COMA", "", tokens[pos].getLine(), tokens[pos].getColumn()));
            pos++;
            parseVarDecl(tokens, pos, varDecl);
        }
    } else {
        errors.push_back("Error on line "+to_string(tokens[pos].getLine())+" column "+to_string(tokens[pos].getColumn())+" Expected ID but got "+tokens[pos].getValue());
        vector<string> expected = {"COMA","DOS_PUNTOS","ID"};
        panicModeRecovery(tokens,pos,root,expected);
    }
}
void AnalizadorSintactico::parseMainCode(vector<Token> tokens, int &pos, ParseTreeNode* &root){
    ParseTreeNode* mainCode = new ParseTreeNode("MainCode", "", tokens[pos].getLine(), tokens[pos].getColumn());
    root->addChild(mainCode);
    if(tokens[pos].getType()!="BEGIN"){
        errors.push_back("Error on line "+to_string(tokens[pos].getLine())+" column "+to_string(tokens[pos].getColumn())+" Expected BEGIN but got "+tokens[pos].getValue());
        vector<string> expected = {"FOR","IF","WHILE","READ","WRITE","ID","END"};
        panicModeRecovery(tokens,pos,mainCode,expected);
    } else{
        mainCode->addChild(new ParseTreeNode("BEGIN", "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
    }
    parseStatementList(tokens, pos, mainCode);
    if(tokens[pos].getType()!="END"){
        errors.push_back("Error on line "+to_string(tokens[pos].getLine())+" column "+to_string(tokens[pos].getColumn())+" Expected END but got "+tokens[pos].getValue());
        vector<string> expected = {"PUNTO"};
        panicModeRecovery(tokens,pos,mainCode,expected);
    } else{
        mainCode->addChild(new ParseTreeNode("END", "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
    }
    if(tokens[pos].getType()!="PUNTO"){
        errors.push_back("Error on line "+to_string(tokens[pos].getLine())+" column "+to_string(tokens[pos].getColumn())+" Expected PUNTO but got "+tokens[pos].getValue());
        return;
    } else{
        mainCode->addChild(new ParseTreeNode("PUNTO", "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
    }
}
void AnalizadorSintactico::analizar(vector<Token> tokens)
{
    if (tokens.size() == 0)
    {
        cout << "←(ಠ_ಠ)→" << endl;
        return;
    }
    ParseTreeNode *root = new ParseTreeNode("root", "", 0, 0);
    tree.root = root;
    ParseTreeNode *current = root;
    int i = 0;
    if(tokens[i].getType() == "PROGRAM")
    {
        current = new ParseTreeNode("program", "", tokens[i].getLine(), tokens[i].getColumn());
        root->addChild(current);
        i++;
    }
    else
    {
        errors.push_back("Error on line "+to_string(tokens[i].getLine())+" column "+to_string(tokens[i].getColumn())+" Expected PROGRAM but got "+tokens[i].getValue());
        vector<string> expected = {"ID","PUNTO_COMA","CONST","VAR","BEGIN"};
        panicModeRecovery(tokens,i,root,expected);
    }
    if(tokens[i].getType() == "ID")
    {
        current->addChild(new ParseTreeNode(tokens[i].getValue(), tokens[i].getValue(), tokens[i].getLine(), tokens[i].getColumn()));
        i++;
    }
    else
    {
        errors.push_back("Error on line "+to_string(tokens[i].getLine())+" column "+to_string(tokens[i].getColumn())+" Expected ID but got "+tokens[i].getValue());
        vector<string> expected = {"PUNTO_COMA","CONST","VAR","BEGIN"};
        panicModeRecovery(tokens,i,root,expected);
    }
    if(tokens[i].getType () == "PUNTO_COMA")
    {
        current->addChild(new ParseTreeNode("PUNTO_COMA", tokens[i].getValue(), tokens[i].getLine(), tokens[i].getColumn()));
        i++;
    }
    else
    {
        errors.push_back("Error on line "+to_string(tokens[i].getLine())+" column "+to_string(tokens[i].getColumn())+" Expected COMA but got "+tokens[i].getValue());
        vector<string> expected = {"CONST","VAR","BEGIN","ID"};
        panicModeRecovery(tokens,i,root,expected);
    }
    // Parsing constblock
    parseConstBlock(tokens, i, current);
    //Parsing varblock
    parseVarBlock(tokens, i, current);
    //Parsing MainCode
    parseMainCode(tokens, i, current);
}
void AnalizadorSintactico::printParseTree(){
    tree.print();
}
void AnalizadorSintactico::toJSON(string filename){
    tree.toJSON(filename);
}
void AnalizadorSintactico::panicModeRecovery(vector<Token> tokens, int &pos, ParseTreeNode* &root, vector<string> expected){
    //panic mode recovery to syncronize with next non terminal
    while(std::count(expected.begin(),expected.end(),tokens[pos].getType()) == 0 && pos!=(tokens.size()-1)){
        pos++;
    }
    root->addChild(new ParseTreeNode("Error","",tokens[pos].getLine(),tokens[pos].getColumn()));
}
ParseTree* AnalizadorSintactico::getParseTree(){
    return &tree;
}