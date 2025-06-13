#include "vector.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int content[3];
} State;

const int CAPACITY[] = {10, 7, 4};
int FOUND = 0;

Vector VISITED;

/*
 * Kiểm tra xem đã đạt điều kiện kết thúc chưa
 * Tham số:
 *  State state: trạng thái của chương trình
 * Trả về:
 *  0 nếu chưa đạt trạng thái kết thúc
 *  1 nếu đã đạt trạng thái kết thúc
 */
int is_finishing_condition(State state) {
    return (state.content[1] == 2 || state.content[2] == 2);
}

/*
 * Tìm số nhỏ nhất trong 2 số
 * Tham số:
 *  int a: số nguyên thứ nhất
 *  int b: số nguyên thứ hai
 * Trả về:
 *  int: số nguyên nhỏ nhất trong hai số
 */
int min(int a, int b) { return a < b ? a : b; }

/*
 * Duyệt qua mảng và kiểm tra xem tất cả các phần tử có bằng nhau không
 * Tham số:
 *  int array1[]: mảng đầu tiên
 *  int array2[]: mảng thứ hai
 * Trả về:
 *  0 nếu hai mảng có chứa ít nhất một phần tử khác nhau
 *  1 nếu hai mảng hoàn toàn giống nhau đến length
 */
int are_arrays_equal(int array1[], int array2[2], int length) {
    for (int i = 0; i < length; i++) {
        if (array1[i] != array2[i]) {
            return 0;
        }
    }
    return 1;
}


int has_visited(Vector *visited_nodes, State state) {
    for (int i = 0; i < visited_nodes->size; i++) {
        State current = *(State *)vector_get(visited_nodes, i);
        if (are_arrays_equal(current.content, state.content, 3)) {
            return 1;
        }
    }
    return 0;
}

void find_moves(Vector *visited_nodes, Vector *moves, State state, int checking) {
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

        if (has_visited(visited_nodes, current_move))
            continue;

        vector_push_back(moves, &current_move);
    }
}

void get_valid_moves(Vector *visited_nodes, Vector *moves, State state) {
    for (int i = 0; i < 3; i++) {
        if (state.content[i] == 0)
            continue;
        find_moves(visited_nodes, moves, state, i);
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

void explore(Vector *visited_nodes, State root) {
    Vector possible_moves;
    vector_setup(&possible_moves, 0, sizeof(State));

    get_valid_moves(visited_nodes, &possible_moves, root);

    for (int i = 0; i < possible_moves.size; i++) {
        State node = *(State *)vector_get(&possible_moves, i);
        vector_push_back(visited_nodes, &node.content);
        print_root_and_node(root, node);

        if (is_finishing_condition(node)) exit(0);
        explore(visited_nodes, node);
    }
}

int main() {
    Vector visited_nodes;
    vector_setup(&visited_nodes, 0, sizeof(State));
    State root = (State){.content = {0, 7, 4}};
    vector_push_back(&visited_nodes, &root);

    explore(&visited_nodes, root);

    return 0;
}
