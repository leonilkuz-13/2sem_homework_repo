#include "graph.h"
#include <stdbool.h>
#include <stdlib.h>

Edge* initEdge(size_t dest, size_t weight)
{
    Edge* node = (Edge*)malloc(sizeof(Edge));
    if (node == NULL) {
        return NULL;
    }

    node->dest = dest;
    node->weight = weight;
    node->next = NULL;
    return node;
}

Graph* initGraph(size_t cntVertex) // количество городов передается в файле изнчально
{
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    if (graph == NULL) {
        return NULL;
    }

    graph->lists = (Edge**)calloc(cntVertex, sizeof(Edge*));
    if (graph->lists == NULL) {
        free((void*)graph);
        return NULL;
    }

    graph->numVertex = (size_t*)calloc(cntVertex, sizeof(size_t));
    if (graph->numVertex == NULL) {
        free((void*)graph->lists);
        free((void*)graph);
        return NULL;
    }
    graph->cntVertex = cntVertex;
    return graph;
}

void addEdge(Graph* graph, size_t src, size_t dest, size_t weight,
             bool* err) // граф неоринтерованный
{
    Edge* newEdge = initEdge(dest, weight);
    if (newEdge == NULL) {
        *err = true;
        return;
    }
    newEdge->next = graph->lists[src];
    graph->lists[src] = newEdge;

    Edge* reverseEdge = initEdge(src, weight);
    if (reverseEdge == NULL) {
        *err = true;
        return;
    }
    reverseEdge->next = graph->lists[dest];
    graph->lists[dest] = reverseEdge;
}

void cleanGraph(Graph** graph)
{
    if (graph == NULL || *graph == NULL) {
        return;
    }
    for (size_t index = 0; index < (*graph)->cntVertex; index++) {
        Edge* current = (*graph)->lists[index];
        while (current != NULL) {
            Edge* temp = current;
            current = current->next;
            free((void*)temp);
        }
    }
    free((void*)(*graph)->lists);
    free((void*)(*graph)->numVertex);
    free((void*)*graph);
    *graph = NULL;
}