#pragma once
#include <stdbool.h>

typedef struct Node {
    char code[4];
    char fullName[128];
    struct Node* left;
    struct Node* right;
    int balanced;
} Node;

typedef struct {
    Node* root;
    int size;
} AVL;

AVL* initTree();
Node* add(Node* root, const char* code, const char* fullname, bool* error);
void avlFree(AVL** treeRef);
bool find(AVL* tree, const char* code);
Node* delete(Node* root, const char* code, bool* error);
