#pragma once
#include <stdio.h>

typedef struct NodeHeap {
    int dist; // ребро до государства
    int city; // номер города
} NodeHeap;

typedef struct Heap {
    size_t capacity;
    size_t len;
    NodeHeap* array; // массив
} Heap;

Heap* initHeap();
void pushHeap(Heap* heap, int dist, int city);
NodeHeap popHeap(Heap* heap);
void cleanHeap(Heap** heap);
