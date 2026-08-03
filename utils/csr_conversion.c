#include <stdio.h>
#include <stdlib.h>
#include "csr_conversion.h"

CSRGraph readAdjacencyListAsCSR(FILE *filename, int isWeighted) {
    CSRGraph graph;

    fscanf(filename, "%d %d", &graph.vertices, &graph.edges);

    int maxPossibleEntries = 2 * graph.edges;

    graph.rowPtr = malloc((graph.vertices + 1) * sizeof(int));
    graph.colIndex = malloc(maxPossibleEntries * sizeof(int));
    graph.edgeWeights  = isWeighted ? malloc(maxPossibleEntries * sizeof(int)) : NULL;

    graph.rowPtr[0] = 0;
    int nextFreeSlot = 0;   // tracks where to write the next neighbor in col_idx

    for (int v = 0; v < graph.vertices; v++) {
        int vertexId, degree;
        fscanf(filename, "%d %d", &vertexId, &degree);

        //add the neighbors of vertex v to the colIndex array
        for (int i = 0; i < degree; i++) {
            fscanf(filename, "%d", &graph.colIndex[nextFreeSlot]);
            if (isWeighted)
                fscanf(filename, "%d", &graph.edgeWeights[nextFreeSlot]);
            nextFreeSlot++;
        }

        //next vertex's section starts wherever we just finished writing
        graph.rowPtr[v + 1] = nextFreeSlot;
    }

    return graph;
}

int readSourceVertex(FILE *filename) {
    char keyword[16];
    int source;
    fscanf(filename, "%s %d", keyword, &source);
    return source;
}

void freeCSRGraph(CSRGraph *graph) {
    free(graph->rowPtr);
    free(graph->colIndex);
    if (graph->edgeWeights != NULL)
        free(graph->edgeWeights);
}

void printCSRGraph(CSRGraph *graph) {
    printf("Number of vertices: %d\n", graph->vertices);
    printf("Number of edges: %d\n", graph->edges);

    printf("row_ptr: ");
    for (int i = 0; i <= graph->vertices; i++)
        printf("%d ", graph->rowPtr[i]);
    printf("\n");

    printf("col_idx: ");
    for (int i = 0; i < graph->rowPtr[graph->vertices]; i++)
        printf("%d ", graph->colIndex[i]);
    printf("\n");

    if (graph->edgeWeights != NULL) {
        printf("edgeWeights:  ");
        for (int i = 0; i < graph->rowPtr[graph->vertices]; i++)
            printf("%d ", graph->edgeWeights[i]);
        printf("\n");
    }
}