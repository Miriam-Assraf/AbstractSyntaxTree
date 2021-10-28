#ifndef __OBJECT_H
#define __OBJECT_H

#include "gen.h"

class Object 
{
public:
    Object();
    Object(char* variable);
    Object(int ival);
    Object(double fval);

    enum objectType _type;
    char _string[100]; // for example "bar"  or "17" or "3.14"
};

#endif