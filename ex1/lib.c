#include "vector.h"

typedef struct {
    int content[3];
} State;

typedef struct {
    State state;
    Vector children;
} Node;
