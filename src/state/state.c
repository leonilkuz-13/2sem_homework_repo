#include "graph.h"
#include "heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main(int argc, char* argv[])
{
    if (argc < 2) {
        return -1;
    }

    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Ошибка: не удалось открыть файл %s\n", argv[1]);
        return -1;
    }

    size_t cntVertex;
    size_t cntEdge;
    if (fscanf(file, "%zu %zu", &cntVertex, &cntEdge) != 2) {
        fclose(file);
        return -1;
    }

    int* visited = malloc(sizeof(int) * cntVertex);
    if (visited == NULL) {
        fclose(file);
        return -1;
    }
    for (size_t index = 0; index < cntVertex; index++) {
        visited[index] = -1;
    }

    Graph* graph = initGraph(cntVertex);
    if (graph == NULL) {
        free(visited);
        fclose(file);
        return -1;
    }

    bool err = false;
    for (size_t index = 0; index < cntEdge; index++) {
        size_t firstCity;
        size_t secondCity;
        size_t weight;
        fscanf(file, "%zu %zu %zu", &firstCity, &secondCity, &weight);
        addEdge(graph, firstCity - 1, secondCity - 1, weight, &err);
        if (err == true) {
            cleanGraph(&graph);
            free(visited);
            fclose(file);
            return -1;
        }
    }

    size_t cntCapital;
    fscanf(file, "%zu", &cntCapital);

    size_t** states = malloc(sizeof(size_t*) * (cntCapital));
    size_t* stateSizes = calloc(cntCapital, sizeof(size_t));
    if (states == NULL || stateSizes == NULL) {
        free(states);
        free(stateSizes);
        cleanGraph(&graph);
        free(visited);
        fclose(file);
        return -1;
    }

    for (size_t index = 0; index < cntCapital; index++) {
        states[index] = malloc(sizeof(size_t) * cntVertex);
        if (states[index] == NULL) {
            for (size_t ind = 0; ind < index; ind++)
                free(states[ind]);
            free(states);
            free(stateSizes);
            cleanGraph(&graph);
            free(visited);
            fclose(file);
            return -1;
        }
    }

    Heap** heapsState = malloc(sizeof(Heap*) * cntCapital);
    for (size_t index = 0; index < cntCapital; index++) {
        heapsState[index] = initHeap();
    }

    // посчитал столицы отдельным циклом
    for (size_t index = 0; index < cntCapital; index++) {
        size_t cap;
        fscanf(file, "%zu", &cap);
        size_t capIdx = cap - 1;

        visited[capIdx] = (int)index;
        states[index][stateSizes[index]++] = cap;

        Edge* curr = graph->lists[capIdx];
        while (curr != NULL) {
            if (visited[curr->dest] == -1) {
                pushHeap(heapsState[index], (int)curr->weight, (int)curr->dest);
            }
            curr = curr->next;
        }
    }

    size_t count = cntCapital;
    while (count < cntVertex) {
        bool changed = false;
        for (size_t index = 0; index < cntCapital; index++) {
            while (heapsState[index]->len > 0) {
                NodeHeap minNode = popHeap(heapsState[index]);
                size_t city = minNode.city;

                if (visited[city] != -1) {
                    continue;
                }

                visited[city] = (int)index;
                states[index][stateSizes[index]++] = city + 1;
                count++;
                changed = true;

                Edge* child = graph->lists[city];
                while (child != NULL) {
                    if (visited[child->dest] == -1) {
                        pushHeap(heapsState[index], (int)child->weight, (int)child->dest);
                    }
                    child = child->next;
                }
                break;
            }
        }
        if (changed == false) {
            break;
        }
    }

    for (size_t index = 0; index < cntCapital; index++) {
        printf("%zu: ", index + 1);
        for (size_t ind = 0; ind < stateSizes[index]; ind++) {
            printf("%zu ", states[index][ind]);
        }
        puts("");
    }

    for (size_t index = 0; index < cntCapital; index++) {
        free(states[index]);
        cleanHeap(&heapsState[index]);
    }
    free(heapsState);
    free(states);
    free(stateSizes);
    free(visited);
    cleanGraph(&graph);
    fclose(file);

    return 0;
}