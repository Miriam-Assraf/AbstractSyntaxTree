# intermediateCodeWithAST
 Three Address Code generator using Abstract Syntax Tree:
 * Translates C++ operations to TAC.
 * Supports Boolean operations.
 * Supports if, while and switch statements.
 *	Recursive descent parsing (top-down) for building ASTs.

 The project contains: 
 * .lex file, which contains regular expressions and actions, that is fed to flex which produces lex.yy.c containing yylex() function to peeform lexical analysis.
 * .y file, which contains the grammer of the language, that is fed to bison which produces a header and c files containing yyparse() function to perform parsing.
 * OOP written in c++ to create the AST and generate the intermediate code.
 * examples folder containing text files for inputs, named as {op}.txt, and outputs text files named as {op}.3.txt.
 * makefile to run over Linux.
 
 The grammar supports binary operations and statements, including if, while (with/without continue), repeat and switch. It also supports constant folding for optimization of arithmetic operations.
 
 In order to run over Linux:
 * run on terminal the following commands:
   * make flex
   * make bison
   * make ast
   * ./ast examples/{fname}, where fname is an input text file from examples dir
 
 In order to run over Windows:
 * requires installation of MinGW (quick and good tutorial https://www.youtube.com/watch?v=8CNRX1Bk5sY)
 * run on cmd the following commands:
   * win_flex ast_win.lex
   * win_bison -d ast.y
   * g++ -c lex.yy.c ast.tab.c expressions.cpp boolExpressions.cpp statements.cpp symtab.cpp object.cpp gen.cpp
   * g++ -o ast.exe lex.yy.o ast.tab.o expressions.o boolExpressions.o statements.o symtab.o object.o gen.o
   * ast.exe examples/{fname}, , where fname is an input text file from examples dir
