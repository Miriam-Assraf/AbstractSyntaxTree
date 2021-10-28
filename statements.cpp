#include "statements.h"
#include "object.h"
#include "case.h"
#include "gen.h"

// ReadStmt
ReadStmt::ReadStmt(IdNode* id, int line) : Stmt(), id(id), line(line) {}

void ReadStmt::genStmt()
{
	myType idtype = id->getType();

	if (idtype == _INT)
		emit("iread %s\n", id->getName());
	else
		emit("fread %s\n", id->getName());
}

// AssignStmt
AssignStmt::AssignStmt(IdNode* lhs, Exp* rhs, int line) : Stmt(), lhs(lhs), rhs(rhs), line(line) {}

void AssignStmt::genStmt()
{
	Object result = rhs->genExp();

	myType idtype = lhs->getType();

	if (idtype == rhs->getType())
		emit("%s = %s\n", lhs->getName(), result.getString());

	else {
		if (idtype == _INT) {
			emit("%s = (int) %s\n", lhs->getName(), result.getString());
			errorMsg(WARNING, "line %d: WARNING - float value assigned to int variable\n", line);
		}
		else {
			emit("%s = (float) %s\n", lhs->getName(), result.getString());
		}
	}
}

// RepeatStmt
RepeatStmt::RepeatStmt(Exp* expression, Stmt* repeatStmt, int line): Stmt(), expression(expression), repeatStmt(repeatStmt), line(line)
{
	if (expression->getType() != _INT)	// expression should be int, because it is the number of times wanted for repeating the statement
		errorMsg(ERR, "line %d: expresion for repeat statement should be an int\n", line);
}

void RepeatStmt::genStmt()
{
	int condlabel = newlabel();
	int exitlabel = newlabel();

	Object numIters = expression->genExp();	// get num iterantions from expression

	emitlabel(condlabel);
	emit("if %s <= 0 goto label%d\n", numIters.getString(), exitlabel);	// check condition before continuing to true statement, alse goto exitlabe

	repeatStmt->genStmt();	// generate statement to be repeated

	emit("%s = %s - 1\n", numIters.getString(), numIters.getString());	// after iteration decrease number of iterations to be done 
	emit("goto label%d\n", condlabel);	// jump back to check condition
	emitlabel(exitlabel);
}

// IfStmt
IfStmt::IfStmt(BoolExp* condition, Stmt* thenStmt, Stmt* elseStmt) : Stmt(), condition(condition), thenStmt(thenStmt), elseStmt(elseStmt) {}

void IfStmt::genStmt()
{
	int elseStmtlabel = newlabel();
	int exitlabel = newlabel();

	condition->genBoolExp(FALL_THROUGH, elseStmtlabel);

	thenStmt->genStmt();
	emit("goto label%d\n", exitlabel);
	emitlabel(elseStmtlabel);
	elseStmt->genStmt();
	emitlabel(exitlabel);
}

// WhileStmt
WhileStmt::WhileStmt(BoolExp* condition, Stmt* body) : Stmt(), condition(condition), body(body) {}

void WhileStmt::genStmt()
{
	int condlabel = newlabel();
	int exitlabel = newlabel();

	continuelabels.push(condlabel);

	emitlabel(condlabel);
	condition->genBoolExp(FALL_THROUGH, exitlabel);

	body->genStmt();

	emit("goto label%d\n", condlabel);
	emitlabel(exitlabel);
	continuelabels.pop();	// after done with current while, pop it's continuelabel from stack
}

// Block
Block::Block(Stmt* stmtlist) : stmtlist(stmtlist) {}

void Block::genStmt()
{
	for (Stmt* stmt = stmtlist; stmt != NULL; stmt = stmt->getNext())
		stmt->genStmt();
}

// BreakStmt
BreakStmt::BreakStmt(int line) : line(line) {}

void BreakStmt::genStmt()
{
	emit("break statements not implemented yet\n");
}

// ContinueStmt
ContinueStmt::ContinueStmt(int line) : line(line) {}

void ContinueStmt::genStmt()
{
	// if continuelabels is empty, continue statement is not inside while loop
	if (continuelabels.empty()) {
		errorMsg(ERR, "line %d: continue statement should be inside while loop\n", line);
	}
	else {
		emit("goto label%d\n", continuelabels.top());	// get label of current while loop (which is on top of continuelabels)
	}
}

// SwitchStmt
SwitchStmt::SwitchStmt(Exp* exp, Case* caselist, Stmt* default_stmt, int line): exp(exp), caselist(caselist), default_stmt(default_stmt), line(line)
{
	if (exp->getType() != _INT)
		errorMsg(ERR, "line %d: switch expression should be int\n", line);
}

void SwitchStmt::genStmt()
{
	Object exp = this->exp->genExp();	// generate switch expression

	for (Case* c = caselist; c != NULL; c = c->getNext())	// for each case add new label and emit goto that label if switch expression matches switch number
	{
		c->setLabel(newlabel());
		emit("if %s == %d goto label%d\n", exp.getString(), c->getNumber(), c->getLabel());
	}
	int defaultlabel = newlabel();	// for default statement
	int exitlabel = newlabel();	// for exit switch (break)

	emit("goto label%d\n", defaultlabel);	// if no case matched, goto default

	for (Case* c = caselist; c != NULL; c = c->getNext())	// for each case emit it's label, it's statement and break (goto exitlabel)
	{
		emitlabel(c->getLabel());
		c->getStmt()->genStmt();
		emit("goto label%d\n", exitlabel);	// assume cases have break at the end
	}

	emitlabel(defaultlabel);	// after all cases emit default label and default statement
	default_stmt->genStmt();

	emitlabel(exitlabel);	// emit exitlabel after all cases to break from switch
}