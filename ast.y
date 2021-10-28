%code {
#include <stdio.h>
#include <string>
#include <stdarg.h>
#include "symtab.h"

// differ between errors and warnnings
#define ERR 1
#define WARNING 2

  /* yylex () and yyerror() need to be declared here */
int yylex (void);
void yyerror (std::string s);

/* void emit (const char *format, ...);  */

// number of errors 
int errors;
int warnings;

// global counter for iota variables
static int counter = 0;
}

%code requires {
#include "gen.h"
//#include "ast.h"
//#include "object.h"
#include "expressions.h"
#include "boolExpressions.h"
#include "statements.h"
#include "case.h"
}

/* note: no semicolon after the union */
%union {
   int ival;
   // float numbers in the source program are stored as double
   double fval;
   enum op op;
   char name[10];
   
   myType type;
   
   //  pointers to AST nodes:
   Stmt *stmt;
   Block *block;
   Stmt *stmtlist; // points to first Stmt in the list (NULL if list is empty)
   ReadStmt *read_stmt;
   RepeatStmt *repeat_stmt;
   AssignStmt *assign_stmt;
   IfStmt *if_stmt;
   WhileStmt *while_stmt;
   SwitchStmt *switch_stmt;
   Case *caselist; //points to first Case in the list
   Case *mycase;
   BreakStmt *break_stmt;
   ContinueStmt *continue_stmt;
   Exp  *exp;
   BoolExp *boolexp;
   bool hasBreak;
}

%token <ival> INT_NUM
%token <fval> FLOAT_NUM
%token <op> ADDOP MULOP RELOP XOR
%token <name> ID

%token READ REPEAT IF ELSE WHILE  FOR INT FLOAT IOTA
%token OR AND NOT NAND SWITCH CASE DEFAULT BREAK CONTINUE

%type <type> type

%type <exp> expression
%type <boolexp> boolexp 
%type <stmt> stmt
%type <block> block
%type <stmtlist> stmtlist
%type <read_stmt> read_stmt 
%type <assign_stmt> assign_stmt 
%type <while_stmt> while_stmt
%type <if_stmt>  if_stmt
%type <switch_stmt> switch_stmt;
%type <caselist> caselist
%type <mycase> case
%type <hasBreak> optional_break;
%type <break_stmt> break_stmt
%type <continue_stmt> continue_stmt
%type <repeat_stmt> repeat_stmt 

%left OR 
%left AND NAND
%left XOR
%left ADDOP
%left MULOP


%error-verbose

%%
program    : declarations stmt { 
                     /* if (errors == 0) { for debugging: generate code even if errors found */
					     $2->genStmt (); emit ("halt\n"); 
						 /*} */
				     }

declarations: declarations type ID ';' { if (!(putSymbol ($3, $2))) 
                                         errorMsg (ERR, "line %d: redeclaration of %s\n", @3.first_line, $3);
					 };
            	| declarations ID '=' IOTA ';' {  if (!(putSymbol ($2, _INT))) // try to enter ID as int (iota initiates var as int), if failed, already declared
                                         errorMsg (ERR, "line %d: redeclaration of %s\n", @2.first_line, $2);
					 else { emit ("%s = %d\n", $2, counter); counter++;}};	// if didn't fail, emit ID = counter and increase counter
		| /* empty */;

type: INT { $$ = _INT; } |
      FLOAT { $$ = _FLOAT; };			  

stmt       :  assign_stmt { $$ = $1; } |
              read_stmt { $$ = $1; } |
              repeat_stmt { $$ = $1; } |
              /* write_stmt | */
			  while_stmt  { $$ = $1; } |
	          if_stmt     { $$ = $1; } |
			  for_stmt    { $$ = 0;  } | /* not implemented yet */
			  switch_stmt { $$ = $1; } |
			  break_stmt  { $$ = $1; } |
              continue_stmt { $$ = $1;}|
			  block       { $$ = $1; } ;
			  
read_stmt:    READ '(' ID ')' ';'{ 
                $$ = new ReadStmt (new IdNode ($3, @3.first_line), @1.first_line); };

repeat_stmt:    REPEAT '(' expression ')' stmt { $$ = new RepeatStmt ($3, $5, @1.first_line); };

