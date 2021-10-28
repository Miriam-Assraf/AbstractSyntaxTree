#ifndef __STATEMENTS_H
#define __STATEMENTS_H

#include <string.h>
#include <stack>
#include "ast.h"
#include "expressions.h"
#include "boolExpressions.h"
#include "case.h"

class Stmt : public ASTnode 
{
public:
	Stmt() : _next(NULL) {};
	virtual void genStmt() = 0; // abstract class - every subclass should override and implement this func

	Stmt* _next;  // used to link together statements in the same block
};

class ReadStmt : public Stmt 
{
public:
	ReadStmt(IdNode* id, int line) : Stmt(), _id(id), _line(line) {};
	void genStmt(); // override	  
private:
	IdNode* _id;  // note: IdNode is subclass of Exp but this IdNode is not really an expression
	int _line; // line in source code 
};

class AssignStmt : public Stmt 
{
public:
	AssignStmt(IdNode* lhs, Exp* rhs, int line) : Stmt(), _lhs(lhs), _rhs(rhs), _line(line) {};
	void genStmt(); // override	  
private:
	IdNode* _lhs; // left hand side
	Exp* _rhs; // right hand side
	int _line; // line in source code 
};

class RepeatStmt : public Stmt 
{
public:
	RepeatStmt(Exp* expression, Stmt* repeatStmt, int line);
	void genStmt(); // override
private:
	Exp* _expression;	// should be int - num times for iteration
	Stmt* _repeatStmt;
	int _line; // line in source code 
};

class IfStmt : public Stmt 
{
public:
	IfStmt(BoolExp* condition, Stmt* thenStmt, Stmt* elseStmt) : Stmt(), _condition(condition), _thenStmt(thenStmt), _elseStmt(elseStmt) {};
	void genStmt(); // override
private:
	BoolExp* _condition;
	Stmt* _thenStmt;
	Stmt* _elseStmt;
};

class WhileStmt : public Stmt 
{
public:
	WhileStmt(BoolExp* condition, Stmt* body) : Stmt(), _condition(condition), _body(body) {};
	void genStmt(); // override			  
private:
	BoolExp* _condition;
	Stmt* _body;
};

//  a block contains a list of statements. For now -- no declarations in a block
class Block : public Stmt 
{
public:
	Block(Stmt* stmtlist) : _stmtlist(stmtlist) {};
	void genStmt(); // override
private:
	Stmt* _stmtlist; // pointer to the first statement. each statement points to the next stmt
};

class BreakStmt : public Stmt 
{
public:
	BreakStmt(int line) : _line(line) {};
	void genStmt(); // override
private:
	int _line;  // source line
};

class ContinueStmt : public Stmt 
{
public:
	ContinueStmt(int line) : _line(line) {};
	void genStmt(); // override
private:
	int _line;  // source line
};

class SwitchStmt : public Stmt 
{
public:
	SwitchStmt(Exp* exp, Case* caselist, Stmt* default_stmt, int line);
	void genStmt(); // override
private:
	Exp* _exp;
	Case* _caselist;	// pointer to first case in caselist
	Stmt* _default_stmt;
	int _line;  // source line of switch token
};

#endif