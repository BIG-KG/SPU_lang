#include <string.h>
//#include <strings.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#include <tree_types.h>
#include <tree_funck.h>
#include <tree_data_base_funk.h>
#include <tree_const.h>
#include <calculator.h>
#include <comporator.h>
#include <compiller_types.h>

static int isInicialiser  (analis_node_t testingNode);
static int isRightBracket (analis_node_t testingNode, int rightBrackes);

static void getInicialArgs(function_t *currFunc, processing_programm_t *currProg);
static void getCommands   (function_t *currFunc, processing_programm_t *currProg);


const int FUNK_NAME_SIZE = 30;
const int NUM_OF_FUNK    = 20;
const int MAX_NUM_OF_FUNK   = 128;

#define NAME_STR(a)  #a
extern char funcs[][20];

#define pp(a) a = a + 1 

#define GLOBAL_ERROR                        \
    printf("Error on line %d\n", __LINE__); \
    global_errors = SYNTAX_ERROR;           \
    return;                                 \


enum codewords
{
    DOUBLE,
    INT,
    IF,
    WHILE
};

int global_errors = 0;

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
        currOp->nodeType = FUNC;
        if (op == '+') currOp->nodeData.func = SUM;
        else           currOp->nodeData.func = SUB;

        currOp->right = prevOp;

        currOp->left  = getMulDiv();
    }

    return currOp;    
}

node_t *getMain(analis_node_t *nodeArr)
{   
    int currNode = 0;
    
    function_t functionArray[MAX_NUM_OF_FUNK] = {};//remake using dynamic memory!!!!!!!!!!
    int        numOfFunction                  = 0;

    processing_programm_t currProg = {};
    currProg.nodeArr       =  nodeArr;  
    currProg.currNode      = &currNode;    
    currProg.numOfFunction = &numOfFunction;
    currProg.functionArr   =  functionArray;


    while (isInicialiser(nodeArr[currNode]))
    {
        getFunction(&currProg);
        numOfFunction ++;

        if (global_errors != 0) break;
    }
    
    if (nodeArr[currNode].nodeType != END_OF_PROGRAM)
    {
        printf("SYNTAX ERROR: NO END OF PROGRAM ELEMENT\n");
        free(nodeArr);
    }
    
    return NULL;
}

void getFunction (processing_programm_t *currProg)
{
    function_t    *functionArray = currProg->functionArr;
    analis_node_t *nodeArr       = currProg->nodeArr;
    int           *currNode      = currProg->currNode;
    int           *numOfFunc     = currProg->numOfFunction;

    functionArray[*numOfFunc].returningType = nodeArr[*currNode].nodeFormat;
    pp(*currNode);

    if (nodeArr[*currNode].nodeType != FUNC)
    {
        printf("ERROR EXPECTED FUNCTION");
        global_errors = SYNTAX_ERROR;
        return;
    }
    functionArray[*numOfFunc].functionCode = nodeArr[*currNode].nodeData.int_el;
    pp(*currNode);



    if (isRightBracket (nodeArr[*currNode], '(') == 0){GLOBAL_ERROR}
    pp(*currNode);

    getInicialArgs(&functionArray[*numOfFunc], currProg);    

    if (isRightBracket (nodeArr[*currNode], ')') == 0){GLOBAL_ERROR}
    pp(*currNode);



    if (isRightBracket (nodeArr[*currNode], '{') == 0){GLOBAL_ERROR}
    pp(*currNode);

    getCommands(&functionArray[*numOfFunc], currProg);

    if (isRightBracket (nodeArr[*currNode], '}') == 0){GLOBAL_ERROR}
    pp(*currNode);
}

static void getInicialArgs(function_t *currFunc, processing_programm_t *currProg)
{
    analis_node_t *nodeArr       = currProg->nodeArr;
    int           *currNode      = currProg->currNode;

    int *numOfArg = &currFunc->numOfArguments;
    int *numOfIts = &currFunc->numberOfIntVar;
    int *numOfDbl = &currFunc->numberOfdoubleVar;
    int  currFrmt =  POISON_FRMT;
    int  currCode =  -1;

    *numOfArg = 0;

    while ( isInicialiser(nodeArr[*currNode]) )
    {   
        if (*numOfArg > 0 && nodeArr[*currNode - 1].nodeType != COMMA) {GLOBAL_ERROR}


        currFrmt = (currFunc->arguments[*numOfArg].argumentFrmt = nodeArr[*currNode].nodeData.int_el);
        pp(*currNode);


        if (nodeArr[*currNode].nodeType != VARIABLE){GLOBAL_ERROR}
        currCode = (currFunc->arguments[*numOfArg].argumentCode = nodeArr[*currNode].nodeData.int_el);
        pp(*currNode);


        if     (currFrmt == INT_FRMT)
        {
            currFunc->intVariablesCODE[*numOfIts] = currCode;
            pp(*numOfIts);
        }

        else if(currFrmt == DOUBLE_FRMT)
        {
            currFunc->doubleVariaclesCODE[*numOfDbl] = currCode;
            pp(*numOfDbl);  
        }

        else {GLOBAL_ERROR}


        pp(*numOfArg);

        if (nodeArr[*currNode].nodeType == COMMA)
        {
            pp(*currNode);
        }
    }

    return;    
}


static void getCommands(function_t *currFunc, processing_programm_t *currProg)
{
    function_t    *functionArray = currProg->functionArr;
    analis_node_t *nodeArr       = currProg->nodeArr;
    int           *currNode      = currProg->currNode;
    int           *numOfFunc     = currProg->numOfFunction;

    if (nodeArr[*currNode].nodeType == CODEWORD)
    {
        getCodeword(currFunc, currProg);
    }
    else
    {
        getExpression()
    }
    
}
 

node_t *getDouble(isInicialiser(nodeArr[*currNode]))
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

    cnstNode->nodeType      = CONST;
    cnstNode->nodeData.cnst = val;

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
        currOp->nodeType = FUNC;
        if (op == '*') currOp->nodeData.func = MUL;
        else           currOp->nodeData.func = DIV;

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

        returningNode->nodeType      = FUNC;
        returningNode->nodeData.func = POW;

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
        
        func_node->nodeType      = FUNC;
        func_node->nodeData.func = findfunc(funk_name);
        func_node->right         = param;

        if (s[p] != ')') assert(0);
        p++;

        return func_node;
    }

    else
    {
        node_t *var_node  = make_element();

        var_node->nodeType     = VARIABLE;
        var_node->nodeData.var = findVar(funk_name);

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


static int isInicialiser(analis_node_t testingNode)
{
    if (testingNode.nodeType == CODEWORD               && 
            (testingNode.nodeData.int_el == INT_CODE_WORD || 
             testingNode.nodeData.int_el == DOUBLE_CODE_WORD  )       ) return 1;
    return 0;
}

static int isRightBracket(analis_node_t testingNode, int rightBrackes)
{
    if (testingNode.nodeData.int_el == rightBrackes &&
            testingNode.nodeType == BRAKES) return 1;

    return 0;
}