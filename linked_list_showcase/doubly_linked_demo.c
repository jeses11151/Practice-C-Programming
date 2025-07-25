#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
    int data;
    char character; 
    struct Node* next;
    struct Node* prev;
};

// Function to create and initialize a new node
struct Node* create_node(int data, char character) {
    struct Node* new_node = malloc(sizeof(struct Node));
    if (!new_node) {
        perror("Failed to allocate memory");
        exit(1);
    }
    new_node->data = data;
    new_node->character = character;
    new_node->next = NULL;
    new_node->prev = NULL;
    return new_node;
}


// Link two nodes
void link_nodes(struct Node* first, struct Node* second) {
    if (first) first->next = second;
    if (second) second->prev = first;
}

// Print the list forward
void print_forward(struct Node* head) {
    printf("→ Forward List:\n");
    struct Node* current = head;
    int index = 0;
    while (current) {
        printf("Node %d: char: '%c', data: %d [addr: %p, prev: %p, next: %p]\n",
        index++, current->character ? current->character : '-',current->data,
            (void*)current, (void*)current->prev, (void*)current->next);
        printf(" [addr: %p, prev: %p, next: %p]\n",
            (void*)current, (void*)current->prev, (void*)current->next);
        current = current->next;
    }
}

// Print the list backward
void print_backward(struct Node* tail) {
    printf("← Backward List:\n");
    struct Node* current = tail;
    int index = 0;
    while (current) {
        printf("Node %d: ", index++);
        if (current->character != '\0') {
            printf("char: '%c'", current->character);
        }
        if (current->data != 0) {
            if (current->character != '\0') printf(", ");
            printf("data: %d", current->data);
        }
        printf(" [addr: %p, prev: %p, next: %p]\n",
            (void*)current, (void*)current->prev, (void*)current->next);
        current = current->next;
    }
}

// Add a node to the end of the list
void add_node(struct Node** head_ref, struct Node** tail_ref, int data, char character) {
    struct Node* new_node = create_node(data, character);
    if (*head_ref == NULL) {
        *head_ref = *tail_ref = new_node;
    } else {
        link_nodes(*tail_ref, new_node);
        *tail_ref = new_node;
    }
    printf("✓ Added node with data %d and character '%c'\n", data, character);
}


// Remove a node from the end of the list
void remove_node(struct Node** head_ref, struct Node** tail_ref) {
    if (*tail_ref == NULL) {
        printf("⚠ List is empty!\n");
        return;
    }

    struct Node* to_delete = *tail_ref;
    *tail_ref = (*tail_ref)->prev;

    if (*tail_ref)
        (*tail_ref)->next = NULL;
    else
        *head_ref = NULL;

    printf("✗ Removed node with data %d\n", to_delete->data);
    free(to_delete);
}
  

// Show command menu
void print_menu() {
    printf("\n====== MENU ======\n");
    printf("[1] Add node\n");
    printf("[2] Remove node\n");
    printf("[3] Print forward\n");
    printf("[4] Print backward\n");
    printf("[5] Show node size\n");
    printf("[0] Exit\n");
    printf("Choose: ");
}

int main() {
    struct Node* head = NULL;
    struct Node* tail = NULL;

    int choice, value;
    int node_count = 0;

    while (1) {
        print_menu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:{
                char input[100];
                printf("Enter a number or a single character to add: ");
                scanf(" %99s", input);  // Read input as string

                int data = 0;
                char character = '\0';

                // Try parsing input as an integer
                if (sscanf(input, "%d", &data) == 1) {
                    // It's a number; leave character = '\0'
                } else if (strlen(input) == 1) {
                    // It's a single character
                    character = input[0];
                } else {
                    printf("❌ Invalid input. Please enter a number or a single character.\n");
                    break;
                }

                add_node(&head, &tail, data, character);
                break;
                }
            case 2:
                remove_node(&head, &tail);
                break;
            case 3:
                print_forward(head);
                break;
            case 4:
                print_backward(tail);
                break;
            case 5:
                printf("Size of Node: %zu bytes\n", sizeof(struct Node));
                break;
            case 0:
                // Free all remaining nodes
                while (head) {
                    struct Node* temp = head;
                    head = head->next;
                    free(temp);
                }
                printf("Exiting.\n");
                return 0;
            default:
                printf("Invalid choice.\n");
        }
    }

    return 0;
}
