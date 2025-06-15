#include <stdio.h>
#include <string.h>
#include "../lib/vector.h"

// Kích thước của một con trỏ, nên đúng với những máy tính dùng kiến trúc x86_64
#define POINTER_SIZE 8

// Tái sử dụng từ bài tập 2, hãy xem mã nguồn của bài tập 2 để biết thêm thông tin về
// struct này
typedef struct {
    char *content;
    Vector nexts;
} Node;

// Tái sử dụng từ bài tập 2
typedef struct {
    Node *current;
    Node *parent;
} VisitingOrder;

// Tái sử dụng từ bài tập 2
void trim(char buffer[]) { buffer[5] = '\0'; }

// Tái sử dụng từ bài tập 2
void read_dict(Vector *words) {
    FILE *file = fopen("ex2/sgb-words.txt", "r");

    char buffer[10] = {};
    while (fgets(buffer, 7, file)) {
        trim(buffer);
        vector_push_back(words, buffer);
    }
}

/*
 * Kiểm tra xem xâu word có chứa ký tự character không, sau khi tìm thấy, xóa kí tự
 * đó đi để thỏa mãn yêu cầu đề bài.
 * Tham số:
 *  char *word: Xâu cần kiểm tra
 *  char character: Ký tự cần kiểm tra
 * Trả về:
 *  0 nếu xâu không chứa ký tự character
 *  1 nếu ngược lại
 */
int contains(char *word, char character) {
    int length = strlen(word);
    for (int i = 0; i < length; i++) {
        if (word[i] == character) {
            // Có thể là bất cứ ký tự nào không phải chữ cái, chỉ nhằm mục đích đánh dấu
            word[i] = ' ';
            return 1;
        }
    }
    return 0;
}

// Tái sử dụng từ bài tập 2, có một số sửa đổi để phù hợp hơn với đề bài 3
int is_word_connected_to(char *word, char *dest) {
    // Rõ ràng một đỉnh không thể kết nối được với chính nó
    if (strncmp(word, dest, 5) == 0) {
        return 0;
    }
    
    char to_compare[10];
    char target[10];

    // Sao chép xâu sang buffer mới để tránh thay đổi nội dung ban đầu
    strncpy(to_compare, word + 1, 9);
    strncpy(target, dest, 9);

    // Thử xâu với yêu cầu của đề bài
    for (int i = 0; i < 4; i++) {
        if (!contains(target, to_compare[i])) {
            return 0;
        }
    }
    return 1;
}

// Tái sử dụng từ bài 2
void connect_node(Node *node, Vector *nodes) {
    for (int i = 0; i < nodes->size; i++) {
        Node *other_node = (Node *)vector_get(nodes, i);
        if (is_word_connected_to(node->content, other_node->content)) {
            vector_push_back(&node->nexts, &other_node);
        }
    }
}

// Tái sử dụng từ bài 2
void connect_nodes(Vector *node_vector) {
    for (int i = 0; i < node_vector->size; i++) {
        Node *node = (Node *)vector_get(node_vector, i);
        connect_node(node, node_vector);
    }
}

// Tái sử dụng từ bài 2
void construct_node_vector(Vector *node_vector, Vector *word_vector) {
    for (int i = 0; i < word_vector->size; i++) {
        Vector children;
        vector_setup(&children, 0, POINTER_SIZE);

        char *current_word = (char *)vector_get(word_vector, i);
        Node current_node = (Node){
            .content = current_word,
            .nexts = children,
        };
        vector_push_back(node_vector, &current_node);
    }
    connect_nodes(node_vector);
}

// TODO: Xóa hàm này khi làm xong
void print_connections(Node *node) {
    printf("Connected to:\n");
    for (int i = 0; i < node->nexts.size; i++) {
        Node *next = *(Node **) vector_get(&node->nexts, i);
        printf("\t%s\n", next->content);
    }
}

// TODO: Xóa hàm này khi làm xong
void print_node(Node *node) {
    printf("Content: %s\n", node->content);
    print_connections(node);
}

// TODO: Xóa hàm này khi làm xong
void print_nodes(Vector *nodes) {
    for (int i = 0; i < nodes->size; i++) {
        Node *node = *(Node **) vector_get(nodes, i);
        print_node(node);
    }
}

/*
 * Kiểm tra xem đỉnh đã được duyệt qua chưa
 * Tham số:
 *  Node *node: Đỉnh cần kiểm tra
 *  Vector *visited: Danh sách các đỉnh đã duyệt qua (chứa con trỏ tới các đỉnh
 *  đã duyệt qua)
 * Trả về:
 *  1 nếu đỉnh đã được duyệt qua
 *  0 nếu ngược lại
 */
int has_visited(Node *node, Vector *visited) {
    for (int i = 0; i < visited->size; i++) {
        Node *visited_node = *(Node **) vector_get(visited, i);
        if (node == visited_node) {
            return 1;
        }
    }
    return 0;
}

