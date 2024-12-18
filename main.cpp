#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./headers/tree_funck.h"
#include "./headers/tree_funck.h"
//#include <tree_data_base_funk.h>
#include "./headers/tree_const.h"
//#include <calculator.h>
#include "./headers/compiller_types.h"
#include "./headers/linganal.h"


char funcs[20][20] = {};

codeWord_t       codeWordArr[20] = {};
funktion_names_t funcsArr[20]    = {};
extern analis_variable_t       variableArr;


#define NAME_STR(a)  #a                 


__attribute__((constructor))
void make_codeWordArr()
{
    codeWordArr[0] = {IF_CODE_WORD,     "if"    };
    codeWordArr[1] = {WHILE_CODE_WORD,  "while" };
    codeWordArr[2] = {INT_CODE_WORD,    "int"   };
    codeWordArr[3] = {RETURN,           "return"};
}

function_t *getMain(analis_node_t *nodeArr);


int main()
{
    analis_node_t *nodes =  startLing("int f(int t){ if(x > i){i = x + 6;}; x = 5 + 2; return 0; }");

    int i = 0;

    while(nodes[i].nodeType != END_OF_PROGRAM)
    {
        printf("%d type = %d\n", i, nodes[i].nodeType);
        i++;
    }

    printf("end lganalis__________________________________________\n");
    printf("######################################################\n\n\n");

    function_t *functionArray = getMain(nodes);

    generate_html(&functionArray->commandStart);


    printf("Hello, word!\n");

    return 0;
}