#include <string.h>
//#include <strings.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#include <tree_types.h>
#include <tree_funck.h>
#include <colors.h>
#include <tree_const.h>
//#include <calculator.h>
#include <compiller_types.h>

#define GLOBAL_ERROR                        \
    printf("%sError hz gde%d%s\n", RED, __LINE__, RESET); \
    return 1;                               \

#define pp(a) a = a + 1 


static int check_Equality (analis_node_t *checking_node);
static int check_ifwhile  (analis_node_t *checking_node);

static int check_Equality(analis_node_t *checking_node)
{
    if(checking_node->left == NULL) return 0;

    if (checking_node->left->nodeType == EQUAL)
    {
        if(checking_node->left->left->nodeType != VARIABLE) {GLOBAL_ERROR}
    }

    return 0;
}

static int check_ifwhile(analis_node_t *checking_node)
{  
    if(checking_node->left == NULL) return 0;  
     
    if (checking_node->left->nodeType == CODEWORD && 
         (checking_node->left->nodeData.int_el == IF_CODE_WORD || 
          checking_node->left->nodeData.int_el == WHILE_CODE_WORD  )    )
    {
        if(checking_node->left->left->nodeType != COMPARISON) {GLOBAL_ERROR}
        easy_checkalltree(checking_node->left->right);
    }
}

int easy_checkalltree(analis_node_t *startChekckingNode)
{
    analis_node_t *currCheckNode = startChekckingNode;
    do
    {
        check_ifwhile  (currCheckNode);
        check_Equality (currCheckNode);

        currCheckNode = currCheckNode->right;

    } while (currCheckNode != NULL);
    
}

struct variableWhithCalls_t
{
    int            varCode    = -1;
    callsequence_t varCallSeq = {};
};

struct callsequence_t
{
    void *callSequenceArray[128] = {};
    int   deepOfCall             = 0;
};

struct vaiableable_t
{
    variableWhithCalls_t variablesArr[128] = {};
    int numOfVariables                     = 0;
};

void make_VriableTable_rec(function_t *currFunction, vaiableable_t *varTable,
                                callsequence_t callsequence, analis_node_t *startingNode)
{
    analis_node_t *currNode = startingNode;
    do{
        if(currNode->left->nodeType == CODEWORD)
        {
            if (currNode->left->nodeData.int_el == INT_CODE_WORD)
            {
                
            }
        }



    } while (startingNode != NULL);
    
    
     
}

int existSame(vaiableable_t *varTable, callsequence_t CurrPosityon)
{
    for (int i = 0; i < varTable->numOfVariables; i ++)
    {
        if (memcmp (&CurrPosityon, &varTable->variablesArr[i].varCallSeq, 
                                    sizeof(callsequence_t)               ) == 0) return 0;
    }

    return 1;
}