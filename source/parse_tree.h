#include <vector>
#include <string>
#include <iostream>
using namespace std;

class ParseTreeNode
{
public:
    ParseTreeNode(string name, string value, int line, int column);
    ParseTreeNode(string name, string value);
    ~ParseTreeNode();
    string name;
    string value;
    int line;
    int column;
    vector<ParseTreeNode *> children;
    void addChild(ParseTreeNode *child);
    void print(int level);
    string toJSON();
};
class ParseTree
{
public:
    ParseTreeNode *root;
    ParseTree();
    void print();
    void toJSON(string filename);
};