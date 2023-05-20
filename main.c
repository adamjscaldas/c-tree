#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEY_LENGTH 200
#define LOCATION_LENGTH sizeof(long long)

typedef struct Node {
    char key[KEY_LENGTH];
    struct Node *parent;  // Points to the Node from which it came from
    struct Node *next;  // Points to a Node that came from the same parent
    struct Node *prev;
    struct Node *children;  // Points to a Node that derives from it
} Node;

typedef struct Tree {
    struct Node *root;
} Tree;

Tree *create_tree() {
    Tree *tree = (Tree *) malloc(sizeof(Tree));
    if (tree != NULL) {
        tree->root = (Node *) malloc(sizeof(Node));
        strcpy(tree->root->key, "root");
        tree->root->parent = NULL;
        tree->root->next = NULL;
        tree->root->prev = NULL;
        tree->root->children = NULL;
        return tree;
    } else {
        printf("Not enough memory to initialize the tree!\n");
        return NULL;
    }
}

void print_tree(Node *node, int indent) {
    printf("+");
    for (int i = 0; i < indent; i++) {
        printf("-");
    }
    printf("Node: %p | Key: %s | Next: %p | Prev: %p | Parent: %p | Children: %p\n", node, node->key,
           node->next, node->prev, node->parent, node->children);
    Node *current_child = node->children;
    while (current_child != NULL) {
        print_tree(current_child, indent + 1);
        current_child = current_child->next;
    }
}

Node *search_element(Tree *tree, char location[LOCATION_LENGTH]) {
    // Location example: "A-1-5-18-3"
    Node *current_node = tree->root;
    char *token = strtok(location, "-");
    char *location_filtered[LOCATION_LENGTH];
    int i = 0;  // added initialization for i
    while (token != NULL) {
        location_filtered[i++] = token;
        token = strtok(NULL, "-");
    }
    int location_len = i;

    for (i = 0; i < location_len; i++) {
        if (strcmp(location_filtered[i], current_node->key) == 0 && i == (location_len - 1)) {
            return current_node;
        } else {
            Node *temp_node = current_node;
            while (temp_node != NULL && strcmp(location_filtered[i], temp_node->key) != 0) {
                temp_node = temp_node->next;
            }
            if (temp_node == NULL) {
                return NULL;
            } else {
                if (temp_node->children == NULL) { current_node = temp_node; }
                else { current_node = temp_node->children; }
            }
        }
    }
    return current_node;
}

Node *create_node() {
    Node *new_node = (Node *) malloc(sizeof(Node));
    if (new_node != NULL) {
        printf("New node key: ");
        scanf("%[^\n]", new_node->key);
        fflush(stdin);
        new_node->parent = NULL;
        new_node->next = NULL;
        new_node->prev = NULL;
        new_node->children = NULL;
        return new_node;
    } else {
        printf("Not enough memory to create a new node!\n");
        return NULL;
    }
}

void append_children(Node *node, Node *node_append) {
    Node *parent;
    if (node->children != NULL) {
        node = node->children;
        parent = node->parent;
        while (node->next != NULL) {
            node = node->next;
        }
        node->next = node_append;
    } else {
        parent = node;
        node->children = node_append;
    }
    node_append->parent = parent;
}

int add_node(Tree *tree, Node *node, char location[LOCATION_LENGTH]) {
    Node *temp = search_element(tree, location);
    if (temp == NULL) {
        printf("Node not found in the specified location.\n");
        return 1;
    } else {
        append_children(temp, node);
        return 0;
    }
}

void choice_setter(int *choice) {
    while (scanf("%d", choice) != 1) {
        printf("Invalid value, please insert a valid value to proceed: ");
        scanf("%*[^\n]");
        fflush(stdin);
    }
    fflush(stdin);
}

void manual_new_node(Tree *tree, char location[LOCATION_LENGTH]) {
    Node *node = create_node();
    printf("Location: ");
    scanf("%[^\n]", location);
    fflush(stdin);
    add_node(tree, node, location);
}

int main() {
    Tree *tree = create_tree();
    int choice;

    char location[LOCATION_LENGTH];
    while (1) {
        printf("0. Break | 1. Add element to tree | 2. Print tree\n");
        choice_setter(&choice);
        switch (choice) {
            case 0:
                printf("Press any button to close the program.");
                getchar();
                return 0;
            case 1:
                manual_new_node(tree, location);
                continue;
            case 2:
                print_tree(tree->root, 0);
                continue;
            default:
                printf("Insert a valid value: ");
        }
    }
}
