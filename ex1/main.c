#include "vector.h"
#include "lib.h"
#include <stdio.h>

const int CAPACITY[] = { 10, 7, 4 };

Vector VISITED;

int min(int a, int b) {
    return a < b ? a : b;
}

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
        State current = *(State *) vector_get(&VISITED, i);
        if (are_arrays_equal(current.content, state.content, 3)) {
            return 1;
        }
    }
    return 0;
}

void find_moves(Vector *moves, State state, int checking) {
    for (int i = 0; i < 3; i ++) {
        State current_move = state;
        if (i == checking) continue;
        if (current_move.content[i] == CAPACITY[i]) continue;

        int to_transfer = min(CAPACITY[i] - current_move.content[i], current_move.content[checking]);
        current_move.content[i] += to_transfer;
        current_move.content[checking] -= to_transfer;

        if (has_visited(current_move)) continue;

        Vector children;
        vector_setup(&children, 0, sizeof(Node));
        Node move = (Node) {
            .state = current_move,
            .children = children,
        };

        vector_push_back(moves, &move);
    }
}

void get_valid_moves(Vector *moves, Node node) {
    for (int i = 0; i < 3; i++) {
        if (node.state.content[i] == 0) continue;
        find_moves(moves, node.state, i);
    }
}

void explore(Node root) {
    Vector possible_moves;
    vector_setup(&possible_moves, 0, sizeof(Node));

    for (int i = 0; i < possible_moves.size; i++) {
        Node node = *(Node *) vector_get(&possible_moves, i);
        printf("%d %d %d\n", node.state.content[0], node.state.content[1], node.state.content[2]);

        explore(node);
    }
}

int main() {
    vector_setup(&VISITED, 0, sizeof(State));
    State initial_state = (State) {
        .content = { 0, 7, 4 }
    };

    Vector children;
    vector_setup(&children, 0, sizeof(Node));
    Node root = (Node) {
        .state = initial_state,
        .children = children,
    };
    explore(root);
    return 0;
}
