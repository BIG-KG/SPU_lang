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

function_t *getMain(analis_node_t *nodeArr)
{   
    printf("start Main\n");

    int currNode = 0;
    
    function_t *functionArray = (function_t *)calloc(sizeof(function_t), MAX_NUM_CMD_FNK);//remake using dynamic memory!!!!!!!!!!
    int        numOfFunction  = 0;

    processing_programm_t currProg = {};
    currProg.nodeArr               =  nodeArr;  
    currProg.currNode              = &currNode;    
    currProg.numOfFunction         = &numOfFunction;
    currProg.functionArr           =  functionArray;

    printf("start ini++++++++++++++++++++++++++++++++++++++++++++++\n");

    while (isInicialiser(nodeArr[currNode]))
    {
        getFunction(&currProg);
        numOfFunction ++;

        if (global_errors != 0) break;
    }

    printf("int init::::::::::::::::::::::::::::::::::::::::::::::::: num of args = %d\n", functionArray->numOfArguments);
    
    if (nodeArr[currNode].nodeType != END_OF_PROGRAM)
    {
        printf("SYNTAX ERROR: NO END OF PROGRAM ELEMENT\n");
        free(nodeArr);
    }
    
    return functionArray;
}

void getFunction (processing_programm_t *currProg)
{

    printf("start getF\n");

    function_t    *functionArray = currProg->functionArr;
    analis_node_t *nodeArr       = currProg->nodeArr;
    int           *currNode      = currProg->currNode;
    int           *numOfFunc     = currProg->numOfFunction;

    functionArray[*numOfFunc].returningType = nodeArr[*currNode].nodeFormat;
    pp(*currNode);

    if (nodeArr[*currNode].nodeType != FUNC)
    {
        printf("ERROR EXPECTED FUNCTION ,  tyoe = %d, c%d\n", nodeArr[*currNode].nodeType, *currNode);
        global_errors = SYNTAX_ERROR;
        return;
    }
    functionArray[*numOfFunc].functionCode = nodeArr[*currNode].nodeData.int_el;
    pp(*currNode);


    getInicialArgs(&functionArray[*numOfFunc], currProg);    

    if (isRightBracket (nodeArr[*currNode], ')') == 0){GLOBAL_ERROR}
    pp(*currNode);

    printf("KNKDJCcurrnode = %d\n", *currNode);



    if (isRightBracket (nodeArr[*currNode], '{') == 0){GLOBAL_ERROR}
    pp(*currNode);

    printf("start scan commands node = %d \n\n", *currNode);
    getCommands(&functionArray[*numOfFunc], currProg, &functionArray[*numOfFunc].commandStart);

    if (isRightBracket (nodeArr[*currNode], '}') == 0) {printf("currNode = %d   ", *currNode); GLOBAL_ERROR}
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

        if(currFrmt == INT_CODE_WORD || currFrmt == INT_CODE_WORD)
        {
            currFunc->VariablesCODE[*numOfVar].variableCode = currCode;
            if(currFrmt == INT_CODE_WORD) currFunc->VariablesCODE[*numOfVar].variableFrmt = INT_FRMT;
            else                          currFunc->VariablesCODE[*numOfVar].variableFrmt = INT_FRMT;  
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
    analis_node_t *emptyNode   = NULL;
    analis_node_t *returningNode = NULL;

    printf("start command\n");

    while( isRightBracket(nodeArr[*currNode], '}') == 0)
    {   
        printf("1111\n");
        if (nodeArr[*currNode].nodeType == CODEWORD)
        {   
            printf("\n\nfindcodeword\n\n");
            returningNode = getCodeword(currFunc, currProg);
        }
        else
        {
            printf("getExp\n");
            returningNode = getExpression(currFunc, currProg);
        }

        

        if (nodeArr[*currNode].nodeType != END_COMMAND) 
        {
            printf("end of f node = %d\n\n", *currNode);
            return;
            assert(0);
        }

        emptyNode = &nodeArr[*currNode];

        emptyNode->nodeType        = EMPTY_NODE;
        emptyNode->nodeData.int_el = 0;

        currCommand->right = emptyNode;
        emptyNode->left    = returningNode;
        emptyNode->right   = NULL;

        printf("\n\n\ncurr com%p  right %p\n", emptyNode, emptyNode->left);

        currCommand = emptyNode;

        pp(*currNode);
    }


    return;
}


static analis_node_t *getExpression(function_t *currFunc, processing_programm_t *currProg)
{   
    analis_node_t *nodeArr    =  currProg->nodeArr;
    int           *currNode   =  currProg->currNode;
    int           *currCommnd = &currFunc->numOfCommands;

    analis_node_t *left  = getSumSub(currFunc, currProg);
    printf("\n\nleft type = %d\n\n", left->nodeType);
    analis_node_t *equal = NULL;
    analis_node_t *right = NULL;

    if (left == NULL) {GLOBAL_ERROR_NULL};


    if (nodeArr[*currNode].nodeType != EQUAL && nodeArr[*currNode].nodeType != COMPARISON)
    {
        printf("no type = %d node = %d\n", nodeArr[*currNode].nodeType, *currNode);
        return left;
    }

    else
    {   
        printf("start equality\n");
        //Remove for rechecking___________________________________________________
        // if (left->nodeType != VARIABLE && nodeArr[*currNode].nodeType == EQUAL)
        // {
        //     printf("You can assign only to variables\n");

        //     GLOBAL_ERROR_NULL;
        // }
        //________________________________________________________________________
        

        equal        = &nodeArr[*currNode];
        equal->left  = left;
        pp(*currNode);

        equal->right = getSumSub(currFunc, currProg); 

        if (equal->right == NULL){GLOBAL_ERROR_NULL}

        //Remove for rechecking___________________________________________________

        // if (equal->left->nodeFormat != equal->right->nodeFormat)
        // {
        //     printf("Сast types before assignment\n");
        //     GLOBAL_ERROR_NULL
        // }

        // equal->nodeType = equal->left->nodeType;

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

    if (left == NULL){GLOBAL_ERROR_NULL};


    while ( nodeArr[*currNode].nodeType        == OPER         && 
             (nodeArr[*currNode].nodeData.int_el == SUM || 
                nodeArr[*currNode].nodeData.int_el == SUB   )           )
    {
        oper = &nodeArr[*currNode];
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

    if (left == NULL){GLOBAL_ERROR_NULL};


    while ( nodeArr[*currNode].nodeType == OPER                    && 
             (nodeArr[*currNode].nodeData.int_el == MUL || 
                nodeArr[*currNode].nodeData.int_el == DIV   )           )
    {
        oper = &nodeArr[*currNode];
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

    //Remove for rechecking__________________________________________________
    if (nodeArr[*currNode].nodeType == VARIABLE)
    {   
        printf("scanvar\n");
        analis_node_t *returningNode = &nodeArr[*currNode];

        pp(*currNode);

        return returningNode;

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

        getFuncArgs(currFunc, &nodeArr[*currNode], currProg, i);

        if (isRightBracket (nodeArr[*currNode], ')') == 0) {GLOBAL_ERROR_NULL}
        pp(*currNode);

        return returningNode;
    }
    
    if (nodeArr[*currNode].nodeType == CONST)
    {
        returningNode = &nodeArr[*currNode];
        pp(*currNode);

        return returningNode;

    }
    
    printf("curr node = %d", *currNode);

    GLOBAL_ERROR_NULL
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
    analis_node_t *returningNode = NULL;

    switch (nodeArr[*currNode].nodeData.int_el)
    {
    case IF_CODE_WORD:
    case WHILE_CODE_WORD:
        return getWhile(currFunc, currProg);

    case RETURN:
        returningNode = &nodeArr[*currNode];
        pp(*currNode);
        returningNode->right = getSumSub(currFunc, currProg);
        return returningNode;
    default:
        break;
    }

    if (isInicialiser(nodeArr[*currNode]))
    {
        return getNewVar(currFunc, currProg);;
    }
    else
    {
        printf("ERROR : %d\n", nodeArr[*currNode].nodeData.int_el);
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
}


static analis_node_t *getWhile (function_t *currFunc, processing_programm_t *currProg)
{
    function_t    *functionArray = currProg->functionArr;
    analis_node_t *nodeArr       = currProg->nodeArr;
    int           *currNode      = currProg->currNode;
    int           *numOfFunc     = currProg->numOfFunction;

    analis_node_t *whileNode = &nodeArr[*currNode];
    pp(*currNode);

    if ( isRightBracket (nodeArr[*currNode], '(') ) {GLOBAL_ERROR_NULL}
    pp(*currNode);
    whileNode->left = getExpression(currFunc, currProg);
    pp(currFunc->numOfCommands);

    if ( isRightBracket (nodeArr[*currNode], ')') ) {GLOBAL_ERROR_NULL}
    pp(*currNode);



    if ( isRightBracket (nodeArr[*currNode], '{') ) {GLOBAL_ERROR_NULL}
    pp(*currNode);
    getCommands(currFunc, currProg, whileNode);
    if ( isRightBracket (nodeArr[*currNode], '}') ) {GLOBAL_ERROR_NULL}
    pp(*currNode);

    return whileNode;     
}

static int isInicialiser(analis_node_t testingNode)
{
    if (testingNode.nodeType == CODEWORD               && 
            (testingNode.nodeData.int_el == INT_CODE_WORD)       ) return 1;
    return 0;
}

static int isRightBracket(analis_node_t testingNode, int rightBrackes)
{

    if (testingNode.nodeData.int_el == rightBrackes &&
            testingNode.nodeType == BRAKES) return 1;

    printf("type = %d, element = %d",testingNode.nodeType,  testingNode.nodeData.int_el);
    return 0;
}