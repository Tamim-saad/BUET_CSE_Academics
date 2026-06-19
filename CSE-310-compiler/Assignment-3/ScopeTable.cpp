#include "SymbolInfo.cpp"

static unsigned long long sdbm(unsigned char *str) {
  unsigned long long hash = 0;
  int c;

  while (c = *str++)
    hash = c + (hash << 6) + (hash << 16) - hash;

  return hash;
}

class ScopeTable {

public:
  ScopeTable *parent;

  string id;
  int suffix;
  unsigned long long bucket;

  SymbolInfo **arr;

  ScopeTable(unsigned long n, ScopeTable *par = nullptr) {

    bucket = n;
    parent = par;
    suffix = 0;
    setId();

    arr = new SymbolInfo *[bucket];

    for (int i = 0; i < bucket; i++) {
      arr[i] = nullptr;
    }
  }

  void setId() {

    if (parent == nullptr) {
      id = "1";
      return;
    }
    int linearId = stoi(parent->id) + (++parent->suffix);
    id = to_string(linearId);
  }

  int hashFunction(const string &name) {
    unsigned long long hashValue = sdbm((unsigned char *)name.c_str());
    return hashValue % bucket;
  }

  SymbolInfo *lookup(const string &name) {
    int index = hashFunction(name);
    SymbolInfo *curr = arr[index];
    int chain = 1;

    while (curr != nullptr) {
      if (curr->getName() == name) {
        return curr;
      }

      curr = curr->next;
      chain++;
    }

    return nullptr;
  }

  SymbolInfo *insertUtil(const string &name) {

    int index = hashFunction(name);

    SymbolInfo *curr = arr[index];

    while (curr != nullptr) {
      if (curr->getName() == name) {
        return curr;
      }
      curr = curr->next;
    }

    return nullptr;
  }

  bool insert(const string &name, const string &type, const string &idType,
              int size) {
    SymbolInfo *symbol = insertUtil(name);

    if (symbol != nullptr) {
      return false;
    }

    symbol = new IdInfo(name, idType, size);

    int index = hashFunction(name);
    int chain = 1;

    if (arr[index] == nullptr) {
      arr[index] = symbol;
      return true;
    }

    chain++;

    SymbolInfo *curr = arr[index];

    while (curr->next != nullptr) {
      curr = curr->next;
      chain++;
    }

    curr->next = symbol;

    return true;
  }

  bool insert(const string &name, const string &type, bool isFunction = false) {
    SymbolInfo *symbol = insertUtil(name);
    if (symbol != nullptr) {
      return false;
    }

    symbol = new FunctionInfo(name, type);

    int index = hashFunction(name);
    int chain = 1;

    if (arr[index] == nullptr) {
      arr[index] = symbol;
      return true;
    }

    chain++;

    SymbolInfo *curr = arr[index];

    while (curr->next != nullptr) {
      curr = curr->next;
      chain++;
    }

    curr->next = symbol;

    return true;
  }

  void printTable() {

    for (int i = 0; i < bucket; i++) {
      SymbolInfo *curr = arr[i];

      if (curr == nullptr) {
        continue;
      }

      while (curr != nullptr) {
        curr = curr->next;
      }
    }
  }

  std::string getTableString() {
    std::string tableString;

    tableString += "\tScopeTable# " + id + "\n";

    for (int i = 0; i < bucket; i++) {

      SymbolInfo *curr = arr[i];

      if (curr == nullptr) {
        continue;
      }

      tableString += "\t" + std::to_string(i + 1) + "--> ";

      while (curr != nullptr) {
        tableString += "<" + curr->getName() + ",";

        if (curr->isFunction) {
          FunctionInfo *currFunc = (FunctionInfo *)curr;
          tableString += "FUNCTION," + currFunc->getReturnType();
        } else {
          IdInfo *currId = (IdInfo *)curr;
          if (currId->size != -1) {
            tableString += "ARRAY";
          } else {
            tableString += currId->idType;
          }
        }
        tableString += "> ";
        curr = curr->next;
      }
      tableString += "\n";
    }

    return tableString;
  }

  SymbolInfo *removeUtil(const string &name) {

    int index = hashFunction(name);

    if (arr[index] == nullptr) {
      return nullptr;
    }

    int chain = 1;

    if (arr[index]->getName() == name) {
      SymbolInfo *temp = arr[index];
      arr[index] = arr[index]->next;
      return temp;
    }

    SymbolInfo *curr = arr[index]->next;
    SymbolInfo *prev = arr[index];

    chain++;

    while (curr != nullptr) {
      if (curr->getName() == name) {
        prev->next = curr->next;
        return curr;
      }
      curr = curr->next;
      prev = curr;
      chain++;
    }
    return nullptr;
  }

  bool remove(const string &name) {
    SymbolInfo *symbol = removeUtil(name);

    if (symbol == nullptr) {
      return false;
    }

    else {
      delete symbol;
      return true;
    }
  }

  ~ScopeTable() {
    for (int i = 0; i < bucket; i++) {
      SymbolInfo *curr = arr[i];
      while (curr != nullptr) {
        SymbolInfo *temp = curr;
        curr = curr->next;
        delete temp;
      }
      arr[i] = nullptr;
    }
    delete[] arr;
  }
};
