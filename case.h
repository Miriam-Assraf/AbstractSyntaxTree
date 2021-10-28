#ifndef __CASE_H
#define __CASE_H

#include "ast.h"
class Stmt;

class  Case : public ASTnode {
private:
    int number;
    Stmt* stmt;
    bool hasBreak;
    Case* next;
    int label;

public:
    // will be set when code is generated
    Case(int number, Stmt* stmt, bool hasBreak) : number(number), stmt(stmt), hasBreak(hasBreak), next(nullptr), label(-1) {};  

    Stmt* getStmt() const { return stmt; };
    Case* getNext() const { return next; };
    int getNumber() const { return number; };
    int getLabel() const { return label; };
    //bool getHasBreak() const { return hasBreak; };

    void setLabel(const int label) { this->label = label; };
    void setNext(Case* next) { this->next = next; };
};

#endif