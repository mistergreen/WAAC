//
//  Shunting - yard: AnthonyDiGirolamo
//

#ifndef Shunt_h
#define Shunt_h

#include <ctype.h>
#include <Arduino.h>

#define MAXOPSTACK 64
#define MAXNUMSTACK 64


float eval_add(float a1, float a2);
float eval_sub(float a1, float a2);
float eval_uminus(float a1, float a2);
float eval_exp(float a1, float a2);
float eval_mul(float a1, float a2);
float eval_div(float a1, float a2);

float eval_mod(float a1, float a2);
struct operator_type *getop(char ch);
void push_opstack(struct operator_type *op);
struct operator_type *pop_opstack();
void push_numstack(float num);
float pop_numstack();
void shunt_op(struct operator_type *op);
int isdigit_or_decimal(int c);
float shuntThis(char *in_formula);

#endif
