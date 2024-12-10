#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "..\headers\tree_types.h"
#include "..\headers\tree_funck.h"
#include "..\headers\tree_data_base_funk.h"
#include "..\headers\colors.h"
#include "..\headers\errors.h"

//#define  CONSOL_DEBUG

enum {
    NO_NODE_START = 1,
    NO_NODE_END,
    NO_NODE_DATA,
    STRING_END,
    UND_BEH
};

extern char funcs[][20];

node_t *make_node (FILE *loading_file, int *errorStream, tree_t *currTree)
{
    
    node_t *newNode         = make_element ();
    char    startElementget = 0;

    if (newNode == NULL)
    {
        *errorStream = NO_RAM_MEM;
        return NULL;
    }

    fscanf (loading_file, "%*[ ]");
    errorCheck (loading_file, errorStream, NO_NODE_START, '{' , free(newNode));


    fscanf (loading_file, "%*[^']");
    errorCheck (loading_file, errorStream, NO_NODE_DATA,  '\'', free(newNode));
    fscanf (loading_file, "%[^']", newNode->data);
    //printf("%s\n", newNode->data);
    
    errorCheck (loading_file, errorStream, NO_NODE_DATA,  '\'', free(newNode));
    

    newNode->left  = scan_node(loading_file, errorStream, currTree);
    if (*errorStream)
    {
        free(newNode);
        return NULL;
    }

    newNode->right = scan_node(loading_file, errorStream, currTree);
    if (*errorStream)
    {
        free(newNode);
        return NULL;
    }


    fscanf(loading_file, "%*[ ]");
    errorCheck (loading_file, errorStream, NO_NODE_START, '}', free(newNode));


    return newNode;
}


tree_t *load_tree (char * const loadingFileName, tree_t *currTree, int *errorStream )
{
    FILE *loadingFile = fopen(loadingFileName, "r");

    #ifdef   CONSOLE_DEBUG_MY
        printf("Start_making_tree\n");
    #endif /*CONSOLE_DEBUG_MY*/

    int innrErrStrng = 0;
    node_t *treeStartNode = make_node (loadingFile, &innrErrStrng, currTree); // scan
    currTree->treeStart = treeStartNode;
    if (errorStream)   *errorStream = innrErrStrng;

    if (innrErrStrng)  return NULL                ;

    #ifdef   CONSOLE_DEBUG_MY
        printf("End_making_tree\n");
    #endif /*CONSOLE_DEBUG_MY*/

    return  currTree;
}


int save_tree (tree_t *savingTree, char * const fileName)
{
    FILE *savingFile = fopen(fileName, "w");

    if ( savingTree == NULL || savingFile == NULL )
    {
        #ifdef CONSOLE_DEBUG
            if ( savingFile == NULL ) printf("%sERROR: error in filename%s\n", RED, RESET);
            if ( savingTree == NULL ) printf("%sERROR: trying to save NULL"
                                              " pointer - tree%s\n", RED, RESET);
        #endif
        
        return NULL_PTR_VALUE;
    } 

    save_node(savingTree->treeStart, savingFile);

    fclose(savingFile);

    return 0;
}

int save_node (node_t *startingNode, FILE *savingFile)
{
    /*Are recurring checks necessary?*/
    if ( startingNode == NULL || savingFile == NULL )
    {
        #ifdef CONSOLE_DEBUG
            if ( savingFile == NULL ) printf("%sERROR: error in filename%s\n", RED, RESET);
            if ( savingTree == NULL ) printf("%sERROR: trying to save NULL"
                                              " pointer - tree%s\n", RED, RESET);
        #endif
        
        return NULL_PTR_VALUE;
    } 

    int error = 0;

    error = fprintf(savingFile, "{ ");
    error = fprintf(savingFile, " \'%s\' ", startingNode->data) - 1;
    if (error != 0) return error;


    if (startingNode->left ) error = save_node (startingNode->left,  savingFile);
    else                     error = fprintf (savingFile, " * ");

    if (error != 0) return error;


    if (startingNode->right) error = save_node (startingNode->right, savingFile);
    else                     error = fprintf (savingFile, " * ");

    if (error != 0)   return error;
    

    if ( fprintf(savingFile, "}") != 0 ) return FPRINTF_ERR;

    return 0;
}


node_t *scan_node (FILE* loading_file, int *errorStream, tree_t *currTree)
{          
    fscanf (loading_file, "%*[ ]");                             
                                                                
    char startElementget = fgetc(loading_file);                 
    if      (startElementget == '*')                            
    {                                                           
        return NULL;                                          
    }                                                           
    else if (startElementget == '{')                            
    {                                                           
        ungetc (startElementget, loading_file);                 
        return make_node (loading_file, errorStream, currTree);}       
    else                                                        
    {                                                           
        ungetc (startElementget, loading_file)                    ; 
        errorCheck (loading_file, errorStream, NO_NODE_START, '{',      ); 
        return NULL;
    }                                                           

    *errorStream = UND_BEH;                                                           
    return NULL;                         
}