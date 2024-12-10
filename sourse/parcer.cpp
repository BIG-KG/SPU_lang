#include <string.h>
#include <strings.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#include "..\headers\tree_types.h"
#include "..\headers\tree_funck.h"
#include "..\headers\tree_data_base_funk.h"
#include "..\headers\tree_const.h"
#include "..\headers\calculator.h"
#include "..\headers\comporator.h"

const int FUNK_NAME_SIZE = 30;
const int NUM_OF_FUNK    = 20;

#define NAME_STR(a)  #a
extern char funcs[][20];

int p = 0;

char s[100] = "d(asin(x^2))$";

// char s[100] = "d(ln(sin(x)^2))$";

node_t *getBrackets()
{
    if(s[p] == '(')
    {
        p++;
        node_t *val = getSumSub();
        if(s[p] != ')') 
        {
            printf("p = %d", p);           
            assert(0);
        }
        p++;
        return val;
    }

    else
    {
        return getDouble();
    }
 
}

node_t *getSumSub()
{
    //printf("start E\n");
    node_t *val  = getMulDiv();

    node_t *prevOp = NULL;
    node_t *currOp = val ;

    while (s[p] == '+' || s[p] == '-')
    {
        char op = s[p];
        p++;        

        if (currOp != NULL)
        {
            prevOp = currOp;
        }

        currOp = make_element();
        currOp->data.nodeType = FUNC;
        if (op == '+') currOp->data.nodeData.func = SUM;
        else           currOp->data.nodeData.func = SUB;

        currOp->right = prevOp;

        currOp->left  = getMulDiv();
    }

    return currOp;    
}


node_t *getMain()
{
    node_t *val = getSumSub();
    if(s[p] != '$')  assert(0);
    return val;
}

node_t *getDouble()
{
    //printf("start N\n");
    if( isalpha(s[p]) ) return getStr();

    node_t *cnstNode = make_element();
    double val = 0;
    int startPos = p;
    while ('0' <= s[p] && s[p] <= '9')
    {
        val = val * 10 + (double)(s[p] - '0');
        p++;
    }
    if(s[p] == '.')
    {   

        double step = 0.1;
        while ('0' <= s[p] && s[p] <= '9')
        {
            val += step * (double)(s[p] - '0');
            p++;
            step *= 0.1;
        }

    }

    if (startPos == p) assert(0);

    cnstNode->data.nodeType      = CONST;
    cnstNode->data.nodeData.cnst = val;

    return cnstNode;
}

node_t *getMulDiv()
{
     node_t *val  = getPow();

    node_t *prevOp = NULL;
    node_t *currOp = val ;

    while (s[p] == '*' || s[p] == '/')
    {
        char op = s[p];
        p++;        

        if (currOp != NULL)
        {
            prevOp = currOp;
        }

        currOp = make_element();
        currOp->data.nodeType = FUNC;
        if (op == '*') currOp->data.nodeData.func = MUL;
        else           currOp->data.nodeData.func = DIV;

        currOp->left  = prevOp;

        currOp->right = getPow();
    }

    return currOp;
}

node_t *getPow()
{
    node_t *val  = getBrackets();
    node_t *val1 = 0;
    node_t *returningNode = val;

    if(s[p] == '^')
    {
        p++;
        val1 = getBrackets();

        returningNode = make_element();

        returningNode->data.nodeType      = FUNC;
        returningNode->data.nodeData.func = POW;

        returningNode->left = val ;
        returningNode->right  = val1;       
    }

    return returningNode;
}




 
node_t *getStr()
{
    char funk_name[FUNK_NAME_SIZE] = {};
    int i = 0;
    printf("etstr = %c, %d\n", s[p], p);

    while( isalpha(s[p]) )
    {
        funk_name[i] = s[p];
        i++;
        p++;
    }

    if(i == 0) return NULL;

    if(s[p] == '(')
    {
        printf("p = %d\n", p);
        p++;
        node_t *param = getSumSub();
        node_t *func_node = make_element();
        
        func_node->data.nodeType      = FUNC;
        func_node->data.nodeData.func = findfunc(funk_name);
        func_node->right         = param;

        if (s[p] != ')') assert(0);
        p++;

        return func_node;
    }

    else
    {
        node_t *var_node  = make_element();

        var_node->data.nodeType     = VARIABLE;
        var_node->data.nodeData.var = findVar(funk_name);

        return var_node;
    }
}


int findfunc(char * const funcName)
{

    strcasecmp(NULL, NULL);

    printf("funk_name = %s\n", funcName);
    for(int i = 0; i < NUM_OF_FUNK; i ++)
    {
        printf("funk_cmp1 =%s\n\n", funcs[i]);
        if(!strcasecmp(funcName, funcs[i]))
        {
            return i;
        }
    }

    return -1;
}

char findVar(char * const valName)
{   
    return (*valName);
}