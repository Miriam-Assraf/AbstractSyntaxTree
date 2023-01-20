GCC = g++
FLEX = flex
BISON = bison

GCC_FLAGS = -o
BISON_FLAGS = -d

EXE_TARGET = ast
FLEX_FILE = lex.yy.c
BISON_FILE = ast.tab.c
BISON_HEADER = ast.tab.h

CPP_FILES = expressions.cpp boolExpressions.cpp statements.cpp symtab.cpp object.cpp gen.cpp
LEX_FILE = ast_linux.lex
Y_FILE = ast.y

INPUT_FILE ?= $(shell bash -c 'read -p "input file path: " input_file; echo $$input_file')

build: $(CPP_FILES)
	$(FLEX) $(LEX_FILE)
	$(BISON) $(BISON_FLAGS) $(Y_FILE)
	$(GCC) $(GCC_FLAGS) $(EXE_TARGET) $(FLEX_FILE) $(BISON_FILE) $(CPP_FILES)

clean:
	rm -f ./$(EXE_TARGET) $(FLEX_FILE) $(BISON_FILE) $(BISON_HEADER)

run:
	./$(EXE_TARGET) $(INPUT_FILE)
