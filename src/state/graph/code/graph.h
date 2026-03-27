#pragma once
#include <stdio.h>
#include <stdbool.h>

// я считаю, что граф неоритированный -- поддерживаю связи в обе стороны

typedef struct Edge {
    size_t dest; // куда направлено ребро
    size_t weight;
    struct Edge* next;
} Edge;

typedef struct Graph {
    size_t* numVertex; // номер вершины
    size_t cntVertex;
    Edge** lists; // все ребра от нее
} Graph;

Edge* initEdge(size_t dest, size_t weight);
Graph* initGraph(size_t cntVertex);
void addEdge(Graph* graph, size_t src, size_t dest, size_t weight);
void cleanGraph(Graph** graph);
