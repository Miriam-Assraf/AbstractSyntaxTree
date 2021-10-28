#ifndef __BOOLEXPRESSIONS_H
#define __BOOLEXPRESSIONS_H

#include "ast.h"
#include "expressions.h"

// this is an abstract class
class BoolExp : public  ASTnode 
{
public:  //  some members  should be private ...
	/*  generate code for boolean expression. The code  should jump to
		truelabel or to falselabel depending on the value of the expression.
		truelabel may  be  FALL_THROUGH meaning  code should fall through to
		next instruction (rather than jump to a label)  when the expression is
		true. falselabel  may also be  FALL_TROUGH */
	virtual void genBoolExp(int truelabel, int falselabel) = 0; // every subclass should
											// override this (or be abstract too)
};

// nodes for  simple boolean expressions having the form 
// expression RELOP expression  
// for example  a < b
// another example:   (a + 3) < (z * 5 + y).    
class SimpleBoolExp : public BoolExp 
{
private:
	enum op op;
	Exp* left; // left operand
	Exp* right; // right operand

public:
	SimpleBoolExp(enum op op, Exp* left, Exp* right);
	void genBoolExp(int truelabel, int falselabel); // override
};

class Or : public BoolExp 
{
private:
	BoolExp* left; // left operand
	BoolExp* right; // right operand

public:
	Or(BoolExp* left, BoolExp* right);
	void genBoolExp(int truelabel, int falselabel); // override
};

class And : public BoolExp 
{
private:
	BoolExp* left; // left operand
	BoolExp* right; // right operand

public:
	And(BoolExp* left, BoolExp* right);
	void genBoolExp(int truelabel, int falselabel); // override
};


class Not : public BoolExp 
{
private:
	BoolExp* operand;

public:
	Not(BoolExp* operand);
	void genBoolExp(int truelabel, int falselabel); // override
};

//class Nand : public BoolExp {
//public:
//	Nand(BoolExp* left, BoolExp* right) { _left = left; _right = right; }
//	void genBoolExp(int truelabel, int falselabel); // override
//
//	BoolExp* _left; // left operand
//	BoolExp* _right; // right operand
//};

#endif