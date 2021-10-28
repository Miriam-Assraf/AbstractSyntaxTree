#include "object.h"

Object::Object() : _type(ERROR)
{
	strcpy(_string, "error found");
}

Object::Object(char* variable) : _type(VARIABLE)
{
	strcpy(_string, variable);
}

Object::Object(int ival) : _type(INT_NUMBER)
{
	sprintf(_string, "%d", ival);
}

Object::Object(double fval) : _type(FLOAT_NUMBER)
{
	sprintf(_string, "%.2f", fval);
}
