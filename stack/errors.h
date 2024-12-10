#ifndef ERRORS_H_INCLUDED
#define ERRORS_H_INCLUDED

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


// something does not feel right
#define IF_RIGHT_STACK if(Stack_error_global == 0){

             /*if(docheck != ' '){
                stack_ok(doChaeck);
            }*/

#define PTR_TEST(doChaeck)\


#define CHECK(doChaeck) ;}                                                  \
    if(Stack_error_global != 0 && Stack_error_global != CHECKED_ERROR){     \
        printf("Error on line %d", __LINE__ );                              \
        if (Stack_error_global != 0){                                       \
            stack_dump(*doChaeck, 1);                                       \                                                                            \
        }                                                                   \
                                                                            \
        stack_dtor(doChaeck);                                               \
    }                                                                       \
                                                                                                                                               \

          /*����� �� ������?(stack_dtor(doChaeck))*/

enum error_codes{

    BROKEN_CANNARY   = 5,
    NULL_STACK_PTR   = 4,
    NULL_DATA_PTR    = 3,
    STACK_OVERFLOW   = 2,
    STACK_UNDERFLOW  = 1,
    CHECKED_ERROR    = -1,
    EXTAL_STACK_INTR = 6,
    WRONG_MY_HASH    = 1000 - 7 - 7,
    OUT_MY_OF_MEMORY = 10,
    BROKEN_INNER_STK = 1000 - 7,
    REFER_NOEXST_STK = 1000 -7 -7 -7,
    NOT_INITLISD_STK = 1000 - 28,

};

#endif // ERRORS_H_INCLUDED
