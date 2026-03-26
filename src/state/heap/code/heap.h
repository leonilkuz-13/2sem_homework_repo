#pragma once
#include <stdio.h>

typedef struct NodeHeap {
    int dist; // расстояние от связующего города
    int city; // номер города или как он будет интерпретирован в гос-ве
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
