#ifndef CHECK_AND_VAR
#define CHECK_AND_VAR

const int NOT_FOUND_VAIABLE = 1;

#include <cstddef>

#include "tree_types.h"
#include "compiller_types.h"


struct callsequence_t
{
    void *callSequenceArray[128] = {};
    int   deepOfCall             = 0;
};

struct variableWhithCalls_t
{
    int            varCode    = -1;
    callsequence_t varCallSeq = {};
};

struct vartable_t
{
    variableWhithCalls_t variablesArr[128] = {};
    int numOfVariables                     = 0;
};

enum varFindRsult
{
    FOUND,
    NOTFOUND
};

static int replaceVariables_rec  (analis_node_t *checkingNode, vartable_t *variableTable, callsequence_t *callSeq);
static int check_Equality        (analis_node_t *checking_node);
static int check_ifwhile         (analis_node_t *checking_node);
static int findMatch             (vartable_t *varTable, int targetVarCode, callsequence_t *currCallSeq);
static int existSame             (vartable_t *varTable, callsequence_t CurrPosityon, int varyableCode);
static int make_VriableTable_rec (function_t *currFunction, vartable_t *varTable, callsequence_t callsequence, analis_node_t *startingNode);
static int easy_checkalltree     (function_t *function);    
static int sameWays(callsequence_t *callSeqOne, callsequence_t *callSeqTwo);


int check_all_prog(function_t *functionArr);
int make_allProgramm(function_t *funcsArray, FILE *printingFile);


#endif