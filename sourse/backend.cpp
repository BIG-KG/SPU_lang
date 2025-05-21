#include <string.h>
#include <strings.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#include <tree_types.h>
#include <check_and_var.h>
#include <tree_funck.h>
#include <colors.h>
#include <tree_const.h>
//#include <calculator.h>
#include <compiller_types.h>

static int make_BranchNode(function_t *funcsArray, analis_node_t *startNode, FILE *printingFile, function_t *currFunc);
static int make_while(function_t *funcsArray, analis_node_t *startNode, FILE *printingFile, function_t *currFunc);
static int make_if(function_t *funcsArray, analis_node_t *startNode, FILE *printingFile, function_t *currFunc);
static int make_funcCall(function_t *funcsArray, analis_node_t *startNode, FILE *printingFile, function_t *currFunc);
static int make_oneNode_rec(function_t *funcsArray, analis_node_t *startNode, FILE *printingFile, function_t *currFunc);


int make_allProgramm(function_t *funcsArray, FILE *printingFile)
{
    int mainNum = -1;
    for (int i = 0; funcsArray[i].initialized == 1 ; i++)
    {
        if (funcsArray[i].functionCode == 0) mainNum = i;
        funcsArray->commandStart
        
    }

    if (mainNum < 0)
    {
        printf("%sNOT FOUND MAIN FUNC\n%s", RED, RESET);
        assert(0);
    }

    make_BranchNode(funcsArray, funcsArray[mainNum].commandStart.right, printingFile, &funcsArray[mainNum]);

    fprintf(printingFile, "end\n");

    for(int i = 0; funcsArray[i].initialized == 1; i ++)
    {
        if(funcsArray[i].functionCode == 0) continue;

        fprintf(printingFile, "\n\n^fn_%d\n", funcsArray[i].functionCode);

        for(int j = funcsArray[i].numOfArguments - 1; j >= 0; j-- )
        {
            fprintf(printingFile, "pop [BX+%d]\n", funcsArray[i].argumentsArray[j].variableCode);
        }

        make_BranchNode(funcsArray, funcsArray[i].commandStart.right, printingFile, &funcsArray[i]);

    }

    printf("%svsio bliat%s", RED, RESET);
    
}

static int make_oneNode_rec(function_t *funcsArray, analis_node_t *startNode, FILE *printingFile, function_t *currFunc)
{
    if (startNode == NULL)
    {
        return 0;
    }
    
    analis_node_t *currNode = startNode;

    switch (currNode->nodeType)
    {
    case EQUAL:
        make_oneNode_rec(funcsArray, currNode->right, printingFile, currFunc);
        fprintf(printingFile,"pop [BX+%d]\n", currNode->left->nodeData.int_el);  
        break;

    case OPER:

        make_oneNode_rec(funcsArray, currNode->left,  printingFile, currFunc);
        make_oneNode_rec(funcsArray, currNode->right, printingFile, currFunc);

        switch (currNode->nodeData.int_el)
        {
        case SUM:
            fprintf(printingFile, "add\n");
            break;
        case SUB:
            fprintf(printingFile, "sub\n");
            break;
        case MUL:
            fprintf(printingFile, "mul\n");
            break;
        case DIV:
            fprintf(printingFile, "div\n");
            break;
        
        default:
            break;
        }

        break;

    case CODEWORD:

        switch (currNode->nodeData.int_el)
        {
        case IF_CODE_WORD:
            make_if(funcsArray, startNode, printingFile, currFunc);
            break;

        case WHILE_CODE_WORD:
            make_while(funcsArray, startNode, printingFile, currFunc);
            break;

        case RETURN:
            if(currFunc->functionCode != 0)
            {
                make_oneNode_rec(funcsArray, currNode->right, printingFile, currFunc);
                fprintf(printingFile, "ret\n");
            }
            else
            {   
                make_oneNode_rec(funcsArray, currNode->right, printingFile, currFunc);
                fprintf(printingFile, "end\n", currFunc->functionCode);
            }
            break;

        case PRINT:
            make_oneNode_rec(funcsArray, currNode->right, printingFile, currFunc);
            fprintf(printingFile, "look\n");
            break;
        case INPUT:
            fprintf(printingFile, "input\n");
            fprintf(printingFile, "pop [BX+%d]\n", currNode->right->nodeData.int_el);
            break;

        default:
            break;
        }
        break;

    case CONST:
        fprintf(printingFile, "push %d\n", currNode->nodeData.int_el);
        break;

    case VARIABLE:
        fprintf(printingFile, "push [BX+%d]\n", currNode->nodeData.int_el);
        break;

    case FUNC:
        make_funcCall(funcsArray, currNode, printingFile, currFunc);
        break;
    }
        
}

