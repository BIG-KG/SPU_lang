#include <string.h>
//#include <strings.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#include <tree_types.h>
#include <tree_funck.h>
#include <colors.h>
#include <tree_const.h>
//#include <calculator.h>
#include <compiller_types.h>

#define GLOBAL_ERROR                        \
    printf("%sError hz gde%d%s\n", RED, __LINE__, RESET); \
    return 1;                               \


// int checkEquality(analis_node_t *checking_node)
// {
//     if (checking_node->left->nodeType == EQUAL)
//     {
//         if(checking_node->left->left->nodeType != VARIABLE) {GLOBAL_ERROR}
//     }

//     return 0;
// }

// int chack_ifwhile(analis_node_t *checking_node)
// {
//     if (checking_node->left->nodeType == CODEWORD && 
//          (checking_node->left->nodeData.int_el == IF_CODE_WORD || 
//           checking_node->left->nodeData.int_el == WHILE_CODE_WORD  )    )
//     {
//         if(checking_node->left->left->nodeType != COMPARISON) {GLOBAL_ERROR}
//     }
// }