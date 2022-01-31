#include "parse_tree.h"
#include <fstream>
ParseTreeNode::ParseTreeNode(string name, string value, int line, int column)
{
    this->name = name;
    this->value = value;
    this->line = line;
    this->column = column;
}
ParseTreeNode::ParseTreeNode(string name, string value)
{
    this->name = name;
    this->value = value;
    this->line = 0;
    this->column = 0;
}
ParseTreeNode::~ParseTreeNode()
{
    for (int i = 0; i < children.size(); i++)
        delete children[i];
}
void ParseTreeNode::addChild(ParseTreeNode *child)
{
    children.push_back(child);
}
void ParseTreeNode::print(int level)
{
    for (int i = 0; i < level; i++)
        cout << "  ";
    cout << name << ": " << value << endl;
    for (int i = 0; i < children.size(); i++)
        children[i]->print(level + 1);
}
ParseTree::ParseTree()
{
    root = new ParseTreeNode("root", "", 0, 0);
}
void ParseTree::print()
{
    root->print(0);
}
void ParseTree::toJSON(string filename)
{
    string json = "";
    json += root->toJSON();
    fstream file;
    string filename_no_ext = filename.substr(0, filename.find_last_of("."));
    file.open(filename_no_ext + ".json", ios::out);
    file << json;
    file.close();
}
string ParseTreeNode::toJSON(){
    string json = "";
    json += "{\n";
    // json += "    \"" + name + "\": {\n";
    // json += "      \"value\": \"" + value + "\",\n";
    // json += "      \"line\": " + to_string(line) + ",\n";
    // json += "      \"column\": " + to_string(column) + ",\n";
    json+= "      \"name\": \"" + name + "\",\n";
    json += "      \"children\": [\n";
    for (int i = 0; i < children.size(); i++)
    {
        json += children[i]->toJSON();
        if (i != children.size() - 1)
            json += ",\n";
    }
    json += "      ]\n";
    json += "    }";
    return json;
}