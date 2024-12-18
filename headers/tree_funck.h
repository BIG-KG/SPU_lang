#ifndef TREE_FUNK
#define TREE_FUNK

#include "tree_const.h"
#include "tree_types.h"

node_t *make_element  (void);

int     generate_html (analis_node_t *tree);
int     printing_dump (analis_node_t *node);
char   *generate_png  (analis_node_t *dumpingTree, int dumpNum);
void    print_node    (analis_node_t *curr_node, FILE *txt_file);
int     printTex      (analis_node_t *currTree, FILE *outputFile);
int     makeTex       (FILE *outputFile);



int     delete_tree   (analis_node_t *deleatingNode);
node_t *copyNode      (analis_node_t *copingNode, tree_t *currTree);

#endif /*TREE_FUNK*/
