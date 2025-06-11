#include <sys/types.h>
#include "vector.h"

#ifndef lib
#define lib

typedef struct {
    int content[3];
} State;

typedef struct {
    State state;
    Vector nexts;
} Node;

#endif
