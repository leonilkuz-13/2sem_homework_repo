#pragma once
#include <stdio.h>

typedef struct nodeHeap {
    int dist; // расстояние
    int city; // номер города или как он будет интерпретирован в гос-ве
} nodeHeap;

typedef struct Heap {
    size_t capacity;
    size_t len;
    nodeHeap* array; // массив
} Heap;

Heap* initHeap();
void pushHeap(Heap* heap, int dist, int city);
nodeHeap popHeap(Heap* heap);
void cleanHeap(Heap** heap);
