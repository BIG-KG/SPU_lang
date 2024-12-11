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
#include "..\headers\errors.h"

const int START_ARR = 16;
const int MAX_COMMAND_SIZE = 128;
char      string[100] = "x = 5y - sin(2);";
int       currEl = 0;

extern codeWord_t codeWordArr[];
extern funktion_t funcsArr[];

static node_t make_funknode(char *funckName);
static int isBracket(char chekingEl);

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
    BRAKES,
    SYNTAX_ERROR
};

int element_type (char checkingEl)
{   
    if (isBracket (checkingEl)) return BRAKES;
    if (isOperation (currEl))   return OPER;
    if (isalpha (checkingEl))   return WORD_EL;
    if (isalnum (checkingEl))   return CONST;
    if (checkingEl == '=')      return EQUAL;
    if (checkingEl == ';')      return END_COMMAND;

    return SYNTAX_ERROR;
}

node_t *startLing (const char *string)
{
    int     currNodeCapacity = START_ARR; 
    node_t *nodeArr  = (node_t *) calloc (START_ARR, sizeof(node_t));
    int     currEL   = 0;
    int     currNode = 0;
    int     currType = ERROR_EL;

    while (string[currEl] != '\0') 
    {
        while ( isspace (string[currEl]) ) currEl++;
        
        currType = element_type (string[currEl]);
        nodeArr[currNode].data.nodeType = currType;

        switch (currType)
        {
            case CONST:
                nodeArr[currNode] = scanDouble (&string, &currEl);
                break;
            case OPER:
                nodeArr[currNode] = scanOperation (&string, &currEl);
                break;
            case WORD_EL:
                nodeArr[currNode] = scanWord (&string, &currEl);
                break;
            case END_COMMAND:
                nodeArr[currNode] = make_endOfCommandNode();
            case SYNTAX_ERROR:
            default:
                printf ("Syntax error in %d symbol", currEl);
        }

        if (currNode == currNodeCapacity - 1)
        {
            node_t *tmpNodeArr = (node_t *) realloc (nodeArr, currNodeCapacity * 2);
            if (tmpNodeArr == NULL)
            {
                printf("Sorry, not enoth memory to load programm :(\n");
                return NULL;
            }

            currNodeCapacity *= 2;
            nodeArr = tmpNodeArr;
        }

        currNode ++;   
    }

    return nodeArr;
}

node_t scanOperation (const char *string, int *startEl)
{
    char command[MAX_COMMAND_SIZE] = "";
    int  currPos = 0;

    while ( isalpha (string[*startEl + currPos]) )
    {
        command[currPos] = string[*startEl + currPos];
        currPos ++;
    }
    command[currPos] = '\0';

    if (string[*startEl + currPos] == '(')
    {
        return make_funknode (string);
    }

    for (int i = 0; codeWordArr[i].codeWordCode != NAO; i++)
    {
        if (strcasecmp (command, codeWordArr[i].wordName) == 0)
        {
            return make_operationNode (i);
        }
    }

    return make_variableNode (command); 
}

static int isBracket (char chekingEl)
{
    switch (chekingEl)
    {
        case '{':
        case '}':
        case '(':
        case ')':
        case '[':
        case ']':
            return 1;
            break;
        default:
            return 0;
            break;
    }
}

static int isOperation(char chekingEl)
{
    switch (chekingEl)
    {
        case '+':
        case '-':
        case '/':
        case '*':
        case '^':
            return 1;
            break;
        default:
            return 0;
            break;
    }
}