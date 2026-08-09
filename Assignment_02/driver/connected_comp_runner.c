#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "wrapper.h"
#include "graph_helper.h"
#include "csr_conversion.h"
#include "connected_comp_runner.h"
#include "connected_comp.h"

void runConnectedComponentsFromFile(const char *path) {
    FILE *inputFile = fopen(path, "r");
    if (!inputFile) {
        printf("Input file not found.");
        exit(EXIT_FAILURE);
    }

    CSRGraph graph = readAdjacencyListAsCSR(inputFile, 0); // unweighted graph
    fclose(inputFile);

    validateUndirectedSymmetry(&graph);

    double startTime = getExecutionTime();
    ConnectedComponentsResult result = runConnectedComponents(&graph);
    double endTime = getExecutionTime();

    printConnectedComponentsResult(&result);
    printExecutionTime(startTime, endTime);

    freeConnectedComponentsResult(&result);
    free(graph.rowPtr);
    free(graph.colIndex);
}
