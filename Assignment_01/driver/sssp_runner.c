#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sssp_runner.h"
#include "csr_conversion.h"
#include "wrapper.h"
#include "sssp.h"

int runSSSPFromFile(char *path) {
    FILE *fp = fopen(path, "r");
    if (!fp) {
        fprintf(stderr, "Error: could not open file '%s'\n", path);
        return -1;
    }

    /* ---- setup: adjacency-list read + CSR conversion -- NOT timed ---- */
    CSRGraph graph = readAdjacencyListAsCSR(fp, 1 /* isWeighted */);
    int source = readSourceVertex(fp);
    fclose(fp);

    int *dist = (int *)malloc(graph.vertices * sizeof(int));

    /* ---- timed region: Dijkstra only ---- */
    double startTime = getExecutionTime();
    sssp_dijkstra(&graph, source, dist);
    double endTime = getExecutionTime();

    /* ---- output -- NOT timed ---- */
    printf("Algorithm: SSSP\n");
    printf("Source: %d\n", source);
    printf("Vertex\tDistance\n");
    for (int i = 0; i < graph.vertices; i++) {
        if (dist[i] == SSSP_INF)
            printf("%d\tINF\n", i);
        else
            printf("%d\t%d\n", i, dist[i]);
    }

    printExecutionTime(startTime, endTime);

    free(dist);
    freeCSRGraph(&graph);
    return 0;
}