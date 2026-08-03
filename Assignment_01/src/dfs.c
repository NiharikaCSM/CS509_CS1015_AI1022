#include <stdio.h>
#include <stdlib.h>
#include "csr_conversion.h"
#include "dfs.h"

DFSResult runDFS(CSRGraph *graph, int source) {
    DFSResult result;
    result.traversalOrder = malloc(graph->vertices * sizeof(int));
    result.traversalCount = 0;

    int *visited = calloc(graph->vertices, sizeof(int));

    //stack capacity is the number of edges in the graph
    int stackCapacity = graph->rowPtr[graph->vertices] + 1;
    int *stack = malloc(stackCapacity * sizeof(int));
    int stackTop = -1;

    stack[++stackTop] = source;

    while (stackTop >= 0) {
        int current = stack[stackTop--];

        if (visited[current])
            continue;   // may have been pushed more than once before being visited

        visited[current] = 1;
        result.traversalOrder[result.traversalCount++] = current;

        int start = graph->rowPtr[current];
        int end = graph->rowPtr[current + 1];

        // Push neighbors in reverse order, so the smallest-index neighbor
        // ends up on top of the stack and gets visited first
        for (int i = end - 1; i >= start; i--) {
            int neighbor = graph->colIndex[i];
            if (!visited[neighbor])
                stack[++stackTop] = neighbor;
        }
    }

    free(visited);
    free(stack);
    return result;
}

void freeDFSResult(DFSResult *result) {
    free(result->traversalOrder);
}