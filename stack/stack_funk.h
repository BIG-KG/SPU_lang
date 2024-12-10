#ifndef STACK_FUNK_H_INCLUDED
#define STACK_FUNK_H_INCLUDED

#define IF_ERR_GO_OUT(a){                                           \
        Stack_error_global = 0;                                     \
        stack_ok(a);                                                \
        if(Stack_error_global != 0){                                \
            ("ERROR, error code= %d\n", Stack_error_global);       \
            return Stack_error_global;                              \
        }                                                           \
}


#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


static int hasher(int64_t name, int changeHash);

int look(int64_t name, int ElNum);

int push(int64_t name, int64_t pushingEl);

int pop(int64_t name);

int64_t stack_ctor(void);

int stack_size_chk (int64_t name, int addingSize);

void* stack_dump(int64_t name, int checkNeed);

int stack_ok(int64_t name);

int stack_dtor(int64_t name);

#endif // STACK_FUNK_H_INCLUDED
