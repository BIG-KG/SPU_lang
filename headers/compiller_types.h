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

struct argument_t
{
    int argumentCode = -1;
    int argumentFrmt = POISON_FRMT;
};

struct function_t
{
    char functionCode   = NOTAFUNC;
    int  returningType  = POISON_FORMAT;

    argument_t arguments    [MAX_NUM_ARGUMEN] = {};
    int  numOfArguments   = -1;

    int intVariablesCODE    [MAX_NUM_INT_VAR] = {};
    int numberOfIntVar    =  0;

    int doubleVariaclesCODE [MAX_NUM_DBL_VAR] = {};
    int numberOfdoubleVar =  0;

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
    COMMA,
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