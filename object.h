#ifndef __OBJECT_H
#define __OBJECT_H

#include "gen.h"

class Object 
{
private:
    enum objectType type;
    char* string; // for example "bar"  or "17" or "3.14"

public:
    Object();
    Object(const char* variable);
    Object(int ival);
    Object(double fval);
    Object(const Object& other);  // copy constructor

    ~Object();  // distructor

    enum objectType getType() const { return type; };
    char* getString() const { return string; };
};

#endif