#ifndef CALC_F
#define CALC_F

int     dif_calc_f   (node_t *currNode, tree_t *currTree);
node_t *dif_div      (node_t *currNode,  tree_t *currTree);
node_t *make_mul_node();
node_t *make_dif_node();
node_t *make_val_node(int nodeType, double val);
node_t *dif_mul      (node_t *currNode,  tree_t *currTree);
node_t *dif_sum_sub  (node_t *currNode,  tree_t *currTree);
node_t *dif_LN       (node_t *currNode,  tree_t *currTree);
node_t *dif_POW      (node_t *currNode,  tree_t *currTree);
node_t *make_const_node();

int ease_tree (node_t *currNode);

#endif /*CALC_F*/