#include <stdio.h>
#include <stdlib.h>
#include "connected_comp.h"

ConnectedComponentsResult runConnectedComponents(const CSRGraph *graph) {
    int V = graph->vertices;
    int *rowPtr = graph->rowPtr;
    int *colIndex = graph->colIndex;

    int *component = malloc(V * sizeof(int));
    for (int i = 0; i < V; i++) component[i] = -1;

    int *queue = malloc(V * sizeof(int));
    int numComponents = 0;

    for (int start = 0; start < V; start++) {
        if (component[start] != -1) continue;

        int head = 0, tail = 0;
        queue[tail++] = start;
        component[start] = numComponents;

        while (head < tail) {
            int u = queue[head++];
            for (int e = rowPtr[u]; e < rowPtr[u + 1]; e++) {
                int v = colIndex[e];
                if (component[v] == -1) {
                    component[v] = numComponents;
                    queue[tail++] = v;
                }
            }
        }
        numComponents++;
    }

    free(queue);

    ConnectedComponentsResult result;
    result.numVertices = V;
    result.numComponents = numComponents;
    result.component = component;
    return result;
}

void printConnectedComponentsResult(const ConnectedComponentsResult *result) {
    printf("Algorithm: Connected Components\n");
    printf("Number of components: %d\n", result->numComponents);
    printf("Vertex Component\n");
    for (int i = 0; i < result->numVertices; i++) {
        printf("%d %d\n", i, result->component[i]);
    }
}

void freeConnectedComponentsResult(ConnectedComponentsResult *result) {
    free(result->component);
    result->component = NULL;
}