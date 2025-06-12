#include "vector.h"
#include "stdio.h"
#include <stdio.h>
#include <stdlib.h>

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

void construct_node_vector(Vector *node_vector, Vector *word_vector) {
    for (int i = 0; i < word_vector->size; i++) {

        Vector children;
        vector_setup(&children, 0, POINTER_SIZE);

        char *current_word = (char *) vector_get(word_vector, i);
        Node current_node = {
            .content = current_word,
            .nexts = children,
        };

        get_connected_nodes(&current_node, word_vector);
        vector_push_back(node_vector, &current_node);
    }
}

int main() {
    Vector words;
    Vector node_vector;
    char *pchar;

    vector_setup(&words, 1000, sizeof(pchar));
    vector_setup(&node_vector, 1000, sizeof(Node));

    read_dict(&words);

    construct_node_vector(&node_vector, &words);
    vector_print(&node_vector);
    return 0;
}
