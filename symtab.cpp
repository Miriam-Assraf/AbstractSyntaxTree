
#include <map>
#include <string>
#include "symtab.h"

using namespace std;

static map<string, myType> symbolTable;

// returns the type of the identifier
// returns UNKOWN when the identifier is not in the symbol table
myType getSymbol(const char* name)
{
    string theName = name;
    map<string, myType>::iterator it;

    it = symbolTable.find(theName);
    if (it == symbolTable.end())
        return UNKNOWN;
    return it->second;
}

// return value: 0 if  name already exists in symboltable
//            otherwise returns 1 
int putSymbol(const char* name, myType type)
{
    string theName = name;
    if (symbolTable.count(name))
        return 0;
    symbolTable[theName] = type;
    return 1;
}

