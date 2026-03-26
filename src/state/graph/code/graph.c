#include "graph.h"
#include <stdlib.h>

Edge* initEdge(size_t dest, size_t weight)
{
    Edge* node = malloc(sizeof(Edge));
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
    Graph* graph = malloc(sizeof(Graph));
    if (graph == NULL) {
        return NULL;
    }

    graph->lists = calloc(cntVertex, sizeof(Edge*));
    if (graph->lists == NULL) {
        free(graph);
        return NULL;
    }

    graph->numVertex = calloc(cntVertex, sizeof(size_t));
    if (graph->numVertex == NULL) {
        free(graph->lists);
        free(graph);
        return NULL;
    }
    graph->cntVertex = cntVertex;
    return graph;
}

void addEdge(Graph* graph, size_t src, size_t dest, size_t weight) // граф неоринтерованный
{
    Edge* newEdge = initEdge(dest, weight);
    if (newEdge == NULL) {
        return;
    }
    newEdge->next = graph->lists[src];
    graph->lists[src] = newEdge;

    Edge* reverseEdge = initEdge(src, weight);
    if (reverseEdge == NULL) {
        graph->lists[src] = newEdge->next;
        free(newEdge);
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
            free(temp);
        }
    }
    free((*graph)->lists);
    free((*graph)->numVertex);
    free(*graph);
    *graph = NULL;
}
