    #ifndef TREE_TYPES
#define TREE_TYPES

#include <stdint.h>
#include "tree_const.h"

const int MAX_OPERATION_NAME_SIZE = 128;
const int MAX_FUNKTION_NAME_SIZE  = 128;
const int NAO = -1;
const int NOTAFUNC = -1;

enum funcs
{
    SIN,
    COS,
    LOGN,
    DF,
    SUM,
    SUB,
    DIV,
    MUL,
    POW,
    LN,
    ASIN,
    ACOS,
    TNG,
    CTG,
    ATNG,
    ACTG,
};

union calc_node_data
{
    double cnst = 0;
    int    var ;
    int    func;
};


struct calc_node
{
    int            nodeType = -1; //no data type has code -1
    calc_node_data nodeData = {};
};

union analis_node_data
{
    double double_el = 0;
    int    int_el;
};


enum dataFormats
{
    POISON_FRMT = -1,
    VOID_FRMT,
    INT_FRMT,
    DOUBLE_FRMT
};

struct lingAnalis_t
{
    int              nodeType   = -1; //no data type has code -1
    analis_node_data nodeData   = {};
    int              nodeFormat = POISON_FRMT;
};

struct analis_node_t
{
    int               nodeType   = -1; //no data type has code -1
    analis_node_data  nodeData   = {};
    int               nodeFormat = POISON_FRMT;
    analis_node_t    *right = NULL; 
    analis_node_t    *left  = NULL;

};

typedef calc_node elem_t;

struct node_t
{
    elem_t data   = {};
    node_t *right = NULL; 
    node_t *left  = NULL;

};

struct tree_t
{
    //void   *occupiedMemStart = NULL; // remove
    node_t *treeStart       = NULL;
    //int     capacity         = 0;
    //int     currentElement   = 0;

};

struct node_way_t
{
    int     depth       = 0;
    int    *way         = NULL;
    node_t *node_addres = NULL;
};

struct codeWord_t
{
    int  codeWordCode = NAO;
    char *wordName     = NULL;
};

struct funktion_names_t
{
    int  funktionCode = NOTAFUNC;
    char funktionName[MAX_FUNKTION_NAME_SIZE] = "";
};

#endif /*TREE_TYPES*/
