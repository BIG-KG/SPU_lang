#ifndef STACK_TYPES_H_INCLUDED
#define STACK_TYPES_H_INCLUDED

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>




typedef int stackEl;

const int START_STACK_SIZE = 10;
const int SIZE_STEP_UP     = 2;
const int SIZE_STEP_DOWN   = SIZE_STEP_UP * SIZE_STEP_UP;
const int ERROR_RETURN     = -2147483648; //INT_MIN;


struct stack_t{

    int inittialisated = 0;
    int64_t strCanry   = 0;
    void *dataPtr      = NULL;
    int currSize       = 0;
    int maxSize        = 0;
    int Hash           = 0;
    int64_t endCamry   = 0;// NE BAG A FICHA

};

struct allStacks_t{

    stack_t *stacksArray = (stack_t *)calloc (START_STACK_SIZE, sizeof(stack_t));;
    int lastNumOFStack      = 0;
    int maxNumOFStack       = 30;
    int holes[30]           = {};
    int numOFholse          = 0;

};

enum hashModes{
        CHECK_HASH = 0,
        CHANGE_HASH = 1
};



#endif // STACK_TYPES_H_INCLUDED
