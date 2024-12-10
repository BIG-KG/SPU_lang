#include "stack_types.h"
#include "stack_funk.h"
#include "errors.h"
#include "const.h"
#include "canary.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

static allStacks_t allStack;
static int expend_all_stacks(int addingNum);
static int dexpend_all_stacks(int decreacing);

int stack_ok (int64_t name)
{

    if (name < 0 || name >= allStack.lastNumOFStack)
    {
        printf("case0\n");
        Stack_error_global = REFER_NOEXST_STK;
        return REFER_NOEXST_STK;
    }

    stack_t * ptrTargetStack = &((allStack.stacksArray)[name]);

    if (!ptrTargetStack->inittialisated){
        printf("case1\n");
        Stack_error_global = NOT_INITLISD_STK;
        return NOT_INITLISD_STK;
    }

    if (!(ptrTargetStack->dataPtr))
    {
        printf ("case2\n");
        Stack_error_global = NULL_DATA_PTR;
        return NULL_DATA_PTR;
    }


    int64_t endcannary = *(    (int64_t *)(  (char *)ptrTargetStack->dataPtr + (((sizeof(stackEl) * ptrTargetStack->maxSize / 8)  + 1) * 8)  )   );

    if(   *((int64_t *)( ptrTargetStack->dataPtr) - 1) != canary()   ||   endcannary != canary()   )
    {
        printf ("case3\n");
        printf ("real canry = %ld, given canry = %ld, lastcanr = %ld\n", canary(), *((int64_t *)(ptrTargetStack->dataPtr) - 1),   endcannary   );
        Stack_error_global = BROKEN_CANNARY;
        return BROKEN_CANNARY;
    }


    if (  (ptrTargetStack->currSize)  >  (ptrTargetStack->maxSize)  )
    {
        printf ("case4\n");
        Stack_error_global = STACK_OVERFLOW;
        return STACK_OVERFLOW;
    }


    if ( (ptrTargetStack->currSize) < 0 )
    {
        printf ("case5\n");
        Stack_error_global = STACK_UNDERFLOW;
        return STACK_UNDERFLOW;
    }


    int tmpErorror = hasher (name, CHECK_HASH);

    if(tmpErorror != 0)
    {
        printf("case6\n");
        Stack_error_global = tmpErorror;
        return tmpErorror;
    }

    return 0;

}

int stack_dump(int64_t name)
{
    stack_t * ptrTargetStack = &((allStack.stacksArray)[name]);

    if (Stack_error_global == NULL_STACK_PTR || ptrTargetStack == NULL)
    {
        printf("ERROR pointer tustruct is NULL.\n returning lastEl");
        return NULL_STACK_PTR;
    }

    printf("currSize = %d, maxsize = %d\n ", ptrTargetStack->currSize, ptrTargetStack->maxSize);

    if (Stack_error_global == NULL_DATA_PTR || ptrTargetStack->dataPtr == NULL)
    {
        printf("ERROR pointer tustruct is NULL");
        return NULL_DATA_PTR;
    }


    for (int i = 0; i < ptrTargetStack->currSize; i++)
    {
        printf ("\n%d", look(name, i));
    }
    printf ("\n");

    return 0;
}

int stack_size_chk (int64_t name, int addingSize)
{

   // IF_ERR_GO_OUT(name);

    stack_t * ptrTargetStack = &((allStack.stacksArray)[name]);
    int sizeAfterAdd         = ptrTargetStack->currSize + addingSize;

    if (  (sizeAfterAdd)  >=  (ptrTargetStack->maxSize)  )
    {

        int newSize      = (ptrTargetStack->maxSize) * SIZE_STEP_UP;
        void *tmpDataPtr = realloc (  ((char *)ptrTargetStack->dataPtr - 8) , ((sizeof(stackEl) * newSize / 8)  + 3) * 8  );

        if (tmpDataPtr == NULL)
        {
            printf("ERROR cant expend stack.");
            return OUT_MY_OF_MEMORY;
        }

        plant_canary(tmpDataPtr, newSize, sizeof(stackEl));
        ptrTargetStack->dataPtr  = (char *)tmpDataPtr + 8;
        ptrTargetStack->maxSize *= SIZE_STEP_UP;

        return 0;

    }


    else if (  ((sizeAfterAdd)  <  (ptrTargetStack->maxSize/SIZE_STEP_DOWN))  &&
              ((ptrTargetStack->maxSize) > START_STACK_SIZE)  )
    {
        ptrTargetStack->maxSize /= SIZE_STEP_DOWN;

        void *tmpDataPtr = realloc (  ((char *)ptrTargetStack->dataPtr - 8) , ((sizeof(stackEl) * ptrTargetStack->maxSize / 8)  + 3) * 8  );

        if(tmpDataPtr == NULL)
        {
            printf("ERROR cant narrow down stack.");
            return OUT_MY_OF_MEMORY;
        }

        plant_canary(tmpDataPtr, ptrTargetStack->maxSize, sizeof(stackEl));
        ptrTargetStack->dataPtr = (char *)tmpDataPtr + 8;

        return 0;

    }


   // IF_ERR_GO_OUT(name);

    return 0;
}

