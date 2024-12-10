#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>

#include "..\headers\tree_const.h"
#include "..\headers\tree_types.h"
#include "..\headers\tree_funck.h"

const int FILE_NAME_SIZE = 64;

char funcs[20][20] = {};

int generate_html(tree_t *tree)
{

    static int  dumpNum = 0                   ;
    char        html_file[FILE_NAME_SIZE] = {};
    strcpy (html_file, "dump.html");

    char    png_name [FILE_NAME_SIZE] = "";
    strcpy (png_name, generate_png (tree, dumpNum));

    FILE *output_file = NULL;

    if (dumpNum == 0) output_file = fopen(html_file, "w");
    else              output_file = fopen(html_file, "a");

    printf("func2\n");
    fprintf(output_file, "<img src=\"C:/MIPT/dbl_linked_list/%s\">", png_name);
    printf("endfunk2\n");

    fclose(output_file);

    dumpNum ++;

    return 0;
}


char *generate_png(tree_t *dumpingTree, int dumpNum)
{
    char        txtFile_name [FILE_NAME_SIZE] = {};
    static char pngFile_name [FILE_NAME_SIZE] = {};

    sprintf (txtFile_name,  "dump_picture%d.txt", dumpNum);
    sprintf (pngFile_name,  "dump_picture%d.png", dumpNum);

    FILE *txtFile = fopen(txtFile_name, "w");

    fprintf    (txtFile, "digraph G {");
    print_node (dumpingTree->treeStart, txtFile);
    fprintf    (txtFile, "}");

    fclose(txtFile);

    char sysCommand[128] = "";
    sprintf (sysCommand, "dot -Tpng %s -o %s", txtFile_name, pngFile_name);
    printf("syst\n");
    system (sysCommand);

    return pngFile_name;
}


void print_node(node_t *curr_node, FILE *txtFile)
{   
    fprintf (txtFile, "\tnode%d [shape=Mrecord, style=\"rounded, filled\"", curr_node);

    if     (curr_node->data.nodeType == CONST)
    {
        fprintf (txtFile, ", label = \"%lf \" ]; \n", curr_node->data.nodeData.cnst);
    }
    else if(curr_node->data.nodeType == VARIABLE)
    {
        fprintf (txtFile, ", label = \"%c\" ]; \n", curr_node->data.nodeData.var);
    }
    else if(curr_node->data.nodeType == FUNC)
    {
        fprintf (txtFile, ", label = \"%s\" ]; \n", funcs[curr_node->data.nodeData.func]);
    }
    else
    {
        fprintf (txtFile, ", label = \"%d\" ]; \n", curr_node->data.nodeType);
    }

    if(curr_node->left != NULL)
    {   
        fprintf(txtFile, "node%d -> node%d;\n", curr_node, curr_node->left );
        print_node(curr_node->left, txtFile);

    }

    if(curr_node->right != NULL)
    {   
        
        //printf("right\n");
        fprintf(txtFile, "node%d -> node%d;\n", curr_node, curr_node->right);
        print_node(curr_node->right, txtFile);
    }

    return;
}