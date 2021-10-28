#ifndef __GEN_H
#define __GEN_H

#define ERR 1
#define WARNING 2

#include <stdio.h>
#include <string.h>
#include <stack>

#pragma warning(disable: 4996)

class Object;

extern int errors;

static std::stack<int> continuelabels;

// An Object is either a variable (e.g. bar or _t5) or a numeric literal (e.g. 17 or 3.14)
enum objectType { INT_NUMBER, FLOAT_NUMBER, VARIABLE, ERROR };
enum myType { _INT, _FLOAT, UNKNOWN };
enum op { PLUS = 0, MINUS, MUL, DIV, XOR_OP, LT, GT, LE, GE, EQ, NE };

Object newTemp();
int newlabel();
void emit(const char* format, ...);
void emitlabel(int label);
void errorMsg(int type, const char* format, ...);

/* convert operator  to  string  suitable for the given type
  e.g  opName (PLUS, _INT)  returns "+"
       opName (PLUS, _FLOAT) returns  "plus"
*/
const char* opName(enum op, myType t);

/* there are two versions of each arithmetic operator in the
   generated code. One is  used for  operands having type int.
   The other is used for operands having type float.
*/
struct operator_names
{
	const char* int_name;
	const char* float_name;
};

static struct operator_names
opNames[] = { {"+", "<+>"},
			   {"-", "<->"},
			   {"*", "<*>"},
			   {"/", "</>"},
			   {"^", "^"} };

#endif // not defined __GEN_H
