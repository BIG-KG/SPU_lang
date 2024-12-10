#ifndef CANARY_H_INCLUDED
#define CANARY_H_INCLUDED

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int64_t canary(void);

void plant_canary(void *targetPointer, int numOfEl, size_t sizeEl);

#endif // CANARY_H_INCLUDED
