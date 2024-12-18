#ifndef TREE_FUNK
#define TREE_FUNK

#include "tree_const.h"
#include "tree_types.h"

node_t *make_element  (void);

int     generate_html (tree_t *tree);
int     printing_dump (node_t *node);
char   *generate_png  (tree_t *dumpingTree, int dumpNum);
void    print_node    (node_t *curr_node, FILE *txt_file);
int     printTex      (tree_t *currTree, FILE *outputFile);
int     makeTex       (FILE *outputFile);



int     delete_tree   (node_t *deleatingNode);
node_t *copyNode      (node_t *copingNode, tree_t *currTree);

#endif /*TREE_FUNK*/
