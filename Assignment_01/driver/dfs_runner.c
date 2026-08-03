#include <stdio.h>
#include "dfs_runner.h"
#include "wrapper.h"
#include "csr_conversion.h"
#include "dfs.h"

int runDFSFromFile(char *filePath) {
    FILE *inputFile = openFile(filePath);

    CSRGraph graph = readAdjacencyListAsCSR(inputFile, 0);   // 0 = unweighted
    int source = readSourceVertex(inputFile);
    fclose(inputFile);

    printf("Algorithm: DFS\n");
    printf("Source: %d\n", source);

    double startTime = getExecutionTime();
    DFSResult result = runDFS(&graph, source);
    double endTime = getExecutionTime();

    printf("Traversal: ");
    for (int i = 0; i < result.traversalCount; i++)
        printf("%d ", result.traversalOrder[i]);
    printf("\n");

    printExecutionTime(startTime, endTime);

    freeDFSResult(&result);
    freeCSRGraph(&graph);
    return 0;
}