int64_t stack_ctor (void)
{

    stack_t *ptrTargetStack = NULL;
    int64_t name = 0;
    if (expend_all_stacks(1) != 0)
    {
        return -1;
    }

    if(allStack.numOFholse == 0)
    {
        //printf("lastnum = %d", allStack.lastNumOFStack);
        ptrTargetStack = &allStack.stacksArray[allStack.lastNumOFStack];
        name = allStack.lastNumOFStack;
        allStack.lastNumOFStack ++;
        //printf("@1\n");
    }

    else
    {
        name = allStack.holes[allStack.numOFholse];
        ptrTargetStack = &allStack.stacksArray[name];
        allStack.numOFholse --;
        //printf("@2\n");
    }


    if(  ((allStack.stacksArray)[name]).inittialisated != 0  )
    {
        printf("inner stak system ckraked\n");
    }

    ((allStack.stacksArray)[name]).inittialisated = 1;


    ptrTargetStack->strCanry = canary(); /* do not make your vars functions*/
    ptrTargetStack->endCamry = canary();

    int realSize        = (  (sizeof(stackEl) * START_STACK_SIZE / 8)  + 3) * 8;
    void *tmpPointer    = (void *)calloc (realSize, 1);

    if (!tmpPointer)
    {
    #ifndef NOT_RETURN_ERRORS
        printf ("Error out of memory");
    #endif
        Stack_error_global = OUT_MY_OF_MEMORY;
        return OUT_MY_OF_MEMORY;
    }

    plant_canary (tmpPointer, START_STACK_SIZE, sizeof(stackEl));
    ptrTargetStack->dataPtr  = (char *)tmpPointer + 8;
    ptrTargetStack->currSize = 0;
    ptrTargetStack->maxSize  = START_STACK_SIZE;

   // hasher (name, CHANGE_HASH);
    //IF_ERR_GO_OUT (name);

    return name;
}

int pop (int64_t name)
{

   // IF_ERR_GO_OUT(name);
    stack_t * ptrTargetStack = &((allStack.stacksArray)[name]);

    if (ptrTargetStack->currSize < 1)
    {
        printf ("Error stack is empty");
        Stack_error_global = STACK_UNDERFLOW;
        return STACK_UNDERFLOW;
    }

    int returningEl = *((int*)(ptrTargetStack->dataPtr) + (ptrTargetStack->currSize-1));

    Stack_error_global = stack_size_chk (name, -1);
    if (Stack_error_global != 0)
    {
        printf ("sorry, can't pop element.");
        return OUT_MY_OF_MEMORY;
    }

    ptrTargetStack->currSize--;

   // hasher (name, CHANGE_HASH);
   // IF_ERR_GO_OUT(name);

    return returningEl;
}

int push(int64_t name, int64_t pushingEl)
{

   // IF_ERR_GO_OUT(name);

    stack_t * ptrTargetStack = &((allStack.stacksArray)[name]);

    Stack_error_global = stack_size_chk(name, 1);
    if (Stack_error_global != 0)
    {
        printf ("sorry, can't pop element.");
        return OUT_MY_OF_MEMORY;
    }

    ptrTargetStack->currSize++;
    *((int*)(ptrTargetStack->dataPtr) + (ptrTargetStack->currSize - 1)) = pushingEl;

   // hasher (name, CHANGE_HASH);
   // IF_ERR_GO_OUT(name);

    return 0; //stack_ok(name);

}

