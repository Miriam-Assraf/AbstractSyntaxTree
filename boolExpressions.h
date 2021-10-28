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
public:
	SimpleBoolExp(enum op op, Exp* left, Exp* right) : _op(op), _left(left), _right(right) {};
	void genBoolExp(int truelabel, int falselabel); // override

	enum op _op;
	Exp* _left; // left operand
	Exp* _right; // right operand
};

class Or : public BoolExp 
{
public:
	Or(BoolExp* left, BoolExp* right) : _left(left), _right(right) {};
	void genBoolExp(int truelabel, int falselabel); // override

	BoolExp* _left; // left operand
	BoolExp* _right; // right operand
};

class And : public BoolExp 
{
public:
	And(BoolExp* left, BoolExp* right) : _left(left), _right(right) {};
	void genBoolExp(int truelabel, int falselabel); // override

	BoolExp* _left; // left operand
	BoolExp* _right; // right operand
};


class Not : public BoolExp 
{
public:
	Not(BoolExp* operand) { _operand = operand; }
	void genBoolExp(int truelabel, int falselabel); // override

	BoolExp* _operand;
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