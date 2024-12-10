#include "canary.h"

int64_t canary(void){

    static int funckStartedTimes = 0;
    static int64_t canary        = 0;

    if (funckStartedTimes == 0){

        canary = canary | ((int64_t)rand())<<32;
        canary = canary | ((int64_t)rand())    ;

        funckStartedTimes ++;
    }

    return 0;

}

void plant_canary(void *targetPointer, int numOfEl, size_t sizeEl){

    char *endptr = (char *)targetPointer + ((sizeEl * numOfEl / 8)  + 2) * 8;

    *((int64_t *)targetPointer) = 0;
    *((int64_t *)endptr       ) = 0;

}
