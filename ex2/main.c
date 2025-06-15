#include "stdio.h"
#include "../lib/vector.h"
#include <stdio.h>
#include <string.h>

Vector VISITED;

// Kích thước của một con trỏ (nên đúng với những CPU x86_64 hiện đại)
#define POINTER_SIZE 8

// Định nghĩa kiểu dữ liệu đỉnh
typedef struct {
    // Nội dung là một xâu ký tự
    char *content;
    // Vector chứa con trỏ đến các đỉnh tiếp theo
    Vector nexts;
} Node;

// Định nghĩa kiểu dữ liệu thứ tự ghé thăm (dùng trong dfs)
typedef struct {
    // Đỉnh con
    Node *current;
    // Đỉnh mẹ mà tại đó, ta có thể đi tới đỉnh con
    Node *parent;
} VisitingOrder;

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

/*
 * Tìm xem đỉnh đã được duyệt qua hay chưa, khác với hàm dfs_has_visited
 * Tham số:
 *  Node *node: Đỉnh đang xét
 *  Vector VISITED: Biến toàn cục, danh sách các đỉnh đã ghé thăm (VISITED hoàn
 *  toàn không nhất thiết phải là biết toàn cục cơ mà nó hoạt động ¯\_(ツ)_/¯)
 * Trả về:
 *  0 nếu đỉnh không tồn tại trong danh sách đã duyệt qua
 *  1 nếu ngược lại
 */
int has_visited(Node *node) {
    for (int i = 0; i < VISITED.size; i++) {
        // Nếu hai đỉnh có cùng nội dung thì có nghĩa là đã duyệt qua đỉnh đó
        Node *other_node = *(Node **)vector_get(&VISITED, i);
        if (strncmp(node->content, other_node->content, 5) == 0) {
            return 1;
        }
    }
    return 0;
}

/*
 * Khám phá tất cả các đỉnh có thể đến được từ node
 * Tham số:
 *  Node *node: Đỉnh đang xét
 * Trả về:
 *  void
 */
void explore(Node *node) {
    // Bỏ qua nếu đỉnh đã được duyệt qua
    if (has_visited(node)) {
        return;
    }
    // Thêm đỉnh vào danh sách đã duyệt qua
    vector_push_back(&VISITED, &node);
    // Duyệt qua các đỉnh con
    for (int i = 0; i < node->nexts.size; i++) {
        Node *next_node = *(Node **)vector_get(&node->nexts, i);
        explore(next_node);
    }
}

/*
 * Đếm phần tử đơn độc (phần tử không kết nối với phần tử nào khác)
 * Tham số:
 *  Vector *node_vector: Danh sách các phần tử
 * Trả về:
 *  int: số các phần tử đơn độc
 */
int count_isolated_words(Vector *node_vector) {
    int counter = 0;
    for (int i = 0; i < node_vector->size; i++) {
        // Nếu đỉnh hiện tại không kết nối với đỉnh nào khác, ta tăng biến đếm
        // lên 1
        Node *node = (Node *)vector_get(node_vector, i);
        if (node->nexts.size == 0) {
            counter++;
        }
    }
    return counter;
}

/*
 * Đếm thành phần liên thông
 * Tham số:
 *  Vector *node_vector: Danh sách các đỉnh
 * Trả về:
 *  void (số thành phần liên thông sẽ được in ra stdout)
 */
void count_components(Vector *node_vector) {
    int components = 0;
    for (int i = 0; i < node_vector->size; i++) {
        Node *node = (Node *)vector_get(node_vector, i);
        // Nếu đỉnh hiện tại đã được duyệt qua, ta bỏ qua
        if (has_visited(node)) {
            continue;
        }
        // Duyệt qua tất cả các đỉnh có thể đến được từ node
        explore(node);
        components++;
    }
    // Rõ ràng một thành phần liên thông phải có trên 1 đỉnh nên ta trừ số thành
    // phần liên thông đã đếm được cho các đỉnh đơn độc
    int isolated_words = count_isolated_words(node_vector);
    printf("Có %d thành phần liên thông trong đồ thị\n",
           components - isolated_words);
}

/*
 * Tìm đỉnh có chứa cùng nội dung với content
 * Tham số:
 *  Vector *nodes: Danh sách các đỉnh
 *  char *content: Nội dung cần tìm
 * Trả về:
 *  Node *: Con trỏ đến node chứa nội dung cần tìm
 */
Node *find_node(Vector *nodes, char *content) {
    for (int i = 0; i < nodes->size; i++) {
        Node *node = (Node *)vector_get(nodes, i);
        if (strncmp(node->content, content, 5) == 0) {
            return node;
        }
    }
    // Nếu không tìm thấy, thông báo cho người dùng và trả về NULL
    printf("Đồ thị không chứa %s\n", content);
    return NULL;
}

/*
 * Thêm các đỉnh con của node vào hàng chờ (dùng cho bfs)
 * Tham số:
 *  Vector *queue: Hàng chờ
 *  Node *node: Đỉnh đang xét
 * Trả về:
 *  void
 */
void add_to_queue(Vector *queue, Node *node) {
    for (int i = 0; i < node->nexts.size; i++) {
        Node *next = *(Node **)vector_get(&node->nexts, i);
        vector_push_back(queue, &next);
    }
}

/*
 * Thêm các đỉnh con của node vào thứ tự tìm kiếm (để tái lập đường đi)
 * Tham số:
 *  Vector *search_order: Danh sách thứ tự tìm kiếm
 *  Node *node: Đỉnh đang xét
 * Trả về:
 *  void
 */