/*
 * Thăm tất cả các đỉnh có thể ghé thăm từ node
 * Tham số:
 *  Node *node: Đỉnh gốc để ghé thăm
 *  Vector *stack: Chồng các đỉnh theo thứ tự ghé thăm, truyền vào NULL nếu
 *  không muốn hàm quản lý chồng 🤨, chứa con trỏ tới các đỉnh đã duyệt qua
 *  Vector *visited: Danh sách các đỉnh đã ghé thăm
 * Trả về:
 *  void
 */
void visit(Node *node, Vector *stack, Vector *visited) {
    if (has_visited(node, visited)) {
        return;
    }
    vector_push_back(visited, &node);

    // Đẩy vào chồng nếu stack không phải NULL
    if (stack != NULL) {
        vector_push_back(stack, &node);
    }
    // DFS
    for (int i = 0; i < node->nexts.size; i++) {
        Node *node_to_visit = *(Node **) vector_get(&node->nexts, i);
        visit(node_to_visit, stack, visited);
    }
}

/*
 * Tìm các đỉnh khác được kết nối với đỉnh node, dùng để chuyển vị đồ thị (graph
 * transposition)
 * Tham số:
 *  Node *node: Đỉnh để từ đó tìm các đỉnh khác có kết nối với đỉnh này
 *  Vector *nodes: Danh sách các đỉnh của đồ thị, chứa Node
 * Trả về:
 *  Một Vector chứa các đỉnh được kết nối với Node, chứa con trỏ tới Node
 */
Vector get_nodes_connected_to(Node *node, Vector *nodes) {
    Vector connected_nodes;
    vector_setup(&connected_nodes, 0, POINTER_SIZE);
    for (int i = 0; i < nodes->size; i++) {
        Node *other_node = (Node *) vector_get(nodes, i);
        if (is_word_connected_to(other_node->content, node->content)) {
            vector_push_back(&connected_nodes, &other_node);
        }
    }
    return connected_nodes;
}

/*
 * Chuyển vị đồ thị
 * Tham số:
 *  Vector *original_graph: Đồ thị gốc cần chuyển vị, chứa Node
 *  Vector *nodes_to_transpose: Vector chứa con trỏ đến các đỉnh cần chuyển vị
 * Trả về:
 *  void
 */
void graph_transposition(Vector *original_graph, Vector *nodes_to_transpose) {
    for (int i = 0; i < nodes_to_transpose->size; i++) {
        Node *current_node = *(Node **) vector_get(nodes_to_transpose, i);
        current_node->nexts =
            get_nodes_connected_to(current_node, original_graph);
    }
}

/*
 * Thuật toán kosarajy để tìm các thành phần liên thông mạnh
 * Tham số:
 *  Vector *node_vector: Vector chứa các đỉnh của đồ thị
 * Trả về:
 *  void
 */
void kosaraju(Vector *node_vector) {
    Vector stack;
    Vector visited;

    vector_setup(&stack, 1000, POINTER_SIZE);
    vector_setup(&visited, 1000, POINTER_SIZE);

    for (int i = 0; i < node_vector->size; i++) {
        Node *node = (Node *) vector_get(node_vector, i);
        visit(node, &stack, &visited);
    }

    // Second phase of the algorithm
    vector_clear(&visited);
    graph_transposition(node_vector, &stack);
    int scc_counter = 0;

    while (stack.size != 0) {
        Node *node = *(Node **) vector_get(&stack, stack.size - 1);
        vector_pop_back(&stack);
        if (has_visited(node, &visited)) {
            continue;
        }
        if (node->nexts.size == 0) {
            continue;
        }
        scc_counter++;
        visit(node, NULL, &visited);
    }
    printf("SCC counter: %d\n", scc_counter);
}

// Tái sử dụng từ bài 2
void bfs_setup(char *start, char *end) {
    printf("Chọn điểm bắt đầu: ");
    fgets(start, 9, stdin);
    printf("Chọn điểm kết thúc: ");
    fgets(end, 9, stdin);
    trim(start);
    trim(end);
}

// Tái sử dụng từ bài 2
Node *find_node(Vector *nodes, char *content) {
    for (int i = 0; i < nodes->size; i++) {
        Node *node = (Node *)vector_get(nodes, i);
        if (strncmp(node->content, content, 5) == 0) {
            return node;
        }
    }
    printf("Đồ thị không chứa %s\n", content);
    return NULL;
}

// Tái sử dụng từ bài 2
int dfs_has_visited(Vector *search_order, Node *node) {
    for (int i = 1; i < search_order->size; i++) {
        VisitingOrder *visited_node =
            (VisitingOrder *)vector_get(search_order, i);
        if (strncmp(visited_node->parent->content, node->content, 5) == 0) {
            return 1;
        }
    }
    return 0;
}

