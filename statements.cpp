#include "statements.h"
#include "object.h"
#include "case.h"
#include "gen.h"

void ReadStmt::genStmt()
{
	myType idtype = _id->_type;

	if (idtype == _INT)
		emit("iread %s\n", _id->_name);
	else
		emit("fread %s\n", _id->_name);
}

void AssignStmt::genStmt()
{
	Object result = _rhs->genExp();

	myType idtype = _lhs->_type;

	if (idtype == _rhs->_type)
		emit("%s = %s\n", _lhs->_name, result._string);

	else {
		if (idtype == _INT) {
			emit("%s = (int) %s\n", _lhs->_name, result._string);
			errorMsg(WARNING, "line %d: WARNING - float value assigned to int variable\n", _line);
		}
		else {
			emit("%s = (float) %s\n", _lhs->_name, result._string);
		}
	}
}


void IfStmt::genStmt()
{
	int elseStmtlabel = newlabel();
	int exitlabel = newlabel();

	_condition->genBoolExp(FALL_THROUGH, elseStmtlabel);

	_thenStmt->genStmt();
	emit("goto label%d\n", exitlabel);
	emitlabel(elseStmtlabel);
	_elseStmt->genStmt();
	emitlabel(exitlabel);
}

void WhileStmt::genStmt()
{
	int condlabel = newlabel();
	int exitlabel = newlabel();

	continuelabels.push(condlabel);

	emitlabel(condlabel);
	_condition->genBoolExp(FALL_THROUGH, exitlabel);

	_body->genStmt();

	emit("goto label%d\n", condlabel);
	emitlabel(exitlabel);
	continuelabels.pop();	// after done with current while, pop it's continuelabel from stack
}

void RepeatStmt::genStmt()
{
	int condlabel = newlabel();
	int exitlabel = newlabel();

	Object numIters = _expression->genExp();	// get num iterantions from expression

	emitlabel(condlabel);
	emit("if %s <= 0 goto label%d\n", numIters._string, exitlabel);	// check condition before continuing to true statement, alse goto exitlabe

	_repeatStmt->genStmt();	// generate statement to be repeated

	emit("%s = %s - 1\n", numIters._string, numIters._string);	// after iteration decrease number of iterations to be done 
	emit("goto label%d\n", condlabel);	// jump back to check condition
	emitlabel(exitlabel);
}

void Block::genStmt()
{
	for (Stmt* stmt = _stmtlist; stmt != NULL; stmt = stmt->_next)
		stmt->genStmt();
}

void SwitchStmt::genStmt()
{
	Object exp = _exp->genExp();	// generate switch expression

	for (Case* c = _caselist; c != NULL; c = c->_next)	// for each case add new label and emit goto that label if switch expression matches switch number
	{
		c->_label = newlabel();
		emit("if %s == %d goto label%d\n", exp._string, c->_number, c->_label);
	}
	int defaultlabel = newlabel();	// for default statement
	int exitlabel = newlabel();	// for exit switch (break)

	emit("goto label%d\n", defaultlabel);	// if no case matched, goto default

	for (Case* c = _caselist; c != NULL; c = c->_next)	// for each case emit it's label, it's statement and break (goto exitlabel)
	{
		emitlabel(c->_label);
		c->_stmt->genStmt();
		emit("goto label%d\n", exitlabel);	// assume cases have break at the end
	}

	emitlabel(defaultlabel);	// after all cases emit default label and default statement
	_default_stmt->genStmt();

	emitlabel(exitlabel);	// emit exitlabel after all cases to break from switch
}

void BreakStmt::genStmt()
{
	emit("break statements not implemented yet\n");
}

void ContinueStmt::genStmt()
{
	// if continuelabels is empty, continue statement is not inside while loop
	if (continuelabels.empty()) {
		errorMsg(ERR, "line %d: continue statement should be inside while loop\n", _line);
	}
	else {
		emit("goto label%d\n", continuelabels.top());	// get label of current while loop (which is on top of continuelabels)
	}
}