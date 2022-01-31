#include <string>
#include <map>
using namespace std;
map<string, string> operators =
    {
        {"+", "OP_SUM"},
        {"-", "OP_RES"},
        {"*", "OP_MULT"},
        {"/", "OP_DIV"},
        {":=", "ASSIGN"},
        {"=", "EQUAL"},
        {"<>", "DIFFERENT"},
        {"<", "LESS"},
        {"<=", "LESS_EQUAL"},
        {">=", "GREATER_EQUAL"},
        {">", "GREATER"},
        {"^", "OP_POT"},
};
map<string, string> compound_operators = {
    {"and", "OP_AND"},
    {"or", "OP_OR"},
    {"not", "OP_NOT"},
    {"div", "OP_DIV_"},
    {"mod", "OP_MOD"},
    {"in", "OP_IN"}};
map<char, string> delimitadores = {
    {',', "COMA"},
    {';', "PUNTO_COMA"},
    {':', "DOS_PUNTOS"},
    {'.', "PUNTO"},
    {'(', "PARENTESIS_IZQ"},
    {')', "PARENTESIS_DER"},
    {'[', "CORCHETE_IZQ"},
    {']', "CORCHETE_DER"},
};
map<string, string> palabras_reservadas = {
    {"array", "ARRAY"},
    {"downto", "DOWNTO"},
    {"function", "FUNCTION"},
    {"of", "OF"},
    {"repeat", "REPEAT"},
    {"until", "UNTIL"},
    {"begin", "BEGIN"},
    {"else", "ELSE"},
    {"goto", "GOTO"},
    {"packed", "PACKED"},
    {"set", "SET"},
    {"var", "VAR"},
    {"case", "CASE"},
    {"end", "END"},
    {"if", "IF"},
    {"procedure", "PROCEDURE"},
    {"then", "THEN"},
    {"while", "WHILE"},
    {"const", "CONST"},
    {"file", "FILE"},
    {"label", "LABEL"},
    {"program", "PROGRAM"},
    {"to", "TO"},
    {"with", "WITH"},
    {"do", "DO"},
    {"for", "FOR"},
    {"nil", "NIL"},
    {"record", "RECORD"},
    {"type", "TYPE"},
    {"write", "WRITE"},
    {"writeln", "WRITELN"},
    //accounting for type
    {"string","STRING_TYPE"},
    {"integer","INTEGER_TYPE"},
    {"real","REAL_TYPE"},
};