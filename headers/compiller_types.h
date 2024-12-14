#include <cstddef>
#ifndef COMPILLER_TYPES
#define COMPILLER_TYPES

#include "tree_types.h"

const int NANODE = -1;
const int NOTAFUNC    = -1;

const int MAX_NUM_INT_VAR = 128;
const int MAX_NUM_DBL_VAR = 128;
const int MAX_NUM_ARGUMEN = 128;
const int MAX_NUM_CMD_FNK = 512;

struct function_t
{
    char functionCode   = NOTAFUNC;
    int  numOfArguments = -1;
    int  returningType  = POISON_FORMAT;

    int argumentsCODE       [MAX_NUM_ARGUMEN] = {};

    int intVariablesCODE    [MAX_NUM_INT_VAR] = {};
    int doubleVariaclesCODE [MAX_NUM_DBL_VAR] = {};

    analis_node_t *commands = NULL;
};

enum element_types
{
    CONST,
    VARIABLE,
    FUNC,
    OPER, 
    EQUAL,
    CODEWORD,
    END_COMMAND,
    ERROR_EL,
    WORD_EL,
    BRAKES,
    END_OF_PROGRAM,
    SYNTAX_ERROR
};

enum codeWords
{
    IF_CODE_WORD,
    WHILE_CODE_WORD,
    INT_CODE_WORD,
    DOUBLE_CODE_WORD
};

enum brackets
{
    OPEN_CIR,
    CLSE_CIR,
    OPEN_SQR,
    CLSE_SQR,
    OPEN_FIG,
    CLSE_FIG,
};

struct processing_programm_t
{
    analis_node_t *nodeArr       = NULL;
    int           *currNode      = NULL;
    function_t    *functionArr   = NULL;
    int           *numOfFunction = NULL;
};


#endif