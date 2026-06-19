#include "SymbolInfo.h"

SymbolInfo::SymbolInfo() {}

SymbolInfo::~SymbolInfo() {}

void SymbolInfo::setName(string name) { this->name = name; }

string SymbolInfo::getName() { return this->name; }

void SymbolInfo::setType(string type) { this->type = type; }

string SymbolInfo::getType() { return this->type; }

void SymbolInfo::setNext(SymbolInfo *nextsymbol) {
  this->nextsymbol = nextsymbol;
}

SymbolInfo *SymbolInfo::getNext() { return this->nextsymbol; }
