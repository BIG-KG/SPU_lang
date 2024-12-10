#include <stdio.h>
#include <stdlib.h>

#include "..\\headers\tree_types.h"
#include "..\\headers\tree_const.h"
#include "..\\headers\tree_funck.h"


node_t *make_element(void)
{
    node_t *tmpPntr_ = (node_t *)calloc(1, sizeof(node_t));

    if (tmpPntr_ == NULL)
    {
        #ifdef CONSOLE_DEBUG
            printf("No RAM space to add new tree element\n");
        #endif

        return NULL;
    }

    return tmpPntr_;
}



node_t *copyNode(node_t *copingNode, tree_t *currTree)
{
    if (copingNode == NULL) return NULL;

    node_t *newNode = make_element ();
    newNode->data = copingNode->data;

    if (copingNode->left  != NULL) 
        newNode->left  = copyNode (copingNode->left,  currTree);
    if (copingNode->right != NULL) 
        newNode->right = copyNode (copingNode->right, currTree);

    return newNode;
}

int delete_tree(node_t *deleatingNode)
{
    if (deleatingNode == NULL) return 0;

    if(deleatingNode->left != NULL)
    {
        delete_tree(deleatingNode->left);
    }

    if(deleatingNode->right != NULL)
    {
        delete_tree(deleatingNode->right);
    }

    free(deleatingNode);

    return 0;
}

