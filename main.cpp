#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>


#include "./headers/tree_funck.h"
#include "./headers/tree_funck.h"
//#include <tree_data_base_funk.h>
#include "./headers/tree_const.h"
#include "./headers/check_and_var.h"
//#include <calculator.h>
#include "./headers/compiller_types.h"
#include "./headers/linganal.h"


char funcs[20][20] = {};

codeWord_t               codeWordArr[20] = {};
funktion_names_t         funcsArr[20]    = {};
extern analis_variable_t variableArr;


#define NAME_STR(a)  #a                 


__attribute__((constructor))
void make_codeWordArr()
{
    codeWordArr[0] = {IF_CODE_WORD,     "if"    };
    codeWordArr[1] = {WHILE_CODE_WORD,  "while" };
    codeWordArr[2] = {INT_CODE_WORD,    "int"   };
    codeWordArr[3] = {RETURN,           "return"};
    codeWordArr[4] = {PRINT,            "print"};
    codeWordArr[5] = {INPUT,            "input"};

    funcsArr[0].funktionCode = 0;
    strcpy(funcsArr[0].funktionName, "main");
}


int main()
{
    // analis_node_t *nodes =  startLing("int factor(int i){ if (i ?= 1){return 1;}; return (factor(i - 1)  * i);}" 
    //                                   "int main(){ int ans = factor(3); print ans;}");

    FILE* file = fopen("factorRec.txt", "r");

    int inputFileSize = file_size(file);

    char *bufferString = (char *)calloc(inputFileSize + 2, sizeof(char));
    fread(bufferString, sizeof(char), inputFileSize, file);

    printf("_%s_`", bufferString);
    
    analis_node_t *nodes =  startLing(bufferString);

    int i = 0;

    while(nodes[i].nodeType != END_OF_PROGRAM)
    {
        printf("%d type = %d   INFO = %d\n", i, nodes[i].nodeType, nodes[i].nodeData.int_el);
        i++;
    }


    function_t *functionArray = getMain(nodes);

    generate_html(&functionArray->commandStart);
    generate_html(&functionArray[1].commandStart);
      
    check_all_prog(functionArray);

    generate_html(&functionArray->commandStart);
    generate_html(&functionArray[1].commandStart);

    printf("Hello, word!\n");

    FILE *openFile = fopen("test.txt", "w");
    make_allProgramm(functionArray, openFile);


    return 0;
}

int file_size(FILE* targetFile){

    fseek(targetFile, 0, SEEK_END);
    int file_size = ftell(targetFile);
	fseek(targetFile, 0, 0       );

	return file_size;

}