#ifndef __AST_H
#define __AST_H

/*
enum myType { _INT, _FLOAT, UNKNOWN };
enum op { PLUS = 0, MINUS, MUL, DIV, LT, GT, LE, GE, EQ, NE };
*/

/*  FALL_THROUGH may be used as an argument to BoolExp::genBoolExp().
    Labels  are represented as positive integers so FALL_THROUGH has negative value */
const int FALL_THROUGH = -1;

// all nodes  in the AST (Abstract Syntax Tree) are of types derived from ASTnode 
class ASTnode {};

#endif  // not defined __AST_H