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
        ParseTreeNode* Value = new ParseTreeNode("value", "", tokens[pos].getLine(), tokens[pos].getColumn());
        factor->addChild(Value);
        Value->addChild(new ParseTreeNode(tokens[pos].getValue(), tokens[pos].getValue() , tokens[pos].getLine(), tokens[pos].getColumn()));
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
            cout << "Error: Se esperaba un ) en la linea " << tokens[pos].getLine() << " columna " << tokens[pos].getColumn() << endl;
            exit(1);
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
        cout << "Error: Se esperaba un ID, INT, STRING, FLOAT o ( en la linea " << tokens[pos].getLine() << " columna " << tokens[pos].getColumn() << endl;
        exit(1);
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
    if(tokens[pos].getType() == "OP_PLUS" || tokens[pos].getType() == "OP_MINUS"){
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
    if(tokens[pos].getType() == "OP_LESS" || tokens[pos].getType() == "OP_GREATER" || tokens[pos].getType() == "OP_LESS_EQ" || tokens[pos].getType() == "OP_GREATER_EQ"){
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
        cout << "Error on line " << tokens[pos].getLine() << " column " << tokens[pos].getColumn() << ": ID expected but got " << tokens[pos].getValue() << endl;
        return;
    }
    temp2->addChild(new ParseTreeNode(tokens[pos].getValue(), "", tokens[pos].getLine(), tokens[pos].getColumn()));
    pos++;
    if(tokens[pos].getType() != "ASSIGN"){
        cout << "Error on line " << tokens[pos].getLine() << " column " << tokens[pos].getColumn() << ": ASSIGN expected but got " << tokens[pos].getValue() << endl;
        return;
    }
    temp2->addChild(new ParseTreeNode("assign", "", tokens[pos].getLine(), tokens[pos].getColumn()));
    pos++;
    if(tokens[pos].getType() != "INT" && tokens[pos].getType() != "STRING" && tokens[pos].getType() != "FLOAT"){
        cout << "Error on line " << tokens[pos].getLine() << " column " << tokens[pos].getColumn() << ": INT, STRING or FLOAT expected but got " << tokens[pos].getValue() << endl;
        return;
    }
    ParseTreeNode* temp3 = new ParseTreeNode("VALUE", tokens[pos].getValue(), tokens[pos].getLine(), tokens[pos].getColumn());
    temp2->addChild(temp3);
    temp3 -> addChild(new ParseTreeNode(tokens[pos].getValue(), "", tokens[pos].getLine(), tokens[pos].getColumn()));
    pos++;
    if(tokens[pos].getType() != "TO" && tokens[pos].getType() != "DOWNTO"){
        cout << "Error on line " << tokens[pos].getLine() << " column " << tokens[pos].getColumn() << ": TO expected but got " << tokens[pos].getValue() << endl;
        return;
    }
    temp2->addChild(new ParseTreeNode("To", "", tokens[pos].getLine(), tokens[pos].getColumn()));
    pos++;
    parseExpression(tokens, pos, temp2);
    if(tokens[pos].getType() != "DO"){
        cout << "Error on line " << tokens[pos].getLine() << " column " << tokens[pos].getColumn() << ": DO expected but got " << tokens[pos].getValue() << endl;
        return;
    }
    temp2->addChild(new ParseTreeNode("do", "", tokens[pos].getLine(), tokens[pos].getColumn()));
    pos++;
    if(tokens[pos].getType() != "BEGIN"){
        cout << "Error on line " << tokens[pos].getLine() << " column " << tokens[pos].getColumn() << ": BEGIN expected but got " << tokens[pos].getValue() << endl;
        return;
    }
    temp2->addChild(new ParseTreeNode("begin", "", tokens[pos].getLine(), tokens[pos].getColumn()));
    pos++;
    parseStatementList(tokens, pos, temp2);
    if(tokens[pos].getType() != "END"){
        cout << "Error on line " << tokens[pos].getLine() << " column " << tokens[pos].getColumn() << ": END expected but got " << tokens[pos].getValue() << endl;
        return;
    }
    temp2->addChild(new ParseTreeNode("end", "", tokens[pos].getLine(), tokens[pos].getColumn()));
    pos++;
    if(tokens[pos].getType() != "PUNTO_COMA"){
        cout << "Error on line " << tokens[pos].getLine() << " column " << tokens[pos].getColumn() << ": PUNTO_COMA expected but got " << tokens[pos].getValue() << endl;
        return;
    }
    temp2->addChild(new ParseTreeNode("PUNTO_COMA", "", tokens[pos].getLine(), tokens[pos].getColumn()));
    pos++;
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
    if(tokens[pos].getType() != "FOR" && tokens[pos].getType() != "IF" && tokens[pos].getType() != "ID" && tokens[pos].getType() != "WRITELN" && tokens[pos].getType() != "WRITE" && tokens[pos].getType() != "BREAK" && tokens[pos].getType() != "CONTINUE"){
        cout<<"Error on line"<<tokens[pos].getLine()<<"column"<<tokens[pos].getColumn()<<"Expected Statement but got"<<tokens[pos].getValue()<<endl;
        return;
    }
    ParseTreeNode* temp = new ParseTreeNode("statement", "", tokens[pos].getLine(), tokens[pos].getColumn());
    root->addChild(temp);
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
            cout << "Error on line " << tokens[pos].getLine() << " column " << tokens[pos].getColumn() << ": ASSIGN expected but got " << tokens[pos].getValue() << endl;
            return;
        }
        temp2->addChild(new ParseTreeNode(tokens[pos].getValue(), "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
        parseExpression(tokens, pos, temp2);
        if(tokens[pos].getType() != "PUNTO_COMA"){
            cout << "Error on line " << tokens[pos].getLine() << " column " << tokens[pos].getColumn() << ": ; expected but got " << tokens[pos].getValue() << endl;
            return;
        }
        temp2->addChild(new ParseTreeNode("PUNTO_COMA", "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
    }
    else if(tokens[pos].getType() == "WRITELN"){
        ParseTreeNode* temp2 = new ParseTreeNode("WRITELN", "", tokens[pos].getLine(), tokens[pos].getColumn());
        temp->addChild(temp2);
        temp2->addChild(new ParseTreeNode("writeLn", "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
        if(tokens[pos].getType() != "PARENTESIS_IZQ"){
            cout << "Error on line " << tokens[pos].getLine() << " column " << tokens[pos].getColumn() << ": ( expected but got " << tokens[pos].getValue() << endl;
            return;
        }
        temp2->addChild(new ParseTreeNode("PARENTESIS_IZQ", "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
        if (tokens[pos].getType() == "STRING" || tokens[pos].getType() == "INT" || tokens[pos].getType() == "FLOAT" || tokens[pos].getType() == "ID") {
            ParseTreeNode* Value = new ParseTreeNode("Value", "", tokens[pos].getLine(), tokens[pos].getColumn());
            temp2->addChild(Value);
            Value->addChild(new ParseTreeNode(tokens[pos].getValue(), tokens[pos].getValue(), tokens[pos].getLine(), tokens[pos].getColumn()));
            pos++;
            if (tokens[pos].getType() == "COMA" ){
                temp2->addChild(new ParseTreeNode("COMA", "", tokens[pos].getLine(), tokens[pos].getColumn()));
                pos++;
                if (tokens[pos].getType() == "STRING" || tokens[pos].getType() == "INT" || tokens[pos].getType() == "FLOAT" || tokens[pos].getType() == "ID") {
                    Value = new ParseTreeNode("Value", "", tokens[pos].getLine(), tokens[pos].getColumn());
                    temp2->addChild(Value);
                    Value->addChild(new ParseTreeNode(tokens[pos].getValue(), tokens[pos].getValue(), tokens[pos].getLine(), tokens[pos].getColumn()));
                    pos++;
                    if (tokens[pos].getType() == "COMA" ){
                        temp2->addChild(new ParseTreeNode("COMA", "", tokens[pos].getLine(), tokens[pos].getColumn()));
                        pos++;
                        if (tokens[pos].getType() == "STRING" || tokens[pos].getType() == "INT" || tokens[pos].getType() == "FLOAT" || tokens[pos].getType() == "ID") {
                            Value = new ParseTreeNode("Value", "", tokens[pos].getLine(), tokens[pos].getColumn());
                            temp2->addChild(Value);
                            Value->addChild(new ParseTreeNode(tokens[pos].getValue(), tokens[pos].getValue(), tokens[pos].getLine(), tokens[pos].getColumn()));
                            pos++;
                        }
                        else{
                            cout << "Error on line " << tokens[pos].getLine() << " column " << tokens[pos].getColumn() << ": STRING, INT, FLOAT or ID expected but got " << tokens[pos].getValue() << endl;
                            return;
                        }
                    }
                }
                else{
                    cout << "Error on line " << tokens[pos].getLine() << " column " << tokens[pos].getColumn() << ": STRING, INT, FLOAT or ID expected but got " << tokens[pos].getValue() << endl;
                    return;
                }
            }
        }
        if(tokens[pos].getType() != "PARENTESIS_DER"){
            cout << "Error on line " << tokens[pos].getLine() << " column " << tokens[pos].getColumn() << ": ) expected but got " << tokens[pos].getValue() << endl;
            return;
        }
        temp2->addChild(new ParseTreeNode("PARENTESIS_DER", "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
        if(tokens[pos].getType() != "PUNTO_COMA"){
            cout << "Error on line " << tokens[pos].getLine() << " column " << tokens[pos].getColumn() << ": ; expected but got " << tokens[pos].getValue() << endl;
            return;
        }
        temp2->addChild(new ParseTreeNode("PUNTO_COMA", "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
    }
    else if(tokens[pos].getType() == "WRITE"){
        ParseTreeNode* temp2 = new ParseTreeNode("WRITE", "", tokens[pos].getLine(), tokens[pos].getColumn());
        temp->addChild(temp2);
        temp2->addChild(new ParseTreeNode("write", "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
        if(tokens[pos].getType() != "PARENTESIS_IZQ"){
            cout << "Error on line " << tokens[pos].getLine() << " column " << tokens[pos].getColumn() << ": ( expected but got " << tokens[pos].getValue() << endl;
            return;
        }
        temp2->addChild(new ParseTreeNode("PARENTESIS_IZQ", "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
        parseExpression(tokens, pos, temp);
        if(tokens[pos].getType() != "PARENTESIS_DER"){
            cout << "Error on line " << tokens[pos].getLine() << " column " << tokens[pos].getColumn() << ": ) expected but got " << tokens[pos].getValue() << endl;
            return;
        }
        temp2->addChild(new ParseTreeNode("PARENTESIS_DER", "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
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
//TA MAL
void AnalizadorSintactico::parseIfStatement(vector<Token> tokens, int &pos, ParseTreeNode* &root){
    if(tokens[pos].getType() != "IF"){
        cout<<"Error on line"<<tokens[pos].getLine()<<"column"<<tokens[pos].getColumn()<<"Expected IF but got"<<tokens[pos].getValue()<<endl;
        return;
    }
    ParseTreeNode* temp = new ParseTreeNode("if", "", tokens[pos].getLine(), tokens[pos].getColumn());
    root->addChild(temp);
    pos++;
    if(tokens[pos].getType() != "PARENTESIS_IZQ"){
        cout<<"Error on line"<<tokens[pos].getLine()<<"column"<<tokens[pos].getColumn()<<"Expected PARENTESIS_IZQ but got"<<tokens[pos].getValue()<<endl;
        return;
    }
    temp = new ParseTreeNode("PARENTESIS_IZQ", "", tokens[pos].getLine(), tokens[pos].getColumn());
    root->addChild(temp);
    pos++;
    parseExpression(tokens, pos, root);
    if(tokens[pos].getType() != "PARENTESIS_DER"){
        cout<<"Error on line"<<tokens[pos].getLine()<<"column"<<tokens[pos].getColumn()<<"Expected PARENTESIS_DER but got"<<tokens[pos].getValue()<<endl;
        return;
    }
    temp = new ParseTreeNode("PARENTESIS_DER", "", tokens[pos].getLine(), tokens[pos].getColumn());
    root->addChild(temp);
    pos++;
    if(tokens[pos].getType() != "THEN"){
        cout<<"Error on line"<<tokens[pos].getLine()<<"column"<<tokens[pos].getColumn()<<"Expected THEN but got"<<tokens[pos].getValue()<<endl;
        return;
    }
    temp = new ParseTreeNode("then", "", tokens[pos].getLine(), tokens[pos].getColumn());
    root->addChild(temp);
    pos++;
    parseStatement(tokens, pos, root);
    if(tokens[pos].getType() != "BEGIN"){
        cout<<"Error on line"<<tokens[pos].getLine()<<"column"<<tokens[pos].getColumn()<<"Expected BEGIN but got"<<tokens[pos].getValue()<<endl;
        return;
    }
    temp = new ParseTreeNode("begin", "", tokens[pos].getLine(), tokens[pos].getColumn());
    root->addChild(temp);
    pos++;
    parseStatementList(tokens, pos, root);
    if(tokens[pos].getType() != "END"){
        cout<<"Error on line"<<tokens[pos].getLine()<<"column"<<tokens[pos].getColumn()<<"Expected END but got"<<tokens[pos].getValue()<<endl;
        return;
    }
    temp = new ParseTreeNode("end", "", tokens[pos].getLine(), tokens[pos].getColumn());
    root->addChild(temp);
    pos++;
    if(tokens[pos].getType() != "PUNTO_COMA"){
        cout<<"Error on line"<<tokens[pos].getLine()<<"column"<<tokens[pos].getColumn()<<"Expected PUNTO_COMA but got"<<tokens[pos].getValue()<<endl;
        return;
    }
    temp = new ParseTreeNode("PUNTO_COMA", "", tokens[pos].getLine(), tokens[pos].getColumn());
    root->addChild(temp);
    pos++;
    if(tokens[pos].getType() != "ELSE"){
        cout<<"Error on line"<<tokens[pos].getLine()<<"column"<<tokens[pos].getColumn()<<"Expected ELSE but got"<<tokens[pos].getValue()<<endl;
        return;
    }
    temp = new ParseTreeNode("else", "", tokens[pos].getLine(), tokens[pos].getColumn());
    root->addChild(temp);
    pos++;
    if(tokens[pos].getType() != "BEGIN"){
        cout<<"Error on line"<<tokens[pos].getLine()<<"column"<<tokens[pos].getColumn()<<"Expected BEGIN but got"<<tokens[pos].getValue()<<endl;
        return;
    }
    temp = new ParseTreeNode("begin", "", tokens[pos].getLine(), tokens[pos].getColumn());
    root->addChild(temp);
    pos++;
    parseStatementList(tokens, pos, root);
    if(tokens[pos].getType() != "END"){
        cout<<"Error on line"<<tokens[pos].getLine()<<"column"<<tokens[pos].getColumn()<<"Expected END but got"<<tokens[pos].getValue()<<endl;
        return;
    }
    temp = new ParseTreeNode("end", "", tokens[pos].getLine(), tokens[pos].getColumn());
    root->addChild(temp);
    pos++;
    if(tokens[pos].getType() != "PUNTO_COMA"){
        cout<<"Error on line"<<tokens[pos].getLine()<<"column"<<tokens[pos].getColumn()<<"Expected PUNTO_COMA but got"<<tokens[pos].getValue()<<endl;
        return;
    }
    temp = new ParseTreeNode("PUNTO_COMA", "", tokens[pos].getLine(), tokens[pos].getColumn());
    root->addChild(temp);
    pos++;
}
void AnalizadorSintactico::parseConstBlock(vector<Token> tokens, int &pos, ParseTreeNode* &root){
    //refactor this since it can be empty
    if(tokens[pos].getType() == "CONST"){
        ParseTreeNode* constBlock = new ParseTreeNode("ConstBlock", "", tokens[pos].getLine(), tokens[pos].getColumn());
        root->addChild(constBlock);
        constBlock->addChild(new ParseTreeNode("CONST", "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
        if(tokens[pos].getType() != "ID"){
            cout<<"Error on line"<<tokens[pos].getLine()<<"column"<<tokens[pos].getColumn()<<"Expected ID but got"<<tokens[pos].getValue()<<endl;
            return;
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
        constList->addChild(new ParseTreeNode("ID", "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
        if(tokens[pos].getType() != "EQUAL"){
            cout<<"Error on line"<<tokens[pos].getLine()<<"column"<<tokens[pos].getColumn()<<"Expected EQUAL but got"<<tokens[pos].getValue()<<endl;
            return;
        }
        constList->addChild(new ParseTreeNode("EQUAL", "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
        if(tokens[pos].getType() != "INT" && tokens[pos].getType() != "FLOAT" && tokens[pos].getType() != "STRING"){
            cout<<"Error on line"<<tokens[pos].getLine()<<"column"<<tokens[pos].getColumn()<<"Expected INT, FLOAT or STRING but got"<<tokens[pos].getValue()<<endl;
            return;
        }
        ParseTreeNode* Value = new ParseTreeNode("Value", "", tokens[pos].getLine(), tokens[pos].getColumn());
        constList->addChild(Value);
        Value->addChild(new ParseTreeNode(tokens[pos].getValue(), "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
        if(tokens[pos].getType() != "PUNTO_COMA"){
            cout<<"Error on line"<<tokens[pos].getLine()<<"column"<<tokens[pos].getColumn()<<"Expected PUNTO_COMA but got"<<tokens[pos].getValue()<<endl;
            return;
        }
        constList->addChild(new ParseTreeNode("PUNTO_COMA", "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
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
        cout<<"Error on line"<<tokens[pos].getLine()<<"column"<<tokens[pos].getColumn()<<"Expected DOS_PUNTOS but got"<<tokens[pos].getValue()<<endl;
        return;
    }
    varList->addChild(new ParseTreeNode("DOS_PUNTOS", "", tokens[pos].getLine(), tokens[pos].getColumn()));
    pos++;
    //revisit since i don't understand
    if(tokens[pos].getType() != "STRING_TYPE"&& tokens[pos].getType() != "INTEGER_TYPE" && tokens[pos].getType() != "REAL_TYPE"){
        cout<<"Error on line"<<tokens[pos].getLine()<<"column"<<tokens[pos].getColumn()<<"Expected STRING, INT or REAL but got"<<tokens[pos].getValue()<<endl;
        return;
    }
    ParseTreeNode* type = new ParseTreeNode("Type", "", tokens[pos].getLine(), tokens[pos].getColumn());
    varList->addChild(type);
    type->addChild(new ParseTreeNode(tokens[pos].getType(), "", tokens[pos].getLine(), tokens[pos].getColumn()));
    pos++;
    if(tokens[pos].getType() != "PUNTO_COMA"){
        cout<<"Error on line"<<tokens[pos].getLine()<<"column"<<tokens[pos].getColumn()<<"Expected PUNTO_COMA but got"<<tokens[pos].getValue()<<endl;
        return;
    }
    varList->addChild(new ParseTreeNode("PUNTO_COMA", "", tokens[pos].getLine(), tokens[pos].getColumn()));
    pos++;
    parseVarList(tokens, pos, varList);
}
void AnalizadorSintactico::parseVarDecl(vector<Token> tokens, int &pos, ParseTreeNode* &root){
    if(tokens[pos].getType() == "ID"){
        ParseTreeNode* varDecl = new ParseTreeNode("VarDecl", "", tokens[pos].getLine(), tokens[pos].getColumn());
        root->addChild(varDecl);
        varDecl->addChild(new ParseTreeNode("ID", "", tokens[pos].getLine(), tokens[pos].getColumn()));
        pos++;
        if(tokens[pos].getType() == "COMA"){
            varDecl->addChild(new ParseTreeNode("COMA", "", tokens[pos].getLine(), tokens[pos].getColumn()));
            pos++;
            parseVarDecl(tokens, pos, varDecl);
        }
    } else {
        cout<<"Error on line"<<tokens[pos].getLine()<<"column"<<tokens[pos].getColumn()<<"Expected ID but got"<<tokens[pos].getValue()<<endl;
    }
}
void AnalizadorSintactico::parseMainCode(vector<Token> tokens, int &pos, ParseTreeNode* &root){
    ParseTreeNode* mainCode = new ParseTreeNode("MainCode", "", tokens[pos].getLine(), tokens[pos].getColumn());
    root->addChild(mainCode);
    if(tokens[pos].getType()!="BEGIN"){
        cout<<"Error on line"<<tokens[pos].getLine()<<"column"<<tokens[pos].getColumn()<<"Expected BEGIN but got"<<tokens[pos].getValue()<<endl;
        return;
    }
    mainCode->addChild(new ParseTreeNode("BEGIN", "", tokens[pos].getLine(), tokens[pos].getColumn()));
    pos++;
    parseStatementList(tokens, pos, mainCode);
    if(tokens[pos].getType()!="END"){
        cout<<"Error on line"<<tokens[pos].getLine()<<"column"<<tokens[pos].getColumn()<<"Expected END but got"<<tokens[pos].getValue()<<endl;
        return;
    }
    mainCode->addChild(new ParseTreeNode("END", "", tokens[pos].getLine(), tokens[pos].getColumn()));
    pos++;
    if(tokens[pos].getType()!="PUNTO"){
        cout<<"Error on line"<<tokens[pos].getLine()<<"column"<<tokens[pos].getColumn()<<"Expected PUNTO but got"<<tokens[pos].getValue()<<endl;
        return;
    }
    mainCode->addChild(new ParseTreeNode("PUNTO", "", tokens[pos].getLine(), tokens[pos].getColumn()));
    pos++;
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
        cout << "Error on line " << tokens[i].getLine() << " column " << tokens[i].getColumn() << ": Program expected but got " << tokens[i].getValue() << endl;
        return;
    }
    if(tokens[i].getType() == "ID")
    {
        current->addChild(new ParseTreeNode("id", tokens[i].getValue(), tokens[i].getLine(), tokens[i].getColumn()));
        i++;
    }
    else
    {
        cout << "Error on line " << tokens[i].getLine() << " column " << tokens[i].getColumn() << ": ID expected but got " << tokens[i].getValue() << endl;
        return;
    }
    if(tokens[i].getType () == "PUNTO_COMA")
    {
        current->addChild(new ParseTreeNode("PUNTO_COMA", tokens[i].getValue(), tokens[i].getLine(), tokens[i].getColumn()));
        i++;
    }
    else
    {
        cout << "Error on line " << tokens[i].getLine() << " column " << tokens[i].getColumn() << ": COMA expected but got " << tokens[i].getValue() << endl;
        return;
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