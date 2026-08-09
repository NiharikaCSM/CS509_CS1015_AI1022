#include "graph_helper.h"
#include "csr_conversion.h"
#include <stdio.h>
#include <stdlib.h>

//compare function for qsort to sort integers in ascending order
static int compareInts(const void *a, const void *b) {
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    return (ia > ib) - (ia < ib);
}

void sortNeighborLists(CSRGraph *graph) {
    for (int v = 0; v < graph->vertices; v++) {
        int start = graph->rowPtr[v];
        int end = graph->rowPtr[v + 1];
        int len = end - start;
        if (len > 1) {
            //sorting neighbor list of vertex v in ascending order using qsort
            qsort(&graph->colIndex[start], len, sizeof(int), compareInts);
        }
    }
}

//binary search for whether directed edge exists in the CSR structure
int hasEdge(CSRGraph *graph, int u, int v) {
    int low = graph->rowPtr[u];
    int high = graph->rowPtr[u + 1] - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        int value = graph->colIndex[mid];
        if (value == v) return 1;
        if (value < v) low = mid + 1;
        else high = mid - 1;
    }
    return 0;
}

//checks that the input file actually describes a proper undirected graph i.e. every edge is listed on both ends
void validateUndirectedSymmetry(CSRGraph *graph) {
    for (int u = 0; u < graph->vertices; u++) {
        int start = graph->rowPtr[u];
        int end = graph->rowPtr[u + 1];
        for (int e = start; e < end; e++) {
            int v = graph->colIndex[e];
            if (!hasEdge(graph, v, u)) {
                fprintf(stderr,
                    "Error: Invalid input file format - edge (%d, %d) is not "
                    "symmetric: vertex %d's adjacency list does not contain "
                    "vertex %d, but the graph must be undirected.\n",
                    u, v, v, u);
                exit(1);
            }
        }
    }
}