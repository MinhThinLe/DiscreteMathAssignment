#include "vector.h"
#include "stdio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Vector VISITED;

// Should be correct on modern, 64bits computers
#define POINTER_SIZE 8

typedef struct {
    char *content;
    Vector nexts;
} Node;

void trim(char buffer[]) {
    buffer[5] = '\0';
}

// Read the dictionary, 
void read_dict(Vector *words) {
    FILE *file = fopen("ex2/sgb-words.txt", "r");

    char *buffer = malloc(10);
    while (fgets(buffer, 7, file)) {
        trim(buffer);
        vector_push_back(words, buffer);
    }
}

void print_connections(Node *node) {
    printf("Conneted to\n");
    for (int i = 0; i < node->nexts.size; i++) {
        Node *connected_node = (Node *) vector_get(&node->nexts, i);
        printf("\t %s\n", connected_node->content);
    }
}

void vector_print(Vector *vector) {
    for (int i = 0; i < vector->size; i++) {
        Node *node = vector_get(vector, i);

        printf("Node\ncontent: %s\n", node->content);
        print_connections(node);
    }
}

int are_words_connected(char *word1, char *word2) {
    int differences = 0;
    for (int i = 0; i < 5; i++) {
        if (word1[i] != word2[i]) {
            differences++;
        }
    }
    return differences == 1;
}

void get_connected_nodes(Node *node, Vector *word_vector) {
    for (int i = 0; i < word_vector->size; i++) {
        char *word = vector_get(word_vector, i);
        if (are_words_connected(node->content, word)) {
            vector_push_back(&node->nexts, &word);
        }
    }
}

void connect_node(Node *node, Vector *nodes) {
    for (int i = 0; i < nodes->size; i++) {
        Node *other_node = (Node *) vector_get(nodes, i);
        if (are_words_connected(node->content, other_node->content)) {
            vector_push_back(&node->nexts, &other_node);
        }
    }
}

void node_print(Node *node) {
    printf("Node \n");
    printf("\t.content = %s\n", node->content);
    printf("\t.nexts = {\n");
    for (int i = 0; i < node->nexts.size; i++) {
        Node *next = (Node *) vector_get(&node->nexts, i);
        printf("\t\t%s\n", next->content);
    }
    printf("\t}\n");
}

void connect_nodes(Vector *node_vector) {
    for (int i = 0; i < node_vector->size; i++) {
        Node *node = (Node *) vector_get(node_vector, i);
        connect_node(node, node_vector);
    }
}

void construct_node_vector(Vector *node_vector, Vector *word_vector) {
    for (int i = 0; i < word_vector->size; i++) {
        Vector children;
        vector_setup(&children, 0, POINTER_SIZE);

        char *current_word = (char *) vector_get(word_vector, i);
        Node current_node = (Node) {
            .content = current_word,
            .nexts = children,
        };
        vector_push_back(node_vector, &current_node);
    }
    connect_nodes(node_vector);
}

int has_visited(Node *node) {
    for (int i = 0; i < VISITED.size; i++) {
        Node *other_node = *(Node **) vector_get(&VISITED, i);
        if (strncmp(node->content, other_node->content, 5) == 0) {
            return 1;
        }
    }
    return 0;
}

void explore(Node *node) {
    if (has_visited(node)) {
        return;
    }
    vector_push_back(&VISITED, &node);
    for (int i = 0; i < node->nexts.size; i++) {
        Node *next_node = *(Node **) vector_get(&node->nexts, i);
        explore(next_node);
    }
}

int count_isolated_words(Vector *node_vector) {
    int counter = 0; 
    for (int i = 0; i < node_vector->size; i++) {
        Node *node = (Node *) vector_get(node_vector, i);
        if (node->nexts.size == 0) {
            counter++;
        }
    }
    return counter;
}

void count_components(Vector *node_vector) {
    int components = 0;
    for (int i = 0; i < node_vector->size; i++) {
        Node *node = (Node *) vector_get(node_vector, i);
        if (has_visited(node)) {
            continue;
        }
        explore(node);
        components++;
    }
    int isolated_words = count_isolated_words(node_vector);
    printf("There are %d components in this graph\n", components - isolated_words);
}


int main() {
    Vector words;
    Vector node_vector;
    char *pchar;

    vector_setup(&words, 1000, sizeof(pchar));
    vector_setup(&VISITED, 1000, sizeof(pchar));
    vector_setup(&node_vector, 1000, sizeof(Node));

    read_dict(&words);

    construct_node_vector(&node_vector, &words);

    count_components(&node_vector);

    return 0;
}
