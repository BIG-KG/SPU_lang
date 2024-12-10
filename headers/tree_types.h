#ifndef TREE_TYPES
#define TREE_TYPES

#include <stdint.h>
#include "tree_const.h"

enum dataTypes
{
    FUNC = 2,
    CONST = 3,
    VARIABLE = 4
};

enum funcs
{
    SIN,
    COS,
    LOGN,
    DIF,
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

#endif /*TREE_TYPES*/
