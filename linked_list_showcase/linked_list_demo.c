#include <stdio.h>
#include <stdlib.h>

// Define a node structure
struct Node {
    int data;
    struct Node* next;
};

int main() {
    // Create nodes
    struct Node* head = malloc(sizeof(struct Node));
    struct Node* second = malloc(sizeof(struct Node));
    struct Node* third = malloc(sizeof(struct Node));

    // Assign data and link nodes
    head->data = 10;
    head->next = second;

    second->data = 20;
    second->next = third;

    third->data = 30;
    third->next = NULL;

    // Print linked list with node sizes
    struct Node* current = head;
    int i = 0;
    printf("=== Linked List ===\n");
    printf("Size of each Node: %zu bytes\n", sizeof(struct Node));
    while (current != NULL) {
        printf("Node %d: data = %d, address = %p, next = %p\n",
               i, current->data, (void*)current, (void*)current->next);
        current = current->next;
        i++;
    }

    // Free allocated memory
    free(third);
    free(second);
    free(head);

    return 0;
}
