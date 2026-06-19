#include "ScopeTable.cpp"
#include <iostream>
#include <list>

using namespace std;

class SymbolTable {
  ScopeTable *head;
  ScopeTable *tail;
  int bucket;

  list<FunctionInfo *> functionList;

public:
  SymbolTable(int n) {
    bucket = n;
    head = new ScopeTable(bucket);
    tail = head;
  }

  void enterScope() {
    ScopeTable *temp = new ScopeTable(bucket, tail);
    tail = temp;
  }

  void exitScope() {
    string str = "\t";

    if (head == tail) {
      str += "ScopeTable# " + tail->id + " cannot be deleted\n";
      return;
    }

    ScopeTable *temp = tail;
    tail = tail->parent;
    delete temp;
  }

  bool insert(const string &name, const string &type, const string &idType,
              int size) {
    return tail->insert(name, type, idType, size);
  }

  bool insert(const string &name, const string &type, bool isFunction = false) {
    if (tail->insert(name, type, isFunction)) {
      functionList.push_back((FunctionInfo *)tail->lookup(name));
      return true;
    }
    return false;
  }

  bool containsFunction(const string &name) {
    for (FunctionInfo *function : functionList) {
      if (function->getName() == name) {
        return true;
      }
    }

    return false;
  }

  bool remove(const string &name) { return tail->remove(name); }

  SymbolInfo *lookup(const string &name) {
    ScopeTable *curr = tail;

    while (curr != nullptr) {
      SymbolInfo *symbol = curr->lookup(name);

      if (symbol != nullptr)
        return symbol;
      curr = curr->parent;
    }
    return nullptr;
  }

  SymbolInfo *lookupCurrentScope(const string &name) {

    return tail->lookup(name);
  }

  SymbolInfo *lookupGlobalScope(const string &name) {
    return head->lookup(name);
  }

  void printCurrent() { tail->printTable(); }

  std::string printAll() {
    std::string temp;
    ScopeTable *curr = tail;

    while (curr != nullptr) {
      temp += curr->getTableString();
      curr = curr->parent;
    }
    return temp;
  }

  ~SymbolTable() {

    while (head != tail) {
      exitScope();
    }
    delete head;
  }
};