#include <stdio.h>
#include <stdlib.h> // exit ()
#include <stdarg.h>
#include <string>
#include "gen.h"
#include "object.h"

using namespace std;
/*    This stack is used to implement  continue statements.
	  "continue"  is implemented as a goto to the label attached
	  to the code for the condition of the most closely enclosing while
	  statement.
	  This label is at the top of the stack. Below it on the stack is the
	  continue label of the enclosing while statement and so on.
	  The stack is empty when we are not inside a  while statement.
*/

Object* newTemp()
{
	static int counter = 1;
	string name = "_t" + to_string(counter++);

	return new Object(name.c_str());
}


// labels are represented by numbers. For example, 3 means label3
int newlabel()
{
	static int counter = 1;
	return counter++;
}

// emit works just like  printf  --  we use emit 
// to generate code and print it to the standard output.
void emit(const char* format, ...)
{
	// if (errors > 0) return; // do not generate code if there are errors.  This should be controlled by if !defined (DEBUG)) 

	printf("    ");  // this is meant to add a nice indentation.
					  // Use emitlabel() to print a label without the indentation.    
	va_list argptr;
	va_start(argptr, format);
	// all the arguments following 'format' are passed on to vprintf
	vprintf(format, argptr);
	va_end(argptr);
}

/* use this  to emit a label without using indentation */
void emitlabel(int label)
{
	printf("label%d:\n", label);
}

/* convert operator  to  string  suitable for the given type
  e.g. opName (PLUS, _INT)  returns "+"
	   opName (PLUS, _FLOAT) returns  "<+>"
*/
const char* opName(enum op op, myType t)
{
	if (op > XOR_OP) { fprintf(stderr, "internal compiler error #1"); exit(1); }
	if (t == _INT)
	{
		return opNames[op].int_name;
	}
	else
		return opNames[op].float_name;
}

