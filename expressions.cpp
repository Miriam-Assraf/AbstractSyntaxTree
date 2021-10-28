#include <stdlib.h>
#include "gen.h"
#include "expressions.h"
#include "object.h"

Object intResult(enum op _op, int left, int right)
{
	Object result;

	switch (_op) {
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

Object floatResult(enum op _op, float left, float right)
{
	Object result;

	switch (_op) {
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
	Object left_operand_result = _left->genExp();
	Object right_operand_result = _right->genExp();

	Object result;

	// if both operands are NumNodes - calculate value 
	if ((left_operand_result._type == INT_NUMBER || left_operand_result._type == FLOAT_NUMBER) &&
		(right_operand_result._type == INT_NUMBER || right_operand_result._type == FLOAT_NUMBER))
	{
		if (left_operand_result._type == INT_NUMBER) {	// left operand determines the type
			// helper function for ints (left operand determines the type) returns Object(result value) based on operands
			result = intResult(_op, atoi(left_operand_result._string), atoi(right_operand_result._string));
		}
		else {
			// helper function for floats (left operand determines the type) returns Object(result value) based on operands
			result = floatResult(_op, atof(left_operand_result._string), atof(right_operand_result._string));
		}
	}

	// else return expression
	else {
		result = newTemp();
		const char* the_op = opName(_op, _type);
		if (_left->_type != _right->_type)
		{
			Object converted = newTemp();

			if (_left->_type == _INT) {
				emit("%s = (int) %s\n", converted._string, right_operand_result._string);
			}
			else {
				emit("%s = (float) %s\n", converted._string, right_operand_result._string);
			}
			emit("%s = %s %s %s\n", result._string, left_operand_result._string,
				the_op, converted._string);
		}
		else
		{
			emit("%s = %s %s %s\n", result._string, left_operand_result._string,
				the_op, right_operand_result._string);
		}
	}

	return result;
}

Object NumNode::genExp()
{
	return (_type == _INT) ? Object(_u.ival) : Object(_u.fval);
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
	return Object(_name);
#if 0
	int result = newTemp();

	emit("_t%d = %s\n", result, _name);
	return result;
#endif
}