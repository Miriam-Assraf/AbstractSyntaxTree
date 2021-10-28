#ifndef __EXPRESSIONS_H
#define __EXPRESSIONS_H

#include "ast.h"
#include "gen.h"

class Exp : public  ASTnode 
{
protected:
    // Exp (int result, myType _type) : ASTnode () { this->_result = result; this->_type = _type; }
    Exp() : type(UNKNOWN) {};
    Exp(myType type) : type(type) {};
    myType type;

public:
    // return value  is the  variable in which the result of the expression will be stored.
    // In case the expression is a numerical literal (for example 2 or 3.14) the literal 
    // is returned.  Both variables and literals are represented here as Objects.
    virtual Object genExp() = 0; // abstract class - every subclass should override and implement this func
    myType getType() const { return type; };
};

class BinaryOp : public Exp 
{
private:
    enum op op;
    Exp* left; // left operand
    Exp* right; // right operand
    int line;  // source line number of operator

public:
    BinaryOp(enum op op, Exp* left, Exp* right, int line);
    Object genExp(); // override
};

class NumNode : public Exp 
{
private:
    union {
        int ival;
        double fval;
    }value;

public:
    NumNode(int ival);
    NumNode(double fval);
    Object genExp(); // override

    int getIval() const { return value.ival; };
    double getFval() const { return value.fval; };
    void setIval(const int ival) { value.ival = ival; };
    void setFval(const double fval) { value.fval = fval; };
};

class IdNode : public Exp 
{
private:
    char* name;
    int line;  // source line number

public:
    IdNode(const char* name, int line);
    IdNode(const IdNode& other);

    ~IdNode();

    Object genExp(); // override

    char* getName() const { return name; };
    int getLine() const { return line; };
};

#endif