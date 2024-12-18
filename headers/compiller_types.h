#ifndef COMPILLER_TYPES
#define COMPILLER_TYPES

#include <cstddef>

#include "tree_types.h"

const int NANODE = -1;

const int MAX_VARIABLE_NAME_SIZE = 128;

const int MAX_NUM_INT_VAR = 128;
const int MAX_NUM_DBL_VAR = 128;
const int MAX_NUM_ARGUMEN = 128;
const int MAX_NUM_CMD_FNK = 512;

struct variable_t
{
    int variableCode = -1;
    int variableFrmt = POISON_FRMT;
};

struct analis_variable_t
{
    int  initialized                          = 0;
    int  variableFrmt                         = POISON_FRMT;
    char variableName[MAX_VARIABLE_NAME_SIZE] = "";
};

struct function_t
{
    char functionCode   = NOTAFUNC;
    int  returningType  = POISON_FRMT;

    variable_t arguments [MAX_NUM_ARGUMEN] = {};
    int        numOfArguments              = -1;

    variable_t VariablesCODE [MAX_NUM_INT_VAR] = {};
    int        numberOfVar                     =  0;

    analis_node_t commandStart  = {};
    int           numOfCommands = 0;
};

enum element_types
{   
    END_OF_PROGRAM,
    CONST,
    VARIABLE,
    FUNC,
    OPER, 
    EQUAL,
    COMPARISON,
    CODEWORD,
    END_COMMAND,
    ERROR_EL,
    WORD_EL,
    FRMT_CHANGE,
    COMMA,
    BRAKES,
    SYNTAX_ERROR,
    EMPTY_NODE
};

enum codeWords
{
    IF_CODE_WORD,
    ENDIF_CODE_WORD,
    ENDWILE_CODE_WORD,
    WHILE_CODE_WORD,
    INT_CODE_WORD,
    RETURN
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


enum comparison
{
    IS_EQUAL,
    IS_NOT_EQUAL,
    GREATER_OR_EQU,
    LESS_OR_EQU,
    LESS,
    GEATER
};


struct processing_programm_t
{
    analis_node_t *nodeArr       = NULL;
    int           *currNode      = NULL;
    function_t    *functionArr   = NULL;
    int           *numOfFunction = NULL;
};


#endif