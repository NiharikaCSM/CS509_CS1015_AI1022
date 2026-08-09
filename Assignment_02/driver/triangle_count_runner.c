#include "triangle_count_runner.h"
#include "csr_conversion.h"
#include "wrapper.h"
#include "triangle_count.h"
#include <stdio.h>
#include <stdlib.h>

#define TC_LISTING_VERTEX_THRESHOLD 100

void runTriangleCountingFromFile(const char *path) {
    FILE *inputFile = fopen(path, "r");
    if (!inputFile) {
        printf("Input file not found.");
        exit(EXIT_FAILURE);
    }

    CSRGraph graph = readAdjacencyListAsCSR(inputFile,  0); //input is unweighted graph
    fclose(inputFile);

    sortNeighborLists(&graph);
    validateUndirectedSymmetry(&graph);

    int printTriangles = (graph.vertices <= TC_LISTING_VERTEX_THRESHOLD);

    double startTime = getExecutionTime();
    TriangleCountingResult result = runTriangleCounting(&graph, printTriangles);
    double endTime = getExecutionTime();

    printTriangleCountingResult(&result);
    printExecutionTime(startTime, endTime);

    freeTriangleCountingResult(&result);
    free(graph.rowPtr);
    free(graph.colIndex);
}
