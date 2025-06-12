#include "vector.h"
#include "stdio.h"
#include <stdio.h>

typedef char Word[7];


void trim(Word word) {
    word[5] = '\0';
}

void read_dict(Vector *words) {
    FILE *file = fopen("ex2/sgb-words.txt", "r");

    Word line;
    while (fgets(line, 7, file)) {
        trim(line);
        vector_push_back(words, line);
    }
}

void vector_print(Vector *vector) {
    for (int i = 0; i < vector->size; i++) {
        printf("%s\n", *(Word *) vector_get(vector, i));
    }
}

int main() {
    Vector words;
    vector_setup(&words, 1000, sizeof(Word));
    read_dict(&words);
    vector_print(&words);
    return 0;
}
