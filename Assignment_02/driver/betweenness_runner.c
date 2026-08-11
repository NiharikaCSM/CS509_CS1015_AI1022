#include "betweenness_runner.h"
#include "graph_helper.h"
#include "csr_conversion.h"
#include "betweenness.h"
#include "wrapper.h"
#include <stdio.h>
#include <stdlib.h>

void runBetweennessCentralityFromFile(const char *path) {
    FILE *file = fopen(path, "r");
    if (!file) {
        printf("Input file not found.");
        exit(EXIT_FAILURE);
    }

    CSRGraph graph = readAdjacencyListAsCSR(file, 0); //input is unweighted graph
    fclose(file);

    sortNeighborLists(&graph);
    validateUndirectedSymmetry(&graph);

    double startTime = getExecutionTime();
    BetweennessCentralityResult result = runBetweennessCentrality(&graph);
    double endTime = getExecutionTime();

    printBetweennessCentralityResult(&result);
    printExecutionTime(startTime, endTime);

    freeBetweennessCentralityResult(&result);
    free(graph.rowPtr);
    free(graph.colIndex);
    free(graph.edgeWeights);
}