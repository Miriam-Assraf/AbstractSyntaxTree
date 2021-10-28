#ifndef __STATEMENTS_H
#define __STATEMENTS_H

#include <string.h>
#include "ast.h"
#include "expressions.h"
#include "boolExpressions.h"
#include "case.h"

class Stmt : public ASTnode 
{
protected:
	Stmt* next;  // used to link together statements in the same block
	Stmt() : next(NULL) {};

public:
	virtual void genStmt() = 0; // abstract class - every subclass should override and implement this fun
	Stmt* getNext() const { return next; };
	void setNext(Stmt* next) { this->next = next; };
};

class ReadStmt : public Stmt 
{
private:
	IdNode* id;  // note: IdNode is subclass of Exp but this IdNode is not really an expression
	int line; // line in source code 

public:
	ReadStmt(IdNode* id, int line);
	void genStmt(); // override	  
};

class AssignStmt : public Stmt 
{
private:
	IdNode* lhs; // left hand side
	Exp* rhs; // right hand side
	int line; // line in source code 

public:
	AssignStmt(IdNode* lhs, Exp* rhs, int line);
	void genStmt(); // override	  
};

class RepeatStmt : public Stmt 
{
private:
	Exp* expression;	// should be int - num times for iteration
	Stmt* repeatStmt;
	int line; // line in source code 

public:
	RepeatStmt(Exp* expression, Stmt* repeatStmt, int line);
	void genStmt(); // override
};

class IfStmt : public Stmt 
{
private:
	BoolExp* condition;
	Stmt* thenStmt;
	Stmt* elseStmt;

public:
	IfStmt(BoolExp* condition, Stmt* thenStmt, Stmt* elseStmt);
	void genStmt(); // override
};

class WhileStmt : public Stmt 
{
private:
	BoolExp* condition;
	Stmt* body;

public:
	WhileStmt(BoolExp* condition, Stmt* body);
	void genStmt(); // override			  
};

//  a block contains a list of statements. For now -- no declarations in a block
class Block : public Stmt 
{
private:
	Stmt* stmtlist; // pointer to the first statement. each statement points to the next stmt

public:
	Block(Stmt* stmtlist);
	void genStmt(); // override
};

class BreakStmt : public Stmt 
{
private:
	int line;  // source line

public:
	BreakStmt(int line);
	void genStmt(); // override
};

class ContinueStmt : public Stmt 
{
private:
	int line;  // source line

public:
	ContinueStmt(int line);
	void genStmt(); // override
};

class SwitchStmt : public Stmt 
{
private:
	Exp* exp;
	Case* caselist;	// pointer to first case in caselist
	Stmt* default_stmt;
	int line;  // source line of switch token

public:
	SwitchStmt(Exp* exp, Case* caselist, Stmt* default_stmt, int line);
	void genStmt(); // override
};

#endif