#include <stdio.h>
#include <stdlib.h>
#include "bfs.h"

BFSResult runBFS(CSRGraph *graph, int source) {
    BFSResult result;
    result.traversalOrder = malloc(graph->vertices * sizeof(int));
    result.traversalCount = 0;
    result.distances = malloc(graph->vertices * sizeof(int));

    int *visited = calloc(graph->vertices, sizeof(int));
    int *queue = malloc(graph->vertices * sizeof(int));

    //insert from queue back and read from queue front
    int queueFront = 0, queueBack = 0;

    //sets the initial distance of all vertices in the graph to -1.
    for (int v = 0; v < graph->vertices; v++)
        result.distances[v] = -1;   // -1 marks "not yet reached"

    // Start from the source
    visited[source] = 1;
    result.distances[source] = 0;
    queue[queueBack++] = source;

    while (queueFront < queueBack) {
        int current = queue[queueFront++];
        result.traversalOrder[result.traversalCount++] = current;

        int start = graph->rowPtr[current];
        int end = graph->rowPtr[current + 1];

        for (int i = start; i < end; i++) {
            int neighbor = graph->colIndex[i];
            if (!visited[neighbor]) {
                visited[neighbor] = 1;
                result.distances[neighbor] = result.distances[current] + 1;
                queue[queueBack++] = neighbor;
            }
        }
    }

    free(visited);
    free(queue);
    return result;
}

void freeBFSResult(BFSResult *result) {
    free(result->traversalOrder);
    free(result->distances);
}