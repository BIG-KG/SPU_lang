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

static int isInicialiser  (analis_node_t testingNode);
static int isRightBracket (analis_node_t testingNode, int rightBrackes);

static void           getInicialArgs (function_t *currFunc, processing_programm_t *currProg);
static void           getCommands    (function_t *currFunc, processing_programm_t *currProg, analis_node_t *startNode);
static analis_node_t *getExpression  (function_t *currFunc, processing_programm_t *currProg);
static void           getFunction    (processing_programm_t *currProg);
static analis_node_t *getSumSub      (function_t *currFunc, processing_programm_t *currProg);
static analis_node_t *getMulDiv      (function_t *currFunc, processing_programm_t *currProg);
static analis_node_t *getBrackets    (function_t *currFunc, processing_programm_t *currProg);
static analis_node_t *getElementar   (function_t *currFunc, processing_programm_t *currProg);
static void           getFuncArgs    (function_t *currFunc, analis_node_t *currfuncCall,  processing_programm_t *currProg, int funckNum);
static analis_node_t *getCodeword    (function_t *currFunc, processing_programm_t *currProg);
static analis_node_t *getNewVar      (function_t *currFunc, processing_programm_t *currProg);
static analis_node_t *getWhile       (function_t *currFunc, processing_programm_t *currProg);



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


#define GLOBAL_ERROR_NULL                   \
    printf("Error on line %d\n", __LINE__); \
    global_errors = SYNTAX_ERROR;           \
    return NULL;                            \

int global_errors = 0;

node_t *getMain(analis_node_t *nodeArr)
{   
    int currNode = 0;
    
    function_t functionArray[MAX_NUM_OF_FUNK] = {};//remake using dynamic memory!!!!!!!!!!
    int        numOfFunction                  = 0;

    processing_programm_t currProg = {};
    currProg.nodeArr               =  nodeArr;  
    currProg.currNode              = &currNode;    
    currProg.numOfFunction         = &numOfFunction;
    currProg.functionArr           =  functionArray;


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

    getCommands(&functionArray[*numOfFunc], currProg, &functionArray[*numOfFunc].commandStart);

    if (isRightBracket (nodeArr[*currNode], '}') == 0){GLOBAL_ERROR}
    pp(*currNode);
}


