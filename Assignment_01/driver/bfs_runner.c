#include <stdio.h>
#include "bfs_runner.h"
#include "wrapper.h"
#include "csr_conversion.h"
#include "bfs.h"

int runBFSFromFile(char *filePath) {
    FILE *inputFile = openFile(filePath);

    CSRGraph graph = readAdjacencyListAsCSR(inputFile, 0);   // bfs is unweighted 
    int source = readSourceVertex(inputFile);
    fclose(inputFile);

    printf("Algorithm: BFS\n");
    printf("Source: %d\n", source);

    double startTime = getExecutionTime();
    BFSResult result = runBFS(&graph, source);
    double endTime = getExecutionTime();

    printf("Traversal: ");
    for (int i = 0; i < result.traversalCount; i++)
        printf("%d ", result.traversalOrder[i]);
    printf("\n");

    printf("Distances:\n");
    for (int v = 0; v < graph.vertices; v++) {
        if (result.distances[v] == -1)
            printf("%d INF\n", v);
        else
            printf("%d %d\n", v, result.distances[v]);
    }

    printExecutionTime(startTime, endTime);

    freeBFSResult(&result);
    freeCSRGraph(&graph);
    return 0;
}