// Tái sử dụng từ bài 2
VisitingOrder locate_node(Vector *search_order, Node *node) {
    for (int i = 0; i < search_order->size; i++) {
        VisitingOrder visit_order =
            *(VisitingOrder *)vector_get(search_order, i);
        if (visit_order.current == node) {
            return visit_order;
        }
    }
    return (VisitingOrder){
        .current = NULL,
        .parent = NULL,
    };
}

// Tái sử dụng từ bài 2
void reconstruct_path(Node *end_node, Node *start_node, Vector *search_order) {
    VisitingOrder visit_order = locate_node(search_order, end_node);
    printf("%s ", visit_order.current->content);
    // Lặp lại thao tác trên cho đến khi gặp đỉnh mẹ là NULL
    while (visit_order.parent != NULL) {
        visit_order = locate_node(search_order, visit_order.parent);
        printf("<- %s ", visit_order.current->content);
    }
    putchar('\n');
}

// Tái sử dụng từ bài 2
void add_to_queue(Vector *queue, Node *node) {
    for (int i = 0; i < node->nexts.size; i++) {
        Node *next = *(Node **)vector_get(&node->nexts, i);
        vector_push_back(queue, &next);
    }
}

// Tái sử dụng từ bài 2
void add_to_search_order(Vector *search_order, Node *node) {
    for (int i = 0; i < node->nexts.size; i++) {
        Node *next = *(Node **)vector_get(&node->nexts, i);
        VisitingOrder order = {
            .parent = node,
            .current = next,
        };
        vector_push_back(search_order, &order);
    }
}

// Tái sử dụng từ bài 2
void breadth_first_search(Vector *nodes, char *start, char *end) {
    Node *start_node = find_node(nodes, start);
    Node *end_node = find_node(nodes, end);
    if (start_node == NULL || end_node == NULL)
        return;

    Vector visiting_order;
    Vector queue;

    VisitingOrder root = {
        .current = start_node,
        .parent = NULL,
    };

    vector_setup(&visiting_order, 10, sizeof(VisitingOrder));
    vector_push_back(&visiting_order, &root);

    vector_setup(&queue, 10, POINTER_SIZE);
    vector_push_back(&queue, &start_node);

    while (!vector_is_empty(&queue)) {
        Node *current_node = *(Node **)vector_get(&queue, 0);
        vector_pop_front(&queue);

        if (dfs_has_visited(&visiting_order, current_node)) {
            continue;
        }

        if (strncmp(current_node->content, end_node->content, 5) == 0) {
            printf("Đường đi ngắn nhất từ %s đến %s như sau\n", start, end);
            reconstruct_path(current_node, start_node, &visiting_order);
            return;
        } else {
            add_to_queue(&queue, current_node);
            add_to_search_order(&visiting_order, current_node);
        }
    }
    printf("Không tồn tại đường đi từ %s đến %s trong đồ thị này\n", start,
           end);
}

int main() {
    Vector words;
    Vector node_vector;
    Vector node_vector_backup;

    vector_setup(&words, 1000, 8);
    vector_setup(&node_vector, 1000, sizeof(Node));

    // Setup
    read_dict(&words);
    construct_node_vector(&node_vector, &words);

    // Không may mắn thay, hàm kosaraju sẽ thay đổi nội dung của node_vector.
    // Thay vì sửa đổi thuật toán để bỏ sự thay đổi này thì chúng ta sẽ tạo ra
    // một bản sao của node_vector trước khi chạy thuật toán vì đây là cách sửa
    // lỗi nhanh nhất.
    // Rant: Vì C được thiết kế là một ngôn ngữ bậc thấp và cũng ngoa khi gọi C
    // là assembly thân thiện hơn nên trình biên dịch C không thể cảnh báo lập
    // trình viên rằng họ sắp tự bắn vào chân mình. Với những lập trình viên C
    // lão luyện thì đây là một sự kiện hằng tuần vào mỗi ngày lẻ. Tuy nhiên, với
    // những người đang cố gắng học C thì đây là một cạm bẫy rất dễ mắc phải.
    // Tất cả mọi cạm bẫy này có thể tránh được bằng cách sử dụng một ngôn ngữ
    // lập trình bậc thấp khác là Rust. Lỗi vô ý thay đổi giá trị từ con trỏ
    // là một lỗi không thể xảy ra ở Rust vì bạn phải cung cấp nhiều thông tin
    // hơn về kiểu dữ liệu để dựa vào đó, trình biên dịch có thể dừng bạn lại
    // trước khi sai lầm này có thể đi vào code base của bạn.
    vector_copy(&node_vector_backup, &node_vector);
    
    // Chạy thuật toán Kosaraju
    kosaraju(&node_vector);

    // Third part of the assignment
    char start[10] = {}, end[10] = {};
    bfs_setup(start, end);
    breadth_first_search(&node_vector_backup, start, end);

    return 0;
}
