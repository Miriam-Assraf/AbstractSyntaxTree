#include <stdlib.h>
#include "gen.h"
#include "expressions.h"
#include "object.h"
#include "symtab.h"

BinaryOp::BinaryOp(enum op op, Exp* left, Exp* right, int line): op(op), left(left), right(right), line(line)
{

	if (op == XOR_OP) {
		if (left->getType() != _INT || right->getType() != _INT) {	// both operands must be int
			errorMsg(ERR, "line %d: operator %s should be applied on opperands of type int only\n", line, opName(op, _INT));
		}
		type = _INT;	// type of returned value should be int
	}

	else {
		type = left->getType();
	}
}

NumNode::NumNode(int ival): Exp(_INT)
{
	setIval(ival);
}

NumNode::NumNode(double fval): Exp(_FLOAT)
{
	setFval(fval);
}

IdNode::IdNode(const char* name, int line): name(nullptr), line(line)
{
	myType t = getSymbol(name);
	if (t == UNKNOWN) {
		errorMsg(ERR, "line %d: variable %s is undefined\n", line, name);
		t = _INT;
	}
	type = t;

	this->name = new char[10];
	strcpy(this->name, name);
}

IdNode::IdNode(const IdNode& other) : name(other.name), line(other.line) {}

IdNode::~IdNode()
{
	delete[] name;
}

Object intResult(enum op op, int left, int right)
{
	Object result;

	switch (op) {
	case PLUS:
		result = Object(left + right);
		break;
	case MINUS:
		result = Object(left - right);
		break;
	case MUL:
		result = Object(left * right);
		break;
	case DIV:
		result = Object(left / right);
		break;
	case XOR_OP:
		if ((left == 0 || right == 0) && left != right)	// only one is 0
			result = Object(1);
		else
			result = Object(0);
		break;
	default:
		fprintf(stderr, "internal compiler error #3\n"); exit(1);
	}

	return result;
}

Object floatResult(enum op op, float left, float right)
{
	Object result;

	switch (op) {
	case PLUS:
		result = Object(left + right);
		break;
	case MINUS:
		result = Object(left - right);
		break;
	case MUL:
		result = Object(left * right);
		break;
	case DIV:
		result = Object(left / right);
		break;
	case XOR_OP:
		if ((left == 0 || right == 0) && left != right)	// only one is 0 than result is 1
			result = Object(1);
		else
			result = Object(0);
		break;
	default:
		fprintf(stderr, "internal compiler error #3\n"); exit(1);
	}

	return result;
}

Object BinaryOp::genExp()
{
	Object left_operand_result = left->genExp();
	Object right_operand_result = right->genExp();

	Object result;

	// if both operands are NumNodes - calculate value 
	if ((left_operand_result.getType() == INT_NUMBER || left_operand_result.getType() == FLOAT_NUMBER) &&
		(right_operand_result.getType() == INT_NUMBER || right_operand_result.getType() == FLOAT_NUMBER))
	{
		if (left_operand_result.getType() == INT_NUMBER) {	// left operand determines the type
			// helper function for ints (left operand determines the type) returns Object(result value) based on operands
			result = intResult(op, atoi(left_operand_result.getString()), atoi(right_operand_result.getString()));
		}
		else 
		{
			// helper function for floats (left operand determines the type) returns Object(result value) based on operands
			result = floatResult(op, atof(left_operand_result.getString()), atof(right_operand_result.getString()));
		}
	}

	// else return expression
	else {
		result = *newTemp();
		const char* the_op = opName(op, type);
		if (left->getType() != right->getType())
		{
			Object converted = *newTemp();

			if (left->getType() == _INT) 
			{
				emit("%s = (int) %s\n", converted.getString(), right_operand_result.getString());
			}
			else 
			{
				emit("%s = (float) %s\n", converted.getString(), right_operand_result.getString());
			}
			emit("%s = %s %s %s\n", result.getString(), left_operand_result.getString(),
				the_op, converted.getString());
		}
		else
		{
			emit("%s = %s %s %s\n", result.getString(), left_operand_result.getString(),
				the_op, right_operand_result.getString());
		}
	}

	return result;
}

Object NumNode::genExp()
{
	return (type == _INT) ? Object(value.ival) : Object(value.fval);
#if 0
	int result = newTemp();
	if (_type == _INT)
		emit("_t%d = %d\n", result, _u.ival);
	else
		emit("_t%d = %.2f\n", result, _u.fval);
	return result;
#endif
}

Object IdNode::genExp()
{
	return Object(name);
#if 0
	int result = newTemp();

	emit("_t%d = %s\n", result, _name);
	return result;
#endif
}