static int make_funcCall(function_t *funcsArray, analis_node_t *startNode, FILE *printingFile, function_t *currFunc)
{
    int funcCode = startNode->nodeData.int_el;
    int i = 0;
    for(; funcsArray[i].initialized == 1; i++)
    {
        if (funcsArray[i].functionCode == funcCode) break;
    }

    make_BranchNode(funcsArray, startNode, printingFile, currFunc);

    fprintf(printingFile, "\npush BX\n");
    fprintf(printingFile, "push %d\n", currFunc->numberOfVar);
    fprintf(printingFile, "add\npop BX\n");

    fprintf(printingFile, "call :fn_%d\n", funcsArray[i].functionCode);

    fprintf(printingFile, "push BX\n");
    fprintf(printingFile, "push %d\n", currFunc->numberOfVar);
    fprintf(printingFile, "sub\npop BX\n\n");
}

static int make_if(function_t *funcsArray, analis_node_t *startNode, FILE *printingFile, function_t *currFunc)
{
    static int numOFif = 0;
    analis_node_t *rightEquality = startNode->left->right;
    analis_node_t *leftEquality  = startNode->left->left;

    #define printEquality_                                         \
        make_oneNode_rec(funcsArray, leftEquality,  printingFile, currFunc); \
        make_oneNode_rec(funcsArray, rightEquality, printingFile, currFunc); \


    switch (startNode->nodeData.int_el)
    {
    case IS_EQUAL:

        printEquality_;
        printEquality_;
        fprintf(printingFile, "jmpl :if_%d\n", numOFif);
        fprintf(printingFile, "jmpm :if_%d\n", numOFif);
        break;

    case IS_NOT_EQUAL:
        printEquality_;
        fprintf(printingFile, "jmpe :if_%d\n", numOFif);
        break;

    case GREATER:
        printEquality_;
        printEquality_;
        fprintf(printingFile, "jmpe :if_%d\n", numOFif);
        fprintf(printingFile, "jmpm :if_%d\n", numOFif);
        break;

    case LESS:
        printEquality_;
        printEquality_;
        fprintf(printingFile, "jmpe :if_%d\n", numOFif);
        fprintf(printingFile, "jmpl :if_%d\n", numOFif);
        break;

    case GREATER_OR_EQU:
        printEquality_;
        fprintf(printingFile, "jmpm :if_%d\n", numOFif);
        break;

    case LESS_OR_EQU:
        printEquality_;
        fprintf(printingFile, "jmpl :if_%d\n", numOFif);
        break;
    
    default:
        break;
    }

    make_BranchNode(funcsArray, startNode->right, printingFile, currFunc);

    fprintf(printingFile, "^if_%d\n", numOFif);

    numOFif ++;
}

static int make_while(function_t *funcsArray, analis_node_t *startNode, FILE *printingFile, function_t *currFunc)
{
    static int numOFwhile = 0;
    analis_node_t *rightEquality = startNode->left->right;
    analis_node_t *leftEquality  = startNode->left->left;

    fprintf(printingFile, "^while_%d\n", numOFwhile);

    make_BranchNode(funcsArray, startNode->right, printingFile, currFunc);


    switch (startNode->nodeData.int_el)
    {
    case IS_NOT_EQUAL:

        printEquality_;
        printEquality_;
        fprintf(printingFile, "jmpl :while_%d\n", numOFwhile);
        fprintf(printingFile, "jmpm :while_%d\n", numOFwhile);
        break;

    case IS_EQUAL:
        printEquality_;
        fprintf(printingFile, "jmpe :while_%d\n", numOFwhile);
        break;

    case LESS_OR_EQU:
        printEquality_;
        printEquality_;
        fprintf(printingFile, "jmpe :while_%d\n", numOFwhile);
        fprintf(printingFile, "jmpm :while_%d\n", numOFwhile);
        break;

    case GREATER_OR_EQU:
        printEquality_;
        printEquality_;
        fprintf(printingFile, "jmpe :while_%d\n", numOFwhile);
        fprintf(printingFile, "jmpl :while_%d\n", numOFwhile);
        break;

    case LESS:
        printEquality_;
        fprintf(printingFile, "jmpm :while_%d\n", numOFwhile);
        break;

    case GREATER:
        printEquality_;
        fprintf(printingFile, "jmpl :while_%d\n", numOFwhile);
        break;
    
    default:
        break;
    }

    numOFwhile ++;
}

static int make_BranchNode(function_t *funcsArray, analis_node_t *startNode, FILE *printingFile, function_t *currFunc)
{
    analis_node_t *currNode = startNode;
    do
    {
        make_oneNode_rec(funcsArray, currNode->left, printingFile, currFunc);
        currNode = currNode->right;

    } while (currNode != NULL);
    
    return 0;
}