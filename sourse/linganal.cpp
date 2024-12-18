#include <string.h>
//#include <strings.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#include <tree_types.h>
#include <tree_funck.h>
//#include <tree_data_base_funk.h>
#include <tree_const.h>
//#include <calculator.h>
#include <compiller_types.h>

#define pp(a) a = a + 1 

const int START_ARR = 16;
const int MAX_COMMAND_SIZE = 128;
int       currEl = 0;

extern codeWord_t        codeWordArr[];
extern funktion_names_t *funcsArr;
analis_variable_t       variableArr[20] = {};


static analis_node_t make_funknode         (char *funckName);
static analis_node_t make_onlyType_node    (int type);
static analis_node_t scanDouble            (const char *string, int *startEl);
static analis_node_t scanOperation         (const char *string, int *startEl);
static analis_node_t scanWord              (const char *string, int *startEl);
static analis_node_t make_endOfCommandNode ();
static analis_node_t make_variableNode     (char * command);
static analis_node_t make_operation_node   (int codewordCode);
static analis_node_t scanEquality          (const char *string, int *startEl);
static analis_node_t make_brakesNode       (const char *string, int *startEl);
static int           isComparison          (char chekingEl);
static int           isOperation           (char chekingEl);
static int           isBracket             (char chekingEl);


int element_type (char checkingEl)
{   
    if (isBracket (checkingEl))   return BRAKES;
    if (isOperation (checkingEl)) return OPER;
    if (isalpha (checkingEl))     return WORD_EL;
    if (isalnum (checkingEl))     return CONST;
    if (checkingEl == ':')        return FRMT_CHANGE;
    if (checkingEl == ',')        return COMMA;
    if (isComparison(checkingEl)) return EQUAL;
    if (checkingEl == ';')        return END_COMMAND;

    return SYNTAX_ERROR;
}

analis_node_t *startLing (const char *string)
{
    analis_node_t *nodeArr   = (analis_node_t *) calloc (START_ARR, sizeof(analis_node_t));
    int     currNodeCapacity = START_ARR; 
    int     currEL           = 0;
    int     currNode         = 0;
    int     currType         = ERROR_EL;
    

    while (string[currEl] != '\0') 
    {
        printf("while\n");
        while ( isspace (string[currEl]) ) currEl++;
        
        currType = element_type (string[currEl]);
        nodeArr[currNode].nodeType = currType;

        switch (currType)
        {
            case CONST:
                nodeArr[currNode] = scanDouble (string, &currEl);
                break;

            case OPER:
                nodeArr[currNode] = scanOperation (string, &currEl);
                break;

            case WORD_EL:
                printf("wordEl\n");
                nodeArr[currNode] = scanWord (string, &currEl);
                break;

            case END_COMMAND:
                nodeArr[currNode] = make_onlyType_node(END_COMMAND);
                currEL ++;
                break;

            case FRMT_CHANGE:
                nodeArr[currNode] = make_onlyType_node(FRMT_CHANGE);
                currEL ++;
                break;

            case EQUAL:
                nodeArr[currNode] = scanEquality (string, &currEl);
                currEL ++;
                break;

            case COMMA:
                nodeArr[currNode] = make_onlyType_node(COMMA);
                currEL ++;
                break;

            case BRAKES:
                nodeArr[currNode] = make_brakesNode(string, &currEl);
                break;

            case SYNTAX_ERROR:
            default:
                printf ("Syntax error in %d symbol", currEl);

            if (nodeArr->nodeType == SYNTAX_ERROR)
            {
                printf("syntax Error;\n");
                free(nodeArr);
            }
        }
                        /*One more element for end_of_program*/
                        /*                 |                 */
        if (currNode == currNodeCapacity - 2)
        {
            analis_node_t *tmpNodeArr = (analis_node_t *) realloc (nodeArr, currNodeCapacity * 2);
            if (tmpNodeArr == NULL)
            {
                printf("Sorry, not enoth memory to load programm :(\n");
                return NULL;
            }

            currNodeCapacity *= 2;
            nodeArr = tmpNodeArr;
        }

        currNode ++; 
        currEl ++;  
    }

    nodeArr[currNode] = make_onlyType_node(END_OF_PROGRAM);

    return nodeArr;
}

static analis_node_t make_onlyType_node(int type)
{
    analis_node_t returningNode = {};

    returningNode.nodeType = type;

    return returningNode;
}

// REFACTOR WITH CODEGEN!!!!!!!!!!!!!!!!!!!!!!!!
static analis_node_t make_brakesNode (const char *string, int *startEl)
{
    analis_node_t returningNode = {};

    returningNode.nodeType = BRAKES;
    // REFACTOR WITH CODEGEN!!!!!!!!!!!!!!!!!!!!!!!!
    switch (string[*startEl])
    {
        case '(':
            returningNode.nodeData.int_el = '(';
            break;
        case ')':
            returningNode.nodeData.int_el = ')';
            break;
        case '[':
            returningNode.nodeData.int_el = '[';
            break;
        case ']':
            returningNode.nodeData.int_el = ']';
            break;
        case '{':
            returningNode.nodeData.int_el = '{';
            returningNode.nodeType        = CODEWORD;
            break;
        case '}':
            returningNode.nodeData.int_el = '}';
            break;
    //REFACTOR WITH CODEGEN!!!!!!!!!!!!!!!!!!!!!!!!
        default:
            returningNode.nodeData.int_el = SYNTAX_ERROR;
            break;
    }

    return returningNode;
}

