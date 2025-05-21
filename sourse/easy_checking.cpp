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

#define GLOBAL_ERROR                        \
    printf("%sError hz gde%d%s\n", RED, __LINE__, RESET); \
    return 1;                               \

#define pp(a) a = a + 1 


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
    }
}

int check_all_prog(function_t *functionArr)
{
    int i = 0;

    while (functionArr[i].initialized == 1)
    {
        if(easy_checkalltree(&functionArr[i]) != 0) break;
        i ++;
    }
}

static int easy_checkalltree(function_t *function)
{
    int findMain             = 0;
    vartable_t varTable      = {}; 
    callsequence_t currCalls = {};

    printf("%sstart check tree%s\n", YELLOW, RESET);
    
    for(int i = 0; i < function->numOfArguments; i ++)
    {
        varTable.variablesArr[i].varCode = function->argumentsArray[i].variableCode;
        function->argumentsArray[i].variableCode = i;
        varTable.numOfVariables += 1;
    }

    if (make_VriableTable_rec (function, &varTable, currCalls, function->commandStart.right) == NOT_FOUND_VAIABLE)
    {
        printf("%sERROR NOT FOUNDVAriable%s", RED, RESET);
        return 1;
    }
    printf("%snum of var = %d\n%s",RED ,function->numberOfVar = varTable.numOfVariables, RESET);


    return 0;
}


static int make_VriableTable_rec(function_t *currFunction, vartable_t *varTable,
                                  callsequence_t callsequence, analis_node_t *startingNode)
{
    analis_node_t *currNode = startingNode;
    int           *currDeep = &callsequence.deepOfCall;
    int           *numofVar = &varTable->numOfVariables;

    printf("%sstart make_VriableTable_rec %s\n", YELLOW, RESET);

    do{
        if(currNode->left->nodeType == CODEWORD)
        {
            if (currNode->left->nodeData.int_el == INT_CODE_WORD)
            {
                // TODO: make this a function
                printf("start int\n");
                analis_node_t *varCode = 0;

                if (currNode->left->right->nodeType == VARIABLE)
                {
                    varCode = currNode->left->right;
                }
                else if (currNode->left->right->nodeType== EQUAL && 
                                currNode->left->right->left->nodeType == VARIABLE)
                {
                    varCode = currNode->left->right->left;
                }
                else
                {
                    printf("Error in definition variable\n");
                    assert(0);
                }
                
                

                if (existSame (varTable, callsequence, varCode->nodeData.int_el))
                {
                    printf ("ERROR:trying to redeclarate \n");
                    assert (0);
                }

                varTable->variablesArr[*numofVar].varCode = (varCode->nodeData.int_el);
                varCode->nodeData.int_el = *numofVar;

                memcpy (&varTable->variablesArr[*numofVar].varCallSeq,  &callsequence,  sizeof (callsequence_t) );

                pp(*numofVar);

                currNode->left = currNode->left->right;
                printf("end int\n");
            }

            else if(currNode->left->nodeData.int_el == IF_CODE_WORD || currNode->left->nodeData.int_el == WHILE_CODE_WORD)
            {
                //check_ifwhile(currNode);

                callsequence.callSequenceArray[*currDeep] = currNode->left;
                pp (*currDeep);
                printf("%sFIND IF\n%s", YELLOW, RESET);
                make_VriableTable_rec (currFunction, varTable, callsequence, currNode->left);
                printf("%sEND IF\n%s", YELLOW, RESET);
                *currDeep = *currDeep - 1;
                callsequence.callSequenceArray[*currDeep] = NULL;
            }

            else
            {
                replaceVariables_rec (currNode->left->left,  varTable, &callsequence);
                replaceVariables_rec (currNode->left->right, varTable, &callsequence);
            }
            
        }

        else
        {
            if( replaceVariables_rec(currNode->left, varTable, &callsequence) == NOTFOUND) 
            {
                printf("adscscsdc\n");
                return NOT_FOUND_VAIABLE;
            }
            

            printf("end rec\n");
        }        

        printf("go right\n");
        currNode = currNode->right;

    } while (currNode != NULL);  

    printf("end while\n");
     
}

static int replaceVariables_rec(analis_node_t *checkingNode, vartable_t *variableTable, callsequence_t *callSeq)
{
    if(checkingNode == NULL) {return FOUND;}

    printf("start replaceVariables_rec\n");
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
        printf("end var\n");
        return FOUND;
    }

    else
    {
        if (replaceVariables_rec (checkingNode->left,  variableTable, callSeq) != FOUND) return NOTFOUND;
        if (replaceVariables_rec (checkingNode->right, variableTable, callSeq) != FOUND) return NOTFOUND;
        
        printf("end var\n");

        return FOUND;        
    }   
}

static int existSame(vartable_t *varTable, callsequence_t CurrPosityon, int varyableCode)
{
    for (int i = 0; i < varTable->numOfVariables; i ++)
    {
        if ( sameWays(&CurrPosityon, &varTable->variablesArr[i].varCallSeq)     && 
                             (varyableCode == varTable->variablesArr[i].varCode )           ) return 1;
    }

    return 0;
}

static int sameWays(callsequence_t *callSeqOne, callsequence_t *callSeqTwo)
{
    if(callSeqOne->deepOfCall != callSeqOne->deepOfCall) return 1;
    for(int i = 0; i < callSeqOne->deepOfCall; i++)
    {
        if(callSeqOne->callSequenceArray[i] != callSeqOne->callSequenceArray[i]) return 1;
    }

    return 0;
}

static int findMatch (vartable_t *varTable, int targetVarCode, callsequence_t *currCallSeq)
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
