#ifndef __CASE_H
#define __CASE_H

#include "ast.h"
class Stmt;

class  Case : public ASTnode {
public:
    // will be set when code is generated
    Case(int number, Stmt* stmt, bool hasBreak) : _number(number), _stmt(stmt), _hasBreak(hasBreak), _next(NULL), _label(-1) {};  

    int _number;
    Stmt* _stmt;
    bool _hasBreak;
    Case* _next;
    int _label;
};

#endif