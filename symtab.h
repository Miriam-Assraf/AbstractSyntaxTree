#ifndef __SYMTAB_H
#define __SYMTAB_H

#include "gen.h"  //  for  myType 

// interface to symbol table 

// returns the type of the identifier
// returns UNKOWN when the identifier is not in the symbol table
extern myType getSymbol(const char* name);

// creates new entry in symbol table.
// return value: 0 if  name already exists in symboltable, otherwise returns 1 
extern int putSymbol(const char* name, myType type);

#endif  // not defined  __SYMTAB_H
