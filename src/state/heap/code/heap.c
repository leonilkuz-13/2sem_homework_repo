#include "heap.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

Heap* initHeap()
{
    Heap* heap = malloc(sizeof(Heap));
    if (heap == NULL) {
        return NULL;
    }

    heap->capacity = 32;
    heap->array = malloc(sizeof(NodeHeap) * heap->capacity);
    if (heap->array == NULL) {
        free(heap);
        return NULL;
    }
    heap->len = 0;
    return heap;
}

static void siftDown(Heap* heap, int index)
{
    if (heap == NULL) {
        return;
    }

    while (1) {
        int left = 2 * index + 1;
        int right = left + 1;
        int small = index;
        if (left < (int)heap->len && heap->array[left].dist < heap->array[small].dist) {
            small = left;
        }
        if (right < (int)heap->len && heap->array[right].dist < heap->array[small].dist) {
            small = right;
        }
        if (small == index) {
            break;
        }

        NodeHeap temp = heap->array[index];
        heap->array[index] = heap->array[small];
        heap->array[small] = temp;
        index = small;
    }
}

static void siftUp(Heap* heap, int index)
{
    if (heap == NULL) {
        return;
    }

    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap->array[parent].dist <= heap->array[index].dist) {
            break;
        }

        NodeHeap temp = heap->array[parent];
        heap->array[parent] = heap->array[index];
        heap->array[index] = temp;
        index = parent;
    }
}

void pushHeap(Heap* heap, int dist, int city)
{
    if (heap == NULL) {
        return;
    }

    if (heap->len == heap->capacity) {
        size_t newCap = heap->capacity * 2;
        NodeHeap* newArr = realloc(heap->array, sizeof(NodeHeap) * newCap);
        if (newArr == NULL) {
            return;
        }
        heap->array = newArr;
        heap->capacity = newCap;
    }
    heap->array[heap->len].dist = dist;
    heap->array[heap->len].city = city;
    heap->len++;
    siftUp(heap, (int)(heap->len - 1));
}

NodeHeap popHeap(Heap* heap)
{
    NodeHeap empty = { 0, 0 };
    if (heap == NULL || heap->len == 0) {
        return empty;
    }

    NodeHeap nodeMin = heap->array[0];
    heap->array[0] = heap->array[heap->len - 1];
    heap->len--;
    siftDown(heap, 0);
    return nodeMin;
}

void cleanHeap(Heap** heap)
{
    if (heap == NULL || *heap == NULL) {
        return;
    }

    free((*heap)->array);
    free(*heap);
    *heap = NULL;
}
