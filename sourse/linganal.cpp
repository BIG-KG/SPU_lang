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

const int START_ARR = 16;
char      string[100] = "x = 5y - sin(2);";
int        p = 0;

enum element_types
{
    CONST,
    VARIABLE,
    FUNC,
    OPER, 
    EQUAL,
    END_COMMAND,
    ERROR_EL,
    WORD_EL,
    SYNTAX_ERROR
};

int element_type(char currEl)
{
    if ( currEl == '+' || currEl == '-' || currEl == '*' ||  
         currEl == '/' || currEl == '^'                    ) return OPER;
    if (isalpha(currEl))                                      return WORD_EL;
    if (isalnum(currEl))                                      return CONST;
    if (currEl == '=')                                        return EQUAL;
    if (currEl == ';')                                        return END_COMMAND;

}

node_t *startLing(const char *string)
{
    node_t *nodeArr  = (node_t *)calloc(START_ARR, sizeof(node_t));
    int     currEL   = 0;
    int     currType = ERROR_EL;

    while (string[p] != '\0') 
    {
        while ( isspace(string[p]) ) p++;

        currType = element_type(string[p]);
        nodeArr[currEL].data.nodeType = currType;

        switch (currType)
        {
            case CONST:
                nodeArr[currEL].data.nodeData.cnst = scanDouble(&string, &p);
                break;
            case OPER:
                nodeArr[currEL].data.nodeData.func = scanOperation(&string, &p);
                break;
            case WORD_EL:
                nodeArr[currEL].data.nodeData.func = scanWord(&string, &p);
                break;
            case SYNTAX_ERROR:
                printf("Syntax error in %d symbol of string:\n\"%s\"", p, )
            
        }    
    }
}