void add_to_search_order(Vector *search_order, Node *node) {
    for (int i = 0; i < node->nexts.size; i++) {
        // Khởi tạo thứ tự tìm kiếm với đỉnh mẹ là node và đỉnh con là các đỉnh
        // con của node
        Node *next = *(Node **)vector_get(&node->nexts, i);
        VisitingOrder order = {
            .parent = node,
            .current = next,
        };
        vector_push_back(search_order, &order);
    }
}

/*
 * Kiểm tra xem đã ghé thăm một đỉnh chưa, khác với has_visited dùng trong đếm
 * thành phần liên thông 
 * Tham số:
 *  Vector *search_order: Danh sách thứ tự tìm kiếm (tái sử dụng để tiết kiệm bộ nhớ) 
 *  Node *node: Đỉnh cần kiểm tra 
 *  Trả về:
 *   0 nếu đỉnh chưa được ghé thăm
 *   1 nếu ngược lại
 */
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

/*
 * Tìm trong vector thứ tự tìm kiếm có chứa node là .current
 * Tham số:
 *  Vector *search_order: Vector chứa thứ tự tìm kiếm
 *  Node *node: Đỉnh cần tìm từ trong vector
 * Trả về:
 *   Một VisitingOrder có .current là node nếu tìm thấy
 *   Một VisitingOrder có .current và .parent đều là NULL nếu không
 */
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

/*
 * Tái tạo đường đi từ đỉnh bắt đầu đến đỉnh kết thúc với danh sách thứ tự duyệt
 * Tham số:
 *  Node *end_node: Đỉnh kết thúc
 *  Node *start_node: Đỉnh bắt đầu
 *  Vector *search_order: Danh sách chứa thứ tự duyệt các đỉnh
 * Trả về:
 *  void (kết quả được in ra stdout)
 */
void reconstruct_path(Node *end_node, Node *start_node, Vector *search_order) {
    // Tìm đỉnh kết thúc và in ra cách để đến đỉnh kết thúc
    VisitingOrder visit_order = locate_node(search_order, end_node);
    printf("%s ", visit_order.current->content);
    // Lặp lại thao tác trên cho đến khi gặp đỉnh mẹ là NULL
    while (visit_order.parent != NULL) {
        visit_order = locate_node(search_order, visit_order.parent);
        printf("<- %s ", visit_order.current->content);
    }
    // Sửa lỗi in ra ký tự cuối là % trên Linux
    putchar('\n');
}

/*
 * Dùng thuật toán breadth first search để tìm đường đi ngắn nhất giữa hai đỉnh
 * trong một unweighted graph?? 
 * Tham số: 
 *  Vector *nodes: Các đỉnh của đồ thị 
 *  char *start: Nội dung của đỉnh bắt đầu 
 *  char *end: Nội dung của đỉnh kết thúc 
 * Trả về:
 *   void
 */
void breadth_first_search(Vector *nodes, char *start, char *end) {
    // Chắc chắn rằng đồ thị có chứa đỉnh cần tìm
    Node *start_node = find_node(nodes, start);
    Node *end_node = find_node(nodes, end);
    if (start_node == NULL || end_node == NULL)
        return;

    // Khởi tạo các tham số cho thuật toán
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

    // Lặp cho đến khi không còn đỉnh để duyệt
    while (!vector_is_empty(&queue)) {
        // Xóa đỉnh theo thứ tự FIFO
        Node *current_node = *(Node **)vector_get(&queue, 0);
        vector_pop_front(&queue);

        // Bỏ qua nếu đã duyệt qua đỉnh
        if (dfs_has_visited(&visiting_order, current_node)) {
            continue;
        }

        // Kiểm tra xem đỉnh này có phải đỉnh kết thúc không
        if (strncmp(current_node->content, end_node->content, 5) == 0) {
            printf("Đường đi ngắn nhất từ %s đến %s như sau\n", start, end);
            reconstruct_path(current_node, start_node, &visiting_order);
            return;
        } else {
            // Tiếp tục thuật toán nếu không phải
            add_to_queue(&queue, current_node);
            add_to_search_order(&visiting_order, current_node);
        }
    }
    // Thông báo rằng không có đường đi nếu không thể tìm
    printf("Không tồn tại đường đi từ %s đến %s trong đồ thị này\n", start,
           end);
}

/*
 * Thiết lập đỉnh bắt đầu và kết thúc cho bfs
 * Tham số:
 *  char *start: Nội dung của đỉnh bắt đầu sẽ được viết vào đây
 *  char *end: Nội dung của đỉnh kết thúc sẽ được viết vào đây
 * Trả về:
 *  void
 */
void bfs_setup(char *start, char *end) {
    printf("Chọn điểm bắt đầu: ");
    fgets(start, 9, stdin);
    printf("Chọn điểm kết thúc: ");
    fgets(end, 9, stdin);
    trim(start);
    trim(end);
}

int main() {
    // Khởi tạo các tham số cần cho chương trình
    Vector words;
    Vector node_vector;

    vector_setup(&words, 1000, POINTER_SIZE);
    vector_setup(&VISITED, 1000, POINTER_SIZE);
    vector_setup(&node_vector, 1000, sizeof(Node));

    // Phần 1 của đề bài
    read_dict(&words);
    construct_node_vector(&node_vector, &words);
    count_components(&node_vector);

    // Phần 2 của đề bài
    char start[10] = {}, end[10] = {};
    bfs_setup(start, end);
    breadth_first_search(&node_vector, start, end);

    return 0;
}