int look(int64_t name, int ElNum){   // add stack to all names

   // IF_ERR_GO_OUT(name);

    stack_t * ptrTtargetStack = &((allStack.stacksArray)[name]);

   // IF_ERR_GO_OUT(name);

    return *((int*)(ptrTtargetStack->dataPtr) + (ElNum));

}

int stack_dtor(int64_t name)
{
    (allStack.stacksArray)[name].inittialisated = 0;

    if (name != allStack.lastNumOFStack - 1)
    {
        //printf("000\n");
        (allStack.holes)[allStack.numOFholse] = name;
        allStack.numOFholse ++;
        //printf("001\n");
    }

    else
    {
        //printf("111\n");
        int i = 0;
        while(i < name)
        {
            //printf("222\n");
            if (  (allStack.stacksArray)[name - i].inittialisated == 1  )
            {
                break;
            }
            i++;
        }

        allStack.lastNumOFStack = name - i + 1;

        for(i = 0; i < allStack.numOFholse; i++)
        {
            if (  (allStack.holes)[i] > allStack.lastNumOFStack  )
            {
                if (i < 30 ) memmove(  &(allStack.holes)[i], &(allStack.holes)[i + 1], 30 - i - 1);
                allStack.numOFholse --;
            }

        }
    }
    //printf("002\n");
    //printf("currsize = %d",     ((allStack.stacksArray)[name]).currSize );
    ((allStack.stacksArray)[name]).currSize   = 0;
    //printf("maxsize = %d",     ((allStack.stacksArray)[name]).maxSize );
    ((allStack.stacksArray)[name]).maxSize    = 0;
    ((allStack.stacksArray)[name]).endCamry   = 0;
    ((allStack.stacksArray)[name]).strCanry = 0;
    //printf("003\n");
    free (  (char*)((allStack.stacksArray)[name]).dataPtr - 8  );
    //printf("004\n");
    if( dexpend_all_stacks(1) != 0)
    {
        return -1;
    }
    //printf("005\n");
    return 0;
}

static int hasher(int64_t name, int changeHash)
{
    // perepisat
    int *targetData       = (int *)((allStack.stacksArray)[name]).dataPtr ;
    int num_ofElements        = ((allStack.stacksArray)[name]).currSize;
    int32_t tmpHash           = 0;

    for (int i = 0; i < num_ofElements; i ++)
    {
        tmpHash += targetData[i];
        tmpHash += (tmpHash << 10);
        tmpHash ^= (tmpHash >> 6 );
    }
    tmpHash += (tmpHash << 3);
    tmpHash ^= (tmpHash >> 11);
    tmpHash += (tmpHash << 15);

    if (changeHash == CHANGE_HASH) ((allStack.stacksArray)[name]).Hash = tmpHash;

    else
    {
        if(((allStack.stacksArray)[name]).Hash != tmpHash)
        {
            printf ("\n\n\n\n\nStack broked :'( \n");
            Stack_error_global = WRONG_MY_HASH;
            return WRONG_MY_HASH;
        }

    }

    return(0);
}

static int expend_all_stacks(int addingNum)
{
    if(allStack.lastNumOFStack + addingNum >= allStack.maxNumOFStack)
    {
        stack_t *tmp = (stack_t *)realloc (allStack.stacksArray, allStack.maxNumOFStack * SIZE_STEP_UP * sizeof(stack_t));
        if(tmp == NULL)
        {
            printf("Error , no memory to create new stack\n");
            return 1;
        }
        allStack.stacksArray = tmp;
    }

    return 0;
}

static int dexpend_all_stacks(int decreacing)
{
    if( (allStack.lastNumOFStack - decreacing) * SIZE_STEP_DOWN < allStack.maxNumOFStack  &&
                allStack.maxNumOFStack / SIZE_STEP_DOWN > START_STACK_SIZE )
    {

        stack_t *tmp = (stack_t *)realloc (allStack.stacksArray, allStack.maxNumOFStack / SIZE_STEP_DOWN * sizeof(stack_t));
        if(tmp == NULL){
            printf("Error , can't delete stack\n");
            return 1;
        }
        allStack.stacksArray = tmp;
    }

    return 0;
}
