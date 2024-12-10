#ifndef COMPORATOR_F
#define COMPORATOR_F

#include "tree_types.h"

node_t *getStr();
node_t *getMain();
node_t *getSumSub();
node_t *getMulDiv();
node_t *getBrackets();
node_t *getDouble();
node_t *getPow();
int     findfunc (char * const funcName);
char    findVar  (char * const valName);

#endif /*COMPORATOR_F*/