#include "../code/graph.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int testsPassed = 0;
int testsFailed = 0;

void checkInt(const char* testName, int expected, int actual)
{
    if (expected != actual) {
        fprintf(stderr, "FAILED: %s | expected=%d actual=%d\n", testName, expected, actual);
        testsFailed++;
    } else {
        testsPassed++;
    }
}

void checkPtrNotNull(const char* testName, void* ptr)
{
    if (ptr == NULL) {
        fprintf(stderr, "FAILED: %s | pointer is NULL\n", testName);
        testsFailed++;
    } else {
        testsPassed++;
    }
}

void checkPtrNull(const char* testName, void* ptr)
{
    if (ptr != NULL) {
        fprintf(stderr, "FAILED: %s | pointer is NOT NULL\n", testName);
        testsFailed++;
    } else {
        testsPassed++;
    }
}

void testInitGraph()
{
    size_t vertices = 5;
    Graph* graph = initGraph(vertices);
    checkPtrNotNull("initGraph", graph);
    if (graph != NULL) {
        checkInt("initGraph cntVertex", (int)vertices, (int)graph->cntVertex);
        checkPtrNotNull("initGraph lists array", graph->lists);
        checkPtrNotNull("initGraph numVertex array", graph->numVertex);
        checkPtrNull("initGraph first list is null", graph->lists[0]);
        cleanGraph(&graph);
        checkPtrNull("cleanGraph pointer null", graph);
    }
}

void testAddEdgeSymmetry()
{
    Graph* graph = initGraph(2);
    for (size_t i = 0; i < graph->cntVertex; i++) {
        graph->lists[i] = NULL;
    }

    addEdge(graph, 0, 1, 10);

    checkPtrNotNull("edge 0->1 exists", graph->lists[0]);
    if (graph->lists[0] != NULL) {
        checkInt("edge 0->1 destination", 1, (int)graph->lists[0]->dest);
        checkInt("edge 0->1 weight", 10, (int)graph->lists[0]->weight);
    }

    checkPtrNotNull("edge 1->0 exists (symmetry)", graph->lists[1]);
    if (graph->lists[1] != NULL) {
        checkInt("edge 1->0 destination", 0, (int)graph->lists[1]->dest);
        checkInt("edge 1->0 weight", 10, (int)graph->lists[1]->weight);
    }

    cleanGraph(&graph);
}

void testMultipleEdges()
{
    Graph* graph = initGraph(3);
    for (size_t i = 0; i < graph->cntVertex; i++) {
        graph->lists[i] = NULL;
    }

    addEdge(graph, 0, 1, 5);
    addEdge(graph, 0, 2, 15);

    int edgeCount = 0;
    Edge* current = graph->lists[0];
    while (current != NULL) {
        edgeCount++;
        current = current->next;
    }

    checkInt("multiple edges count at vertex 0", 2, edgeCount);
    cleanGraph(&graph);
}

void testCleanGraphTwice()
{
    Graph* graph = initGraph(2);
    for (size_t i = 0; i < graph->cntVertex; i++) {
        graph->lists[i] = NULL;
    }
    cleanGraph(&graph);
    cleanGraph(&graph);
    checkPtrNull("cleanGraph twice", graph);
}

int runTests()
{
    testInitGraph();
    testAddEdgeSymmetry();
    testMultipleEdges();
    testCleanGraphTwice();

    printf("\n--- Graph Testing Results ---\n");
    printf("Tests passed: %d\n", testsPassed);
    printf("Tests failed: %d\n", testsFailed);

    return (testsFailed == 0) ? 0 : 1;
}

int main()
{
    return runTests();
}