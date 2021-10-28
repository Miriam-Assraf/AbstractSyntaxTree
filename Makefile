# note: bison and flex think they are  generating C files  but here all files
# are compiled with g++ (the C++ compiler)
 
ast :
	g++ -o ast lex.yy.c ast.tab.c expressions.cpp boolExpressions.cpp statements.cpp symtab.cpp object.cpp gen.cpp

bison:
	bison -d ast.y

flex:
	flex ast_linux.lex

clean :
	rm ast ast.tab.c ast.tab.h lex.yy.c