static void getInicialArgs(function_t *currFunc, processing_programm_t *currProg)
{
    analis_node_t *nodeArr       = currProg->nodeArr;
    int           *currNode      = currProg->currNode;

    int *numOfArg = &currFunc->numOfArguments;
    int *numOfVar = &currFunc->numberOfVar;
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

        if(currFrmt == DOUBLE_FRMT || currFrmt == INT_FRMT)
        {
            currFunc->VariablesCODE[*numOfVar].variableCode = currCode;
            currFunc->VariablesCODE[*numOfVar].variableFrmt = currFrmt;
            pp(*numOfVar);  
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


static void getCommands(function_t *currFunc, processing_programm_t *currProg, analis_node_t *startNode)
{
    function_t    *functionArray = currProg->functionArr;
    analis_node_t *nodeArr       = currProg->nodeArr;
    int           *currNode      = currProg->currNode;
    int           *numOfFunc     = currProg->numOfFunction;

    analis_node_t *currCommand = startNode;

    analis_node_t *returningNode = NULL;

    while( isRightBracket(nodeArr[*currNode], '}') == 0)
    {
        if (nodeArr[*currNode].nodeType == CODEWORD)
        {
            returningNode = getCodeword(currFunc, currProg);
        }
        else
        {
            returningNode = getExpression(currFunc, currProg);
        }

        currCommand->right = returningNode;

        if (nodeArr[*currNode].nodeType != CODEWORD     ||
              nodeArr[*currNode].nodeData.int_el != END_COMMAND) return;

        nodeArr[*currNode].nodeType = EMPTY_NODE;
        nodeArr[*currNode].nodeData.int_el != 0;

        currCommand = &nodeArr[*currNode];
    }


    return;
}


static analis_node_t *getExpression(function_t *currFunc, processing_programm_t *currProg)
{   
    analis_node_t *nodeArr    =  currProg->nodeArr;
    int           *currNode   =  currProg->currNode;
    int           *currCommnd = &currFunc->numOfCommands;

    analis_node_t *left  = getSumSub(currFunc, currProg);
    analis_node_t *equal = NULL;
    analis_node_t *right = NULL;

    if (left == NULL) {GLOBAL_ERROR_NULL};


    if (nodeArr[*currCommnd].nodeType != EQUAL && nodeArr[*currCommnd].nodeType != COMPARISON)
    {
        
        return left;
    }

    else
    {   
        //Remove for rechecking___________________________________________________
        if (left->nodeType != VARIABLE && nodeArr[*currCommnd].nodeType == EQUAL)
        {
            printf("You can assign only to variables\n");

            GLOBAL_ERROR_NULL;
        }
        //________________________________________________________________________
        

        equal        = &nodeArr[*currCommnd];
        equal->left  = left;
        pp(*currNode);

        equal->right = getSumSub(currFunc, currProg); 

        if (equal->right == NULL){GLOBAL_ERROR_NULL}

        //Remove for rechecking___________________________________________________

        if (equal->left->nodeFormat != equal->right->nodeFormat)
        {
            printf("Сast types before assignment\n");
            GLOBAL_ERROR_NULL
        }

        equal->nodeType = equal->left->nodeType;

        //__________________________________________________________________________
        return equal;
    }
    
}
 

static analis_node_t *getSumSub (function_t *currFunc, processing_programm_t *currProg)
{
    analis_node_t *nodeArr    =  currProg->nodeArr;
    int           *currNode   =  currProg->currNode;
    int           *currCommnd = &currFunc->numOfCommands;

    analis_node_t *left  = getMulDiv (currFunc, currProg);
    analis_node_t *oper = NULL;

    if (left == NULL) return NULL;


    while ( nodeArr[*currCommnd].nodeType        == OPER         && 
             (nodeArr[*currCommnd].nodeData.int_el == SUM || 
                nodeArr[*currCommnd].nodeData.int_el == SUB   )           )
    {
        oper = &nodeArr[*currCommnd];
        oper->left  = left;

        pp(*currNode);
        oper->right = getMulDiv(currFunc, currProg); 

        if (oper->right == NULL){GLOBAL_ERROR_NULL}

        //Remove for rechecking___________________________________________

        if (oper->left->nodeFormat != oper->right->nodeFormat)
        {
            printf ("Сast types before assignment in SumSub\n");
            GLOBAL_ERROR_NULL
        }

        oper->nodeType = oper->left->nodeType;

        //_________________________________________________________________

        left = oper;
    }
    return left;
}

static analis_node_t * getMulDiv (function_t *currFunc, processing_programm_t *currProg)
{
    analis_node_t *nodeArr    =  currProg->nodeArr;
    int           *currNode   =  currProg->currNode;
    int           *currCommnd = &currFunc->numOfCommands;

    analis_node_t *left  = getBrackets (currFunc, currProg);
    analis_node_t *oper = NULL;
    analis_node_t *right = NULL;

    if (left == NULL) return NULL;


    while ( nodeArr[*currCommnd].nodeType == OPER                    && 
             (nodeArr[*currCommnd].nodeData.int_el == MUL || 
                nodeArr[*currCommnd].nodeData.int_el == DIV   )           )
    {
        oper = &nodeArr[*currCommnd];
        oper->left  = left;

        pp(*currNode);
        oper->right = getBrackets (currFunc, currProg); 

        if (oper->right == NULL){GLOBAL_ERROR_NULL}

        //_______________________________________________________________

        if (oper->left->nodeFormat != oper->right->nodeFormat)
        {
            printf ("Сast types before assignment in SumSub\n");
            GLOBAL_ERROR_NULL
        }

        oper->nodeType = oper->left->nodeType;

        //_______________________________________________________________

        left = oper;
    }
    return left;
}

static analis_node_t *getBrackets(function_t *currFunc, processing_programm_t *currProg)
{
    analis_node_t *nodeArr       = currProg->nodeArr;
    int           *currNode      = currProg->currNode;
    analis_node_t *returningNode = NULL;
    
    if ( isRightBracket(nodeArr[*currNode], '(') )
    {
        pp(*currNode);

        returningNode =  getSumSub(currFunc, currProg);

        if (isRightBracket(nodeArr[*currNode], ')') == 0){GLOBAL_ERROR_NULL}

        pp(*currNode);  

        return returningNode;
   }


    returningNode = getElementar(currFunc, currProg);

    return returningNode;

}

static analis_node_t *getElementar(function_t *currFunc, processing_programm_t *currProg)
{
    analis_node_t *nodeArr       = currProg->nodeArr;
    int           *currNode      = currProg->currNode;
    analis_node_t *returningNode = NULL;


    if (nodeArr[*currNode].nodeType == FRMT_CHANGE)
    {
        if( !isInicialiser(nodeArr[*currNode + 1]) || 
             nodeArr[*currNode].nodeType != FRMT_CHANGE) {GLOBAL_ERROR_NULL}

        pp(*currNode);

        analis_node_t *chengingFRMT = &nodeArr[*currNode];
        chengingFRMT->nodeType = FRMT_CHANGE;

        pp(*currNode);
        pp(*currNode);

        chengingFRMT->right = getElementar(currFunc, currProg);  
        return chengingFRMT;         
    }


    //Remove for rechecking__________________________________________________
    if (nodeArr[*currNode].nodeType == VARIABLE)
    {   
        int i = 0;
        while (currFunc->VariablesCODE[i].variableCode != -1 && 
                    currFunc->VariablesCODE[i].variableCode != nodeArr[*currNode].nodeData.int_el &&
                     i < 128)
        {
            i++;
        }

        if (currFunc->VariablesCODE[i].variableCode != -1 && i < 128)
        {
            analis_node_t *returningNode = &nodeArr[*currNode];

            returningNode->nodeType = INT_FRMT;

            pp(*currNode);

            return returningNode;
        }

    //________________________________________________________________________________
    }

    if (nodeArr[*currNode].nodeType == FUNC)
    {   

        //Remove for rechecking________________________________________________________
        bool findFunc = false;
        int i = 0;

        returningNode = &nodeArr[*currNode];

        for (i = 0; i < *(currProg->numOfFunction); i ++)
        {
            if(currProg->functionArr[i].functionCode == nodeArr[*currNode].nodeData.int_el)
            {
                findFunc = true;
                break;
            }
        }

        if (findFunc == false)
        {
            printf("NO SUCH FUNC\n");
            GLOBAL_ERROR_NULL;
        }

        //______________________________________________________________________________

        pp(*currNode);
        if (isRightBracket (nodeArr[*currNode], '(') == 0) {GLOBAL_ERROR_NULL}
        pp(*currNode);

        getFuncArgs(currFunc, &nodeArr[*currNode], currProg, i);

        if (isRightBracket (nodeArr[*currNode], '(') == 0) {GLOBAL_ERROR_NULL}
        pp(*currNode);

        return returningNode;
    }
    
    if (nodeArr[*currNode].nodeType == CONST)
    {
        return &nodeArr[*currNode];
    }
    
    return NULL;
}

static void getFuncArgs(function_t *currFunc, analis_node_t *currfuncCall,  processing_programm_t *currProg, int funckNum)
{
    function_t    *functionArray = currProg->functionArr;
    analis_node_t *nodeArr       = currProg->nodeArr;
    int           *currNode      = currProg->currNode;
    int           *numOfFunc     = currProg->numOfFunction;


    analis_node_t *right = currfuncCall;
    argument_t *argTypes = currProg->functionArr[funckNum].arguments;
    int numOfArgs        = currProg->functionArr[funckNum].numOfArguments;


//If you remove this check, you can add functions to a variable number of variables. + improve structuring by stages
    for(int i = 0; i < numOfArgs; i ++)
    {
        right->right = getSumSub(currFunc, currProg);

        if (right->right == NULL)
        {
            printf("Not enpth arguments\n");
            GLOBAL_ERROR
        }

        right = right->right;

        if (i < numOfArgs - 1)
        {
            if (nodeArr[*currNode].nodeType != COMMA)
            {
                printf("Not enoth arguments or no comma betwin args\n");
                GLOBAL_ERROR
            }

            pp(*currNode);
        }
    }
}


static analis_node_t *getCodeword (function_t *currFunc, processing_programm_t *currProg)
{
    function_t    *functionArray = currProg->functionArr;
    analis_node_t *nodeArr       = currProg->nodeArr;
    int           *currNode      = currProg->currNode;
    int           *numOfFunc     = currProg->numOfFunction;

    switch (nodeArr[*currNode].nodeData.int_el)
    {
    case IF_CODE_WORD:
    case WHILE_CODE_WORD:
        getWhile(currFunc, currProg);
        return NULL;
    default:
        break;
    }

    if (isInicialiser(nodeArr[*currNode]))
    {
        getNewVar(currFunc, currProg);
        return NULL;
    }
    else
    {
        printf("ERROR\n");
        GLOBAL_ERROR_NULL
    }
}

static analis_node_t *getNewVar (function_t *currFunc, processing_programm_t *currProg)
{
    function_t    *functionArray = currProg->functionArr;
    analis_node_t *nodeArr       = currProg->nodeArr;
    int           *currNode      = currProg->currNode;
    int           *numOfFunc     = currProg->numOfFunction;

    analis_node_t *inicialiser = &nodeArr[*currNode];

    inicialiser->right = getExpression(currFunc, currProg);

    return inicialiser;

    return;    
}


static analis_node_t *getWhile (function_t *currFunc, processing_programm_t *currProg)
{
    function_t    *functionArray = currProg->functionArr;
    analis_node_t *nodeArr       = currProg->nodeArr;
    int           *currNode      = currProg->currNode;
    int           *numOfFunc     = currProg->numOfFunction;

    analis_node_t *whileNode = &nodeArr[*currNode];
    pp(*currNode);

    if ( isRightBracket (nodeArr[*currNode], '(') ) {GLOBAL_ERROR}
    pp(*currNode);
    whileNode->left = getExpression(currFunc, currProg);
    pp(currFunc->numOfCommands);

    if ( isRightBracket (nodeArr[*currNode], ')') ) {GLOBAL_ERROR}
    pp(*currNode);



    if ( isRightBracket (nodeArr[*currNode], '{') ) {GLOBAL_ERROR}
    pp(*currNode);
    getCommands(currFunc, currProg, whileNode);
    if ( isRightBracket (nodeArr[*currNode], '}') ) {GLOBAL_ERROR}
    pp(*currNode);

    return whileNode;     
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