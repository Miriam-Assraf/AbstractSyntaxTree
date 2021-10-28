#include "object.h"
#include <cstring>

Object::Object() : string(nullptr), type(ERROR)
{
	this->string = new char[10];
	strcpy(string, "error found");
}

Object::Object(const char* variable) : string(nullptr), type(VARIABLE)
{
	this->string = new char[10];
	strcpy(string, variable);
}

Object::Object(int ival) : string(nullptr), type(INT_NUMBER)
{
	this->string = new char[10];
	sprintf(string, "%d", ival);
}

Object::Object(double fval) : string(nullptr), type(FLOAT_NUMBER)
{
	this->string = new char[10];
	sprintf(string, "%.2f", fval);
}

Object::Object(const Object& other) : string(other.string), type(other.type){}

Object::~Object()
{
	delete[] string;
}