static analis_node_t scanOperation (const char *string, int *startEl)
{
    analis_node_t returningNode = {};

    switch (string[*startEl])
    {
    case '+':
        returningNode.nodeData.int_el = SUM;
        break;
    case '-':
        returningNode.nodeData.int_el = SUB;
        break;
    case '*':
        returningNode.nodeData.int_el = MUL;
        break;
    case '/':
        returningNode.nodeData.int_el = DIV;
        break;
    case '^':
        returningNode.nodeData.int_el = POW;
        break;
    default:
        printf("Error in operation: %c\n", string[*startEl]);
        returningNode.nodeType = SYNTAX_ERROR;
        return returningNode;
        break;
    }

    *startEl = *startEl + 1;
    return returningNode;
}

static analis_node_t scanDouble (const char *string, int *startEl)
{
    double value = 0.0;
    analis_node_t returningNode = {CONST, };

    while (isalnum(string[*startEl]))
    {
        value = value * 10 + string[*startEl] - '0';
        *startEl = *startEl + 1;
    }

    if (string[*startEl] == '.')
    {
        *startEl = *startEl + 1;
        double pow = 0.1;
        while (isalnum (string[*startEl]))
        {
            value += pow * (string[*startEl] - '0'); 
            *startEl = *startEl + 1;
        }
    }

    returningNode.nodeData.double_el = value;

    return returningNode;
}



analis_node_t scanWord (const char *string, int *startEl)
{
    char command[MAX_COMMAND_SIZE] = "";
    int  currPos = 0;

    while ( isalpha (string[*startEl + currPos]) )
    {
        command[currPos] = string[*startEl + currPos];
        currPos ++;
    }
    command[currPos] = '\0';
    *startEl = *startEl + currPos;

    //printf("test func\n");

    if (string[*startEl + currPos] == '(')
    {   
        *startEl = *startEl + 1;
        return make_funknode (command);
    }
    
    //printf("test codeWord\n");

    for (int i = 0; codeWordArr[i].codeWordCode != NAO; i++)
    {
        if (strcasecmp (command, codeWordArr[i].wordName) == 0)
        {
            return make_operation_node (i);
        }
    }

    //printf("variabl\n");
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
            return 1;
            break;
        default:
            return 0;
            break;
    }
}

static int isComparison(char chekingEl)
{
    switch (chekingEl)
    {
        case '=':
        case '>':
        case '<':
        case '!':
        case '?':
            return 1;
            break;
        default:
            return 0;
            break;
    }
}

static analis_node_t make_funknode(char * command)
{
    analis_node_t returningNode = {FUNC,};

    if(command == NULL)
    {
        printf("ERROR: null ptr in make_difficult_node function\n");        
        returningNode.nodeType = SYNTAX_ERROR;
        return returningNode;
    }

    int i = 0;
    for(i; funcsArr[i].funktionCode != NOTAFUNC; i++)
    {
        if (strcmp (funcsArr[i].funktionName, command) == 0)
        {
            returningNode.nodeData.int_el = funcsArr[i].funktionCode;
            return returningNode;
        }    
    }
    funcsArr[i].funktionCode = i;
    strcpy (funcsArr[i].funktionName, command);
    returningNode.nodeData.int_el = i;

    return returningNode;
}

static analis_node_t scanEquality (const char *string, int *startEl)
{   
    char operation[3]           = "";
    analis_node_t returningNode = {};

    returningNode.nodeType      = COMPARISON;

    if ( isComparison (string[*startEl + 1]) )
    {
        memcpy(operation, string + *startEl, 2);
        *startEl = *startEl + 2;

        if ( !strcmp(operation, "?=") ) returningNode.nodeData.int_el = IS_EQUAL;
        if ( !strcmp(operation, "!=") ) returningNode.nodeData.int_el = IS_EQUAL;
        if ( !strcmp(operation, ">=") ) returningNode.nodeData.int_el = GREATER_OR_EQU;
        if ( !strcmp(operation, "<=") ) returningNode.nodeData.int_el = LESS_OR_EQU;
    }
    else
    {
        operation[0] = string[*startEl];
        pp(*startEl);

        if( !strcmp(operation, ">")  ) returningNode.nodeData.int_el = GEATER;
        if( !strcmp(operation, "<")  ) returningNode.nodeData.int_el = LESS;
        if( !strcmp(operation, "=")  ) 
        {
            returningNode.nodeType = EQUAL;
        }
    }

    return returningNode;
}


static analis_node_t make_operation_node(int codewordCode)
{
    analis_node_t returningNode = {CODEWORD,};

    returningNode.nodeData.int_el = codewordCode;

    return returningNode;
}

static analis_node_t make_variableNode (char * command)
{
    analis_node_t returningNode = {VARIABLE,};

    if(command == NULL)
    {
        printf("ERROR: null ptr in make_difficult_node function\n");        
        returningNode.nodeType = SYNTAX_ERROR;
        return returningNode;
    }

    int i = 0;

    for (int i; variableArr[i].initialized != 0; i++)
    {
        if (strcmp (variableArr[i].variableName, command) == 0)
        {
            returningNode.nodeData.int_el = i;
            return returningNode;
        }
    }

    variableArr[i].initialized = 1;
    strcpy(variableArr[i].variableName, command);
    returningNode.nodeData.int_el = i;

    return returningNode;
}