/* write_stmt:   WRITE '(' expression ')' ';' ; */
                
assign_stmt:  ID '='  expression ';' { $$ = new AssignStmt (new IdNode ($1, @1.first_line),
                                                            $3, @2.first_line); };

while_stmt :  WHILE '(' boolexp ')' stmt { $$ = new WhileStmt ($3, $5); };

if_stmt    :  IF '(' boolexp ')' stmt ELSE stmt { $$ = new IfStmt ($3, $5, $7); };

for_stmt   :  FOR '(' assign_stmt boolexp ';' assign_stmt ')' stmt
										   
											   
switch_stmt : SWITCH '(' expression ')' '{' caselist DEFAULT ':' stmt '}' { $$ = new SwitchStmt ($3, $6, $9, @1.first_line); };

/*  semantic value of caselist is a pointer to the first case in a list of Cases. Each Case points to the 
    next Case on the list */ 
caselist : case caselist { $1->setNext($2);   
                           $$ = $1; };
        						   

caselist : case  { $$ = $1;};

case : CASE INT_NUM ':' stmt optional_break { $$ = new Case ($2, $4, $5); };

optional_break: BREAK ';'{ $$ = true; } | /* empty */ { $$ = false; } ;

break_stmt :  BREAK ';'  { $$ = new BreakStmt (@1.first_line); };

continue_stmt :  CONTINUE ';'  { $$ = new ContinueStmt (@1.first_line); };

block: '{' declarations stmtlist '}' { $$ = new Block ($3); };

/*  right recursion is used here because it simplifies the code  a bit.
    but left recursion  (i.e. stmtlist: stmtlist stmt)  would be better
    because it uses less stack space.
    Semantic value of stmtlist is a pointer to the first Stmt in a list 
	of Stmts. Each Stmt points to the next Stmt on the list
*/
stmtlist:  stmt stmtlist {  $1->setNext($2);  // also works when $2 is NULL
                            $$ = $1;  
						 };
stmtlist:  /* empty */ { $$ = NULL; };
	  
expression : expression ADDOP expression {
                  $$ = new BinaryOp ($2, $1, $3, @2.first_line); } |
	     expression MULOP expression {
                  $$ = new BinaryOp ($2, $1, $3, @2.first_line); }; |
	     expression XOR expression {
                  $$ = new BinaryOp ($2, $1, $3, @2.first_line); };
             
expression: '(' expression ')' { $$ = $2; } |
            ID          { $$ = new IdNode ($1, @1.first_line);} |
            INT_NUM     { $$ = new NumNode ($1); } |
			FLOAT_NUM   { $$ = new NumNode ($1); };
			
boolexp: expression RELOP expression { $$ = new SimpleBoolExp ($2, $1, $3); };
boolexp: boolexp OR boolexp { $$ = new Or ($1, $3); } |
         boolexp AND boolexp { $$ = new And ($1, $3); } |
		 boolexp NAND boolexp { $$ = new Not(new And ($1, $3)); } |
         NOT '(' boolexp ')' { $$ = new Not ($3); } |
		 '(' boolexp ')'  { $$ = $2;}
		 ;

%%
main (int argc, char **argv)
{
  extern FILE *yyin;
  if (argc != 2) {
     fprintf (stderr, "Usage: %s <input-file-name>\n", argv[0]);
	 return 1;
  }
  yyin = fopen (argv [1], "r");
  if (yyin == NULL) {
       fprintf (stderr, "failed to open %s\n", argv[1]);
	   return 2;
  }
  
  errors = 0;
  yyparse ();
   
  if (errors > 0) {
      fprintf(stderr, "compilation failed\n");
	  return 3;
  }
  
  fclose (yyin);
  return 0;
}

void yyerror (std::string s)
{
  extern int yylineno;  // defined by flex
  errorMsg(ERROR, "line %d : %s\n", yylineno, s.c_str());
}

void errorMsg (int type, const char *format, ...)
{
    	va_list argptr;
	va_start (argptr, format);
	// all the arguments following 'format' are passed on to vfprintf
	vfprintf (stderr, format, argptr); 
	va_end (argptr);
	
	if(type == ERROR) {
		errors++;
	}
	else {
		warnings++;
	}
}




