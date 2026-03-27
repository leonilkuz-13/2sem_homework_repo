#pragma once
#include <stdbool.h>

typedef struct Node {
    char* code;
    char* fullName;
    struct Node* left;
    struct Node* right;
    int balanced;
} Node;

typedef struct {
    Node* root;
    int size;
} Avl;

Avl* initTree();
Node* initNode(const char* code, const char* fullName);
void freeNode(Node* node);
void avlFree(Avl** treeRef);
Node* add(Node* root, const char* code, const char* fullName, bool* error);
Node* find(Avl* tree, const char* code);
Node* delete(Node* root, const char* code, bool* error);