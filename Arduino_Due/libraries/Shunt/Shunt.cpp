//
//  Shunting - yard: AnthonyDiGirolamo
//

#include "Shunt.h"
//#include <math.h>

/////////////////////////// shunt //////////////////////////////////

float eval_add(float a1, float a2) { return a1+a2; }
float eval_sub(float a1, float a2) { return a1-a2; }
float eval_uminus(float a1, float a2) { return -a1; }
float eval_exp(float a1, float a2) { return a2<0 ? 0 : (a2==0?1:a1*eval_exp(a1, a2-1)); }
float eval_mul(float a1, float a2) { return a1*a2; }
float eval_div(float a1, float a2) {
    if(!a2) {
        Serial.println(F("ERROR: Division by zero\n"));
        
    }
    return a1/a2;
}
float eval_mod(float a1, float a2) {
    if(!a2) {
        Serial.println(F("ERROR: Division by zero\n"));
        
    }
    return fmod(a1, a2);
    
}

enum {ASSOC_NONE=0, ASSOC_LEFT, ASSOC_RIGHT};

struct operator_type {
    char op;
    int prec;
    int assoc;
    int unary;
    float (*eval)(float a1, float a2);
} operators[]={
    {'_', 10, ASSOC_RIGHT, 1, eval_uminus},
    {'^', 9,  ASSOC_RIGHT, 0, eval_exp},
    {'*', 8,  ASSOC_LEFT,  0, eval_mul},
    {'/', 8,  ASSOC_LEFT,  0, eval_div},
    {'%', 8,  ASSOC_LEFT,  0, eval_mod},
    {'+', 5,  ASSOC_LEFT,  0, eval_add},
    {'-', 5,  ASSOC_LEFT,  0, eval_sub},
    {'(', 0,  ASSOC_NONE,  0, NULL},
    {')', 0,  ASSOC_NONE,  0, NULL}
};

struct operator_type *getop(char ch) {
    int i;
    for(i=0; i<sizeof operators/sizeof operators[0]; ++i) {
        if(operators[i].op==ch) return operators+i;
    }
    return NULL;
}

struct operator_type *opstack[MAXOPSTACK];
int nopstack=0;

float numstack[MAXNUMSTACK];
int nnumstack=0;

void push_opstack(struct operator_type *op)
{
    if(nopstack>MAXOPSTACK-1) {
        Serial.println(F("ERROR: Operator stack overflow\n"));
        
    }
    opstack[nopstack++]=op;
}

struct operator_type *pop_opstack()
{
    if(!nopstack) {
        Serial.println(F("ERROR: Operator stack empty\n"));
        
    }
    return opstack[--nopstack];
}

void push_numstack(float num)
{
    if(nnumstack>MAXNUMSTACK-1) {
        Serial.println(F("ERROR: Number stack overflow\n"));
        
    }
    numstack[nnumstack++]=num;
}

float pop_numstack()
{
    if(!nnumstack) {
        Serial.println(F("ERROR: Number stack empty\n"));
        
    }
    return numstack[--nnumstack];
}


void shunt_op(struct operator_type *op)
{
    struct operator_type *pop;
    float n1, n2;
    
    if(op->op=='(') {
        push_opstack(op);
        return;
        
    } else if(op->op==')') {
        while(nopstack>0 && opstack[nopstack-1]->op!='(') {
            pop=pop_opstack();
            n1=pop_numstack();
            
            if(pop->unary) push_numstack(pop->eval(n1, 0));
            else {
                n2=pop_numstack();
                push_numstack(pop->eval(n2, n1));
            }
        }
        
        if(!(pop=pop_opstack()) || pop->op!='(') {
            Serial.println(F("ERROR: Stack error. No matching \'(\'\n"));
            
        }
        return;
    }
    
    if(op->assoc==ASSOC_RIGHT) {
        while(nopstack && op->prec<opstack[nopstack-1]->prec) {
            pop=pop_opstack();
            n1=pop_numstack();
            if(pop->unary) push_numstack(pop->eval(n1, 0));
            else {
                n2=pop_numstack();
                push_numstack(pop->eval(n2, n1));
            }
        }
    } else {
        while(nopstack && op->prec<=opstack[nopstack-1]->prec) {
            pop=pop_opstack();
            n1=pop_numstack();
            if(pop->unary) push_numstack(pop->eval(n1, 0));
            else {
                n2=pop_numstack();
                push_numstack(pop->eval(n2, n1));
            }
        }
    }
    push_opstack(op);
}

int isdigit_or_decimal(int c) {
    if (c == '.' || isdigit(c))
        return 1;
    else
        return 0;
}

float shuntThis(char *in_formula) {
    char expression[35];
    nopstack=0;
    nnumstack=0;
    
    strcpy(expression, in_formula);
    
    char *expr;
    char *tstart=NULL;
    struct operator_type startop={'X', 0, ASSOC_NONE, 0, NULL};  //Dummy operator to mark start 
    struct operator_type *op=NULL;
    float n1, n2;
    struct operator_type *lastop=&startop;
    
    for (expr=expression; *expr; ++expr) {
        if (!tstart) {
            
            if ((op=getop(*expr))) {
                
                if (lastop && (lastop == &startop || lastop->op != ')')) {
                    if (op->op == '-')
                        op=getop('_');
                    else if (op->op!='(') {
                        Serial.println(F("ERROR: Illegal use of binary operator (%c)\n"));
                        return 0;
                    }
                }
                shunt_op(op);
                lastop=op;
            } else if (isdigit_or_decimal(*expr)) tstart = expr;
            else if (!isspace(*expr)) {
                Serial.println(F("ERROR: Syntax error\n"));
                return 0;
            }
        } else {
            if (isspace(*expr)) {
                push_numstack(atof(tstart));
                tstart=NULL;
                lastop=NULL;
            } else if ((op=getop(*expr))) {
                push_numstack(atof(tstart));
                tstart=NULL;
                shunt_op(op);
                lastop=op;
            } else if (!isdigit_or_decimal(*expr) ) {
                Serial.println(F("ERROR: Syntax error\n"));
                return 0;
            }
        }
    }
    if(tstart) push_numstack(atof(tstart));
    
    
    while(nopstack) {
        op=pop_opstack();
        n1=pop_numstack();
        if(op->unary) push_numstack(op->eval(n1, 0));
        else {
            n2=pop_numstack();
            push_numstack(op->eval(n2, n1));
        }
    }
    
    if(nnumstack!=1) {
        Serial.println(F("ERROR: Number stack has %d elements after evaluation. Should be 1.\n"));
        Serial.println(nnumstack);
        
        return 0;
    }
    //print value of formula numstack[0]
    //printf("%f\n", numstack[0]);
    
    return numstack[0];
    
}




