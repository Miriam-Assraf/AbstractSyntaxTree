#ifndef __EXPRESSIONS_H
#define __EXPRESSIONS_H

#include "ast.h"
#include "gen.h"

class Exp : public  ASTnode 
{
public:
    // Exp (int result, myType _type) : ASTnode () { this->_result = result; this->_type = _type; }
    Exp() : _type(UNKNOWN) {};

    // return value  is the  variable in which the result of the expression will be stored.
    // In case the expression is a numerical literal (for example 2 or 3.14) the literal 
    // is returned.  Both variables and literals are represented here as Objects.
    virtual Object genExp() = 0; // abstract class - every subclass should override and implement this func
    myType _type;
};

class BinaryOp : public Exp 
{
public:
    BinaryOp(enum op op, Exp* left, Exp* right, int line);
    Object genExp(); // override

    enum op _op;
    Exp* _left; // left operand
    Exp* _right; // right operand
    int _line;  // source line number of operator
};

class NumNode : public Exp 
{
public:
    NumNode(int ival) { _u.ival = ival; _type = _INT; }
    NumNode(double fval) { _u.fval = fval; _type = _FLOAT; }
    Object genExp(); // override

    union {
        int ival;
        double fval;
    } _u;
};

class IdNode : public Exp 
{
public:
    IdNode(char* name, int line);
    Object genExp(); // override

    char _name[100];
    int _line;  // source line number
};

#endif