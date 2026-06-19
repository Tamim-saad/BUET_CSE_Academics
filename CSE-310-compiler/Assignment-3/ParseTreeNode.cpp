#include "SymbolTable.cpp"
#include <limits.h>
#include <string>
#include <vector>

using namespace std;

class ParseTreeNode {

public:
  string name; // name is for non-terminals
  bool isLeaf;
  int startLine, endLine;
  string token; // token,lexeme for terminals
  string lexeme;
  string dataType; // INT, FLOAT OR VOID

  vector<ParseTreeNode *> children;
  string nameList;
  string lastFoundLexeme;
  string lastFoundToken;

  ParseTreeNode(const string &n) { // non-terminal
    name = n;
    isLeaf = false;
    startLine = INT_MAX;
    endLine = INT_MIN;
    nameList = "";
    dataType = "";
  }

  ParseTreeNode(const string &l, const string &t, int sL) {
    token = t;
    lexeme = l;
    startLine = sL;
    endLine = sL;
    isLeaf = true;
  }

  void addChild(ParseTreeNode *child) {
    startLine = min(startLine, child->startLine);
    endLine = max(endLine, child->endLine);
    string childName;

    if (child->isLeaf) {
      childName = child->token;
      lastFoundLexeme = child->lexeme;
      lastFoundToken = child->token;
    } else {
      childName = child->name;
      // let's update my lastFoundLexeme
      lastFoundLexeme = child->lastFoundLexeme;
      lastFoundToken = child->lastFoundToken;
    }
    nameList += " " + childName;
    // concatening children's name
    children.push_back(child);
  }
};