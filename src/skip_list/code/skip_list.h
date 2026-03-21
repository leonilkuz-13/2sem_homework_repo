#pragma once

typedef struct Node {
    int key;                 // значение
    struct Node** levelNode; // массив указателей на одном уровне (для итерации по одному уровню)
    int level;
} Node;

typedef struct List {
    Node* tail;
    Node* head;
    size_t len;
    int maxLevel;
} List;

Node* initNode();
List* intiList();
Node* find(List* list, int value);
void insert(List* list, int value);
void delete(List* list, int value);
