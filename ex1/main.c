#include "vector.h"
#include "lib.h"
#include <stdio.h>

const int CAPACITY[] = { 10, 7, 4 };

int min(int a, int b) {
    return a < b ? a : b;
}

void find_moves(Vector *moves, State state, int checking) {
    for (int i = 0; i < 3; i ++) {
        State current_move = state;
        if (i == checking) continue;
        if (current_move.content[i] == CAPACITY[i]) continue;

        int to_transfer = min(CAPACITY[i] - current_move.content[i], current_move.content[checking]);
        current_move.content[i] += to_transfer;
        current_move.content[checking] -= to_transfer;

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
    get_valid_moves(&possible_moves, root);

    for (int i = 0; i < possible_moves.size; i++) {
        Node node = *(Node *) vector_get(&possible_moves, i);
        printf("%d %d %d\n", node.state.content[0], node.state.content[1], node.state.content[2]);
    }
}

int main() {
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
