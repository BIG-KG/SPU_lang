#ifndef  TREE_DATA_BASE_F
#define  TREE_DATA_BASE_F

#include "tree_types.h"

#ifdef CONSOL_DEBAG
    #define errorCheck (loading_file, errorStream, ERROR_NUM, symb, preendCommand)                      \
    if (fgetc (loading_file) != symb)                                                   \
    {                                                                                   \
        preendCommand;                                                                  \
        printf("ERROR: syntax error no node start: symb  curr word %c", symb);          \
        *errorStream = ERROR_NUM;                                                       \
        return NULL;                                                                    \
    }
#else
    #define errorCheck(loading_file, errorStream, ERROR_NUM, symb, preendCommand)                     \
    if (fgetc (loading_file) != symb )                                                  \
    {                                                                                   \
        preendCommand;                                                                  \
        *errorStream = ERROR_NUM;                                                       \
        return NULL;                                                                    \
    }
#endif



tree_t        *make_tree (FILE *loading_file, int *errorStream);
tree_t        *load_tree (char *loadingFileName, tree_t *currTree, int *errorStream );
int            save_tree (tree_t *savigThee, char * const fileName);
int            save_node  (node_t *startingNode, FILE *savingFile);
node_t        *make_node  (FILE *loading_file, int *errorStream, tree_t *currtree);
node_t        *scan_node  (FILE* loading_file, int *errorStream, tree_t *currtree);



#endif /*TREE_DATA_BASE_F*/
