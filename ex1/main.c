#include "vector.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int content[3];
} State;

const int CAPACITY[] = {10, 7, 4};
int FOUND = 0;

Vector VISITED;

int is_finishing_condition(State state) {
    return (state.content[1] == 2 || state.content[2] == 2);
}

int min(int a, int b) { return a < b ? a : b; }

int are_arrays_equal(int array1[], int array2[2], int length) {
    for (int i = 0; i < length; i++) {
        if (array1[i] != array2[i]) {
            return 0;
        }
    }
    return 1;
}

int has_visited(State state) {
    for (int i = 0; i < VISITED.size; i++) {
        State current = *(State *)vector_get(&VISITED, i);
        if (are_arrays_equal(current.content, state.content, 3)) {
            return 1;
        }
    }
    return 0;
}

void find_moves(Vector *moves, State state, int checking) {
    for (int i = 0; i < 3; i++) {
        State current_move = state;
        if (i == checking)
            continue;
        if (current_move.content[i] == CAPACITY[i])
            continue;

        int to_transfer = min(CAPACITY[i] - current_move.content[i],
                              current_move.content[checking]);
        current_move.content[i] += to_transfer;
        current_move.content[checking] -= to_transfer;

        if (has_visited(current_move))
            continue;

        vector_push_back(moves, &current_move);
    }
}

void get_valid_moves(Vector *moves, State state) {
    for (int i = 0; i < 3; i++) {
        if (state.content[i] == 0)
            continue;
        find_moves(moves, state, i);
    }
}

void vector_extend(Vector *destination, Vector *source) {
    for (int i = 0; i < source->size; i++) {
        vector_push_back(destination, vector_get(source, i));
    }
}

void print_root_and_node(State root, State node) {
    printf("_%d_%d_%d_ -> _%d_%d_%d_\n", root.content[0], root.content[1],
           root.content[2], node.content[0], node.content[1], node.content[2]);
}

void explore(State root) {
    Vector possible_moves;
    vector_setup(&possible_moves, 0, sizeof(State));

    get_valid_moves(&possible_moves, root);

    for (int i = 0; i < possible_moves.size; i++) {
        State node = *(State *)vector_get(&possible_moves, i);
        vector_push_back(&VISITED, &node.content);
        print_root_and_node(root, node);

        if (is_finishing_condition(node)) exit(0);
        explore(node);
    }
}

int main() {
    vector_setup(&VISITED, 0, sizeof(State));
    State root = (State){.content = {0, 7, 4}};
    vector_push_back(&VISITED, &root);

    explore(root);

    return 0;
}
