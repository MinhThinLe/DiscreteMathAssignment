#include <stdio.h>
#include "vector.h"

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

/*
 * Kiểm tra xem hai từ có được kết nối với nhau không với điều kiện kết nối là
 * hai từ khác nhau đúng một chữ cái
 * Tham số:
 * char *word1: Từ thứ nhất
 * char *word2: Từ thứ hai
 * Trả về: 0 nếu hai từ không được kết nối với nhau
 * 1 nếu ngược lại
 */
int are_words_connected(char *word1, char *word2) {
    int differences = 0;
    // Đếm ký tự khác biệt giữa hai từ
    for (int i = 0; i < 5; i++) {
        if (word1[i] != word2[i]) {
            differences++;
        }
    }
    // kiểm tra xem có đúng 1 ký tự khác biệt hay không
    return differences == 1;
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
        if (are_words_connected(node->content, other_node->content)) {
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

int main() {
    Vector words;
    Vector node_vector;

    vector_setup(&words, 1000, 8);
    vector_setup(&node_vector, 1000, sizeof(Node));

    read_dict(&words);
    construct_node_vector(&node_vector, &words);

    return 0;
}
