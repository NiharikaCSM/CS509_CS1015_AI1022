#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>

#include "csr_conversion.h"
#include "maxflow_mincut.h"
#include "maxflow_mincut_runner.h"

/* Reads the "SINK t" line that terminates a Maxflow-Mincut input file. */
static int readSinkVertex(FILE *fp) {
    char keyword[16];
    int sink;

    if (fscanf(fp, "%15s %d", keyword, &sink) != 2) {
        fprintf(stderr, "Error: Invalid input file format - expected a 'SINK t' line at the end of the file\n");
        exit(1);
    }
    if (strcasecmp(keyword, "sink") != 0) {
        fprintf(stderr, "Error: Invalid input file format - expected the keyword 'SINK' before the sink vertex id\n");
        exit(1);
    }
    return sink;
}

static double elapsedMs(clock_t start, clock_t end) {
    return ((double)(end - start) * 1000.0) / CLOCKS_PER_SEC;
}

void runMaxflowMincutFromFile(const char *path) {
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: could not open input file '%s'\n", path);
        return;
    }


    CSRGraph graph = readAdjacencyListAsCSR(fp, 1);
    int source = readSourceVertex(fp);
    int sink   = readSinkVertex(fp);
    fclose(fp);

   
    if (source < 0 || source >= graph.vertices) {
        fprintf(stderr, "Error: source vertex %d is out of range [0, %d]\n", source, graph.vertices - 1);
        freeCSRGraph(&graph);
        return;
    }
    if (sink < 0 || sink >= graph.vertices) {
        fprintf(stderr, "Error: sink vertex %d is out of range [0, %d]\n", sink, graph.vertices - 1);
        freeCSRGraph(&graph);
        return;
    }
    if (source == sink) {
        fprintf(stderr, "Error: source and sink must be different vertices (got %d for both)\n", source);
        freeCSRGraph(&graph);
        return;
    }

    
    clock_t start = clock();

    MaxflowResult result = dinicMaxflow(&graph, source, sink);

    clock_t end = clock();
    double timeMs = elapsedMs(start, end);

   
    printf("Algorithm: Maxflow-Mincut\n");
    printf("Source: %d\n", source);
    printf("Sink: %d\n", sink);
    printf("Maximum flow: %lld\n", result.maxFlow);
    printf("Minimum cut capacity: %lld\n", result.maxFlow);

    printf("Source side:");
    for (int i = 0; i < result.sourceSideCount; i++)
        printf(" %d", result.sourceSide[i]);
    printf("\n");

    printf("Sink side:");
    for (int i = 0; i < result.sinkSideCount; i++)
        printf(" %d", result.sinkSide[i]);
    printf("\n");

    printf("Cut edges:\n");
    for (int i = 0; i < result.cutEdgeCount; i++)
        printf("%d %d %d\n", result.cutFrom[i], result.cutTo[i], result.cutCap[i]);

    printf("Execution time: %.4f ms\n", timeMs);

    freeMaxflowResult(&result);
    freeCSRGraph(&graph);
}