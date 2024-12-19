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

const int NOT_FOUND_VAIABLE = 1;

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

struct vartable_t
{
    variableWhithCalls_t variablesArr[128] = {};
    int numOfVariables                     = 0;
};

void make_VriableTable_rec(function_t *currFunction, vartable_t *varTable,
                                callsequence_t callsequence, analis_node_t *startingNode)
{
    analis_node_t *currNode = startingNode;
    int           *currDeep = &callsequence.deepOfCall;
    int           *numofVar = &varTable->numOfVariables;

    do{
        if(currNode->left->nodeType == CODEWORD)
        {
            if (currNode->left->nodeData.int_el == INT_CODE_WORD)
            {
                if (existSame(varTable, callsequence, currNode->left->right->nodeData.int_el))
                {
                    printf("ERROR:trying to resctyvstr cymbo\n");
                    assert(0);
                }

                varTable->variablesArr[*numofVar].varCode = currNode->left->right->nodeData.int_el;

                memcpy (&varTable->variablesArr[*numofVar].varCallSeq,  &callsequence,  sizeof (callsequence_t) );
            }

            else if(currNode->left->nodeData.int_el == IF_CODE_WORD || currNode->left->nodeData.int_el == WHILE_CODE_WORD)
            {
                callsequence.callSequenceArray[*currDeep] = currNode->left;
                pp(*currDeep);
                make_VriableTable_rec(currFunction, varTable, callsequence, currNode->left);
                *currDeep = *currDeep - 1;
                callsequence.callSequenceArray[*currDeep] = NULL;
            }
        }

        else
        {
            if(replaceVariables_rec(currNode->left, varTable, &callsequence) == NOTFOUND) return NOT_FOUND_VAIABLE;
        }        

        currNode = currNode->right;
    } while (startingNode != NULL);  
     
}

enum varFindRsult
{
    FOUND,
    NOTFOUND
};

int replaceVariables_rec(analis_node_t *checkingNode, vartable_t *variableTable, callsequence_t *callSeq)
{
    int tmpVarCode = -1;
    if(checkingNode->nodeType == VARIABLE)
    {
        tmpVarCode = findMatch(variableTable, checkingNode->nodeData.int_el, callSeq);
        if(tmpVarCode < 0)
        {
            printf("No such variable declareted\n");
            return NOTFOUND;
        }

        checkingNode->nodeData.int_el = tmpVarCode;
        return FOUND;
    }

    else
    {
        if (replaceVariables_rec (checkingNode->left,  variableTable, callSeq) != FOUND) return NOTFOUND;
        if (replaceVariables_rec (checkingNode->right, variableTable, callSeq) != FOUND) return NOTFOUND;

        return FOUND;        
    }
}

int existSame(vartable_t *varTable, callsequence_t CurrPosityon, int varyableCode)
{
    for (int i = 0; i < varTable->numOfVariables; i ++)
    {
        if ( (memcmp (&CurrPosityon, &varTable->variablesArr[i].varCallSeq, 
                                    sizeof(callsequence_t)               ) == 0 )     && 
                             (varyableCode == varTable->variablesArr[i].varCode )           ) return 1;
    }

    return 0;
}

int findMatch (vartable_t *varTable, int targetVarCode, callsequence_t *currCallSeq)
{
    int *numOfVar     = &varTable->numOfVariables;
    int currDepth     = currCallSeq->deepOfCall;
    int varTableElemDepth = -1;

    int bestMutchNum   = -1;
    int bestMutchScore = -257; //deeper than the max possible recursion depth

    for (int i = 0; i < *numOfVar; i ++)
    {
        if (varTable->variablesArr[i].varCode == targetVarCode)
        {
            varTableElemDepth = varTable->variablesArr[i].varCallSeq.deepOfCall;
            if (varTableElemDepth > currDepth) break;

            for (int j = 0; j < varTableElemDepth; j ++)
            {
                if(varTable->variablesArr[i].varCallSeq.callSequenceArray[j] != currCallSeq->callSequenceArray[j])
                {
                    break;
                }
            }

            if (varTableElemDepth - currDepth > bestMutchScore)
            {
                bestMutchNum = i;
                bestMutchScore = varTableElemDepth - currDepth;
            }
        }
    }

    if (bestMutchNum < 0)
    {
        printf("no Such variable in this scope\n");
    }

    return bestMutchNum;
}