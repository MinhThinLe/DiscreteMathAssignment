#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../lib/vector.h"

#define POINTER_SIZE 8

// Định nghĩa kiểu dữ liệu đỉnh
typedef struct {
    // Nội dung là một xâu ký tự
    char *content;
    // Vector chứa con trỏ đến các đỉnh tiếp theo
    Vector nexts;
} Node;

// Cắt phần dư của xâu, giả định rằng xâu có đúng 5 bytes (5 ký tự ASCII)
void trim(char buffer[]) { buffer[5] = '\0'; }

/*
 * Đọc từ điển và thêm nội dung của nó vào words
 * Tham số:
 *  Vector *words: Con trỏ đến các từ sau khi đọc sẽ được viết vào đây
 * Trả về:
 *  void (con trỏ đến dữ liệu sau khi đọc sẽ được viết vào words)
 */
void read_dict(Vector *words) {
    // Mở file từ điển
    FILE *file = fopen("ex2/sgb-words.txt", "r");

    char buffer[10] = {};
    // Đọc đến file từ điển và thêm nó vào words
    while (fgets(buffer, 7, file)) {
        trim(buffer);
        vector_push_back(words, buffer);
    }
}

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

/*
 * Kiểm tra xem hai từ có được kết nối với nhau không với điều kiện kết nối là
 * hai từ khác nhau đúng một chữ cái
 * Tham số:
 * char *word1: Từ thứ nhất
 * char *word2: Từ thứ hai
 * Trả về: 0 nếu hai từ không được kết nối với nhau
 * 1 nếu ngược lại
 */
int is_word_connected_to(char *word, char *dest) {
    char to_compare[10];
    char target[10];

    if (strncmp(word, dest, 5) == 0) {
        // Rõ ràng một đỉnh không thể kết nối được với chính nó
        return 0;
    }

    // Bắt đầu kiểm tra từ chữ cái thứ 2
    strncpy(to_compare, word + 1, 9);
    strncpy(target, dest, 9);

    for (int i = 0; i < 4; i++) {
        if (!contains(target, to_compare[i])) {
            return 0;
        }
    }
    return 1;
}

/*
 * Nối 1 đỉnh với các đỉnh còn lại thỏa mãn điều kiện
 * Tham số:
 *  Node *node: Đỉnh đang xét
 *  Vector *nodes: Các đỉnh còn lại
 * Trả về:
 *  void
 */
void connect_node(Node *node, Vector *nodes) {
    for (int i = 0; i < nodes->size; i++) {
        Node *other_node = (Node *)vector_get(nodes, i);
        // Nếu tìm thấy 1 đỉnh khác thỏa mãn, thêm con trỏ của nó vào node->next
        if (is_word_connected_to(node->content, other_node->content)) {
            vector_push_back(&node->nexts, &other_node);
        }
    }
}

/*
 * Nối tất cả đỉnh lại với nhau (hàm wrapper cho connect_node)
 * Tham số:
 *  Vector *node_vector: Danh sách các đỉnh
 * Trả về:
 *  void
 */
void connect_nodes(Vector *node_vector) {
    for (int i = 0; i < node_vector->size; i++) {
        Node *node = (Node *)vector_get(node_vector, i);
        connect_node(node, node_vector);
    }
}

/*
 * Xây dựng danh sách các đỉnh từ danh sách các từ và nối chúng lại với nhau
 * Tham số:
 *  Vector *node_vector: Sau khi hàm kết thúc, dữ liệu sẽ được viết vào đây
 *  Vector *word_vector: Danh sách các từ
 * Trả về:
 *  void
 * Thuật toán này thật sự không tối ưu với thời gian chạy khoảng gần 3s trên
 * CPU Intel Core i5 12450H nhưng mà ai bảo nó hoạt động làm gì ¯\_(ツ)_/¯
 */
void construct_node_vector(Vector *node_vector, Vector *word_vector) {
    for (int i = 0; i < word_vector->size; i++) {
        // Khởi tạo đỉnh với nội dung từ word_vector
        Vector children;
        vector_setup(&children, 0, POINTER_SIZE);

        char *current_word = (char *)vector_get(word_vector, i);
        Node current_node = (Node){
            .content = current_word,
            .nexts = children,
        };
        vector_push_back(node_vector, &current_node);
    }
    // Nối các đỉnh với nhau
    connect_nodes(node_vector);
}

void print_connections(Node *node) {
    printf("Connected to:\n");
    for (int i = 0; i < node->nexts.size; i++) {
        Node *next = *(Node **) vector_get(&node->nexts, i);
        printf("\t%s\n", next->content);
    }
}

void print_node(Node *node) {
    printf("Content: %s\n", node->content);
    print_connections(node);
}

void print_nodes(Vector *nodes) {
    for (int i = 0; i < nodes->size; i++) {
        Node *node = *(Node **) vector_get(nodes, i);
        print_node(node);
    }
}

int has_visited(Node *node, Vector *visited) {
    for (int i = 0; i < visited->size; i++) {
        Node *visited_node = *(Node **) vector_get(visited, i);
        if (node == visited_node) {
            return 1;
        }
    }
    return 0;
}

void visit(Node *node, Vector *stack, Vector *visited) {
    if (has_visited(node, visited)) {
        return;
    }
    vector_push_back(visited, &node);
    if (stack != NULL) {
        vector_push_back(stack, &node);
    }
    for (int i = 0; i < node->nexts.size; i++) {
        Node *node_to_visit = *(Node **) vector_get(&node->nexts, i);
        visit(node_to_visit, stack, visited);
    }
}
// Vector *node: A vector containing Nodes
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

// Vector *original_graph: A vector containing Nodes
// Vector *stack: A stack of graph containing pointers to Node to be transposed 
void graph_transposition(Vector *original_graph, Vector *stack) {
    for (int i = 0; i < stack->size; i++) {
        Node *current_node = *(Node **) vector_get(stack, i);
        current_node->nexts = get_nodes_connected_to(current_node, original_graph);
    }
}

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

int main() {
    Vector words;
    Vector node_vector;

    vector_setup(&words, 1000, 8);
    vector_setup(&node_vector, 1000, sizeof(Node));

    clock_t start_of_read_dict = clock();
    read_dict(&words);
    clock_t end_of_read_dict = clock();

    clock_t start_of_construct_node_vector = clock();
    construct_node_vector(&node_vector, &words);
    clock_t end_of_construct_node_vector = clock();
    
    clock_t start_of_kosaraju = clock();
    kosaraju(&node_vector);
    clock_t end_of_kosaraju = clock();

    printf("Reading dict took: %f seconds\n", (float) (end_of_read_dict - start_of_read_dict) / CLOCKS_PER_SEC);
    printf("Construct node vector took: %f seconds\n", (float) (end_of_construct_node_vector - start_of_construct_node_vector) / CLOCKS_PER_SEC);
    printf("Kosaraju took: %f seconds\n", (float) (end_of_kosaraju - start_of_kosaraju) / CLOCKS_PER_SEC);
    return 0;
}
