#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>

typedef struct {
    int vertices;
    int edges;
    int *rowPtr;     // size: numVertices + 1
    int *colIndex;   // size: numEdges (or 2*numEdges for undirected)
    int *edgeWeights; // NULL if the graph is unweighted
} CSRGraph;

CSRGraph readAdjacencyListAsCSR(FILE *fp, int isWeighted);

void printCSRGraph(CSRGraph *graph);
int readSourceVertex(FILE *filename);
void freeCSRGraph(CSRGraph *graph);

#endif