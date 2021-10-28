#include <stdlib.h>
#include "boolExpressions.h"
#include "object.h"
#include "gen.h"

SimpleBoolExp::SimpleBoolExp(enum op op, Exp* left, Exp* right) : op(op), left(left), right(right) {};

void SimpleBoolExp::genBoolExp(int truelabel, int falselabel)
{
	if (truelabel == FALL_THROUGH && falselabel == FALL_THROUGH)
		return; // no need for code 

	const char* the_op;

	Object left_result = left->genExp();
	Object right_result = right->genExp();

	switch (op) {
	case LT:
		the_op = "<";
		break;
	case GT:
		the_op = ">";
		break;
	case LE:
		the_op = "<=";
		break;
	case GE:
		the_op = ">=";
		break;
	case EQ:
		the_op = "==";
		break;
	case NE:
		the_op = "!=";
		break;
	default:
		fprintf(stderr, "internal compiler error #3\n"); exit(1);
	}

	if (truelabel == FALL_THROUGH)
		emit("ifFalse %s %s %s goto label%d\n", left_result.getString(), the_op,
			right_result.getString(), falselabel);
	else if (falselabel == FALL_THROUGH)
		emit("if %s %s %s goto label%d\n", left_result.getString(), the_op,
			right_result.getString(), truelabel);
	else { // no fall through
		emit("if %s %s %s goto label%d\n", left_result.getString(), the_op,
			right_result.getString(), truelabel);
		emit("goto label%d\n", falselabel);
	}
}

Or::Or(BoolExp* left, BoolExp* right) : left(left), right(right) {};

void Or::genBoolExp(int truelabel, int falselabel)
{
	if (truelabel == FALL_THROUGH && falselabel == FALL_THROUGH)
		return; // no need for code 

	if (truelabel == FALL_THROUGH) {
		int next_label = newlabel(); // FALL_THROUGH implemented by jumping to next_label
		left->genBoolExp(next_label, // if left operand is true then the OR expression
									   //is true so jump to next_label (thus falling through
									   // to the code following the code for the OR expression)
			FALL_THROUGH); // if left operand is false then 
						   // fall through and evaluate right operand   
		right->genBoolExp(FALL_THROUGH, falselabel);
		emitlabel(next_label);
	}
	else if (falselabel == FALL_THROUGH) {
		left->genBoolExp(truelabel, // if left operand is true then the OR expresson is true 
									  // so jump to  truelabel (without evaluating right operand)
			FALL_THROUGH); // if left operand is false then 
							// fall through and evaluate right operand
		right->genBoolExp(truelabel, FALL_THROUGH);
	}
	else { // no fall through
		left->genBoolExp(truelabel, // if left operand is true then the or expresson is true 
									  // so jump to  truelabel (without evaluating right operand)
			FALL_THROUGH); // if left operand is false then 
							// fall through and evaluate right operand
		right->genBoolExp(truelabel, falselabel);
	}
}

And::And(BoolExp* left, BoolExp* right) : left(left), right(right) {};

void And::genBoolExp(int truelabel, int falselabel)
{
	if (truelabel == FALL_THROUGH && falselabel == FALL_THROUGH)
		return; // no need for code 

	if (truelabel == FALL_THROUGH) {
		left->genBoolExp(FALL_THROUGH, // if left operand is true then fall through and evaluate
										 // right operand.
			falselabel); // if left operand is false then the AND expression is
						 // false so jump to falselabel);
		right->genBoolExp(FALL_THROUGH, falselabel);
	}
	else if (falselabel == FALL_THROUGH) {
		int next_label = newlabel(); // FALL_THROUGH implemented by jumping to next_label
		left->genBoolExp(FALL_THROUGH, // if left operand is true then fall through and
										 // evaluate right operand
			next_label); // if left operand is false then the AND expression 
						 //  is false so jump to next_label (thus falling through to
						 // the code following the code for the AND expression)
		right->genBoolExp(truelabel, FALL_THROUGH);
		emitlabel(next_label);
	}
	else { // no fall through
		left->genBoolExp(FALL_THROUGH, 	// if left operand is true then fall through and
										 // evaluate right operand
			falselabel); // if left operand is false then the AND expression is false
						 // so jump to falselabel (without evaluating the right operand)
		right->genBoolExp(truelabel, falselabel);
	}
}

Not::Not(BoolExp* operand) { operand = operand; }

void Not::genBoolExp(int truelabel, int falselabel)
{
	operand->genBoolExp(falselabel, truelabel);
}

//void Nand::genBoolExp(int truelabel, int falselabel)
//{
//	if (truelabel == FALL_THROUGH && falselabel == FALL_THROUGH)
//		return; // no need for code 
//
//	if (truelabel == FALL_THROUGH) {
//		int next_label = newlabel(); // create new label for true statement (following code after nand)  
//		// if left operand is true than fall through and evaluate right operand
//		// if left operand is false than NAND expression is true so jump to true statement (following code after nand)  
//		_left->genBoolExp(FALL_THROUGH, next_label);
//		// if got here than left operand is true
//		// if right operand is true than NAND expression is false so jump to falselabel
//		// if right operand is false than NAND expression is true so continue to true statement (following code)
//		_right->genBoolExp(falselabel, FALL_THROUGH);
//		emitlabel(next_label);	// emit label before true statement
//	}
//	else if (falselabel == FALL_THROUGH) {
//		// if left operand is true than fall through and evaluate right operand
//		// if left operand is false than NAND expression is true so jump to truelabel  
//		_left->genBoolExp(FALL_THROUGH, truelabel);
//		// if got here than left operand is true
//		// if right operand is true than NAND expression is false so continue to false statement (following code)
//		// if right operand is false than NAND expression is true so jump to truelabel
//		_right->genBoolExp(FALL_THROUGH, truelabel);
//	}
//	else { // no fall through
//		// if left operand is true than fall through and evaluate right operand
//		// if left operand is false than NAND expression is true so jump to truelabel  
//		_left->genBoolExp(FALL_THROUGH, truelabel);
//		// if got here than left operand is true
//		// if right operand is true than NAND expression is false so jump to false label
//		// if right operand is false than NAND expression is true so jump to truelabel
//		_right->genBoolExp(falselabel, truelabel);
//	}
//}