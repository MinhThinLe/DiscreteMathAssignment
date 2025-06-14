#include "../lib/vector.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int content[3];
} State;

// Sức chứa tối đa của các bình, theo đề bài là 10 7 và 4
const int CAPACITY[] = {10, 7, 4};

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

/*
 * Kiểm tra xem đã duyệt qua đỉnh có chứa nội dung trong state hay chưa
 * Tham số:
 *  Vector *visited_nodes: Một Vector chứa các đỉnh đã duyệt qua
 *  State state: Trạng thái của đỉnh cần tìm trong các đỉnh đã duyệt
 * Trả về:
 *  0 nếu đỉnh chứa state chưa được duyệt qua
 *  1 nếu ngược lại
 */
int has_visited(Vector *visited_nodes, State state) {
    for (int i = 0; i < visited_nodes->size; i++) {
        // Lấy thông tin về đỉnh đang kiểm tra từ vector
        State current = *(State *)vector_get(visited_nodes, i);
        // Nếu nội dung hai trạng thái giống nhau, trả về 1
        if (are_arrays_equal(current.content, state.content, 3)) {
            return 1;
        }
    }
    // Trả về 0 nếu không tìm thấy đỉnh nào thỏa mãn
    return 0;
}

/*
 * Lập danh sách những bước có thể đi từ state và kiểm tra xem đã đi đến nước đó
 * chưa rồi thêm vào moves
 * Tham số:
 *  Vector *visited_nodes: Những đỉnh đã duyệt qua
 *  Vector *moves: Những nước đi hợp lệ sẽ được viết vào đây
 *  State state: Trạng thái hiện tại của chương trình
 *  int checking: Chỉ số của bình nước đang kiểm tra
 * Trả về:
 *  void (dữ liệu được viết vào moves)
 */
void find_moves(Vector *visited_nodes, Vector *moves, State state,
                int checking) {
    // Duyệt qua các bình khác
    for (int i = 0; i < 3; i++) {
        State current_move = state;
        // Bỏ qua vì không thể đổ nước từ bình i sang chính nó
        if (i == checking)
            continue;
        // Bỏ qua nếu bình i đã đầy
        if (current_move.content[i] == CAPACITY[i])
            continue;

        // Lượng nước được chuyển sẽ bằng số bé hơn giữa lượng nước có thể
        // chuyển và lượng nước có thể chứa thêm
        int to_transfer = min(CAPACITY[i] - current_move.content[i],
                              current_move.content[checking]);
        // Chuyển nước
        current_move.content[i] += to_transfer;
        current_move.content[checking] -= to_transfer;

        // Nếu sau khi chuyển nước giữa các bình lặp lại một trong các trạng
        // thái đã duyệt thì ta bỏ qua
        if (has_visited(visited_nodes, current_move))
            continue;

        // Nếu mọi thứ thành công, thêm nước đi hợp lệ vào vector nước đi
        vector_push_back(moves, &current_move);
    }
}

/*
 * Tìm các nước đi hợp lệ (hàm wrapper cho find_moves)
 * Tham số:
 *  Vector *visited_nodes: Các đỉnh đã duyệt qua
 *  Vector *moves: Các nước đi hợp lệ sẽ được viết vào đây
 *  State state: Trạng thái hiện tại đang xét
 * Trả về
 *  void (các nước đi hợp lệ sẽ được viết vào moves)
 */
void get_valid_moves(Vector *visited_nodes, Vector *moves, State state) {
    for (int i = 0; i < 3; i++) {
        if (state.content[i] == 0)
            continue;
        find_moves(visited_nodes, moves, state, i);
    }
}

/*
 * In ra bước đi
 * Tham số:
 *  State root: Đỉnh mẹ
 *  State node: Đỉnh con
 * Trả về:
 *  void
 */
void print_root_and_node(State root, State node) {
    printf("_%d_%d_%d_ -> _%d_%d_%d_\n", root.content[0], root.content[1],
           root.content[2], node.content[0], node.content[1], node.content[2]);
}

/*
 * Khám phá tất cả các nước đi hợp lệ, sau đó in ra cây DFS dưới dạng
 * đỉnh mẹ -> đỉnh con
 * Tham số:
 *  Vector *visited_nodes: Các đỉnh đã duyệt qua
 *  State root: Đỉnh gốc, trạng thái bắt đầu của chương trình
 * Trả về:
 *  void
 */
void explore(Vector *visited_nodes, State root) {
    // Thiết lập vector chứa các nước đi có thể chọn
    Vector possible_moves;
    vector_setup(&possible_moves, 0, sizeof(State));

    // Lấy các nước đi có thể chọn cho đỉnh hiện tại
    get_valid_moves(visited_nodes, &possible_moves, root);

    // Với mỗi đỉnh trong các nước đi có thể chọn, duyệt qua nó
    for (int i = 0; i < possible_moves.size; i++) {
        State node = *(State *)vector_get(&possible_moves, i);
        // Thêm đỉnh hiện tại vào danh sách đã duyệt
        vector_push_back(visited_nodes, &node.content);
        // In ra bước đi
        print_root_and_node(root, node);

        // Nếu đỉnh hiện tại là điều kiện kết thúc, kết thúc chương trình
        if (is_finishing_condition(node))
            exit(0);
        // Nếu không, tiếp tục duyệt qua các đỉnh khác
        explore(visited_nodes, node);
    }
}

int main() {
    // Khởi tạo vector các đỉnh đã ghé thăm với kích thước dữ liệu của một phần
    // từ là State
    Vector visited_nodes;
    vector_setup(&visited_nodes, 0, sizeof(State));
    // Khởi tạo trạng thái ban đầu
    State root = (State){.content = {0, 7, 4}};
    // Đánh dấu rằng đã ghé thăm trạng thái ban đầu
    vector_push_back(&visited_nodes, &root);

    // Bắt đầu mô phỏng
    explore(&visited_nodes, root);

    return 0;
}
